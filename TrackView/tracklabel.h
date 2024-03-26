#ifndef TRACKLABEL_H
#define TRACKLABEL_H

#include <QWidget>
#include "MyStation/mystation.h"
#include "routehandwidget.h"
#include "TextAnalysis/TextAnalysis.h"

#define WNDTYPE_PREVIEW     1  //预告
#define WNDTYPE_REPORT      2  //报点
#define WNDTYPE_WORK        3  //作业

#define PREVIEW_ARRIVE    1  //接车预告
#define PREVIEW_DEPART    2  //发车预告

#define REPORT_ARRIVE     1  //到达报点
#define REPORT_DEPART     2  //出发报点

namespace Ui {
class TrackLabel;
}

class TrackLabel : public QWidget
{
    Q_OBJECT

public:
    explicit TrackLabel(QWidget *parent = nullptr);
    ~TrackLabel();
    static TrackLabel* Create(MyStation* pStation, TrafficLog* pTrafficLog, int viewType);

    void InitProcBtn();
    void Init(MyStation* pStation, TrafficLog* pTrafficLog, int viewType);
    void InitBtnConnect();
    void InitWorkState(QPushButton* pPushBotton, int state);

    void SetWidgetBk(bool b);
    void UpdataBtnColor();
    void UpdataTrackView();
    void UpdataTrackIndex(int index = 0);
    void UpdataRouteOrder();
    void ShowWorkTaskWidget(int wndType, int workType);
    void ShowRouteHandWidget();

    bool eventFilter(QObject* o, QEvent* e);
    void SetAutoTrigger(TrainRouteOrder* pRouteOrder);
    void SetAutoTrigger_LeftButton(TrainRouteOrder* pRouteOrder);
    int getMinimumWidth();

    TrafficLog* GetTrafficLog() { return  m_pTrafficLog; }
    TrainRouteOrder* GetRouteOrderJC() { return  m_pRouteOrderJC; }
    TrainRouteOrder* GetRouteOrderFC() { return  m_pRouteOrderFC; }

signals:
    void sendTrafficLogReportingMsg(int logId, int type, QDateTime time); //发送行车日志报点命令
    void sendTrafficLogWorkMsg(int logId, int type, int flag, QDateTime time); //发送行车日志更新命令
    void sendRouteOrderMsg(int type, int routeId, int nData = 0, QString strData = "");
    void workFlowFinish(int time);
    void SendTextAnalysisSignal(int index, QPoint pt, QString dev = "");
private:
    Ui::TrackLabel *ui;
    MyStation* m_pMyStation = nullptr;
    TrafficLog* m_pTrafficLog = nullptr;
    TrainRouteOrder* m_pRouteOrderJC = nullptr;
    TrainRouteOrder* m_pRouteOrderFC = nullptr;

    TrainRouteOrder m_RouteOrderJCBackup;
    TrainRouteOrder m_RouteOrderFCBackup;
    //int index = 0;
    RouteHandWidget* pRouteHandWidget = nullptr;

    QString bkColorGray;
    QString bkColorGreen;
    QString bkColorRed;
    QString bkColorYellow;

    int nVewType = -1;
    int nWndType = -1;
    TextAnalysis* pTextAnalysis = nullptr;

};

#endif // TRACKLABEL_H
