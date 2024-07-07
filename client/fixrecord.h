#ifndef FIXRECORD_H
#define FIXRECORD_H

#include <QWidget>

namespace Ui {
class FixRecord;
}

class FixRecord : public QWidget
{
    Q_OBJECT

public:
    explicit FixRecord(QWidget *parent = nullptr);
    ~FixRecord();

private:
    Ui::FixRecord *ui;
};

#endif // FIXRECORD_H
