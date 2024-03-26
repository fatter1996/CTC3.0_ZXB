#include "zxbtrackwidget.h"
#include "ui_zxbtrackwidget.h"
#include <QScrollBar>
#include <QSpacerItem>

ZXBTrackWidget::ZXBTrackWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZXBTrackWidget)
{
    ui->setupUi(this);
    ui->scrollArea->horizontalScrollBar()->hide();
}

ZXBTrackWidget::~ZXBTrackWidget()
{
    delete ui;
}

void ZXBTrackWidget::SetWindowCon(ZXBWindowCon* mZXBWindowCon, QVector<MyStation*>mMyStationArray)
{
    pGDlayout = new QVBoxLayout();
    tabWidget = nullptr;
    QSpacerItem* pSpacer = nullptr;

    if(!mZXBWindowCon->bGDHV)
    {
        tabWidget = new QTabWidget;
        QFont font;
        font.setFamily("微软雅黑");
        font.setPointSize(12);
        font.setBold(true);
        tabWidget->setFont(font);
        tabWidget->setDocumentMode(false);
        //tabWidget->setStyleSheet("QTabWidget::pane { border: none; background-color: rgb(0, 241, 251);}");
        tabWidget->tabBar()->setStyleSheet("QTabBar::tab{"
                                               "background-color: rgb(220, 220, 220);"
                                               "font: bold 16px \"微软雅黑\";"
                                               "color: black;"
                                           "}"
                                           "QTabBar::tab:selected {"
                                               "background-color: rgb(250, 250, 250);"
                                               "font: bold 18px \"微软雅黑\";"
                                               "color: black;"
                                           "}");
        connect(tabWidget, &QTabWidget::currentChanged, this, [=](int index){
            //tabWidget->setFixedHeight(tabWidget->widget(index)->height());
            qDebug() << tabWidget->size() << tabWidget->widget(index)->size();
        });

        pGDlayout->addWidget(tabWidget);
    }

    //创建股道视图
    for(int i=0; i<mZXBWindowCon->nGDView; i++)
    {
        TrackViewWidget* pTrackView = new TrackViewWidget;
        pTrackView->SetStationID(mZXBWindowCon->nGDViewStationID[i]);
        connect(pTrackView, &TrackViewWidget::SendAntiSlipSignal, this, [=](GDData* pGDData,bool bSet, int nstationid){
           emit SendAntiSlipSignal(pGDData, bSet, nstationid);
        });
        m_vecTableView.append(pTrackView);

        if(mZXBWindowCon->bGDHV)
            pGDlayout->addWidget(pTrackView);
        else
        {
            pGDlayout->setSpacing(0);
            pGDlayout->setMargin(0);
            QString strstationname;
            for(int j=0;j<mMyStationArray.size();j++)
            {
                if(mZXBWindowCon->nRouteViewStationID[i]==mMyStationArray[j]->getStationID())
                {
                    strstationname=mMyStationArray[j]->getStationName();
                }
            }
            tabWidget->addTab(pTrackView, strstationname);
        }
    }

    if(mZXBWindowCon->bGDHV)
    {
        pGDlayout->setSpacing(4);
        pGDlayout->setMargin(0);
        pSpacer = new QSpacerItem(0, 860, QSizePolicy::Expanding, QSizePolicy::Expanding);
        pGDlayout->addSpacerItem(pSpacer);
    }
    ui->scrollAreaWidgetContents->setLayout(pGDlayout);
}

//根据车站获取对应窗口指针
TrackViewWidget* ZXBTrackWidget::GetTrackViewWidgetByStationID(int nID)
{
    for(int i=0;i<m_vecTableView.size();i++)
    {
        if(m_vecTableView[i]->GetStationID()==nID)
        {
            return m_vecTableView[i];
        }
    }
    return nullptr;
}

void ZXBTrackWidget::UpdataMinimumWidth()
{
    int maxMinimumWidth = 0;
    for(int i=0;i<m_vecTableView.size();i++)
    {
        maxMinimumWidth = (m_vecTableView.at(i)->getMinimumWidth() > maxMinimumWidth) ?
                            m_vecTableView.at(i)->getMinimumWidth() : maxMinimumWidth;
    }
    this->setMinimumWidth(maxMinimumWidth);
}

void ZXBTrackWidget::paintEvent(QPaintEvent *event)
{
    if(tabWidget!=nullptr)
    {
        tabWidget->tabBar()->setFixedWidth(tabWidget->width());
        //qDebug() << tabWidget->size();
    }
    return QWidget::paintEvent(event);
}
