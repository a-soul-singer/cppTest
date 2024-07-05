#ifndef OPTLOGVIEW_H
#define OPTLOGVIEW_H

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

private:
    Ui::OptLogView *ui;
};

#endif // OPTLOGVIEW_H
