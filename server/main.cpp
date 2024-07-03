#include "socket.h"
#include <iostream>

using namespace std;

int main() {
  int res = ServerUtil::CreateClient();
  if (res != 0) {
    LOG_ERROR("连接失败");
    return -1;
  }
  return 0;
}