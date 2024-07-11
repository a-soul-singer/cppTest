#include "syssettingview.h"
#include "ui_syssettingview.h"
#include <QFile>

SysSettingView::SysSettingView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SysSettingView)
{
    ui->setupUi(this);

    QStringList crr={"蓝色妖姬","红色风暴","绿色毒刺","粉色回忆","黑色沉默","灰色轨迹","银色杀手","金色传说"};
    ui->comboBoxSysSetting->addItems(crr);
    ui->comboBoxSysSetting->setCurrentIndex(-1);
    ui->spinBox->setRange(10, 50);
    ui->spinBox->setValue(20);

    m_map = {
        {"蓝色妖姬","../client/qss/SysSettingBlue.qss"},
        {"红色风暴","../client/qss/SysSettingRed.qss"},
        {"绿色毒刺","../client/qss/SysSettingGreen.qss"},
        {"粉色回忆","../client/qss/SysSettingPink.qss"},
        {"黑色沉默","../client/qss/SysSettingBlack.qss"},
        {"灰色轨迹","../client/qss/SysSettingGray.qss"},
        {"银色杀手","../client/qss/SysSettingSilver.qss"},
        {"金色传说","../client/qss/SysSettingGilden.qss"},
    };

    connect(ui->comboBoxSysSetting, &QComboBox::currentTextChanged, this, &SysSettingView::setStylesheetFromFile);
    connect(ui->fontComboBox, &QComboBox::currentTextChanged, this, &SysSettingView::setFontFamily);
    connect(ui->spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &SysSettingView::setFontSize);
    // 从数据库中读取当前的主题的设置
    //    setStylesheetFromFile("灰色", false);


//    m_sqlcls = SqliteClient::getInstance();
//    QString sql="insert into t_style_setting(styleName) values('%1');";
//    if(!m_sqlcls->execOtherSql(sql.arg("黑色"))){
//        return;
//    }
//    sql="select styleName from t_style_setting";
//    auto res=m_sqlcls->execQuerySqlLimitOne(sql);
//    if(!res.isEmpty()){
//        ui->comboBoxSysSetting->setStyleSheet(res.front().toString());
//    }

    m_sqlcls = SqliteClient::getInstance();
    QString sql="insert into t_style_setting(styleName) values('%1');";

//    foreach (const QString &key, m_map.keys()) {
//        QString sql="insert into t_style_setting(styleName) values('%1');";
//        m_sqlcls->execOtherSql(sql.arg(ui->comboBoxSysSetting->currentText()));
//    }

    sql="select * from t_style_setting";
    auto res=m_sqlcls->execQuerySqlLimitOne(sql);
    if(!res.isEmpty()){
        ui->comboBoxSysSetting->setStyleSheet(res.front().toString());
    }
}

SysSettingView::~SysSettingView()
{
    delete ui;
}

void SysSettingView::setStylesheetFromFile(const QString &item)
{
    QFile file(m_map[item]);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }
    emit setStyleSheetSignal(file.readAll());
    file.close();
}

void SysSettingView::setFontFamily(const QString &font)
{
    emit setStyleSheetSignal(QString("*{font-family:%1;}").arg(font));
}

void SysSettingView::setFontSize(int fontSize)
{
    emit setStyleSheetSignal(QString("*{font-size:%1px;}").arg(fontSize));
}

void SysSettingView::on_comboBoxSysSetting_currentTextChanged(const QString &arg1)
{
    m_sqlcls = SqliteClient::getInstance();
    QString sql="TRUNCATE TABLE t_style_setting;";
    m_sqlcls->execOtherSql(sql);
    foreach (const QString &key, m_map.keys()) {
        QString sql="insert into t_style_setting(styleName) values('%1');";
        m_sqlcls->execOtherSql(sql.arg(ui->comboBoxSysSetting->currentText()));
    }
}

