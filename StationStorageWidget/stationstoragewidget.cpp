#include "stationstoragewidget.h"
#include "ui_stationstoragewidget.h"

StationStorageWidget::StationStorageWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StationStorageWidget)
{
    ui->setupUi(this);
}

StationStorageWidget::~StationStorageWidget()
{
    delete ui;
}

MyStation* StationStorageWidget::GetRouteOrderWidgetStation()
{
   return pCurStation;
}
void StationStorageWidget::AddStationStorageData(MyStation* pStation)
{
    //pStation
    pCurStation=pStation;
    QLayout * pGDlayout = ui->scrollAreaWidgetContents->layout();
    for(int i=0;i<pCurStation->m_ArrayGDData.size();i++)
    {
        StationStorageWidgetGD* pStationStorageWidgetGD= new StationStorageWidgetGD(ui->scrollAreaWidgetContents);
        pStationStorageWidgetGD->Init();
        pGDlayout->addWidget(pStationStorageWidgetGD);
        pStationStorageWidgetGD->SetWidgetBk(i%2);
        pStationStorageWidgetGD->SetStationID(pCurStation->getStationID(), pCurStation->strAntislip, pCurStation->bSleepers);
        pStationStorageWidgetGD->InitGD(pCurStation->m_ArrayGDData[i]);
        connect(pStationStorageWidgetGD, &StationStorageWidgetGD::SendAntiSlipSignal, this, [=](GDData* pGDData,bool bSet, int nstationid){
            int a;
            a=0;
            emit SendAntiSlipSignal(pGDData, bSet, nstationid);
        });
        connect(this, &StationStorageWidget::UpDataStationStorage, pStationStorageWidgetGD, &StationStorageWidgetGD::onUpDataStationStorage);
        mVectStationStorageWidgetGD.append(pStationStorageWidgetGD);

        connect(pStationStorageWidgetGD, &StationStorageWidgetGD::SendTextAnalysisSignal,[=](int index, QPoint pt, QString dev){
            emit this->SendTextAnalysisSignal(index, /*mapFromGlobal*/(pt), dev);
        });
    }
    QSpacerItem* space=new QSpacerItem(0,0,QSizePolicy::Maximum,QSizePolicy::Expanding);
    pGDlayout->addItem(space);
}
//站场重置
void StationStorageWidget::ResetStation()
{
    for(int i=0;i<mVectStationStorageWidgetGD.size();i++)
    {
        mVectStationStorageWidgetGD[i]->pGDData->Init();
        mVectStationStorageWidgetGD[i]->InitGD(mVectStationStorageWidgetGD[i]->pGDData);
        mVectStationStorageWidgetGD[i]->SetBtnInco();
        mVectStationStorageWidgetGD[i]->mlab->setText("");
    }
}
