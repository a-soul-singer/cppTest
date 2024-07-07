#ifndef SYSTEMWARNING_H
#define SYSTEMWARNING_H

#include <QWidget>

namespace Ui {
class SystemWarning;
}

class SystemWarning : public QWidget
{
    Q_OBJECT

public:
    explicit SystemWarning(QWidget *parent = nullptr);
    ~SystemWarning();

private:
    Ui::SystemWarning *ui;
};

#endif // SYSTEMWARNING_H
