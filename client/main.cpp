#include "homewidget.h"

#include <QWidget>
#include <QApplication>
#include <QDesktopWidget>

void setCenterWin(HomeWidget& win)
{
    // 实现设置主界面的大小，然后设置主界面居中显示
    win.setGeometry(0, 0, 1400, 800);
    QRect screenRect = QApplication::desktop()->screenGeometry();
    int centerX = (screenRect.width() - win.width()) / 2;
    int centerY = (screenRect.height() - win.height()) / 2;
    win.move(centerX, centerY);
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    HomeWidget win;
    setCenterWin(win);
     win.show();
    return app.exec();
}
