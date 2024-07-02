#include "server.h"
#include <iostream>
#include <string>

using namespace std;

int main() {
  string  ip;
  unsigned short port;
  cin>>ip>>port;
  Server server(ip,port);
  
  int res = Server::CreateConnection(server);
  if (res != 0) {
    cout << "连接失败" << endl;
  }
  return 0;
}
