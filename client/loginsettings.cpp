#include "loginsettings.h"
#include "ui_loginsettings.h"

#include <QString>
#include <QDebug>

LoginSettings::LoginSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginSettings)
{
    ui->setupUi(this);
    connect(ui->pushButtonConfirm, &QPushButton::clicked,this, &LoginSettings::handleConfirmbtnClicked);
    m_sqlCls = SqliteClient::getInstance();
    auto res=m_sqlCls->execQuerySqlLimitOne("select host,port from t_server");
    if(res.isEmpty()){
        return;
    }else {
        qDebug()<<res;
        ui->lineEditHost->setText(res[0].toString());
        ui->lineEditPort->setText(res[1].toString());
    }
}

LoginSettings::~LoginSettings()
{
    delete ui;
}

void LoginSettings::handleConfirmbtnClicked()
{
    emit LogSetSignal(ui->lineEditHost->text(), ui->lineEditPort->text());
    this->hide();
}

void LoginSettings::handleCancelbtnClicked()
{
    this->hide();
}
