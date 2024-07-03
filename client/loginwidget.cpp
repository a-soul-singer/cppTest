#include "loginwidget.h"
#include "ui_loginwidget.h"

LoginWidget::LoginWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWidget)
{
    ui->setupUi(this);
}

LoginWidget::~LoginWidget()
{
    delete ui;
}

void LoginWidget::setStatus(const QString &message)
{
    ui->labelStatus->setText(message);
}

void LoginWidget::on_pushButtonLogin_clicked()
{
    QString username = ui->lineEditUsername->text();
    QString password = ui->lineEditPassword->text();
    if (username.isEmpty() || password.isEmpty()) {
        setStatus("用户名和密码不能为空");
        return;
    }
    QJsonObject obj;
    obj["username"] = username;
    obj["password"] = password;
    emit sendSocketData(obj);
}
