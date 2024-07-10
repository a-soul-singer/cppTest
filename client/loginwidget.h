#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QJsonObject>
#include <QWidget>

#include "sqliteclient.h"
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
    void handleLogSetSignal(const QString& host, const QString& port);

signals:
    void sendSocketData(int type, QJsonObject& body);
private:
    Ui::LoginWidget *ui;
    bool m_isAutoLogin;
    LoginSettings* m_loginSettingWidget;
    SqliteClient* m_sqliteCls;
};

#endif // LOGINWIDGET_H
