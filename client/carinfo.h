#ifndef CARINFO_H
#define CARINFO_H

#include <QWidget>

namespace Ui {
class Carinfo;
}

class Carinfo : public QWidget
{
    Q_OBJECT

public:
    explicit Carinfo(QWidget *parent = nullptr);
    ~Carinfo();

private:
    Ui::Carinfo *ui;
};

#endif // CARINFO_H
