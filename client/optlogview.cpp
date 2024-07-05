#include "optlogview.h"
#include "ui_optlogview.h"

OptLogView::OptLogView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OptLogView)
{
    ui->setupUi(this);
}

OptLogView::~OptLogView()
{
    delete ui;
}
