#ifndef ZXBROUTEWIDGET_H
#define ZXBROUTEWIDGET_H

#include <QWidget>
#include "GlobalHeaders/Global.h"
#include "GlobalHeaders/GlobalFuntion.h"
#include "GlobalHeaders/GlobalStructure.h"
#include "RouteOrder/routeorderwidget.h"
#include <QBoxLayout>
#include <QResizeEvent>
namespace Ui {
class ZXBRouteWidget;
}

class ZXBRouteWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ZXBRouteWidget(QWidget *parent = nullptr);
    ~ZXBRouteWidget();

    void SetWindowCon(ZXBWindowCon* mZXBWindowCon,QVector<MyStation*>mMyStationArray);
    //根据车站获取对应窗口指针
    RouteOrderWidget* GetRouteOrderWidgetByStationID(int nID);

signals:
    void SendTextAnalysisSignal(int index, QPoint pt, QString dev = "");

private:
    virtual void resizeEvent(QResizeEvent *event) override;

public:
    QLayout* playout = nullptr;
    QTabWidget* tabWidget = nullptr;
    //进路序列窗口指针
    QVector<RouteOrderWidget*> pVectRouteOrderWidget;
private:
    Ui::ZXBRouteWidget *ui;
    ZXBWindowCon* pZXBWindowCon=nullptr;

};

#endif // ZXBROUTEWIDGET_H
