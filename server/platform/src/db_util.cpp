#include "db_util.h"

#include <iostream>

using namespace std;

MysqlConnection::MysqlConnection()
{
    m_mysql = mysql_init(nullptr); // 初始化
    mysql_real_connect(m_mysql,
                       "127.0.0.1",
                       "root",
                       "root",
                       "cpp0219",
                       3306,
                       nullptr,
                       0);
}

vector<vector<string>> MysqlConnection::ExecSelect(const string &sql)
{
    int res = mysql_real_query(m_mysql, sql.c_str(), sql.size());
    if (res != 0)
    {
        cout << mysql_error(m_mysql);
        return {};
    }
    vector<vector<string>> data;
    MYSQL_RES *srcData = mysql_store_result(m_mysql);
    int colCnt = mysql_field_count(m_mysql);
    MYSQL_ROW rowData = mysql_fetch_row(srcData);
    while (rowData != nullptr)
    {
        vector<string> row;
        for (int i = 0; i < colCnt; ++i)
        {
            row.push_back(rowData[i]);
        }
        data.push_back(row);
        rowData = mysql_fetch_row(srcData);
    }
    return data;
}

int MysqlConnection::ExecUpdate(const string &sql)
{
    return mysql_real_query(m_mysql, sql.c_str(), sql.size());
}

int MysqlConnection::ExecDelete(const string &sql)
{
    return mysql_real_query(m_mysql, sql.c_str(), sql.size());
}

int MysqlConnection::ExecInsert(const string &sql)
{
    return mysql_real_query(m_mysql, sql.c_str(), sql.size());
}
