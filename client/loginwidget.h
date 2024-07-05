#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QJsonObject>
#include <QSqlDatabase>
#include <QWidget>

namespace Ui {
class LoginWidget;
}

class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = nullptr);
    ~LoginWidget();

    void setStatus(const QString& message);
    void reloadSetting();
    bool checkIsAutoLogin();

private slots:
    void on_pushButtonLogin_clicked();

public slots:
    void handleLoginRes(bool isSuccess);

signals:
    void sendSocketData(const QJsonObject& body);

private:
    Ui::LoginWidget *ui;
    QSqlDatabase m_db;
    bool m_isAutoLogin;
};

#endif // LOGINWIDGET_H
