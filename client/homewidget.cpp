#include "homewidget.h"
#include "ui_homewidget.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include <QPushButton>

#include "socket_data.h"


HomeWidget::HomeWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HomeWidget)
{
    ui->setupUi(this);
    m_client = new QTcpSocket(this);
    m_loginWidget = new LoginWidget();
    m_companyinfoview = new CompanyInfoView();
    m_optlogview = new OptLogView();
    m_syssettingview = new SysSettingView();
    m_usermanageview = new UserManageView();
    m_carinfo = new Carinfo();
    m_clientmanage = new ClientManage();
    m_fixrecord = new FixRecord();
    m_stuffmanage = new StuffManage();
    m_systemwarning = new SystemWarning();

    connect(m_loginWidget, &LoginWidget::sendSocketData, this, &HomeWidget::handleSendSocketData);
    connect(this, &HomeWidget::loginRes, m_loginWidget, &LoginWidget::handleLoginRes);
    connect(m_client, &QTcpSocket::readyRead, this, &HomeWidget::handleReadyRead);
    bool autoLogin = m_loginWidget->checkIsAutoLogin();
    if (!autoLogin) {
        m_loginWidget->show();
    }

    buttonList.append(ui->pushButtonCompanyInfo);
    buttonList.append(ui->pushButtonOptLog);
    buttonList.append(ui->pushButtonSysSetting);
    buttonList.append(ui->pushButtonStuffManage);
    buttonList.append(ui->pushButtonCarInfo);
    buttonList.append(ui->pushButtonClientManage);
    buttonList.append(ui->pushButtonFix);
    buttonList.append(ui->pushButtonwarning);
    buttonList.append(ui->pushButtonUserManage);


    pageMap.insert(ui->pushButtonCompanyInfo,m_companyinfoview);
    pageMap.insert(ui->pushButtonOptLog,m_optlogview);
    pageMap.insert(ui->pushButtonSysSetting,m_syssettingview);
    pageMap.insert(ui->pushButtonUserManage,m_usermanageview);
    pageMap.insert(ui->pushButtonStuffManage,m_stuffmanage);
    pageMap.insert(ui->pushButtonCarInfo,m_carinfo);
    pageMap.insert(ui->pushButtonClientManage,m_clientmanage);
    pageMap.insert(ui->pushButtonFix,m_fixrecord);
    pageMap.insert(ui->pushButtonwarning,m_systemwarning);

    ui->stackedWidget->addWidget(m_companyinfoview);
    ui->stackedWidget->addWidget(m_optlogview);
    ui->stackedWidget->addWidget(m_syssettingview);
    ui->stackedWidget->addWidget(m_usermanageview);
    ui->stackedWidget->addWidget(m_stuffmanage);
    ui->stackedWidget->addWidget(m_carinfo);
    ui->stackedWidget->addWidget(m_clientmanage);
    ui->stackedWidget->addWidget(m_fixrecord);
    ui->stackedWidget->addWidget(m_systemwarning);

    for (QPushButton *button : buttonList) {
        connect(button, &QPushButton::clicked, this, &HomeWidget::changePage);
    }

    ui->stackedWidget->setCurrentWidget(m_companyinfoview);

    m_optLogView = new OptLogView();
    connect(ui->pushButtonOptLog,&QPushButton::clicked,this,&HomeWidget::handleOptLogBtnClicked);
}

HomeWidget::~HomeWidget()
{
    delete ui;
    if (m_loginWidget != nullptr) {
        delete m_loginWidget;
    }
}

void HomeWidget::handleSendSocketData(const QJsonObject &body)
{
    m_client->connectToHost("192.168.1.5", 10086);
    qDebug() << "-----------------";
    if (m_client->waitForConnected()) {
        // json对象转为字符串
        QJsonDocument doc(body);
        QByteArray bytes = doc.toJson(QJsonDocument::Compact);
        Head head;
        head.type = LOGIN_REQ;
        head.length = sizeof(Head) + bytes.size();
        char *request = new char[head.length];
        struct Data *data = (struct Data *)request;
        data->head = head;
        memcpy(data->body, bytes.data(), bytes.size());
        qint64 writeSize = m_client->write(request, head.length);
        qDebug() << "发送的请求类型:" << head.type;
        qDebug() << "发送的请求字节大小:" << head.length;
        qDebug() << "发送的请求的body:" << bytes;
        qDebug() << "实际发送的字节大小:" << writeSize;
    } else {
        qDebug() << "连接失败" << endl;
    }
}

void HomeWidget::handleReadyRead()
{
    Head head;
    int readSize = 0;
    // 读取请求头部信息
    int headSize = m_client->read((char *)&head, sizeof(Head));
    readSize += headSize;
    qDebug() << "读取的头部大小" << headSize << " " << readSize;
    int bodySize = head.length - sizeof(Head); // 4097
    char* body = new char[bodySize + 1]; // 总的
    body[bodySize] = '\0';
    int unitSize = m_client->read(body, bodySize);
    readSize += unitSize;
    qDebug()  << "实际读取的大小" << unitSize << " " << readSize << endl;

    qDebug() << "收到的响应类型:" << head.type << endl;
    qDebug() << "收到的响应字节大小:" << head.length << endl;
    qDebug() << "收到的响应的body:" << body << endl;
    qDebug() << "实际收到的字节大小:" << readSize << endl;
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(body, &error);
    if (error.error != QJsonParseError::NoError) {
        qDebug() << "解析失败";
    } else {
        QJsonObject obj = doc.object();
        QString code = obj["code"].toString();
        if (code == "200") {
            emit loginRes(true);
            m_loginWidget->hide();
            this->show();
        } else {
            m_loginWidget->show();
            m_loginWidget->setStatus(obj["message"].toString());
        }
    }
}

void HomeWidget::changePage()
{
    QPushButton *senderButton = qobject_cast<QPushButton*>(sender());
    if (senderButton) {
        QWidget *targetPage = pageMap.value(senderButton, nullptr);
        if (targetPage) {
            ui->stackedWidget->setCurrentWidget(targetPage);
        }
    }
}


void HomeWidget::handleOptLogBtnClicked()
{
    this->hide();

    m_optLogView->setGeometry(0,0,1000,800);
    QRect screenRect = QApplication::desktop()->screenGeometry();
    int centerX = (screenRect.width() - m_optLogView->width()) / 2;
    int centerY = (screenRect.height() - m_optLogView->height()) / 2;
    m_optLogView->move(centerX, centerY);

    m_optLogView->show();
    if(!m_optLogView->isVisible()){
        this->show();
    }
}

