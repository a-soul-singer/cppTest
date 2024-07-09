#include "loginsettings.h"
#include "ui_loginsettings.h"

#include <QSqlQuery>
#include <QString>


LoginSettings::LoginSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginSettings)
{
    ui->setupUi(this);

    connect(ui->pushButtonConfirm,&QPushButton::clicked,this,&LoginSettings::handleConfirmbtnClicked);
    connect(ui->pushButtonCancel,&QPushButton::clicked,this,&LoginSettings::handleCancelbtnClicked);

}

LoginSettings::~LoginSettings()
{
    delete ui;
}

void LoginSettings::handleConfirmbtnClicked()
{
    QStringList list;
    list.append(ui->lineEditHost->text());
    list.append(ui->lineEditPort->text());
    emit LogSetSignal(list);
    this->hide();
}

void LoginSettings::handleCancelbtnClicked()
{
    emit LogCancelSignal();
    this->hide();
}
