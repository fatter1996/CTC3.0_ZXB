#include "stationstoragewidgetgd.h"
#include "ui_stationstoragewidgetgd.h"
#include <QMessageBox>

StationStorageWidgetGD::StationStorageWidgetGD(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StationStorageWidgetGD)
{
    ui->setupUi(this);

    Timer_ID_500 = startTimer(500);
}
StationStorageWidgetGD::~StationStorageWidgetGD()
{
    delete ui;
}
void StationStorageWidgetGD::Init(bool bFLView)
{
    if(bFLView)
       connect(ui->pushButton, &QPushButton::clicked, this, &StationStorageWidgetGD::ZCCGLOper_Fun);

    pAntiSlipWidget = new AntiSlipWidget();

    //绑定股道防溜界面与股道界面信号槽
    connect(pAntiSlipWidget, SIGNAL(SendAntiSlipSignal(GDData*,bool,int)), this, SLOT(RecAntiSlipSolt(GDData*,bool,int)));

    connect(pAntiSlipWidget, &AntiSlipWidget::SendTextAnalysisSignal,[=](int index, QPoint pt, QString dev){
        emit this->SendTextAnalysisSignal(index, /*mapFromGlobal*/(pt), dev);
    });

    pStationStorageSetDialog=new StationStorageSetDialog();
    connect(pStationStorageSetDialog, &StationStorageSetDialog::SendTextAnalysisSignal,[=](int index, QPoint pt, QString dev){
        emit this->SendTextAnalysisSignal(index, /*mapFromGlobal*/(pt), dev);
    });
    QFont mFont;
    mFont.setFamily("微软雅黑");
    mFont.setPointSize(12);//字号
    mFont.setBold(true);
    ui->pushButton->setFont(mFont);
    mlab=new QLabel();
    mlab->setAlignment(Qt::AlignCenter);
    mlab->setFont(mFont);
    ui->widget->layout()->addWidget(mlab);
}
void StationStorageWidgetGD::InitMenu(bool bHavePlan)
{
    m_menu = ui->pushButton->menu();
    if(m_menu)
    {
        delete m_menu;
        m_menu = nullptr;
    }
    m_menu = new QMenu(this);
    m_menu->setStyleSheet("QMenu{"
                              "background-color: rgb(229, 241, 251);"
                              "color: rgb(0, 0, 0);"
                              "margin: 2px;"
                          "}"
                          "QMenu::item{"
                              "padding: 2px 20px 2px 20px;"
                              "margin: 0px;"
                          "}"
                          "QMenu::item:selected {"
                              "color: rgb(0, 0, 0);"
                              "background-color: rgb(204, 228, 247);"
                          "}");

    QAction* pAction = new QAction("设置站存车", m_menu);//设置站存车
    m_menu->addAction(pAction);
    connect(pAction, &QAction::triggered, this, &StationStorageWidgetGD::ZCCGLOper_Fun);

    if(bHavePlan)
    {
        QAction* pAction1 = new QAction("结束作业流程", m_menu);//结束作业流程
        m_menu->addAction(pAction1);
        connect(pAction1, &QAction::triggered, this, [=](){
            emit SendTextAnalysisSignal(15, QCursor::pos());
            int ret = QMessageBox::information(nullptr, "注意-TDCS/CTC3.0", QString("确认结束吗?"), "是", "否", "", 0);
            if(ret == 0)
                emit FinishWorkFlow();
        });

        m_menu->addSeparator();

        QAction* pAction2 = new QAction("列车摘头安排", m_menu);//列车摘头安排
        m_menu->addAction(pAction2);

        QAction* pAction3 = new QAction("列车挂头安排", m_menu);//列车挂头安排
        m_menu->addAction(pAction3);
    }
    ui->pushButton->setMenu(m_menu);
}
void StationStorageWidgetGD::InitGD(GDData* mGDData)
{
    if(mGDData)
    {
        pGDData=mGDData;
        ui->pushButton->setText(pGDData->pGD->getName());
        ui->label->setText(pGDData->pGD->getName());
        ui->pushButton_2->setIconSize(QSize(ui->pushButton_2->size().width()-4,ui->pushButton_2->size().height()-4));
        ui->pushButton_3->setIconSize(QSize(ui->pushButton_3->size().width()-4,ui->pushButton_3->size().height()-4));
    }
    else
    {
        ui->pushButton->hide();
        ui->pushButton_2->hide();
        ui->pushButton_3->hide();
        ui->label->hide();
        QLabel* tempLabel1 = new QLabel;
        tempLabel1->setFixedSize(108, 32);
        tempLabel1->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
        QLabel* tempLabel2 = new QLabel;
        tempLabel2->setFixedSize(108, 32);
        tempLabel2->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
        QHBoxLayout* pLayout = (QHBoxLayout*)ui->widget_2->layout();
        pLayout->insertWidget(0, tempLabel1);
        pLayout->insertWidget(pLayout->indexOf(ui->widget) + 1, tempLabel2);
        //ui->widget->setGeometry(116, 0, ui->widget->width(), ui->widget->height());
    }
}

