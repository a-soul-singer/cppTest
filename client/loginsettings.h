#ifndef LOGINSETTINGS_H
#define LOGINSETTINGS_H

#include <QWidget>
#include <QSqlDatabase>

namespace Ui {
class LoginSettings;
}

class LoginSettings : public QWidget
{
    Q_OBJECT

public:
    explicit LoginSettings(QWidget *parent = nullptr);
    ~LoginSettings();
signals:
    void LogSetSignal(const QString& host, const QString& port);
private slots:
    void handleConfirmbtnClicked();
    void handleCancelbtnClicked();
private:
    Ui::LoginSettings *ui;
};

#endif // LOGINSETTINGS_H
