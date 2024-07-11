#ifndef SYSSETTINGVIEW_H
#define SYSSETTINGVIEW_H

#include <QWidget>
#include <QMap>

#include "sqliteclient.h"

namespace Ui {
class SysSettingView;
}

class SysSettingView : public QWidget
{
    Q_OBJECT

public:
    explicit SysSettingView(QWidget *parent = nullptr);
    ~SysSettingView();
private slots:
    void setStylesheetFromFile(const QString &item);
    void setFontFamily(const QString &fontFamily);
    void setFontSize(int fontSize);

    void on_comboBoxSysSetting_currentTextChanged(const QString &arg1);

signals:
    void setStyleSheetSignal(const QString& style);

private:
    Ui::SysSettingView *ui;
    QMap<QString,QString>m_map;
    SqliteClient* m_sqlcls;
};

#endif // SYSSETTINGVIEW_H
