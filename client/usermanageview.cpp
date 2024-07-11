#include "usermanageview.h"
#include "ui_usermanageview.h"

#include <QLabel>
#include <QSpacerItem>
#include <QHBoxLayout>
#include <QMap>
#include <QDebug>
#include <QFileDialog>
#include <QHeaderView>
#include <QTableWidgetItem>

#include "xlsxdocument.h"
#include "socket_data.h"

using namespace std;

UserManageView::UserManageView(QWidget *parent) :
    QWidget(parent)
    //ui(new Ui::UserManageView)
{
    //ui->setupUi(this);

    QLabel *label = new QLabel("用户名");
    lineEdit = new QLineEdit();

    lineEdit->setPlaceholderText("请输入需要查询的用户名");
    queryButton = new QPushButton("查询");
    QSpacerItem *horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    exportButton = new QPushButton("导出");

    QHBoxLayout * horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(label);
    horizontalLayout->addWidget(lineEdit);
    horizontalLayout->addWidget(queryButton);
    horizontalLayout->addItem(horizontalSpacer);
    horizontalLayout->addWidget(exportButton);

    tableWidget = new QTableWidget;
    QVBoxLayout *verticalLayout = new QVBoxLayout();
    verticalLayout->addLayout(horizontalLayout);
    verticalLayout->addWidget(tableWidget);
    this->setLayout(verticalLayout);


    tableWidget->setColumnCount(3);

    QStringList headrs;
    headrs << "ID" << "用户名" << "密码";
    tableWidget->setHorizontalHeaderLabels(headrs);

    tableWidget->verticalHeader()->setVisible(false);
    tableWidget->setColumnWidth(0, 50);
    tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);


    connect(queryButton, &QPushButton::clicked, this, &UserManageView::handQueryButtonClicked);
    connect(exportButton, &QPushButton::clicked,this, &UserManageView::handExportButtonClicked);
}

UserManageView::~UserManageView()
{
    //delete ui;
}

QLineEdit* UserManageView::getLineEdit()
{
    return lineEdit;
}

void UserManageView::changeWindowRequest()
{
    QJsonObject obj;
    obj["action"] = "query_users";
    emit sendSocketData(USERS_REQ, obj);
}

void UserManageView::handleUserResponse(const QJsonObject &obj)
{
    tableWidget->clearContents();
    // 解析obj
    QString code = obj["code"].toString();
    QJsonObject data = obj["data"].toObject();
    if (code == "200") {
        QJsonArray users = data["users"].toArray();
        tableWidget->setRowCount(users.size());
        for (int i = 0; i < users.size(); ++i) {
            QJsonArray log = users[i].toArray();
            for (int j = 0; j < log.size(); ++j) {
                QTableWidgetItem* item = new QTableWidgetItem(log[j].toString());
                tableWidget->setItem(i, j, item);
                item->setTextAlignment(Qt::AlignCenter);
            }
        }
    } else {

    }
}


void UserManageView::handQueryButtonClicked()
{
    QString res = lineEdit->text();// 获取输入框中的文本

    if (res.isEmpty()) {
        // 如果搜索文本为空，则显示所有行
        for (int row = 0; row < tableWidget->rowCount(); ++row) {
            tableWidget->setRowHidden(row, false);
        }
    } else {
        // 否则，根据用户名进行过滤
        for (int row = 0; row < tableWidget->rowCount(); ++row) {
            QTableWidgetItem *item = tableWidget->item(row, 1); // 获取用户名所在的列（索引为1）
            if (item && item->text().contains(res, Qt::CaseInsensitive)) {
                tableWidget->setRowHidden(row, false); // 显示匹配的行
            } else {
                tableWidget->setRowHidden(row, true); // 隐藏不匹配的行
            }
        }

    }
}

//导出
void UserManageView::handExportButtonClicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("导出 .xlsx 文件"), "", tr("Excel 文件 (*.xlsx)")); // 打开 .xlsx 文件保存对话框
    if (!fileName.isEmpty()) {
        QXlsx::Document xlsx;
        // 写入表头
        QStringList headers;
        for (int col = 0; col <tableWidget->columnCount(); ++col) {
            headers.append(tableWidget->horizontalHeaderItem(col)->text());
            xlsx.write(1, col + 1, tableWidget->horizontalHeaderItem(col)->text());
        }
        // 写入表格数据
        for (int row = 0; row <tableWidget->rowCount(); ++row) {
            for (int col = 0; col < tableWidget->columnCount(); ++col) {
                QTableWidgetItem *item = tableWidget->item(row, col);
                if (item) {
                    xlsx.write(row + 2, col + 1, item->text());
                }
            }
        }
        // 保存文件
        xlsx.saveAs(fileName);
    }
}
