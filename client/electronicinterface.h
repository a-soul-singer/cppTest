#ifndef ELECTRONICINTERFACE_H
#define ELECTRONICINTERFACE_H

#include <QWidget>
#include <QtCharts>
#include <QList>

struct PeopleMessage {
    int id;
    QString name;
    int Banz;
    int Aodi;
    int BMW;
    int Porsche;
};

namespace Ui {
class Electronicinterface;
}

class Electronicinterface : public QWidget
{
    Q_OBJECT

public:
    explicit Electronicinterface(QWidget *parent = nullptr);
    ~Electronicinterface();

private:
    Ui::Electronicinterface *ui;
    QChartView *pieChart1View;
    QChartView *pieChart2View;
    QChartView *m_barChartView;
    QList<PeopleMessage>m_stu;
};

#endif // ELECTRONICINTERFACE_H
