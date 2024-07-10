#include "loginsettings.h"
#include "ui_loginsettings.h"

#include <QString>

LoginSettings::LoginSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginSettings)
{
    ui->setupUi(this);
    connect(ui->pushButtonConfirm, &QPushButton::clicked,
            this, &LoginSettings::handleConfirmbtnClicked);
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
