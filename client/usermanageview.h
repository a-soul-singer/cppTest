#ifndef USERMANAGEVIEW_H
#define USERMANAGEVIEW_H

#include <QWidget>

#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <vector>
#include <QString>
#include <QJsonObject>
#include <QJsonArray>


namespace Ui {
class UserManageView;
}

class UserManageView : public QWidget
{
    Q_OBJECT

public:
    explicit UserManageView(QWidget *parent = nullptr);
    ~UserManageView();
    QLineEdit* getLineEdit();
    void handleUserResponse(const QJsonObject& obj);
    void changeWindowRequest();

private slots:
    void handQueryButtonClicked();
    void handExportButtonClicked();



signals:
    void sendSocketData(int type, QJsonObject& obj);

private:

    QLineEdit *lineEdit;
    QPushButton *queryButton;
    QTableWidget *tableWidget;
    QPushButton *exportButton;
    std::map<int, std::vector<QString>> rowData;
    //Ui::UserManageView *ui;
};

#endif // USERMANAGEVIEW_H
