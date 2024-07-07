#ifndef CLIENTMANAGE_H
#define CLIENTMANAGE_H

#include <QWidget>

namespace Ui {
class ClientManage;
}

class ClientManage : public QWidget
{
    Q_OBJECT

public:
    explicit ClientManage(QWidget *parent = nullptr);
    ~ClientManage();

private:
    Ui::ClientManage *ui;
};

#endif // CLIENTMANAGE_H
