#include "user_controller.h"

#include "fmt/core.h"
#include "db_util.h"

using namespace std;
using namespace fmt;
using namespace Json;

Json::Value UserController::HandleRequest(const Json::Value &value)
{
  MysqlConnection mysql;
  string action = value["action"].asString();
  Value responseBody;
  if (action == "query_users")
  {
    responseBody["code"] = "200";
    responseBody["message"] = "query success";
    string sql = format("select * from t_user;");
    auto users = mysql.ExecSelect(sql);
    Value usersVal;
    for (auto user : users)
    {
      Value userVal;
      for (auto val : user)
      {
        userVal.append(val);
      }
      usersVal.append(userVal);
    }
    responseBody["data"] = usersVal;
  }
  else
  {
    responseBody["code"] = "500";
    responseBody["message"] = "action error";
    responseBody["data"] = Value();
  }
  return responseBody;
}