#ifndef ZXBFLWIDGET_H
#define ZXBFLWIDGET_H

#include <QWidget>
#include "GlobalHeaders/Global.h"
#include "GlobalHeaders/GlobalFuntion.h"
#include "GlobalHeaders/GlobalStructure.h"
#include <QBoxLayout>
#include <QResizeEvent>
#include "StationStorageWidget/stationstoragewidget.h"
namespace Ui {
class ZXBFLWidget;
}

class ZXBFLWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ZXBFLWidget(QWidget *parent = nullptr);
    ~ZXBFLWidget();

    QLayout* playout=nullptr;
    QTabWidget* tabWidget=nullptr;
    //防溜与站存车窗口指针
    QVector<StationStorageWidget*> pVectStationStorageWidget;
   void SetWindowCon(ZXBWindowCon* mZXBWindowCon,QVector<MyStation*>mMyStationArray);
   //根据车站获取对应窗口指针
   StationStorageWidget* GetStationStorageWidgetByStationID(int nID);

signals:
    void SendAntiSlipSignal(GDData* pGDData,bool bSet, int nstationid);
    //void UpDataStationStorageWidgetGDSignal();
    void SendTextAnalysisSignal(int index, QPoint pt, QString dev = "");
public slots:
    void ResetStationSolt();//站场重置
private:
    Ui::ZXBFLWidget *ui;
    ZXBWindowCon* pZXBWindowCon=nullptr;
    virtual void resizeEvent(QResizeEvent *event) override;
};

#endif // ZXBFLWIDGET_H
