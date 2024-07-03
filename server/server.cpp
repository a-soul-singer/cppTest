#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>

#include "json/json.h"
#include "fmt/core.h"

#include "socket_data.h"
#include "db_util.h"
#include "ini.h"
#include "secure.h"
#include "logger.h"

using namespace std;
using namespace Json;
using namespace fmt;

#define SOCKET_UNIT (8192)
#define ERROR_CODE (-1)

void writeJsonResponse(int clientFd, int type, const Value &body)
{
    FastWriter fastWriter;
    string jsonStr = fastWriter.write(body);
    // head + body
    Head head;
    head.type = type;
    head.length = sizeof(Head) + jsonStr.size();
    char *request = new char[head.length];
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
    if (writeSize == ERROR_CODE)
    {
        cout << "发送失败" << endl;
        return;
    }
    cout << "发送的请求类型:" << type << endl;
    cout << "发送的请求字节大小:" << head.length << endl;
    // cout << "发送的请求的body:" << jsonStr << endl;
    cout << "实际发送的字节大小:" << writeSize << endl;
}

bool readJsonRequest(int clientFd, int& type, Value& value)
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
    char* body = new char[bodySize];
    currReadSize = read(clientFd, body, bodySize);
    readSize += currReadSize;
    if (currReadSize == -1 || currReadSize == 0) {
        return false;
    }
    LOG_INFO("收到的响应类型:%d", head.type);
    cout << "收到的响应字节大小:" << head.length << endl;
    cout << "收到的响应的body:" << body << endl;
    cout << "实际收到的字节大小:" << readSize << endl;
    Value root; 
    Reader reader;
    if (reader.parse(body, root)) {
        type = head.type;
        value = root;
        return true;
    } else {
        // 解析失败
        cout << "Failed to parse JSON" << endl;
        return false;
    }
}

void subThread(int fd)
{
    MysqlConnection mysql;
    while (1)
    {
        int type = -1;
        Value value;
        bool readRes = readJsonRequest(fd, type, value);
        if (!readRes) {
            cout << "读取失败" << endl;
            break;
        }
        if (type == LOGIN_REQ) {
            string username = value["username"].asString();
            string password = value["password"].asString();
            string sql = format("select password from t_user where username = '{}';", username);
            auto res = mysql.ExecSelect(sql);
            if (res.empty()) {
                Value responseBody;
                responseBody["code"] = "500";
                responseBody["message"] = "username not found";
                responseBody["data"] = Value();
                writeJsonResponse(fd, LOGIN_RES, responseBody);
            } else if (SecureUtil::Sha256Salt(password) == res.front().front()) {
                Value responseBody;
                responseBody["code"] = "200";
                responseBody["message"] = "login success";
                responseBody["data"] = Value();
                writeJsonResponse(fd, LOGIN_RES, responseBody);
            } else {
                Value responseBody;
                responseBody["code"] = "500";
                responseBody["message"] = "username or password error";
                responseBody["data"] = Value();
                writeJsonResponse(fd, LOGIN_RES, responseBody);
            }
        } else if (type == USERS_REQ) {
            string action = value["action"].asString();
            if (action == "query_users") {
                Value responseBody;
                responseBody["code"] = "200";
                responseBody["message"] = "query success";
                string sql = format("select * from t_user;");
                auto users = mysql.ExecSelect(sql);
                Value usersVal;
                for (auto user: users) {
                    Value userVal;
                    for (auto val : user) {
                        userVal.append(val);
                    }
                    usersVal.append(userVal);
                }
                responseBody["data"] = usersVal;
                writeJsonResponse(fd, USERS_RES, responseBody);
            }
        }
    }
    // (7) 关闭套接字
    // int close(int fd);
    int res = close(fd);
    if (res == ERROR_CODE)
    {
        LOG_ERROR("关闭socket失败");
        return;
    }
}

int main()
{   
    LOG_INFO("server is running");
    IniParse ins("../config/platform.ini");
    auto serverData = ins.GetSectionData("server");
    // (1) 创建一个监听套接字
    // int socket(int domain, int type, int protocol);
    int listenFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenFd == ERROR_CODE)
    {
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
    // serverAddr.sin_addr.s_addr = htonl((192 << 24) | (168 << 16 ) | (246 << 8 ) | 151);

    // 字符串型的ip转换为int型ip格式
    // int inet_pton(int af, const char *src, void *dst);
    inet_pton(AF_INET, serverData["host"].c_str(), &serverAddr.sin_addr.s_addr);
    res = bind(listenFd, (struct sockaddr *)&serverAddr,
               sizeof(struct sockaddr_in));
    if (res == ERROR_CODE)
    {
        cout << "绑定套接字失败:" << strerror(errno) << endl;
        return 1;
    }
    cout << "绑定套接字成功" << endl;

    // (3) 监听客户端的连接
    // int listen(int sockfd, int backlog);
    res = listen(listenFd, 5);
    if (res == ERROR_CODE)
    {
        cout << "监听套接字失败:" << strerror(errno) << endl;
        return 1;
    }
    cout << "监听套接字成功" << endl;
    while (1)
    {
        // (4) 接收客户端的连接 【秘书】
        // int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
        struct sockaddr_in clientAddr;
        socklen_t len = sizeof(struct sockaddr_in);
        int fd = accept(listenFd, (sockaddr *)&clientAddr, &len);
        if (fd == ERROR_CODE)
        {
            cout << "接收客户端的连接失败:" << strerror(errno) << endl;
            return 1;
        }
        int bufsize;
        socklen_t bufsize_len = sizeof(bufsize);
        getsockopt(fd, SOL_SOCKET, SO_SNDBUF, &bufsize, &bufsize_len);
        printf("修改前发送缓冲区大小: %d\n", bufsize);

        bufsize = 1024 * 1024; // 1MB 发送缓冲区大小
        setsockopt(fd, SOL_SOCKET, SO_SNDBUF, &bufsize, sizeof(bufsize));

        bufsize;
        getsockopt(fd, SOL_SOCKET, SO_SNDBUF, &bufsize, &bufsize_len);
        printf("修改后发送缓冲区大小: %d\n", bufsize);

        int flag = 1; // 启用 TCP_NODELAY
        setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (void *)&flag, sizeof(int));

        // const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);
        char ip[16] = {0};
        inet_ntop(AF_INET, &clientAddr.sin_addr.s_addr, ip, sizeof(ip));
        cout << "客户端的IP：" << ip << endl;
        cout << "接收客户端的连接成功" << endl;
        thread t1(subThread, fd);
        t1.detach();
    }
    res = close(listenFd);
    if (res == ERROR_CODE)
    {
        cout << "关闭失败" << endl;
        return 1;
    }
    return 0;
}