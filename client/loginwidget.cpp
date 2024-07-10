#include "loginwidget.h"
#include "ui_loginwidget.h"

#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

#include "socket_data.h"

LoginWidget::LoginWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWidget)
{
    ui->setupUi(this);
    m_sqliteCls = SqliteClient::getInstance();
    m_isAutoLogin = false;
    reloadSetting();

    m_loginSettingWidget = new LoginSettings();
    // 将设置界面变为模态窗口
    m_loginSettingWidget->setWindowModality(Qt::ApplicationModal);
    connect(ui->pushButtonSetting,&QPushButton::clicked,this,&LoginWidget::handleSetBtnClicked);
    connect(m_loginSettingWidget,&LoginSettings::LogSetSignal,this,&LoginWidget::handleLogSetSignal);

    m_registerWidget = new Register();
    // 将设置界面变为模态窗口
    m_registerWidget->setWindowModality(Qt::ApplicationModal);
    connect(ui->pushButtonRegister,&QPushButton::clicked,this,&LoginWidget::handleRegisterShow);
}

LoginWidget::~LoginWidget()
{
    delete ui;
    delete m_loginSettingWidget;
}

void LoginWidget::setStatus(const QString &message)
{
    ui->labelStatus->setText(message);
}

void LoginWidget::reloadSetting()
{
    QString sql = "select id, username, password from t_user order by last_time desc;";
    auto res = m_sqliteCls->execQuerySqlLimitOne(sql);
    if (!res.isEmpty()) {
        int dbId = res[0].toInt();
        QString dbUsername = res[1].toString();
        QString dbPassword = res[2].toString();

        QString selectSettingsql = "select remember_pwd, auto_login from t_setting where user_id = %1;";
        res = m_sqliteCls->execQuerySqlLimitOne(selectSettingsql.arg(dbId));
        if (!res.isEmpty()) {
            int rememberPwd = res[0].toInt();
            int autoLogin = res[1].toInt();
            if (rememberPwd) {
                ui->checkBoxRememberPwd->setCheckState(Qt::Checked);
                ui->lineEditUsername->setText(dbUsername);
                ui->lineEditPassword->setText(dbPassword);
            } else {
                ui->checkBoxRememberPwd->setCheckState(Qt::Unchecked);
            }
            if (autoLogin) {
                ui->checkBoxAutoLogin->setCheckState(Qt::Checked);
                m_isAutoLogin = true;
            } else {
                ui->checkBoxAutoLogin->setCheckState(Qt::Unchecked);
            }
        }
    }
}

bool LoginWidget::checkIsAutoLogin()
{
    if (m_isAutoLogin) {
        on_pushButtonLogin_clicked();
    }
    return m_isAutoLogin;
}

void LoginWidget::on_pushButtonLogin_clicked()
{
    QString username = ui->lineEditUsername->text();
    QString password = ui->lineEditPassword->text();
    if (username.isEmpty() || password.isEmpty()) {
        setStatus("用户名和密码不能为空");
        return;
    }
    qDebug() << "************" << endl;
    QJsonObject obj;
    obj["username"] = username;
    obj["password"] = password;
    emit sendSocketData(LOGIN_REQ, obj);
}

void LoginWidget::handleLoginRes(bool isSuccess)
{
    if (!isSuccess) {
        return;
    }
    int checkBoxRememberPwd = (int)ui->checkBoxRememberPwd->isChecked();
    int checkBoxAutoLogin = (int)ui->checkBoxAutoLogin->isChecked();
    QString username = ui->lineEditUsername->text();
    QString password = ui->lineEditPassword->text();
    // 组装sql语句
    QString sql = "select id, username, password from t_user;";
    auto res = m_sqliteCls->execQuerySql(sql);
    if (res.isEmpty() || res.front().isEmpty()) {
        return;
    }
    int cnt = 0;
    bool isExist = false;
    int existId = -1;
    // 判断当前用户在不在数据库中
    for (const auto& rowData : res) {
        int dbId = rowData[0].toInt();
        QString dbUsername = rowData[1].toString();
        QString dbPassword = rowData[2].toString();
        if (dbUsername == username && password == dbPassword) {
            isExist = true;
            existId = dbId;
        }
        cnt++;
    }
    // 获取当前的时间
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString currTime = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");
    if (!isExist) { // 说明用户不存在
        // 增加一个用户
        QString insertUserSql = "insert into t_user(username, password, last_time) values('%1', '%2', '%3');";
        m_sqliteCls->execOtherSql(insertUserSql.arg(username).arg(password).arg(currTime));
        // 并且增加其配置
        QString insertSettingSql = "insert into t_setting(remember_pwd, auto_login, user_id) values(%1, %2, %3);";
        m_sqliteCls->execOtherSql(insertSettingSql.arg(checkBoxRememberPwd).arg(checkBoxAutoLogin).arg(cnt + 1));
    } else {
        // 更新用户表
        QString updateUserSql = "update t_user set last_time='%1' where id = %2;";
        m_sqliteCls->execOtherSql(updateUserSql.arg(currTime).arg(existId));
        // 更新该用户的设置
        QString updateSettingSql = "update t_setting set remember_pwd=%1, auto_login=%2 where user_id = %3;";
        m_sqliteCls->execOtherSql(updateSettingSql.arg(checkBoxRememberPwd).arg(checkBoxAutoLogin).arg(existId));
    }
}

void LoginWidget::handleSetBtnClicked()
{
    m_loginSettingWidget->show();
}

void LoginWidget::handleLogSetSignal(const QString& host, const QString& port) // 处理确定按钮的信号
{
    // 先判断数据库中有没有数据
    QString sql = "select * t_server limit 1;";
    auto res = m_sqliteCls->execQuerySqlLimitOne(sql);
    if (!res.isEmpty()) { // 有, 更新操作
         sql = "update t_server set host = '%1', port = '%2';";
    } else { // 没有，插入操作
         sql = "insert into t_server(host, port) values('%1', '%2');";
    }
    m_sqliteCls->execOtherSql(sql.arg(host).arg(port));
}

void LoginWidget::handleRegisterShow()
{
    m_registerWidget->show();
}
