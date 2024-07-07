#include "systemwarning.h"
#include "ui_systemwarning.h"

SystemWarning::SystemWarning(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SystemWarning)
{
    ui->setupUi(this);
}

SystemWarning::~SystemWarning()
{
    delete ui;
}
