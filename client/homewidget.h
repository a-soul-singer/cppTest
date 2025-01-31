#ifndef HOMEWIDGET_H
#define HOMEWIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QPushButton>

#include "loginwidget.h"
#include "companyinfoview.h"
#include "optlogview.h"
#include "socket_data.h"
#include "syssettingview.h"
#include "usermanageview.h"
#include "fixrecord.h"
#include "stuffmanage.h"
#include "carinfo.h"
#include "systemwarning.h"
#include "clientmanage.h"
#include "sqliteclient.h"
#include "electronicinterface.h"


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
    void handleSendSocketData(int type, QJsonObject& body);
    void handleReadyRead();
    void handleChangePage();
    void handleSetStyleSheetSignal(const QString& style);

    void on_pushButtonExitLogin_clicked();



signals:
    void loginRes(bool);
private:
    Ui::HomeWidget *ui;
    LoginWidget* m_loginWidget;
    QTcpSocket* m_client;

    CompanyInfoView * m_companyinfoview;
    OptLogView* m_optlogview;
    SysSettingView* m_syssettingview;
    UserManageView* m_usermanageview;
    Carinfo* m_carinfo;
    ClientManage* m_clientmanage;
    FixRecord* m_fixrecord;
    StuffManage* m_stuffmanage;
    SystemWarning* m_systemwarning;
    Electronicinterface* m_electronicinterface;

    QMap<QPushButton*, QWidget*> pageMap;
    QList<QPushButton*> buttonList;

    QString m_host;
    QString m_port;
    QString m_currUser;
    SqliteClient* m_sqliteCls;
};
#endif // HOMEWIDGET_H
