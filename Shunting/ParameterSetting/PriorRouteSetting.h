#ifndef PRIORROUTESETTING_H
#define PRIORROUTESETTING_H

#include <QWidget>

namespace Ui {
class PriorRouteSetting;
}

class PriorRouteSetting : public QWidget
{
    Q_OBJECT

public:
    explicit PriorRouteSetting(QWidget *parent = nullptr);
    ~PriorRouteSetting();

private:
    Ui::PriorRouteSetting *ui;
};

#endif // PRIORROUTESETTING_H
