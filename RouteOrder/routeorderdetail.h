#ifndef ROUTEORDERDETAIL_H
#define ROUTEORDERDETAIL_H

#include <QWidget>
#include "MyStation/mystation.h"

namespace Ui {
class RouteOrderDetail;
}

class RouteOrderDetail : public QWidget
{
    Q_OBJECT

public:
    explicit RouteOrderDetail(QWidget *parent = nullptr);
    ~RouteOrderDetail();
    void RouteOrderDataInit(MyStation* pStation, TrainRouteOrder* pOrder);

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::RouteOrderDetail *ui;
};

#endif // ROUTEORDERDETAIL_H
