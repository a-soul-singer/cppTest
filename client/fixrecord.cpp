#include "fixrecord.h"
#include "ui_fixrecord.h"

FixRecord::FixRecord(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FixRecord)
{
    ui->setupUi(this);
}

FixRecord::~FixRecord()
{
    delete ui;
}
