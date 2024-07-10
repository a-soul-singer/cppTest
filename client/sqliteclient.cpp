#include "sqliteclient.h"

#include <QSqlRecord>

SqliteClient *SqliteClient::getInstance()
{
    static SqliteClient client;
    return &client;
}

bool SqliteClient::execOtherSql(const QString &sql)
{
    return m_query.exec(sql);
}

QList<QList<QVariant>> SqliteClient::execQuerySql(const QString &sql)
{
    QList<QList<QVariant>> res;
    m_query.exec(sql);
    while (m_query.next()) {
        QSqlRecord record = m_query.record();
        int colCnt = record.count();
        QList<QVariant> subRes;
        for (int i = 0; i < colCnt; ++i) {
            subRes.append(m_query.value(i));
        }
        res.append(subRes);
    }
    return res;
}

QList<QVariant> SqliteClient::execQuerySqlLimitOne(const QString &sql)
{
    auto res = execQuerySql(sql);
    if (res.isEmpty()) {
        return {};
    }
    return res.front();
}

SqliteClient::SqliteClient()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("..\\client\\datatbase\\system.db");
    if (!m_db.open()) {
        return;
    }
    m_query = QSqlQuery(m_db);
}

SqliteClient::~SqliteClient()
{
    m_db.close();
}
