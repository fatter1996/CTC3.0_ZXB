#ifndef STATIONSTORAGEWIDGETGD_H
#define STATIONSTORAGEWIDGETGD_H

#include <QWidget>
#include <QMenu>
#include "GDData/gddata.h"
#include "antislipwidget.h"
#include "Dialog/dialog_zchmanage.h"
#include "stationstoragesetdialog.h"
#include "TrackView/tracklabel.h"

namespace Ui {
class StationStorageWidgetGD;
}

class StationStorageWidgetGD : public QWidget
{
    Q_OBJECT

public:
    explicit StationStorageWidgetGD(QWidget *parent = nullptr);
    ~StationStorageWidgetGD();
    void Init(bool bFLView = true);
    void InitMenu(bool bHavePlan = false);
    void InitGD(GDData* pGDData);

    //设置上下行按钮图标
    void SetBtnInco();
    //设置背景颜色
    void SetWidgetBk(bool b);
    //设置对应车站ID
    void SetStationID(int nID, QString strAntislip, int bSleepers) {
        nStationID = nID;
        m_strAntislip = strAntislip;
        m_bSleepers = bSleepers;
    }
    //获取对应车站ID
    int GetStationID() { return  nStationID; }
    //设置股道存车信息显示
    void SetGDStationStorageShow(bool b,QString str);
    //防溜设置窗口
    AntiSlipWidget* pAntiSlipWidget=nullptr;
    //站存车设置窗口
    StationStorageSetDialog* pStationStorageSetDialog=nullptr;

    void InsertTrackLabel(TrackLabel* pTrackLabel);
    void InsertStationName(QString strName);

    TrackLabel* takeTrackLabel();
    void ClearTrackLabel();

    void UpDataTrainEnterTrack();

    QString getGDName() { return pGDData->pGD->getName(); }
    bool IsHaveTrainMsg() { return pGDData->pGD->m_sTrainInfoShow != ""; }
    TrackLabel* GetTrackLabel() { return m_pTrackLabel; }
    QLabel* mlab=nullptr;
    GDData* pGDData=nullptr;
private:
    virtual void timerEvent(QTimerEvent *event);

private slots:
    //槽-左侧防溜设置按钮
    void on_pushButton_2_pressed();
    //槽-右侧防溜设置
    void on_pushButton_3_pressed();
public slots:
    //槽-刷新股道防溜界面显示
    void onUpDataStationStorage();
    //槽-设置站存车
    void ZCCGLOper_Fun();
    //槽-股道防溜设置
    void RecAntiSlipSolt(GDData* pGDData,bool bSet,int nstationid);

signals:
    void SendAntiSlipSignal(GDData* pGDData,bool bSet, int nstationid);
    void FinishWorkFlow();
    void StationStorageChange();
    void SendTextAnalysisSignal(int index, QPoint pt, QString dev = "");

private:
    Ui::StationStorageWidgetGD *ui;

    QString oldTrainInfoShow;
    int nStationID = -1;
    QMenu *m_menu;

    TrackLabel* m_pTrackLabel = nullptr;
    int Timer_ID_500 = 0;

    QString m_strAntislip;
    bool m_bSleepers = true;
};

#endif // STATIONSTORAGEWIDGETGD_H
