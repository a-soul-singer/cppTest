#include "syssettingview.h"
#include "ui_syssettingview.h"
#include <QFile>

SysSettingView::SysSettingView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SysSettingView)
{
    ui->setupUi(this);

    QStringList crr={"黑色","白色","灰色"};
    ui->comboBoxSysSetting->addItems(crr);
    ui->comboBoxSysSetting->setCurrentIndex(-1);
    ui->spinBox->setRange(10, 50);
    ui->spinBox->setValue(20);

    m_map={{"文艺体","../client/qss/FontWy.qss"},
           {"佛系体","../client/qss/FontFx.qss"},
           {"好身体","../client/qss/FontSt.qss"},
           {"10","../client/qss/FontSize10.qss"},
           {"15","../client/qss/FontSize15.qss"},
           {"20","../client/qss/FontSize20.qss"},
           {"黑色","../client/qss/SysSettingBlack.qss"},
           {"白色","../client/qss/SysSettingWhite.qss"},
           {"灰色","../client/qss/SysSettingGray.qss"},
          };

    connect(ui->comboBoxSysSetting, &QComboBox::currentTextChanged, this, &SysSettingView::setStylesheetFromFile);
    connect(ui->fontComboBox, &QComboBox::currentTextChanged, this, &SysSettingView::setFontFamily);
    connect(ui->spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &SysSettingView::setFontSize);
    // 从数据库中读取当前的主题的设置
//    setStylesheetFromFile("灰色", false);
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
