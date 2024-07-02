#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <netinet/tcp.h>

#include "socket_data.h"
#include "json/json.h"

using namespace std;
using namespace Json;


#define READ_UNIT (8192)
#define ERROR_CODE (-1)

void writeJsonRequest(int clientFd, int type, const Value &body)
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
    int writeSize = write(clientFd, request, head.length);
    if (writeSize == ERROR_CODE)
    {
        cout << "发送失败" << endl;
        return;
    }
    cout << "发送的请求类型:" << type << endl;
    cout << "发送的请求字节大小:" << head.length << endl;
    cout << "发送的请求的body:" << jsonStr << endl;
    cout << "实际发送的字节大小:" << writeSize << endl;
}

int readJsonResponse(int clientFd, int& type, Value& value)
{
    Head head;
    int readSize = 0;
    // 读取请求头部信息
    int headSize = read(clientFd, &head, sizeof(Head)); 
    readSize += headSize;
    cout << "读取的头部大小" << headSize << " " << readSize << endl;
    
    int bodySize = head.length - sizeof(Head); // 4097
    char* body = new char[bodySize + 1]; // 总的
    body[bodySize] = '\0';

    char buffer[READ_UNIT] = {0}; // 每一次读
    int cnt = bodySize / READ_UNIT;
    for (int i = 0; i < cnt; ++i) {
        int unitSize = read(clientFd, buffer, sizeof(buffer));
        readSize += unitSize;
        cout << "每一次实际读取的大小" << unitSize << " " << readSize << endl;
        memcpy(body + i * READ_UNIT, buffer, sizeof(buffer));
    }
    int mod = bodySize % READ_UNIT;
    if (mod != 0) { // 如果不能整除，需要读取余下的字节数
        int unitSize = read(clientFd, buffer, mod);
        readSize += unitSize;
        cout << "每一次实际读取的余数大小" << unitSize << " " << readSize << endl;
        memcpy(body + cnt * READ_UNIT, buffer, mod);
    }

    cout << "收到的响应类型:" << head.type << endl;
    cout << "收到的响应字节大小:" << head.length << endl; // 243632
    // cout << "收到的响应的body:" << body << endl;
    cout << "实际收到的字节大小:" << readSize << endl; // 243624
    Value root; 
    Reader reader;
    if (reader.parse(body, root)) {
        type = head.type;
        value = root;
        return 0;
    } else {
        // 解析失败
        cout << "Failed to parse JSON" << endl;
        return -1;
    }
}

int startClient()
{
    // (1) 创建一个套接字
    // int socket(int domain, int type, int protocol);
    int clientFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientFd == ERROR_CODE)
    {
        cout << "创建套接字失败:" << strerror(errno) << endl;
        return 1;
    }
    cout << "创建套接字成功" << endl;

    int bufsize;
    socklen_t bufsize_len = sizeof(bufsize);
    getsockopt(clientFd, SOL_SOCKET, SO_RCVBUF, &bufsize, &bufsize_len);
    printf("修改前接收缓冲区大小: %d\n", bufsize);

    bufsize = 1024 * 1024; // 1MB 接收缓冲区大小
    setsockopt(clientFd, SOL_SOCKET, SO_RCVBUF, &bufsize, sizeof(bufsize));

    bufsize;
    getsockopt(clientFd, SOL_SOCKET, SO_RCVBUF, &bufsize, &bufsize_len);
    printf("修改后接收缓冲区大小: %d\n", bufsize);

    int flag = 1; // 启用 TCP_NODELAY
    setsockopt(clientFd, IPPROTO_TCP, TCP_NODELAY, (void *)&flag, sizeof(int));

    int res = ERROR_CODE;
    // (2) 连接服务端
    // int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(10086);
    inet_pton(AF_INET, "192.168.246.151", &serverAddr.sin_addr.s_addr);
    res = connect(clientFd, (struct sockaddr *)&serverAddr, sizeof(struct sockaddr_in));
    if (res == ERROR_CODE)
    {
        cout << "连接服务端失败:" << strerror(errno) << endl;
        return 1;
    }
    cout << "连接服务端成功" << endl;
    bool isLoginSuccess = false;
    while (1)
    {
        if (!isLoginSuccess)
        {
            string username;
            string password;
            cout << "请输入用户名:";
            cin >> username;
            cout << "请输入密码:";
            cin >> password;
            Value value;
            value["username"] = username;
            value["password"] = password;
            writeJsonRequest(clientFd, LOGIN_REQ, value);
        } else {
            Value value;
            value["action"] = "query_users";
            writeJsonRequest(clientFd, USERS_REQ, value);
        }
        int resType = -1;
        Value responseBody;
        int res = readJsonResponse(clientFd, resType, responseBody);
        if (res == ERROR_CODE) {
            isLoginSuccess = false;
            continue;
        }
        if (resType == LOGIN_RES) {
            if (responseBody["code"].asString() == "200") {
                cout << "登录成功" << endl;
                isLoginSuccess = true;
            } else  {
                cout << responseBody["message"].asString() << endl;
            }
        } else if (resType == USERS_RES) {
            if (responseBody["code"].asString() == "200") {
                int row = responseBody["data"].size();
                for (int i = 0; i < row; ++i) {
                    int col = responseBody["data"][i].size();
                    for (int j = 0; j < col; ++j) {
                        // cout << responseBody["data"][i][j].asString() << " ";
                    }
                    cout << endl;
                }
                isLoginSuccess = false;
            } else  {
                cout << responseBody["message"].asString() << endl;
            }
        }
    }
    // （5）关闭客户端套接字
    // int close(int fd);
    res = close(clientFd);
    if (res == ERROR_CODE)
    {
        cout << "关闭失败" << endl;
        return 1;
    }
    cout << "关闭成功" << endl;
}

int main()
{
    startClient();
    return 0;
}