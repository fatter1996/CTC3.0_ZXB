#ifndef ROUTEORDERWIDGET_H
#define ROUTEORDERWIDGET_H

#include <QWidget>
#include "TableView/tableview.h"
#include "trainrouteorderwidget.h"
#include "trainrouteorderwidget.h"
#include "ZXBWindow/permissionswidget.h"
#include "TextAnalysis/TextAnalysis.h"

namespace Ui {
class RouteOrderWidget;
}

class RouteOrderWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RouteOrderWidget(QWidget *parent = nullptr);
    ~RouteOrderWidget();

    void AddRouteOrderData(MyStation* pStation);
    //获取进路序列窗口对应车站指针
    MyStation* GetRouteOrderWidgetStation();
    void RouteOrderTableUpData();
    TrainRouteOrderWidget* GetTrainRouteOrderWidget();
    //设置对应车站ID
    void SetStationID(int nID) { nStationID=nID; }
    //获取对应车站ID
    int GetStationID() { return  nStationID; }
    //更新进路序列
    //void UpDataRouteWidgetLable();
protected:
    void timerEvent(QTimerEvent* enent);
signals:
    void SetLableSignal(bool bPermission);
signals:
    void SendTextAnalysisSignal(int index, QPoint pt, QString dev = "");

public slots:
    //更新进路序列
    void onTrainRouteOrderUpData(int type, int route_id);
    //修改申请（刷新）
    void onApplyForPermission();
    //只读申请（释放权限）
    void onReleasePermission();

    //上下行
    void onDirectionDisplay(int type);
    //接车口
    void onArriveDisplay(int code);
    //发车口
    void onDepartureDisplay(int code);

private:
    Ui::RouteOrderWidget *ui;
    TrainRouteOrderWidget* pTrainRouteOrder = nullptr;
    PermissionsWidget* pPermissionsWidget = nullptr;
    MyStation* pCurStation = nullptr;
    int Timer_ID_500 = -1;
    int Timer_ID_500_2 = -1;
    int curPermission = 0;
    bool curFCZKMode = false;
    int curModeState = 0;
    int nStationID=-1;//进路序列对应车站ID
    TextAnalysis* pTextAnalysis = nullptr;
};

#endif // ROUTEORDERWIDGET_H
