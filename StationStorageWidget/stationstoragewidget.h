#ifndef STATIONSTORAGEWIDGET_H
#define STATIONSTORAGEWIDGET_H

#include <QWidget>
#include "MyStation/mystation.h"
#include "GDData/gddata.h"
#include "stationstoragewidgetgd.h"
#include <QSpacerItem>

/**/
namespace Ui {
class StationStorageWidget;
}

class StationStorageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StationStorageWidget(QWidget *parent = nullptr);
    ~StationStorageWidget();    
     MyStation* pCurStation = nullptr;
     QVector<StationStorageWidgetGD*>mVectStationStorageWidgetGD;
    //占线版股道数组
   // QVector<GDData*>mGDDataArray;
     //设置对应车站ID
     void SetStationID(int nID) { nStationID = nID; }
     //获取对应车站ID
     int GetStationID() { return  nStationID; }
    //获取进路序列窗口对应车站指针
    MyStation* GetRouteOrderWidgetStation();
    void AddStationStorageData(MyStation* pStation);
    //站场重置
    void ResetStation();

signals:
    void SendAntiSlipSignal(GDData* pGDData,bool bSet, int nstationid);
    void UpDataStationStorage();
    void SendTextAnalysisSignal(int index, QPoint pt, QString dev = "");

private:
    Ui::StationStorageWidget *ui;
    int nStationID=-1;

};

#endif // STATIONSTORAGEWIDGET_H
