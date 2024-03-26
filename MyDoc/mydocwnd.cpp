#include "mydoc.h"
#include <QtDebug>
#include <QSettings>
#include <QMessageBox>
#include <QMenuBar>
#include <QToolBar>
#include <QLayout>
#include "GlobalHeaders/GlobalFuntion.h"

//设置中文
#pragma execution_character_set("utf-8")

//初始化主窗口信息
//void MyDoc::InitMainWnd(QMainWindow *widget)
void MyDoc::InitMainWnd(MainWindow *widget)
{
    //系统窗口类型-1卡斯柯，2卡斯柯集控，3铁科，4通号...
    pMainWindow = widget;

    //供电臂设置
     connect(pMainWindow, SIGNAL(PowerSupplyArmDialogShowSignal()), this, SLOT(PowerSupplyArmDialogShowSolt()));
     connect(pPowerSupplyArmDialog, SIGNAL(SendPowerSupplyArmDataToSeverSignal(int,QString,bool)), this, SLOT(SendPowerSupplyArmDataToSeverSolt(int,QString,bool)));
     //封锁设备操作
     connect(pMainWindow, SIGNAL(BlockingDevDialogShowSignal()), this, SLOT(BlockingDevDialogShowSolt()));
     connect(pBlockingDevDialog, SIGNAL(SendBlockingDevDataToSeverSignal(int,QList<int>,bool)), this, SLOT(SendBlockingDevDataToSeverSolt(int,QList<int>,bool)));
    //批量操作
    connect(widget, &MainWindow::SendZoneBatchDevOperateToSeverSignal, this, &MyDoc::SendZoneBatchDevOperateToSeverSolt);


    pAgainstWrongWidget = new AgainstWrongWidget();
    connect(pAgainstWrongWidget, SIGNAL(sendAgainstWrongMsg(int, int, int)),this,SLOT(SendAgainstWrong(int, int, int)));
    //站场重置
     connect(this, SIGNAL(ResetStationSignal()), pMainWindow, SLOT(ResetMemorandumDialog()));

     connect(pMainWindow, &MainWindow::SendTextAnalysisSignal,[=](int index, QPoint pt, QString dev){
         pTextAnalysis->sendTextAnalysisMsg(index, pZXBMainWindow->mapFromGlobal(pt), dev);
     });
     connect(pAgainstWrongWidget, &AgainstWrongWidget::SendTextAnalysisSignal,[=](int index, QPoint pt, QString dev){
         pTextAnalysis->sendTextAnalysisMsg(index, pZXBMainWindow->mapFromGlobal(pt), dev);
     });

}

