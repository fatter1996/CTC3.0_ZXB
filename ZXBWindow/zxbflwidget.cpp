#include "zxbflwidget.h"
#include "ui_zxbflwidget.h"

ZXBFLWidget::ZXBFLWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZXBFLWidget)
{
    ui->setupUi(this);
    playout=this->layout();
    //this->showMaximized();
}

ZXBFLWidget::~ZXBFLWidget()
{
    delete ui;
}
void ZXBFLWidget::SetWindowCon(ZXBWindowCon* mZXBWindowCon, QVector<MyStation*>mMyStationArray)
{
    pZXBWindowCon=mZXBWindowCon;
    //创建视图
    if(pZXBWindowCon->bFLHV)
    {
        for(int i=0;i<pZXBWindowCon->nFLView;i++)
        {
            StationStorageWidget * mStationStorageWidget=new StationStorageWidget(this);

            mStationStorageWidget->SetStationID(pZXBWindowCon->nFLViewStationID[i]);
            playout->addWidget(mStationStorageWidget);
            connect(mStationStorageWidget, &StationStorageWidget::SendAntiSlipSignal, this, [=](GDData* pGDData,bool bSet, int nstationid){
               emit SendAntiSlipSignal(pGDData, bSet, nstationid);
            });
            connect(mStationStorageWidget, &StationStorageWidget::SendTextAnalysisSignal,[=](int index, QPoint pt, QString dev){
                emit this->SendTextAnalysisSignal(index, /*mapFromGlobal*/(pt), dev);
            });
            pVectStationStorageWidget.append(mStationStorageWidget);

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
        for(int i=0;i<pZXBWindowCon->nFLView;i++)
        {
             StationStorageWidget* mStationStorageWidget=new StationStorageWidget();
             mStationStorageWidget->SetStationID(pZXBWindowCon->nFLViewStationID[i]);
             QString strstationname="";
             for(int j=0;j<mMyStationArray.size();j++)
             {
                 if(pZXBWindowCon->nFLViewStationID[i]==mMyStationArray[j]->getStationID())
                 {
                     strstationname=mMyStationArray[j]->getStationName();
                 }
             }
             tabWidget->addTab(mStationStorageWidget, strstationname + "-防溜与站存车");

             connect(mStationStorageWidget, &StationStorageWidget::SendAntiSlipSignal, this, [=](GDData* pGDData,bool bSet, int nstationid){
                 emit SendAntiSlipSignal(pGDData, bSet, nstationid);
             });
             connect(mStationStorageWidget, &StationStorageWidget::SendTextAnalysisSignal,[=](int index, QPoint pt, QString dev){
                 emit this->SendTextAnalysisSignal(index, /*mapFromGlobal*/(pt), dev);
             });
             pVectStationStorageWidget.append(mStationStorageWidget);
        }
    }

}
//根据车站获取对应窗口指针
StationStorageWidget* ZXBFLWidget::GetStationStorageWidgetByStationID(int nID)
{
    for(int i=0;i<pVectStationStorageWidget.size();i++)
    {
        if(pVectStationStorageWidget[i]->GetStationID()==nID)
        {
            return pVectStationStorageWidget[i];
        }
    }
    return nullptr;
}
void ZXBFLWidget::ResetStationSolt()
{
    for(int i=0;i<pVectStationStorageWidget.size();i++)
    {
        pVectStationStorageWidget[i]->ResetStation();
    }
}
void ZXBFLWidget::resizeEvent(QResizeEvent *event)
{
    if(tabWidget!=nullptr)
    {
        tabWidget->tabBar()->setFixedWidth(tabWidget->width());
    }
    return QWidget::resizeEvent(event);
}
