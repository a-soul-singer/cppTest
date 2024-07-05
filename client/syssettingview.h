#ifndef SYSSETTINGVIEW_H
#define SYSSETTINGVIEW_H

#include <QWidget>

namespace Ui {
class SysSettingView;
}

class SysSettingView : public QWidget
{
    Q_OBJECT

public:
    explicit SysSettingView(QWidget *parent = nullptr);
    ~SysSettingView();

private:
    Ui::SysSettingView *ui;
};

#endif // SYSSETTINGVIEW_H
