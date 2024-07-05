#include "companyinfoview.h"
#include "ui_companyinfoview.h"

CompanyInfoView::CompanyInfoView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompanyInfoView)
{
    ui->setupUi(this);
}

CompanyInfoView::~CompanyInfoView()
{
    delete ui;
}
