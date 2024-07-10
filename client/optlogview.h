#ifndef OPTLOGVIEW_H
#define OPTLOGVIEW_H

#include <QJsonObject>
#include <QJsonArray>
#include <QMap>
#include <QWidget>

namespace Ui {
class OptLogView;
}

class OptLogView : public QWidget
{
    Q_OBJECT

public:
    explicit OptLogView(QWidget *parent = nullptr);
    ~OptLogView();

    void changeWindowRequest();
    void handleResponse(const QJsonObject& obj);

signals:
    void sendSocketData(int type, QJsonObject& obj);

private:
    Ui::OptLogView *ui;
    QMap<QString, int> m_comboxText;
};

#endif // OPTLOGVIEW_H
