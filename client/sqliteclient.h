#ifndef SQLITECLIENT_H
#define SQLITECLIENT_H

#include <QList>
#include <QSqlDatabase>
#include <QSqlQuery>

class SqliteClient
{
public:
    // 获取实例
    static SqliteClient* getInstance();
    // 插入/更新/删除
    bool execOtherSql(const QString& sql);
    // 查询
    QList<QList<QVariant>> execQuerySql(const QString& sql);
    // 查询单个记录
    QList<QVariant> execQuerySqlLimitOne(const QString& sql);
private:
    SqliteClient();
    ~SqliteClient();
    SqliteClient(const SqliteClient& other) = delete;
    SqliteClient& operator=(const SqliteClient& other) = delete;
    QSqlDatabase m_db;
    QSqlQuery m_query;
};

#endif // SQLITECLIENT_H
