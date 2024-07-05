#ifndef HOMEWIDGET_H
#define HOMEWIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>

#include "loginwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class HomeWidget; }
QT_END_NAMESPACE

class HomeWidget : public QWidget
{
    Q_OBJECT

public:
    HomeWidget(QWidget *parent = nullptr);
    ~HomeWidget();
private slots:
    void handleSendSocketData(const QJsonObject& body);
    void handleReadyRead();
signals:
    void loginRes(bool);
private:
    Ui::HomeWidget *ui;
    LoginWidget* m_loginWidget;
    QTcpSocket* m_client;
};
#endif // HOMEWIDGET_H
