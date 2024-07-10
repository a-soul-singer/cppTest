#ifndef REGISTER_H
#define REGISTER_H

#include <QWidget>
#include <QJsonObject>



namespace Ui {
class Register;
}

class Register : public QWidget
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);
    ~Register();

    void setStatus(const QString& message);

signals:
    void sendSocketData(int type, QJsonObject& body);

private slots:
    void handleConfirmbtnClicked();
    void handleCancelbtnClicked();


private:
    Ui::Register *ui;
};

#endif // REGISTER_H
