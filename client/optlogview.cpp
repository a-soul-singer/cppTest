#include "optlogview.h"
#include "ui_optlogview.h"

#include "socket_data.h"

OptLogView::OptLogView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OptLogView)
{
    ui->setupUi(this);
    m_comboxText = {
        {"15条/页", 15},
        {"20条/页", 20},
        {"50条/页", 50},
    };
    QStringList labels = {"序号", "操作类型", "动作", "操作者", "开始时间", "结束时间", "详情"};
    ui->tableWidget->setColumnCount(labels.size());
    ui->tableWidget->setHorizontalHeaderLabels(labels);
    // TODO 设置表头的格式，列宽，列如何伸缩, 禁止单元格编辑，内容显示方向
}

OptLogView::~OptLogView()
{
    delete ui;
}

void OptLogView::changeWindowRequest()
{
    QJsonObject obj;
    obj["each_page_cnt"] = m_comboxText[ui->comboBox->currentText()];
    obj["curr_page"] = ui->spinBox->value() == 0 ? 1 : ui->spinBox->value();
    emit sendSocketData(OPT_LOG_REQ, obj);
}

void OptLogView::handleResponse(const QJsonObject& obj)
{
    ui->tableWidget->clearContents();
    // 解析obj
    QString code = obj["code"].toString();
    QJsonObject data = obj["data"].toObject();
    if (code == "200") {
        int totalPage = data["total_page"].toInt();
        ui->spinBox->setRange(1, totalPage);
        int currPage = data["curr_page"].toInt();
        ui->spinBox->setValue(currPage);
        QJsonArray logs = data["logs"].toArray();
        ui->tableWidget->setRowCount(logs.size());
        for (int i = 0; i < logs.size(); ++i) {
            QJsonArray log = logs[i].toArray();
            for (int j = 0; j < log.size(); ++j) {
                QTableWidgetItem* item = new QTableWidgetItem(log[j].toString());
                ui->tableWidget->setItem(i, j, item);
            }
        }
    } else {

    }
}
