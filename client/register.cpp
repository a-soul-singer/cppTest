#include "register.h"
#include "ui_register.h"
#include <QDebug>

#include "socket_data.h"

Register::Register(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    connect(ui->pushButtonConfirm, &QPushButton::clicked,
            this, &Register::handleConfirmbtnClicked);
    connect(ui->pushButtonCancel, &QPushButton::clicked,
            this, &Register::handleCancelbtnClicked);
}

Register::~Register()
{
delete ui;
}

void Register::setStatus(const QString &message)
{
    ui->labelState->setText(message);
}

void Register::handleConfirmbtnClicked()
{
    QString username = ui->lineEditUserName->text();
    QString password = ui->lineEditPassWord->text();
    if (username.isEmpty() || password.isEmpty()) {
        setStatus("用户名或密码不能为空");
        return;
    }
    qDebug() << "************" << endl;
    QJsonObject obj;
    obj["username"] = username;
    obj["password"] = password;
    emit sendSocketData(REGISTER_REQ, obj);
}

void Register::handleCancelbtnClicked()
{
    this->hide();
}
