#ifndef USERMANAGEVIEW_H
#define USERMANAGEVIEW_H

#include <QWidget>

namespace Ui {
class UserManageView;
}

class UserManageView : public QWidget
{
    Q_OBJECT

public:
    explicit UserManageView(QWidget *parent = nullptr);
    ~UserManageView();

private:
    Ui::UserManageView *ui;
};

#endif // USERMANAGEVIEW_H
