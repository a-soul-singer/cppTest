#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QJsonObject>
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

private slots:
    void on_pushButtonLogin_clicked();
signals:
    void sendSocketData(const QJsonObject& body);

private:
    Ui::LoginWidget *ui;
};

#endif // LOGINWIDGET_H
