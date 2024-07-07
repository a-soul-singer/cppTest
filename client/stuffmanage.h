#ifndef STUFFMANAGE_H
#define STUFFMANAGE_H

#include <QWidget>

namespace Ui {
class StuffManage;
}

class StuffManage : public QWidget
{
    Q_OBJECT

public:
    explicit StuffManage(QWidget *parent = nullptr);
    ~StuffManage();

private:
    Ui::StuffManage *ui;
};

#endif // STUFFMANAGE_H
