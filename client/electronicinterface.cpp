#include "electronicinterface.h"
#include "ui_electronicinterface.h"

Electronicinterface::Electronicinterface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Electronicinterface)
{
    ui->setupUi(this);


    //第一个
    QVBoxLayout *layout = new QVBoxLayout;

    QHBoxLayout *horizontalLayout = new QHBoxLayout;



    this->setStyleSheet("background-color: lightcyan");

    QPieSeries *ElectronicSignage1 = new QPieSeries();
    ElectronicSignage1->append("Man", 60);
    ElectronicSignage1->append("Woman", 40);

    QChart *pieChart1 = new QChart();
    pieChart1->addSeries(ElectronicSignage1);
    pieChart1->setTitle("男女比例");

    QChartView *pieChart1View = new QChartView(pieChart1);
    pieChart1View->setRenderHint(QPainter::Antialiasing);
    pieChart1View->setMinimumSize(100,100);
    pieChart1View->setStyleSheet("background-color: lightgree");
    horizontalLayout->addWidget(pieChart1View);



    // 创建第二个饼状图
    QPieSeries *pieSeries2 = new QPieSeries();
    pieSeries2->append("id", 33);
    pieSeries2->append("Usename", 33);
    pieSeries2->append("password", 34);

    QChart *pieChart2 = new QChart();
    pieChart2->addSeries(pieSeries2);
    pieChart2->setTitle("peopleMassage");

    QChartView *pieChart2View = new QChartView(pieChart2);
    pieChart2View->setRenderHint(QPainter::Antialiasing); // 设置抗锯齿渲染
    pieChart2View->setMinimumSize(100, 100); // 设置最小大小
    pieChart2View->setStyleSheet("background-color: lightblue");
    horizontalLayout->addWidget(pieChart2View);



    //创建柱状图

    m_stu = {
        {1,"Alice",24,42,45,41},
        {2,"Taylor",34,24,58,42},
        {3,"Cidy",34,15,6,37},
        {4,"Jenny",33,5,45,21},
    };

    QBarSet *setBenz = new QBarSet("Benz");
    QBarSet *setAodi = new QBarSet("Aodi");
    QBarSet *setBMW = new QBarSet("BMW");
    QBarSet *setPorsche = new QBarSet("Porsche");
    QStringList catMassage;
    for(int i = 0;i<m_stu.size();++i){
        setBenz->append(m_stu[i].Banz);
        setAodi->append(m_stu[i].Aodi);
        setBMW->append(m_stu[i].BMW);
        setPorsche->append(m_stu[i].Porsche);
        catMassage.append(m_stu[i].name);
    }

    QBarSeries *serise = new QBarSeries();
    serise->append(setBenz);
    serise->append(setAodi);
    serise->append(setBMW);
    serise->append(setPorsche);
    serise->setLabelsVisible(true);
    serise->setLabelsPosition(QAbstractBarSeries::LabelsOutsideEnd);


    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(catMassage);
    axisX->setRange(catMassage.at(0),catMassage.at(catMassage.count()-1));

    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0,100);
    axisY->setTitleText("数量");
    axisY->setTickCount(6);
    axisY->setLabelFormat("%.0f");

    QChart *chart = new QChart();
    chart->addSeries(serise);
    chart->addSeries(serise);
    chart->setTitle("柱状图");
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->addAxis(axisX,Qt::AlignBottom);
    serise->attachAxis(axisY);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->setTheme(QChart::ChartThemeDark);

    //    QList<QChartView*> m_barChartView;
    //    QChartView *chartView = new QChartView(this);
    //    chartView->setRenderHint(QPainter::Antialiasing,true);
    //    chartView->setChart(chart);
    //    m_barChartView.append(chartView);


    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing, true);
    layout->addLayout(horizontalLayout);
    layout->addWidget(chartView);
    setLayout(layout);



}


Electronicinterface::~Electronicinterface()
{
    delete ui;
}
