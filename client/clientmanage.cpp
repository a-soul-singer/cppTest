#include "clientmanage.h"
#include "ui_clientmanage.h"

ClientManage::ClientManage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientManage)
{
    ui->setupUi(this);
}

ClientManage::~ClientManage()
{
    delete ui;
}
