#include "login_controller.h"

#include "fmt/core.h"

#include "db_util.h"
#include "secure.h"

using namespace std;
using namespace fmt;
using namespace Json;

Json::Value LoginController::HandleRequest(const Json::Value &value)
{
    MysqlConnection mysql;
    string username = value["username"].asString();
    string password = value["password"].asString();
    string sql = format("select password from t_user where username = '{}';",
                        username);
    auto res = mysql.ExecSelect(sql);
    Value responseBody;
    if (res.empty())
    {
        responseBody["code"] = "500";
        responseBody["message"] = "username not found";
        responseBody["data"] = Value();
    }
    else if (SecureUtil::Sha256Salt(password) == res.front().front())
    {
        responseBody["code"] = "200";
        responseBody["message"] = "login success";
        responseBody["data"] = Value();
    }
    else
    {
        responseBody["code"] = "500";
        responseBody["message"] = "username or password error";
        responseBody["data"] = Value();
    }
    return responseBody;
}