#include "carinfo.h"
#include "ui_carinfo.h"

Carinfo::Carinfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Carinfo)
{
    ui->setupUi(this);
}

Carinfo::~Carinfo()
{
    delete ui;
}
