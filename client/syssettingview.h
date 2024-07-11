#ifndef SYSSETTINGVIEW_H
#define SYSSETTINGVIEW_H

#include <QWidget>
#include <QMap>

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

signals:
    void setStyleSheetSignal(const QString& style);

private:
    Ui::SysSettingView *ui;
    QMap<QString,QString>m_map;
};

#endif // SYSSETTINGVIEW_H
