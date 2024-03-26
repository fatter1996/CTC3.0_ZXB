#include "routeorderwidget.h"
#include "ui_routeorderwidget.h"
#include <QPushButton>
#include <QMessageBox>
#pragma execution_character_set("utf-8")
RouteOrderWidget::RouteOrderWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RouteOrderWidget)
{
    ui->setupUi(this);
    pTrainRouteOrder = ui->widget;
    pPermissionsWidget = ui->tittleWidget;
    Timer_ID_500 = startTimer(500);
    pTrainRouteOrder->SetRouteOrderTableShowSet(0, 0);
    pTrainRouteOrder->SetRouteOrderTableShowSet(1, 0);
    pTrainRouteOrder->SetRouteOrderTableShowSet(2, 0);
}

RouteOrderWidget::~RouteOrderWidget()
{
    delete ui;
}

void RouteOrderWidget::AddRouteOrderData(MyStation* pStation)
{
    pCurStation = pStation;
    pPermissionsWidget->Init(pCurStation);
    pTrainRouteOrder->InitTableViewMenu(pCurStation);
    connect(pPermissionsWidget, &PermissionsWidget::applyForPermission, this, &RouteOrderWidget::onApplyForPermission);
    connect(pPermissionsWidget, &PermissionsWidget::releasePermission, this, &RouteOrderWidget::onReleasePermission);

    connect(pPermissionsWidget, &PermissionsWidget::DirectionDisplay, this, &RouteOrderWidget::onDirectionDisplay);
    connect(pPermissionsWidget, &PermissionsWidget::ArriveDisplay, this, &RouteOrderWidget::onArriveDisplay);
    connect(pPermissionsWidget, &PermissionsWidget::DepartureDisplay, this, &RouteOrderWidget::onDepartureDisplay);

    connect(this, &RouteOrderWidget::SetLableSignal,pPermissionsWidget, &PermissionsWidget::SetLableSolt);

//    pTextAnalysis =new TextAnalysis();
//    pTextAnalysis->TextAnalysisInfoInit();
//    connect(this, &RouteOrderWidget::SendTextAnalysisSignal,[=](int index, QPoint pt, QString dev){
//        pTextAnalysis->sendTextAnalysisMsg(index, mapFromGlobal(pt), dev);
//    });
    connect(pPermissionsWidget, &PermissionsWidget::SendTextAnalysisSignal,[=](int index, QPoint pt, QString dev){
        emit this->SendTextAnalysisSignal(index, /*mapFromGlobal*/(pt), dev);
    });
//    connect(pPermissionsWidget, &PermissionsWidget::SendTextAnalysisSignal,[=](int index, QPoint pt, QString dev){
//        emit this->SendTextAnalysisSignal(index, mapFromGlobal(pt), dev);
//    });
    connect(pTrainRouteOrder, &TrainRouteOrderWidget::SendTextAnalysisSignal,[=](int index, QPoint pt, QString dev){
        emit this->SendTextAnalysisSignal(index, /*mapFromGlobal*/(pt), dev);
    });
}
//获取进路序列窗口对应车站指针
MyStation* RouteOrderWidget::GetRouteOrderWidgetStation()
{
   return pCurStation;
}
void RouteOrderWidget::RouteOrderTableUpData()
{
    pTrainRouteOrder->RouteOrderUpData();
}

TrainRouteOrderWidget* RouteOrderWidget::GetTrainRouteOrderWidget()
{
    return pTrainRouteOrder;
}

void RouteOrderWidget::onTrainRouteOrderUpData(int type, int route_id)
{
    RouteOrderTableUpData();
}

void RouteOrderWidget::timerEvent(QTimerEvent* enent)
{
    if(enent->timerId() == Timer_ID_500)
    {
        if(pCurStation)
        {
            if(curPermission != pCurStation->m_nRoutePermission || curFCZKMode != pCurStation->m_nFCZKMode
                    || curModeState != pCurStation->m_nModeState)
            {
                RouteOrderTableUpData();
                curPermission = pCurStation->m_nRoutePermission;
                curFCZKMode = pCurStation->m_nFCZKMode;
                curModeState = pCurStation->m_nModeState;
                emit SetLableSignal(pCurStation->m_nRoutePermission == 2 && !pCurStation->m_nFCZKMode && pCurStation->m_nModeState == 1);
            }
        }
    }

    if(enent->timerId() == Timer_ID_500_2)
    {
        RouteOrderTableUpData();
        killTimer(Timer_ID_500_2);
    }
}

//修改申请（刷新）
void RouteOrderWidget::onApplyForPermission()
{
    if(pCurStation)
    {
        //申请权限
        emit ui->widget->sendRouteOrderMsg(pCurStation, MSGTYPE_PERMISSION, 0, 0xaa);
        ui->widget->ClearData();
        Timer_ID_500_2 = startTimer(500);
    }
}

//只读申请（释放权限）
void RouteOrderWidget::onReleasePermission()
{
    if(pCurStation && (pCurStation->m_nRoutePermission == 2))
    {
        //只读
        int ret = QMessageBox::information(nullptr, tr("注意-STPC/CTC3.0 通用车站操作终端"), tr("只读申请后,将无法对进路序列进行修改,确定要只读申请吗?"), tr("是"), tr("否"), "", 0);
        if(ret == 0)
        {
            emit ui->widget->sendRouteOrderMsg(pCurStation, MSGTYPE_PERMISSION, 0, 0xbb);
            RouteOrderTableUpData();
        }
    }
}

//上下行
void RouteOrderWidget::onDirectionDisplay(int type)
{
    pTrainRouteOrder->SetRouteOrderTableShowSet(0, type);
    RouteOrderTableUpData();
}
//接车口
void RouteOrderWidget::onArriveDisplay(int code)
{
    pTrainRouteOrder->SetRouteOrderTableShowSet(1, code);
    RouteOrderTableUpData();
}
//发车口
void RouteOrderWidget::onDepartureDisplay(int code)
{
    pTrainRouteOrder->SetRouteOrderTableShowSet(2, code);
    RouteOrderTableUpData();
}
