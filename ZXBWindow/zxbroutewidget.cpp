#include "zxbroutewidget.h"
#include "ui_zxbroutewidget.h"

ZXBRouteWidget::ZXBRouteWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZXBRouteWidget)
{
    ui->setupUi(this);
    playout = this->layout();
}

ZXBRouteWidget::~ZXBRouteWidget()
{
    delete ui;
}
void ZXBRouteWidget::SetWindowCon(ZXBWindowCon* mZXBWindowCon,QVector<MyStation*>mMyStationArray)
{
    pZXBWindowCon=mZXBWindowCon;
    //创建视图
    if(pZXBWindowCon->bJLHV)
    {
        for(int i=0;i<pZXBWindowCon->nJLView;i++)
        {
            RouteOrderWidget * mRouteOrderWidget=new RouteOrderWidget(this);
            mRouteOrderWidget->SetStationID(pZXBWindowCon->nRouteViewStationID[i]);
            playout->addWidget(mRouteOrderWidget);
            pVectRouteOrderWidget.append(mRouteOrderWidget);
        }
    }
    else
    {
        tabWidget = new QTabWidget(this);
        QFont font;
        font.setFamily("微软雅黑");
        font.setPointSize(10);
        tabWidget->setFont(font);
        tabWidget->setTabPosition(QTabWidget::South);
        //tabWidget->setDocumentMode(false);
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
        QString strstationname;
        for(int i=0;i<pZXBWindowCon->nJLView;i++)
        {
             RouteOrderWidget* mRouteOrderWidget = new RouteOrderWidget;
             mRouteOrderWidget->SetStationID(pZXBWindowCon->nRouteViewStationID[i]);

             for(int j=0;j<mMyStationArray.size();j++)
             {
                 if(pZXBWindowCon->nRouteViewStationID[i]==mMyStationArray[j]->getStationID())
                 {
                     strstationname=mMyStationArray[j]->getStationName();
                 }
             }
             tabWidget->addTab(mRouteOrderWidget, strstationname + "-列车进路序列");
             pVectRouteOrderWidget.append(mRouteOrderWidget);
             connect(mRouteOrderWidget, &RouteOrderWidget::SendTextAnalysisSignal,[=](int index, QPoint pt, QString dev){
                 emit this->SendTextAnalysisSignal(index, /*mapFromGlobal*/(pt), dev);
             });
        }
    }
}
//根据车站获取对应窗口指针
RouteOrderWidget* ZXBRouteWidget::GetRouteOrderWidgetByStationID(int nID)
{
    for(int i=0;i<pVectRouteOrderWidget.size();i++)
    {
        if(pVectRouteOrderWidget[i]->GetStationID()==nID)
            return pVectRouteOrderWidget[i];
    }
    return nullptr;
}
void ZXBRouteWidget::resizeEvent(QResizeEvent *event)
{
    if(tabWidget)
        tabWidget->tabBar()->setFixedWidth(tabWidget->width());

    return QWidget::resizeEvent(event);
}
