#ifndef USERMANAGEVIEW_H
#define USERMANAGEVIEW_H

#include <QWidget>

#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <vector>
#include <QString>



namespace Ui {
class UserManageView;
}

class UserManageView : public QWidget
{
    Q_OBJECT

public:
    explicit UserManageView(QWidget *parent = nullptr);
    ~UserManageView();

private slots:
    void handQueryButtonClicked();
    void handExportButtonClicked();

private:

    QLineEdit *lineEdit;
    QPushButton *queryButton;
    QTableWidget *tableWidget;
    QPushButton *exportButton;
    std::map<int, std::vector<QString>> rowData;
    //Ui::UserManageView *ui;
};

#endif // USERMANAGEVIEW_H
