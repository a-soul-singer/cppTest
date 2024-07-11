#include "homewidget.h"
#include "ui_homewidget.h"


#include <QDebug>
#include <QPushButton>
#include <QSqlQuery>

#include "socket_data.h"

HomeWidget::HomeWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HomeWidget)
{
    ui->setupUi(this);
    m_sqliteCls = SqliteClient::getInstance();

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
    m_electronicinterface = new Electronicinterface();

    // 将不同的界面上的sendSocketData的信号，绑定主界面的handleSendSocketData这个槽
    // 然后在这个槽对发送的请求，进行统一处理
    connect(m_loginWidget, &LoginWidget::sendSocketData, this, &HomeWidget::handleSendSocketData);
    connect(m_optlogview, &OptLogView::sendSocketData, this, &HomeWidget::handleSendSocketData);
    connect(m_usermanageview, &UserManageView::sendSocketData, this, &HomeWidget::handleSendSocketData);

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
    buttonList.append(ui->pushButtonInterface);


    pageMap.insert(ui->pushButtonCompanyInfo, m_companyinfoview);
    pageMap.insert(ui->pushButtonOptLog, m_optlogview);
    pageMap.insert(ui->pushButtonSysSetting, m_syssettingview);
    pageMap.insert(ui->pushButtonUserManage, m_usermanageview);
    pageMap.insert(ui->pushButtonStuffManage, m_stuffmanage);
    pageMap.insert(ui->pushButtonCarInfo, m_carinfo);
    pageMap.insert(ui->pushButtonClientManage, m_clientmanage);
    pageMap.insert(ui->pushButtonFix, m_fixrecord);
    pageMap.insert(ui->pushButtonwarning, m_systemwarning);
    pageMap.insert(ui->pushButtonInterface,m_electronicinterface);

    ui->stackedWidget->addWidget(m_companyinfoview);
    ui->stackedWidget->addWidget(m_optlogview);
    ui->stackedWidget->addWidget(m_syssettingview);
    ui->stackedWidget->addWidget(m_usermanageview);
    ui->stackedWidget->addWidget(m_stuffmanage);
    ui->stackedWidget->addWidget(m_carinfo);
    ui->stackedWidget->addWidget(m_clientmanage);
    ui->stackedWidget->addWidget(m_fixrecord);
    ui->stackedWidget->addWidget(m_systemwarning);
    ui->stackedWidget->addWidget(m_electronicinterface);

    for (QPushButton *button : buttonList) {
        connect(button, &QPushButton::clicked, this, &HomeWidget::handleChangePage);
    }

    ui->stackedWidget->setCurrentWidget(m_companyinfoview);
}

HomeWidget::~HomeWidget()
{
    delete ui;
    if (m_loginWidget != nullptr) {
        delete m_loginWidget;
    }
}

void HomeWidget::handleSendSocketData(int type, QJsonObject &body)
{
    if (type == LOGIN_REQ) {
        // 通过查询数据库中的数据，判断host和port是否存在
        auto res = m_sqliteCls->execQuerySqlLimitOne("select * from t_server limit 1;");
        if (!res.isEmpty()) {
            m_host = res[1].toString();
            m_port = res[2].toString();
        } else {
            m_loginWidget->setStatus("请设置服务端信息");
            return;
        }
        m_client->connectToHost(m_host, m_port.toUShort());
        qDebug() << "-----------------";
        if (m_client->waitForConnected()) {
            m_currUser = body["username"].toString();
            // json对象转为字符串
            QJsonDocument doc(body);
            QByteArray bytes = doc.toJson(QJsonDocument::Compact);
            Head head;
            head.type = type;
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
            m_loginWidget->setStatus("连接失败");
        }
    } else if (type == OPT_LOG_REQ){
        body["username"] = m_currUser;
        // json对象转为字符串
        QJsonDocument doc(body);
        QByteArray bytes = doc.toJson(QJsonDocument::Compact);
        Head head;
        head.type = type;
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
    }else if (type == USERS_REQ){
        // json对象转为字符串
        QJsonDocument doc(body);
        QByteArray bytes = doc.toJson(QJsonDocument::Compact);
        Head head;
        head.type = type;
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
    }else{

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
        if (head.type == LOGIN_RES) {
            if (code == "200") {
                emit loginRes(true);
                m_loginWidget->hide();
                this->show();
            } else {
                m_loginWidget->show();
                m_loginWidget->setStatus(obj["message"].toString());
            }
        } else if (head.type == OPT_LOG_RES) {
            m_optlogview->handleResponse(obj);
        } else if(head.type ==USERS_RES){
            m_usermanageview->handleUserResponse(obj);
        }
    }
}

void HomeWidget::handleChangePage()
{
    QPushButton *senderButton = qobject_cast<QPushButton*>(sender());
    if (senderButton) {
        QWidget *targetPage = pageMap[senderButton];
        if (targetPage == m_optlogview) { // 当前是操作日志界面
            // 向后端发送请求
            m_optlogview->changeWindowRequest();
        }else if(targetPage == m_usermanageview){//当前是用户管理界面
            m_usermanageview->changeWindowRequest();
        }
        ui->stackedWidget->setCurrentWidget(targetPage);
    }
}

void HomeWidget::on_pushButtonExitLogin_clicked()
{
    this->hide();
    m_loginWidget->show();
}