//初始化占线版主界面窗口
void MyDoc::InitZXBMainWnd(ZXBWindow *pZXBWindow)
{
    pZXBMainWindow = pZXBWindow;
    InitTrackView(pZXBMainWindow->pTrackWidget);
    InitZXBRoutWnd(pZXBMainWindow->pRouteWidget);
    InitZXBFLWnd(pZXBMainWindow->pFLWidget);

    pTextAnalysis =new TextAnalysis();
    pTextAnalysis->TextAnalysisInfoInit();
    connect(pTextAnalysis, SIGNAL(TextFramePacket(TextFrame)), this, SLOT(SendTextAnalysis(TextFrame)));

//    connect(pZXBMainWindow, &ZXBWindow::SendTextAnalysisSignal,[=](int index, QPoint pt, QString dev){
//        pTextAnalysis->sendTextAnalysisMsg(index, pt, dev);
//    });

}
//股道视图初始化
void MyDoc::InitTrackView(ZXBTrackWidget* pZXBTrackWidget)
{
    //向股道视图各窗口传递对应车站指针 绑定对应车站信号槽
    for(int i = 0; i < mZXBWindowCon.nGDView; i++)
    {
        int nStationID = mZXBWindowCon.nGDViewStationID[i];
        //车站指针
        MyStation* pStation = getMyStationByStaIDInStaArray(nStationID);
        if(pZXBTrackWidget != nullptr && pStation != nullptr)
        {
            TrackViewWidget* pTrackViewWidget = pZXBTrackWidget->GetTrackViewWidgetByStationID(pStation->getStationID());
            if(pTrackViewWidget)
            {
                pTrackViewWidget->Init(pStation, mZXBWindowCon.nViewType, mZXBWindowCon.nMaxRow, mZXBWindowCon.bGDHV && mZXBWindowCon.nGDView > 1);
                connect(pTrackViewWidget, &TrackViewWidget::sendRouteOrderMsg, this, &MyDoc::SendRouteOrder);
                //行车日志更新
                connect(pStation, &MyStation::TrafficLogUpData, pTrackViewWidget, &TrackViewWidget::onTrafficLogUpData);
                //进路序列更新
                connect(pStation, &MyStation::TrainRouteOrderUpData, pTrackViewWidget, &TrackViewWidget::onTrainRouteOrderUpData);
                //股道防溜视图更新
                connect(pStation, &MyStation::StationStorageUpData, pTrackViewWidget, &TrackViewWidget::onGDSignalUpData);
                connect(pTrackViewWidget, &TrackViewWidget::SendAntiSlipSignal, this, &MyDoc::SendAntiSlipSolt);

                //发送行车日志相关命令
                connect(pTrackViewWidget, &TrackViewWidget::sendTrafficLogMsg, this, &MyDoc::SendTrafficLog);
                connect(pTrackViewWidget, &TrackViewWidget::updataTrafficLog, this, &MyDoc::SendTrafficLogUpdata);
                connect(pTrackViewWidget, &TrackViewWidget::updataTrafficLogFlow, this, &MyDoc::SendTrafficLogWorkFlow);
                connect(pTrackViewWidget, &TrackViewWidget::SendTextAnalysisSignal,[=](int index, QPoint pt, QString dev){
                    pTextAnalysis->sendTextAnalysisMsg(index, pZXBMainWindow->mapFromGlobal(pt), dev);
                });
            }
        }
    }
}
//初始化占线版进路序列窗口信息
void MyDoc::InitZXBRoutWnd(ZXBRouteWidget *pZXBRouteWidget)
{
    //向进路序列各窗口传递对应车站指针 绑定对应车站信号槽
    for(int i=0;i<mZXBWindowCon.nJLView;i++)
    {
        int nStationID=mZXBWindowCon.nRouteViewStationID[i];
        //车站指针
        MyStation* pStation=getMyStationByStaIDInStaArray(nStationID);
        //初始化占线版进路序列窗口
        if(pZXBRouteWidget!=nullptr&&pStation!=nullptr)
        {
            RouteOrderWidget* pRouteOrder=pZXBRouteWidget->GetRouteOrderWidgetByStationID(pStation->getStationID());
            pRouteOrder->AddRouteOrderData(pStation);

            connect(pRouteOrder->GetTrainRouteOrderWidget(),SIGNAL(sendRouteOrderMsg(MyStation*, int, int, int, QString)),this,SLOT(SendRouteOrder(MyStation*, int, int, int, QString)));
            //进路序列更新
            connect(pStation, &MyStation::TrainRouteOrderUpData, pRouteOrder, &RouteOrderWidget::onTrainRouteOrderUpData);

            connect(pRouteOrder, &RouteOrderWidget::SendTextAnalysisSignal,[=](int index, QPoint pt, QString dev){
                pTextAnalysis->sendTextAnalysisMsg(index, pZXBMainWindow->mapFromGlobal(pt), dev);
            });
            connect(pZXBRouteWidget, &ZXBRouteWidget::SendTextAnalysisSignal,[=](int index, QPoint pt, QString dev){
                pTextAnalysis->sendTextAnalysisMsg(index, pZXBMainWindow->mapFromGlobal(pt), dev);
            });
        }
    }
}
//初始化占线版防溜视图
void MyDoc::InitZXBFLWnd(ZXBFLWidget* pZXBFLWidget)
{
    //初始化占线版防溜 绑定对应车站信号槽
    for(int i=0;i<mZXBWindowCon.nFLView;i++)
    {
        //车站指针
        MyStation* pStation = getMyStationByStaIDInStaArray(mZXBWindowCon.nFLViewStationID[i]);
        //初始化占线版进路序列窗口

        if(pZXBFLWidget && pStation)
        {
            StationStorageWidget* pStationStorageWidget = pZXBFLWidget->GetStationStorageWidgetByStationID(pStation->getStationID());
            pStationStorageWidget->AddStationStorageData(pStation);
            connect(pStation, &MyStation::StationStorageUpData, pZXBFLWidget, [=](){
                emit pStationStorageWidget->UpDataStationStorage();
            });

            //站场重置
            connect(this, SIGNAL(ResetStationSignal()), pZXBFLWidget, SLOT(ResetStationSolt()));
        }
    }
    connect(pZXBFLWidget, SIGNAL(SendAntiSlipSignal(GDData*,bool,int)), this, SLOT(SendAntiSlipSolt(GDData*,bool,int)));
    connect(pZXBFLWidget, &ZXBFLWidget::SendTextAnalysisSignal,[=](int index, QPoint pt, QString dev){
        pTextAnalysis->sendTextAnalysisMsg(index, pZXBMainWindow->mapFromGlobal(pt), dev);
    });

}
//初始化供电臂设置窗口视图
void MyDoc::InitPowerSupplyArmDialog()
{
   pPowerSupplyArmDialog->Init(vectMyStation);
}
//初始化封锁设备窗口视图
void MyDoc::InitBlockingDevDialog()
{
    pBlockingDevDialog->Init(vectMyStation);
}

//槽-供电臂设置
void MyDoc::PowerSupplyArmDialogShowSolt()
{
    pPowerSupplyArmDialog->UpdatePowerData();
    pPowerSupplyArmDialog->show();
}
//槽-封锁设备操作
void MyDoc::BlockingDevDialogShowSolt()
{
    pBlockingDevDialog->UpdatePowerData();
    pBlockingDevDialog->show();
}


