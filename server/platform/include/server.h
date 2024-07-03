#ifndef SERVER_H
#define SERVER_H

#include <arpa/inet.h>
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



class serverUtil
{
public:
static void writeJsonResponse(int clientFd, int type, const  Json::Value &body);
static bool readJsonRequest(int clientFd, int& type, Json::Value& value);
static void subThread(int fd);
static int createClient();
};



#endif