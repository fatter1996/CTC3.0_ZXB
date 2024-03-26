#ifndef SHUNTROUTEWIDGET_H
#define SHUNTROUTEWIDGET_H

#include <QWidget>
#include "MyStation/mystation.h"

namespace Ui {
class ShuntRouteWidget;
}

class ShuntRouteWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ShuntRouteWidget(QWidget *parent = nullptr);
    ~ShuntRouteWidget();

    //设置对应车站ID
    void SetStationID(int nID) { nStationID = nID; }
    //获取对应车站ID
    int GetStationID() { return  nStationID; }

private:
    Ui::ShuntRouteWidget *ui;
    MyStation* pCurStation = nullptr;
    int nStationID=-1;
};

#endif // SHUNTROUTEWIDGET_H
