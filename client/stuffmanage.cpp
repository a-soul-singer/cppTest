#include "stuffmanage.h"
#include "ui_stuffmanage.h"

StuffManage::StuffManage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StuffManage)
{
    ui->setupUi(this);
}

StuffManage::~StuffManage()
{
    delete ui;
}
