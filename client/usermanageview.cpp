#include "usermanageview.h"
#include "ui_usermanageview.h"

UserManageView::UserManageView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserManageView)
{
    ui->setupUi(this);
}

UserManageView::~UserManageView()
{
    delete ui;
}