//设置上下行按钮图标
void StationStorageWidgetGD::SetBtnInco()
{
    QString stricon="";
    //左侧按钮
    {
        QString strBtnShow;
        if(pGDData->pGD->bIronShoesSetS)//铁鞋设置
        {
            if(pGDData->pGD->nManualBrakeS==1)//铁鞋+人力制动机
                stricon="border-image:url(://res/左边铁鞋+制动机.bmp);";
            else if(pGDData->pGD->nManualBrakeS==2)//铁鞋+人力制动机紧固器
                stricon="border-image:url(://res/左边铁鞋+紧固器.bmp);";
            else stricon="border-image:url(://res/左边铁鞋.bmp);";//铁鞋
            strBtnShow = pGDData->pGD->m_nLTxNum == 0 ? "" : QString::number(pGDData->pGD->m_nLTxNum);
        }
        else
        {
            if(pGDData->pGD->nManualBrakeS==1)//人力制动机
                stricon="border-image:url(://res/制动机.bmp);";
            else if(pGDData->pGD->nManualBrakeS==2)//人力制动机紧固器
                stricon="border-image:url(://res/紧固器.bmp);";
            else stricon="";
            strBtnShow = pGDData->pGD->m_nLJgqNum == 0 ? "" : QString::number(pGDData->pGD->m_nLJgqNum);
        }

        QLabel* imageLabel = nullptr;
        QVBoxLayout* topLayout = (QVBoxLayout*)ui->pushButton_2->layout();
        if(!topLayout)
        {
            imageLabel = new QLabel;
            imageLabel->setStyleSheet(stricon);
            imageLabel->setAlignment(Qt::AlignBottom|Qt::AlignHCenter);
            topLayout = new QVBoxLayout();
            topLayout->setMargin(4);
            topLayout->addWidget(imageLabel);
            ui->pushButton_2->setLayout(topLayout);
        }
        else
        {
            imageLabel = (QLabel*)topLayout->itemAt(0)->widget();
            imageLabel->setStyleSheet(stricon);
        }
        imageLabel->setText(strBtnShow);
    }
    //右侧按钮
    {
        QString strBtnShow;
        if(pGDData->pGD->bIronShoesSetX)//铁鞋设置
        {
            if(pGDData->pGD->nManualBrakeX==1)//铁鞋+人力制动机
                stricon="border-image:url(://res/右边铁鞋+制动机.bmp);";
            else if(pGDData->pGD->nManualBrakeX==2)//铁鞋+人力制动机紧固器
                stricon="border-image:url(://res/右边铁鞋+紧固器.bmp);";
            else stricon="border-image:url(://res/右边铁鞋.bmp);";//铁鞋
            strBtnShow = pGDData->pGD->m_nRTxNum == 0 ? "" :  QString::number(pGDData->pGD->m_nRTxNum);
        }
        else
        {
            if(pGDData->pGD->nManualBrakeX==1)//人力制动机
                stricon="border-image:url(://res/制动机.bmp);";
            else if(pGDData->pGD->nManualBrakeX==2)//人力制动机紧固器
                stricon="border-image:url(://res/紧固器.bmp);";
            else stricon="";
            strBtnShow = pGDData->pGD->m_nRJgqNum == 0 ? "" : QString::number(pGDData->pGD->m_nRJgqNum);
        }

        QLabel* imageLabel = nullptr;
        QVBoxLayout* topLayout = (QVBoxLayout*)ui->pushButton_3->layout();
        if(!topLayout)
        {
            imageLabel = new QLabel;
            imageLabel->setStyleSheet(stricon);
            imageLabel->setAlignment(Qt::AlignBottom|Qt::AlignHCenter);
            topLayout = new QVBoxLayout();
            topLayout->setMargin(4);
            topLayout->addWidget(imageLabel);
            ui->pushButton_3->setLayout(topLayout);
        }
        else
        {
            imageLabel = (QLabel*)topLayout->itemAt(0)->widget();
            imageLabel->setStyleSheet(stricon);
        }
        imageLabel->setText(strBtnShow);
    }
}
//设置背景颜色
void StationStorageWidgetGD::SetWidgetBk(bool b)
{
    if(b)
    {
        ui->widget_2->setStyleSheet("QWidget#widget_2{background-color: rgb(210, 216, 219);}");
        ui->widget->setStyleSheet("background-color: rgb(210, 216, 219);");
    }
    else
    {
        ui->widget_2->setStyleSheet("QWidget#widget_2{background-color: rgb(186, 196, 207);}");
        ui->widget->setStyleSheet("background-color: rgb(186, 196, 207);");
    }
}
//设置股道存车信息显示
void StationStorageWidgetGD::SetGDStationStorageShow(bool b,QString str)
{
    if(b)//股道设置站存车信息
    {
        if(str != "")
        {
            pGDData->pGD->m_sTrainInfoShow = str;
        }
        else
        {
          pGDData->pGD->m_sTrainInfoShow = pGDData->pGD->m_sTrainInfoFL;
        }

    }
    else//防溜设置中 存车信息
    {
        bool b=pGDData->pGD->GetTrainInfoFLShow();
        //TODO
        if(pGDData->pGD->m_sTrainInfoZCC == "" /*&& pGDData->pGD->GetTrainInfoFLShow()*/)
        {
            pGDData->pGD->m_sTrainInfoShow = str;
        }
        //else pGDData->pGD->m_sTrainInfoShow = "";
    }
}
//槽-左侧防溜设置按钮
void StationStorageWidgetGD::on_pushButton_2_pressed()
{
    emit SendTextAnalysisSignal(166, QCursor::pos());
    pAntiSlipWidget->SetStationID(nStationID, m_strAntislip, m_bSleepers);
    pAntiSlipWidget->Init(pGDData);
    pAntiSlipWidget->show();
}
//槽-右侧防溜设置
void StationStorageWidgetGD::on_pushButton_3_pressed()
{
    emit SendTextAnalysisSignal(166, QCursor::pos());
    pAntiSlipWidget->SetStationID(nStationID, m_strAntislip, m_bSleepers);
    pAntiSlipWidget->Init(pGDData);
    pAntiSlipWidget->show();
}
//槽-刷新股道防溜界面显示
void StationStorageWidgetGD::onUpDataStationStorage()
{
    SetBtnInco();
    if(oldTrainInfoShow != pGDData->pGD->m_sTrainInfoShow)
       emit StationStorageChange();

    oldTrainInfoShow = pGDData->pGD->m_sTrainInfoShow;
    mlab->setText(pGDData->pGD->m_sTrainInfoShow);
}
void StationStorageWidgetGD::ZCCGLOper_Fun()
{
    emit SendTextAnalysisSignal(164, QCursor::pos());
    pStationStorageSetDialog->InitGD(pGDData);
    pStationStorageSetDialog->exec();
    if(pStationStorageSetDialog->bok)
    {
        pStationStorageSetDialog->bok=false;
        QString str=pStationStorageSetDialog->GetTextBrowserAll();
        pGDData->pGD->m_sTrainInfoZCC=str;
        SetGDStationStorageShow(true,pGDData->pGD->m_sTrainInfoZCC);
        emit SendAntiSlipSignal(pGDData,true,nStationID);
    }
}

