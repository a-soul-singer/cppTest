#ifndef COMPANYINFOVIEW_H
#define COMPANYINFOVIEW_H

#include <QWidget>

namespace Ui {
class CompanyInfoView;
}

class CompanyInfoView : public QWidget
{
    Q_OBJECT

public:
    explicit CompanyInfoView(QWidget *parent = nullptr);
    ~CompanyInfoView();

private:
    Ui::CompanyInfoView *ui;
};

#endif // COMPANYINFOVIEW_H
