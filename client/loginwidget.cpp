#include "loginwidget.h"
#include "ui_loginwidget.h"


#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

LoginWidget::LoginWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWidget)
{
    ui->setupUi(this);
    m_db=QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("..\\client\\datatbase\\system.db");
    if (!m_db.open()) {
        return;
    }
    m_isAutoLogin = false;
    reloadSetting();

    m_loginSettingWidget=new LoginSettings();
    connect(ui->pushButtonSetting,&QPushButton::clicked,this,&LoginWidget::handleSetBtnClicked);
    connect(m_loginSettingWidget,&LoginSettings::LogSetSignal,this,handleLogSetSignal);
    connect(m_loginSettingWidget,&LoginSettings::LogCancelSignal,this,handleLogCancelSignal);
}

LoginWidget::~LoginWidget()
{
    delete ui;
    if (m_db.isOpen()) {
        m_db.close();
    }
}

void LoginWidget::setStatus(const QString &message)
{
    ui->labelStatus->setText(message);
}

void LoginWidget::reloadSetting()
{
    QSqlQuery query(m_db);
    // 组装sql语句
    QString sql = "select id, username, password from t_user order by last_time desc;";
    query.exec(sql);
    if (query.next()) {
        int dbId = query.value("id").toInt();
        QString dbUsername = query.value("username").toString();
        QString dbPassword = query.value("password").toString();
        QString selectSettingsql = "select remember_pwd, auto_login from t_setting where user_id = %1;";
        query.exec(selectSettingsql.arg(dbId));
        if (query.next()) {
            int rememberPwd = query.value("remember_pwd").toInt();
            int autoLogin = query.value("auto_login").toInt();
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
    emit sendSocketData(obj);
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
    QSqlQuery query(m_db);
    // 组装sql语句
    QString sql = "select id, username, password from t_user;";
    bool res = query.exec(sql);
    if (!res) {
        return;
    }
    int cnt = 0;
    bool isExist = false;
    int existId = -1;
    // 判断当前用户在不在数据库中
    while(query.next()) {
        int dbId = query.value("id").toInt();
        QString dbUsername = query.value("username").toString();
        QString dbPassword = query.value("password").toString();
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
        query.exec(insertUserSql.arg(username).arg(password).arg(currTime));
        // 并且增加其配置
        QString insertSettingSql = "insert into t_setting(remember_pwd, auto_login, user_id) values(%1, %2, %3);";
        query.exec(insertSettingSql.arg(checkBoxRememberPwd).arg(checkBoxAutoLogin).arg(cnt + 1));
    } else {
        // 更新用户表
        QString updateUserSql = "update t_user set last_time='%1' where id = %2;";
        query.exec(updateUserSql.arg(currTime).arg(existId));
        // 更新该用户的设置
        QString updateSettingSql = "update t_setting set remember_pwd=%1, auto_login=%2 where user_id = %3;";
        query.exec(updateSettingSql.arg(checkBoxRememberPwd).arg(checkBoxAutoLogin).arg(existId));
    }
}

void LoginWidget::handleSetBtnClicked()
{
    this->close();
    m_loginSettingWidget->show();
}

void LoginWidget::handleLogSetSignal(QStringList& list) // 处理确定按钮的信号
{
    QSqlQuery query(m_db);
    QString sql = "insert into t_server(host, port) values('%1', '%2');";
    query.exec(sql.arg(list[0]).arg(list[1]));
    emit sendClientData(list);
    this->show();
}

void LoginWidget::handleLogCancelSignal() // 处理取消按钮的信号
{
    this->show();
}
