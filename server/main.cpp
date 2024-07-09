#include "socket.h"
#include <iostream>

using namespace std;

int main()
{
  try
  {
    int res = ServerUtil::CreateClient();
    if (res != 0)
    {
      LOG_ERROR("连接失败");
      return -1;
    }
  }
  catch (const std::exception &err)
  {
    LOG_ERROR("未知错误：%s", err.what());
    RunLog::release();
  }
  return 0;
}