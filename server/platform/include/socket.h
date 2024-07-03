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

#include "fmt/core.h"
#include "json/json.h"

#include "db_util.h"
#include "ini.h"
#include "logger.h"
#include "secure.h"
#include "socket_data.h"

class ServerUtil {
public:
  static void WriteJsonResponse(int clientFd, int type,const Json::Value &body);
  static bool ReadJsonRequest(int clientFd, int &type, Json::Value &value);
  static void SubThread(int fd);
  static int CreateClient();
};

#endif