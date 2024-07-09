#include "usermanageview.h"
#include "ui_usermanageview.h"

#include <QLabel>
#include <QSpacerItem>
#include <QHBoxLayout>
#include <QMap>
#include <QFileDialog>

#include "xlsxdocument.h"

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


    tableWidget->setColumnCount(4);

    QStringList headrs;
    headrs << "ID" << "用户名" << "密码" << "登录时间";
    tableWidget->setHorizontalHeaderLabels(headrs);

    tableWidget->setColumnWidth(0, 50);
    tableWidget->setColumnWidth(1, 200);
    tableWidget->setColumnWidth(2, 200);
    tableWidget->setColumnWidth(3, 300);

    map<int, vector<QString>> rowData={
        {1, {"1","admin", "admin123", "2024-07-09 21:00"}},
        {2, {"2","root","root","2024-07-09 21:10"}}
    };
    for (int row = 3; row < 10; ++row) {
        QString id = QString::number(row);
        QString username = "user" + id;
        QString password = "password" + id;
        QString loginTime = "2024-07-09 10:00" + QString::number(row);
        rowData[row] = {id, username, password, loginTime};
    }

    tableWidget->setRowCount(rowData.size()); // 设置表格行数为 rowData 的大小

    int row = 0;
    for (const auto &pair : rowData) {
        const auto &rowData = pair.second;
        for (int col = 0; col < rowData.size(); ++col) {
            QTableWidgetItem *item = new QTableWidgetItem(rowData[col]);
            item->setTextAlignment(Qt::AlignCenter);//居中显示
            tableWidget->setItem(row, col, item);
        }
        ++row;
    }

    connect(queryButton, &QPushButton::clicked, this, &UserManageView::handQueryButtonClicked);
    connect(exportButton, &QPushButton::clicked,this, &UserManageView::handExportButtonClicked);
}

UserManageView::~UserManageView()
{
    //delete ui;
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
