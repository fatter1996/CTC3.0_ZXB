#include "zxbshuntwidget.h"
#include "ui_zxbshuntwidget.h"

ZXBShuntWidget::ZXBShuntWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZXBShuntWidget)
{
    ui->setupUi(this);
    playout = this->layout();
}

ZXBShuntWidget::~ZXBShuntWidget()
{
    delete ui;
}

void ZXBShuntWidget::SetWindowCon(ZXBWindowCon* mZXBWindowCon, QVector<MyStation*>mMyStationArray)
{
    pZXBWindowCon = mZXBWindowCon;
    //创建视图
    if(pZXBWindowCon->bDCHV)
    {
        for(int i=0;i<pZXBWindowCon->nDCView;i++)
        {
            ShuntRouteWidget * pShuntRouteWidget = new ShuntRouteWidget();
            pShuntRouteWidget->SetStationID(pZXBWindowCon->nShuntViewStationID[i]);
            playout->addWidget(pShuntRouteWidget);
            pVectShuntRouteWidget.append(pShuntRouteWidget);
        }
    }
    else
    {
        tabWidget = new QTabWidget();
        QFont font;
        font.setFamily("微软雅黑");
        font.setPointSize(10);
        tabWidget->setFont(font);
        tabWidget->setTabPosition(QTabWidget::South);
        tabWidget->setDocumentMode(false);
        tabWidget->setStyleSheet("QTabWidget::pane { border: none; }");
        tabWidget->tabBar()->setStyleSheet("QTabBar::tab{"
                                               "background-color: rgb(220, 220, 220);"
                                               "color: gray;"
                                           "}"
                                           "QTabBar::tab:selected {"
                                               "background-color: rgb(250, 250, 250);"
                                               "color: black;"
                                           "}");
        playout->addWidget(tabWidget);
        for(int i=0; i<pZXBWindowCon->nDCView; i++)
        {
             ShuntRouteWidget* pShuntRouteWidget = new ShuntRouteWidget();
             pShuntRouteWidget->SetStationID(pZXBWindowCon->nShuntViewStationID[i]);
             QString strstationname="";
             for(int j=0;j<mMyStationArray.size();j++)
             {
                 if(pZXBWindowCon->nShuntViewStationID[i]==mMyStationArray[j]->getStationID())
                 {
                     strstationname=mMyStationArray[j]->getStationName();
                 }
             }
             tabWidget->addTab(pShuntRouteWidget, strstationname + "调车作业-全部机车");

             //connect(pShuntRouteWidget, &StationStorageWidget::SendAntiSlipSignal, this, [=](GDData* pGDData,bool bSet, int nstationid){
             //   emit SendAntiSlipSignal(pGDData, bSet, nstationid);
             //});
             pVectShuntRouteWidget.append(pShuntRouteWidget);
        }
    }
}

//根据车站获取对应窗口指针
ShuntRouteWidget* ZXBShuntWidget::GetShuntRouteWidgetByStationID(int nID)
{
    for(int i=0;i<pVectShuntRouteWidget.size();i++)
    {
        if(pVectShuntRouteWidget[i]->GetStationID()==nID)
        {
            return pVectShuntRouteWidget[i];
        }
    }
    return nullptr;
}

void ZXBShuntWidget::resizeEvent(QResizeEvent *event)
{
    if(tabWidget!=nullptr)
    {
        tabWidget->tabBar()->setFixedWidth(tabWidget->width());
    }
    return QWidget::resizeEvent(event);
}
