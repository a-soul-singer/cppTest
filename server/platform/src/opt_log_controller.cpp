#include "opt_log_controller.h"

#include "fmt/core.h"

#include "db_util.h"
#include "secure.h"

#include <iostream>

using namespace std;
using namespace fmt;
using namespace Json;

Json::Value OptLogController::HandleRequest(const Json::Value &value)
{
    // 解析value，根据value中的数据，做出响应
    MysqlConnection mysql;
    string username = value["username"].asString();
    int eachPageCnt = value["each_page_cnt"].asInt();
    int currPage = value["curr_page"].asInt();
    int offsetCnt = (currPage - 1) * eachPageCnt;

    // 查询操作日志的总条数
    string cntSql = format("select count(*) from t_opt_log where username = '{}';", username);
    auto cntRes = mysql.ExecSelect(cntSql);
    int totalCnt = 0;
    if (!cntRes.empty() && !cntRes.front().empty()) {
        totalCnt = stoi(cntRes.front().front());
    }

    // 计算日志分页展示的总页数
    int totalPage = totalCnt / eachPageCnt;
    if (totalCnt % eachPageCnt != 0) {
        totalPage += 1;
    }

    // 通过limit和offset配合使用，实现分页查询
    string pageSql =
        format("select * from t_opt_log where username = '{}' limit {} offset {};", username, eachPageCnt, offsetCnt);
    auto res = mysql.ExecSelect(pageSql);

    // 返回对应响应
    Value responseBody;
    if (res.empty()) {
        responseBody["code"] = "500";
        responseBody["message"] = "log is empty";
        responseBody["data"] = Value();
    } else {
        responseBody["code"] = "200";
        responseBody["message"] = "query success";
        responseBody["data"]["curr_page"] = currPage;
        responseBody["data"]["total_page"] = totalPage;
        Value logsVal;
        for (auto log : res) {
            Value logVal;
            for (auto val : log) {
                logVal.append(val);
            }
            logsVal.append(logVal);
        }
        responseBody["data"]["logs"] = logsVal;
    }
    return responseBody;
}