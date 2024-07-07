#ifndef USERMANAGEVIEW_H
#define USERMANAGEVIEW_H

#include <QWidget>

#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>

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

    QLineEdit *usernameEdit;
    QPushButton *addButton;
    QTableWidget *userTable;
    //Ui::UserManageView *ui;
};

#endif // USERMANAGEVIEW_H
