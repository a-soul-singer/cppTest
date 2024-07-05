#include "syssettingview.h"
#include "ui_syssettingview.h"

SysSettingView::SysSettingView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SysSettingView)
{
    ui->setupUi(this);
}

SysSettingView::~SysSettingView()
{
    delete ui;
}
