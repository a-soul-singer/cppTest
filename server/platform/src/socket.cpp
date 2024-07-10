#include "socket.h"

#include <memory>
#include <unordered_map>

#include "ini.h"
#include "json/json.h"

#include "login_controller.h"
#include "user_controller.h"
#include "opt_log_controller.h"
#include "controller.h"

using namespace std;
using namespace Json;
using namespace fmt;

#define SOCKET_UNIT (8192)
#define ERROR_CODE (-1)

void ServerUtil::WriteJsonResponse(int clientFd, int type, const Value &body)
{
    FastWriter fastWriter;
    string jsonStr = fastWriter.write(body);
    // head + body
    Head head;
    head.type = type;
    head.length = sizeof(Head) + jsonStr.size();
    unique_ptr<char[]> up(new char[head.length]);
    char *request = up.get();
    struct Data *data = (struct Data *)request;
    data->head = head;
    memcpy(data->body, jsonStr.c_str(), jsonStr.size());
    // 先发头部
    int writeSize = 0;
    writeSize += write(clientFd, request, sizeof(Head));
    // 按整数倍发送
    int cnt = jsonStr.size() / SOCKET_UNIT;
    for (int i = 0; i < cnt; ++i) {
        writeSize += write(clientFd, request + sizeof(Head) + i * SOCKET_UNIT, SOCKET_UNIT);
    }
    // 发余数
    int mod = jsonStr.size() % SOCKET_UNIT;
    if (mod != 0) {
        writeSize += write(clientFd, request + sizeof(Head) + cnt * SOCKET_UNIT, mod);
    }
    if (writeSize == ERROR_CODE) {
        LOG_ERROR("发送失败");
        return;
    }
    LOG_INFO("发送的请求类型:", to_string(type));
    LOG_INFO("发送的请求字节大小:", to_string(head.length));
    LOG_INFO("发送的请求的body:", jsonStr);
    LOG_INFO("实际发送的字节大小:", to_string(writeSize));
}

bool ServerUtil::ReadJsonRequest(int clientFd, int &type, Value &value)
{
    Head head;
    int currReadSize = 0;
    int readSize = 0;
    currReadSize = read(clientFd, &head, sizeof(Head));
    readSize += currReadSize;
    if (currReadSize == -1 || currReadSize == 0) {
        return false;
    }
    int bodySize = head.length - sizeof(Head);
    unique_ptr<char[]> up(new char[bodySize]);
    char *body = up.get();
    currReadSize = read(clientFd, body, bodySize);
    readSize += currReadSize;
    if (currReadSize == -1 || currReadSize == 0) {
        return false;
    }
    LOG_INFO("收到的请求类型:%d", head.type);
    LOG_INFO("收到的请求字节大小:", to_string(head.length));
    LOG_INFO("收到的请求的body:", body);
    LOG_INFO("实际收到的字节大小:", to_string(readSize));
    Value root;
    Reader reader;
    if (reader.parse(body, root)) {
        type = head.type;
        value = root;
        return true;
    } else {
        // 解析失败
        LOG_ERROR("Failed to parse JSON");
        return false;
    }
}

void ServerUtil::SubThread(int fd)
{
    unordered_map<int, pair<int, shared_ptr<Controller>>> ctrls = {
        {LOGIN_REQ, {LOGIN_RES, shared_ptr<Controller>(new LoginController)}},
        {USERS_REQ, {USERS_RES, shared_ptr<Controller>(new UserController)}},
        {OPT_LOG_REQ, {OPT_LOG_RES, shared_ptr<Controller>(new OptLogController)}},
    };
    while (1) {
        int type = -1;
        Value value;
        bool readRes = ReadJsonRequest(fd, type, value);
        if (!readRes) {
            LOG_ERROR("读取失败");
            break;
        }
        int reqType = ctrls[type].first;
        Controller *ctrl = ctrls[type].second.get();
        Value responseBody = ctrl->HandleRequest(value);
        WriteJsonResponse(fd, reqType, responseBody);
    }
    // (7) 关闭套接字
    // int close(int fd);
    int res = close(fd);
    if (res == ERROR_CODE) {
        LOG_ERROR("关闭socket失败");
        return;
    }
}

int ServerUtil::CreateClient()
{
    cout << "server is running" << endl;
    IniParse ins("../config/platform.ini");
    auto serverData = ins.GetSectionData("server");
    // (1) 创建一个监听套接字
    // int socket(int domain, int type, int protocol);
    int listenFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenFd == ERROR_CODE) {
        cout << "创建套接字失败:" << strerror(errno) << endl;
        return 1;
    }
    cout << "创建套接字成功" << endl;

    // 设置端口复用
    int opt = 1;
    setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // (2) 绑定协议，IP, 端口
    // int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
    int res = ERROR_CODE;
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(stoi(serverData["port"]));
    // INADDR_ANY 0.0.0.0
    // INADDR_LOOPBACK 127.0.0.1
    // serverAddr.sin_addr.s_addr = htonl((192 << 24) | (168 << 16 ) | (246 << 8 )
    // | 151);

    // 字符串型的ip转换为int型ip格式
    // int inet_pton(int af, const char *src, void *dst);
    inet_pton(AF_INET, serverData["host"].c_str(), &serverAddr.sin_addr.s_addr);
    res = bind(listenFd, (struct sockaddr *)&serverAddr, sizeof(struct sockaddr_in));
    if (res == ERROR_CODE) {
        LOG_ERROR("绑定套接字失败:", strerror(errno));
        return 1;
    }
    cout << "绑定套接字成功" << endl;

    // (3) 监听客户端的连接
    // int listen(int sockfd, int backlog);
    res = listen(listenFd, 5);
    if (res == ERROR_CODE) {
        LOG_ERROR("监听套接字失败:", strerror(errno));
        return 1;
    }
    cout << "监听套接字成功" << endl;
    while (1) {
        // (4) 接收客户端的连接 【秘书】
        // int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
        struct sockaddr_in clientAddr;
        socklen_t len = sizeof(struct sockaddr_in);
        int fd = accept(listenFd, (sockaddr *)&clientAddr, &len);
        if (fd == ERROR_CODE) {
            LOG_ERROR("接收客户端的连接失败:", strerror(errno));
            return 1;
        }
        int bufsize;
        socklen_t bufsize_len = sizeof(bufsize);
        getsockopt(fd, SOL_SOCKET, SO_SNDBUF, &bufsize, &bufsize_len);
        // cout << "修改前发送缓冲区大小:" << bufsize<<endl;

        bufsize = 1024 * 1024; // 1MB 发送缓冲区大小
        setsockopt(fd, SOL_SOCKET, SO_SNDBUF, &bufsize, sizeof(bufsize));

        bufsize;
        getsockopt(fd, SOL_SOCKET, SO_SNDBUF, &bufsize, &bufsize_len);
        // cout << "修改前发送缓冲区大小:" << bufsize<<endl;

        int flag = 1; // 启用 TCP_NODELAY
        setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (void *)&flag, sizeof(int));

        // const char *inet_ntop(int af, const void *src, char *dst, socklen_t
        // size);
        char ip[16] = {0};
        inet_ntop(AF_INET, &clientAddr.sin_addr.s_addr, ip, sizeof(ip));
        cout << "客户端的IP:" << ip << endl;
        cout << "接收客户端的连接成功" << endl;
        thread t1(SubThread, fd);
        t1.detach();
    }
    res = close(listenFd);
    if (res == ERROR_CODE) {
        LOG_ERROR("关闭失败");
        return 1;
    }
    return 0;
}
