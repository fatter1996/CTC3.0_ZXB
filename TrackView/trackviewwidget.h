#ifndef TRACKVIEWWIDGET_H
#define TRACKVIEWWIDGET_H

#include <QWidget>
#include <QVector>
#include "ZXBWindow/permissionswidget.h"
#include "StationStorageWidget/stationstoragewidgetgd.h"
#include "tracklabel.h"
#include "RouteOrder/trainrouteorderwidget.h"
#include <QSpacerItem>
namespace Ui {
class TrackViewWidget;
}

class TrackViewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TrackViewWidget(QWidget *parent = nullptr);
    ~TrackViewWidget();

    void Init(MyStation* pStation, int type = 1, int maxRow = 3, bool bMuit = false);
    void Insert(TrackLabel* pTrackLabel, bool bUpData = false);
    void TrackLabelClear();
    void TrackLabelDelete(TrackLabel* pDelTrackLabel);
    void TrackLabelRefresh();

    QVector<TrackLabel*> takeAllTrackLabel();
    TrackLabel* takeTrackLabelByTrafficLog(int logId);

    bool TrafficLogFilter(TrafficLog* pTrafficLog);
    void UpdataTrack();

    TrackLabel* GetTrackLabelByTrainNum(QString strTrainNum);

    int getMinimumWidth();
    //设置对应车站ID
    void SetStationID(int nID) { nStationID = nID; }
    //获取对应车站ID
    int GetStationID() { return  nStationID; }

    TrainRouteOrderWidget* GetRouteOrderWidget() { return RouteOrderWidget; }
protected:
    void timerEvent(QTimerEvent* enent);

    struct TrackLabelInfo {
        int nType = 1;
        int nOrderNum = 0;
        bool bColor = false;
        GDData* pGDData = nullptr;
        StationStorageWidgetGD* pStorageWidget = nullptr;
        QVector<StationStorageWidgetGD*> m_vecSubStorage;
        QVector<TrackLabel*> vecSubTrackLabel;
    };

    MyStation* GetMyStation() { return m_pMyStation; }

signals:
    void SetLableSignal(bool bPermission);
    void sendRouteOrderMsg(MyStation* pMyStation, int type, int routeId, int nData = 0, QString strData = "");

    void sendTrafficLogMsg(int stationId, int logId, int type, QDateTime date);
    void updataTrafficLog(int stationId, int type, TrafficLog pTrafficLog);
    void updataTrafficLogFlow(int stationId, int logId, int type, int flag, QDateTime date);
    void SendTextAnalysisSignal(int index, QPoint pt, QString dev = "");

public slots:
    //更新股道视图
    void onTrafficLogUpData(int type, int logId);
    //更新进路序列
    void onTrainRouteOrderUpData(int type, int route_id);
    //股道防溜视图更新
    void onGDSignalUpData();

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

signals:
    void SendAntiSlipSignal(GDData* pGDData,bool bSet, int nstationid);
    void UpDataStationStorage();

private:
    Ui::TrackViewWidget *ui;

    QWidget* TrackWidget = nullptr;
    TrainRouteOrderWidget* RouteOrderWidget = nullptr;
    QSpacerItem* pSpacer = nullptr;
    bool bShowType = true;

    int nStationID = -1;
    int m_nType = 1;
    int m_nMaxRow = 3;
    bool m_bMuit = false;
    bool bColor = false;
    int nIndex = 1;
    MyStation* m_pMyStation = nullptr;
    PermissionsWidget* m_pPermissionsWidget = nullptr;
    QVector<TrackLabelInfo> m_vecTrackLabelInfo;
    QVector<TrackLabel*> m_vecTrackLabel;
    TrackLabel* m_vecTrackLabelTemp[3] = { nullptr,nullptr,nullptr };

    int curPermission = 0;
    bool curFCZKMode = true;
    int curModeState = 0;

    int Timer_ID_500 = 0;
    int Timer_ID_500_2 = 0;
    int Timer_ID_5000 = 0;

    int nDirection = 0; //上下行
    int nJCXHDCode = 0; //接车口
    int nFCXHDCode = 0;  //发车口
};

#endif // TRACKVIEWWIDGET_H
