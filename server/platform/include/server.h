#ifndef SERVER_UNTIL
#define SERVER_UNTIL

#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>

#include "db_util.h"
#include "fmt/core.h"
#include "socket_data.h"
#include "json/json.h"

using namespace Json;
using namespace fmt;

class Server {
private:
  /* data */
public:
  Server();
  Server(std::string &ip, unsigned short &port);
  ~Server();

public:
  static void writeJsonResponse(int clientFd, int type, const Value &body);
  static void readJsonRequest(int clientFd, int &type, Value &value);
  static void subThread(int fd);
  static int CreateConnection(const Server&Server);
private:
  std::string m_ip;
  unsigned short m_port;
};
#endif // !SERVER_UNTIL