void StationStorageWidgetGD::InsertTrackLabel(TrackLabel* pTrackLabel)
{
    m_pTrackLabel = pTrackLabel;
    ui->widget->layout()->addWidget(pTrackLabel);
    pTrackLabel->show();
}

void StationStorageWidgetGD::InsertStationName(QString strName)
{
    QHBoxLayout* pLayout = (QHBoxLayout*)ui->widget_2->layout();
    QLabel* pLabel = new QLabel;
    pLabel->setFixedSize(96, 32);
    pLabel->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
    pLabel->setText(strName);
    pLabel->setAlignment(Qt::AlignCenter);
    pLabel->setFont(QFont("微软雅黑", 11));
    pLayout->insertWidget(0, pLabel);
}

TrackLabel* StationStorageWidgetGD::takeTrackLabel()
{
    if(m_pTrackLabel)
        m_pTrackLabel->hide();
    ui->widget->layout()->removeWidget(m_pTrackLabel);
    TrackLabel* pTrackLabel = m_pTrackLabel;
    m_pTrackLabel = nullptr;

    return pTrackLabel;
}

void StationStorageWidgetGD::ClearTrackLabel()
{
    if(m_pTrackLabel)
    {
        ui->widget->layout()->removeWidget(m_pTrackLabel);
        delete m_pTrackLabel;
        m_pTrackLabel = nullptr;
    }
}

//槽-股道防溜设置
void StationStorageWidgetGD::RecAntiSlipSolt(GDData* pGDData,bool bSet,int nstationid)
{
    SetGDStationStorageShow(false,pGDData->pGD->m_sTrainInfoFL);
    emit SendAntiSlipSignal(pGDData,bSet,nstationid);
}

void StationStorageWidgetGD::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == Timer_ID_500)
    {
        UpDataTrainEnterTrack();
    }
}

void StationStorageWidgetGD::UpDataTrainEnterTrack()
{
    QFont font = ui->label->font();
    if(!pGDData)
        return;
    if(pGDData->pGD->getState(QDZY))
    {
        ui->pushButton->setStyleSheet("border:2px groove red; color: red;");
        ui->label->setStyleSheet("color: red;");
        font.setBold(true);
        ui->label->setFont(font);
    }
    else
    {
        ui->pushButton->setStyleSheet("");
        ui->label->setStyleSheet("");
        font.setBold(false);
        ui->label->setFont(font);
    }
}

