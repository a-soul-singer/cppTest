#include "syssettingview.h"
#include "ui_syssettingview.h"
#include <QFile>

SysSettingView::SysSettingView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SysSettingView)
{
    ui->setupUi(this);


    QStringList arr={"文艺体","佛系体","好身体"};
    ui->comboBoxFont->addItems(arr);
    ui->comboBoxFont->setCurrentIndex(-1);

    QStringList brr={"10","15","20"};
    ui->comboBoxFontSize->addItems(brr);
    ui->comboBoxFontSize->setCurrentIndex(-1);

    QStringList crr={"黑色","白色","灰色"};
    ui->comboBoxSysSetting->addItems(crr);
    ui->comboBoxSysSetting->setCurrentIndex(-1);

    m_map={{"文艺体","../qss/FontWy.qss"},
           {"佛系体","../qss/FontFx.qss"},
           {"好身体","../qss/FontSt.qss"},
           {"10","../qss/FontSize10.qss"},
           {"15","../qss/FontSize15.qss"},
           {"20","../qss/FontSize20.qss"},
           {"黑色","../qss/SysSettingBlack.qss"},
           {"白色","../qss/SysSettingWhite.qss"},
           {"灰色","../qss/SysSettingGray.qss"},
          };

    connect(ui->comboBoxFont, &QComboBox::currentTextChanged, this, &SysSettingView::setStylesheetFromFile);
    connect(ui->comboBoxFontSize, &QComboBox::currentTextChanged, this, &SysSettingView::setStylesheetFromFile);
    connect(ui->comboBoxSysSetting, &QComboBox::currentTextChanged, this, &SysSettingView::setStylesheetFromFile);

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
    QString content=this->styleSheet();
    content += QLatin1String(file.readAll());
    this->setStyleSheet(content);
    file.close();

}
