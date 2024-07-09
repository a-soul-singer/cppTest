#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QJsonObject>
#include <QSqlDatabase>
#include <QWidget>

#include "loginsettings.h"

namespace Ui {
class LoginWidget;
}

class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = nullptr);
    ~LoginWidget();

    void setStatus(const QString& message);
    void reloadSetting();
    bool checkIsAutoLogin();

private slots:
    void on_pushButtonLogin_clicked();

public slots:
    void handleLoginRes(bool isSuccess);
    void handleSetBtnClicked();
    void handleLogSetSignal(QStringList& list);
    void handleLogCancelSignal();

signals:
    void sendSocketData(const QJsonObject& body);
    void sendClientData(QStringList&list);
private:
    Ui::LoginWidget *ui;
    bool m_isAutoLogin;
    QSqlDatabase m_db;
    LoginSettings* m_loginSettingWidget;
};

#endif // LOGINWIDGET_H
