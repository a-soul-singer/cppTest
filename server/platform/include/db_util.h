#ifndef DB_UTIL
#define DB_UTILS

#include <string>
#include <vector>

#include "mysql.h"

class MysqlConnection {
public:
    MysqlConnection();
    
    std::vector<std::vector<std::string>> ExecSelect(const std::string& sql);

    int ExecUpdate(const std::string& sql);

    int ExecDelete(const std::string& sql);

    int ExecInsert(const std::string& sql);
private:
    MYSQL* m_mysql;
};

#endif