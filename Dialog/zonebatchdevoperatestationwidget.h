#ifndef ZONEBATCHDEVOPERATESTATIONWIDGET_H
#define ZONEBATCHDEVOPERATESTATIONWIDGET_H

#include <QWidget>
//#include <QPaintEvent>
#include "GlobalHeaders/Global.h"
#include "GlobalHeaders/GlobalFuntion.h"
#include "GlobalHeaders/GlobalStructure.h"
#include "MyStation/mystation.h"
#include <QMutex>

#define  DIPLOID  0.6

namespace Ui {
class ZoneBatchDevOperateStationWidget;
}

class ZoneBatchDevOperateStationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ZoneBatchDevOperateStationWidget(QWidget *parent = nullptr);
    ~ZoneBatchDevOperateStationWidget();
     MyStation* pMyStation=nullptr;

     QRect SelectRect;
     QMenu *pMenu = nullptr;
     void Init(MyStation* myStation);
     void InitMenu();
     //重绘界面
     void magicTime();
     //初始化选中区域起始点坐标
     void InitRectPoint();
     //获取选中区域
     QRect CountRectFun();
     QRect CountRectFun(QPoint pointEnd);
     void SelectDevFun();
     void GetSelectDev(QRect Rect, QVector<DeviceInfo>* vecDevData);
     //选中区域绘制
     void BaseRectDraw(QPainter *painter, int LineType,QColor LineColor,QColor BlackColor, QRect rect);
signals:
     void SendSelectDevsignal(QVector<DeviceInfo> vecDevData);
public slots:
     void ClearSelectFun();
     void setZCRefresh();
private:
    Ui::ZoneBatchDevOperateStationWidget *ui;

    int nElapsed=0;
    float nDiploid=0;
    QPoint pointStart=QPoint(-1,-1);
    QPoint pointEnd=QPoint(-1,-1);
    QPoint pointEnd2=QPoint(-1,-1);
    bool  SelectFlag=false;
    bool bDCNameShowFlag=true;
    bool bXHNameShowFlag=true;
    bool bQDNameShowFlag=true;
    virtual void timerEvent(QTimerEvent *event) override;//定时器
    //virtual void paintEvent(QPaintEvent *e) override;
    virtual bool eventFilter(QObject *obj,QEvent *event) override;
    //鼠标按下事件
    virtual void mousePressEvent(QMouseEvent * event)override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    //virtual void mouseMoveEvent(QMouseEvent *event)override;
    void DevNameShowOrHide(QAction *action);
};

#endif // ZONEBATCHDEVOPERATESTATIONWIDGET_H
