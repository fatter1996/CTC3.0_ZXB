#include "tracklabel.h"
#include "ui_tracklabel.h"
#include "worktaskwidget.h"
#include <QPixmap>
#include <QMessageBox>
TrackLabel::TrackLabel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrackLabel)
{
    bkColorGreen = "QPushButton{background-color: rgb(20, 170, 20);}"
                           "QPushButton:hover{background-color: rgb(20, 150, 20);}"
                           "QPushButton:pressed{background-color: rgb(0, 130, 0);}";

    bkColorRed = "QPushButton{background-color: rgb(230, 65, 55);}"
                         "QPushButton:hover{background-color: rgb(230, 40, 30);}"
                         "QPushButton:pressed{background-color: rgb(210, 65, 50);}";

    bkColorYellow = "QPushButton{background-color: rgb(255, 170, 0);}"
                            "QPushButton:hover{background-color: rgb(240, 170, 20);}"
                            "QPushButton:pressed{background-color: rgb(240, 160, 25);}";

    bkColorGray = "QPushButton{"
                    "background-color: rgb(240, 240, 240);"
                    "border:1px groove gray;"
                    "border-radius:4px;"
                    "padding:1px 2px;"
                    "border-style:outset;"
                  "}"
                  "QPushButton:hover{"
                    "background-color: rgb(229, 241, 251);"
                    "color:black;"
                  "}"
                    "QPushButton:pressed{"
                    "background-color: rgb(204, 228, 247);"
                    "border-style:inset;"
                  "}"
                  "QPushButton{"
                    "background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));"
                  "}";

    ui->setupUi(this);

    ui->arriveTimeBtn->setEnabled(false);
    ui->departTimeBtn->setEnabled(false);
    installEventFilter(this);

    ui->JCRouteBtn->installEventFilter(this);
    ui->FCRouteBtn->installEventFilter(this);

}

TrackLabel::~TrackLabel()
{
    delete ui;
}

TrackLabel* TrackLabel::Create(MyStation* pStation, TrafficLog* pTrafficLog, int viewType)
{
    if(!pTrafficLog || !pStation)
        return nullptr;
    TrackLabel* pTrackLabel = new TrackLabel;
    pTrackLabel->Init(pStation, pTrafficLog, viewType);

    return pTrackLabel;
}


void TrackLabel::InitProcBtn()
{
    QMenu* m_menu = new QMenu(this);
    m_menu->setStyleSheet("QMenu{"
                              "background-color: rgb(229, 241, 251);"
                              "color: rgb(0, 0, 0);"
                              "margin: 4px;"
                          "}"
                          "QMenu::item{"
                              "padding: 8px 24px 8px 24px;"
                              "margin: 0px;"
                          "}"
                          "QMenu::item:selected {"
                              "color: rgb(0, 0, 0);"
                              "background-color: rgb(204, 228, 247);"
                          "}");

    if(m_pTrafficLog->m_btBeginOrEndFlg != 0xbb)
    {
        //办理接车预告
        QAction* pAction1 = new QAction();
        pAction1->setText(PROCESS_JCYG);
        m_menu->addAction(pAction1);
        ui->trainProcBtn->setMenu(m_menu);
        connect(pAction1, &QAction::triggered, this, [=](bool checked){
            emit SendTextAnalysisSignal(9, QCursor::pos());
            ui->trainProcBtn->setText(PROCESS_JCYG);
            ui->trainProcBtn->setIcon(QIcon());
        });
        //办理接车进路
        QAction* pAction2 = new QAction();
        pAction2->setText(PROCESS_BLJL);
        m_menu->addAction(pAction2);
        ui->trainProcBtn->setMenu(m_menu);
        connect(pAction2, &QAction::triggered, this, [=](bool checked){
            emit SendTextAnalysisSignal(10, QCursor::pos());
            ui->trainProcBtn->setText(PROCESS_BLJL);
            QIcon icon;
            icon.addFile("://res/接.png");
            ui->trainProcBtn->setIcon(icon);
        });
        //列车到达(通过)报点
        QAction* pAction3 = new QAction();
        pAction3->setText(PROCESS_LCDD);
        m_menu->addAction(pAction3);
        ui->trainProcBtn->setMenu(m_menu);
        connect(pAction3, &QAction::triggered, this, [=](bool checked){
            emit SendTextAnalysisSignal(11, QCursor::pos());
            ui->trainProcBtn->setText(PROCESS_LCDD);
            QIcon icon;
            icon.addFile("://res/列车到达通过报点.png");
            ui->trainProcBtn->setIcon(icon);
        });
    }

    if(m_pTrafficLog->m_btBeginOrEndFlg != 0xcc)
    {
        //办理发车预告
        QAction* pAction4 = new QAction();
        pAction4->setText(PROCESS_FCYG);
        m_menu->addAction(pAction4);
        ui->trainProcBtn->setMenu(m_menu);
        connect(pAction4, &QAction::triggered, this, [=](bool checked){
            emit SendTextAnalysisSignal(12, QCursor::pos());
            ui->trainProcBtn->setText(PROCESS_FCYG);
            ui->trainProcBtn->setIcon(QIcon());
        });
        //办理发车进路
        QAction* pAction5 = new QAction();
        pAction5->setText(PROCESS_BLFL);
        m_menu->addAction(pAction5);
        ui->trainProcBtn->setMenu(m_menu);
        connect(pAction5, &QAction::triggered, this, [=](bool checked){
            emit SendTextAnalysisSignal(13, QCursor::pos());
            ui->trainProcBtn->setText(PROCESS_BLFL);
            QIcon icon;
            icon.addFile("://res/发.png");
            ui->trainProcBtn->setIcon(icon);
        });
        //发车报点
        QAction* pAction6 = new QAction();
        pAction6->setText(PROCESS_LCCF);
        m_menu->addAction(pAction6);
        ui->trainProcBtn->setMenu(m_menu);
        connect(pAction6, &QAction::triggered, this, [=](bool checked){
            emit SendTextAnalysisSignal(14, QCursor::pos());
            ui->trainProcBtn->setText(PROCESS_LCCF);
            QIcon icon;
            icon.addFile("://res/发.png");
            ui->trainProcBtn->setIcon(icon);
        });
    }

    m_menu->addSeparator();
    //流程终止
    QAction* pAction7 = new QAction();
    pAction7->setText("结束作业流程");
    m_menu->addAction(pAction7);
    ui->trainProcBtn->setMenu(m_menu);
    connect(pAction7, &QAction::triggered, this, [=](bool checked){

        emit SendTextAnalysisSignal(15, QCursor::pos());
        int ret = QMessageBox::information(nullptr, "注意-TDCS/CTC3.0", QString("确认结束吗?"), "是", "否", "", 0);
        if(ret == 0)
        {
            m_pTrafficLog->m_bFinish = true;
            emit workFlowFinish(100);

            ui->trainProcBtn->setText(PROCESS_FINISH);
            QIcon icon;
            icon.addFile("://res/流程终止.png");
            ui->trainProcBtn->setIcon(icon);
        }

    });
}

void TrackLabel::Init(MyStation* pStation, TrafficLog* pTrafficLog, int viewType)
{
    if(!pStation || !pTrafficLog)
        return;
    nVewType = viewType;
    m_pMyStation = pStation;
    m_pTrafficLog = pTrafficLog;
    m_pRouteOrderJC = m_pMyStation->getRouteOrderByTrainNum(m_pTrafficLog->m_strTrainNum, 0);
    m_pRouteOrderFC = m_pMyStation->getRouteOrderByTrainNum(m_pTrafficLog->m_strTrainNum, 1);
    if(m_pRouteOrderJC)
        m_RouteOrderJCBackup = *m_pRouteOrderJC;
    if(m_pRouteOrderFC)
        m_RouteOrderFCBackup = *m_pRouteOrderFC;
    QString strStyle = QString("color: %1;")
            .arg(m_pTrafficLog->m_nLHFlg?"rgb(255, 0, 0)":"rgb(0, 0, 0)");

    ui->trainNumLabelL->setStyleSheet(strStyle);
    ui->trainNumLabelR->setStyleSheet(strStyle);
    ui->timeLabel->setStyleSheet(strStyle);
    ui->timeLabel2->setStyleSheet(strStyle);
    ui->trainPosistatusLabel->setStyleSheet(strStyle);
    ui->directionLabel->setStyleSheet(strStyle);
    ui->JCTrackBtn->setStyleSheet(strStyle);
    ui->FCTrackBtn->setStyleSheet(strStyle);

    InitProcBtn();
    InitBtnConnect();

    //预告按钮
    for (int i = 0; i < m_pMyStation->StaConfigInfo.JFCKInfo.size(); i++)
    {
        MyStation::JFCK_T info = m_pMyStation->StaConfigInfo.JFCKInfo.at(i);
        //接车预告
        if((info.strJCKName == m_pTrafficLog->m_strXHD_JZk && ((info.bJCKAdvance&0x01) == 0)) ||
           (info.strFCKName == m_pTrafficLog->m_strXHD_JZk && ((info.bFCKAdvance&0x01) == 0)))
        {
            ui->JCPreviewBtn->hide();
            ui->arriveTimeBtn->setEnabled(true);
        }
        //发车预告
        if((info.strJCKName == m_pTrafficLog->m_strXHD_CZk && ((info.bJCKAdvance&0x02) == 0)) ||
           (info.strFCKName == m_pTrafficLog->m_strXHD_CZk && ((info.bFCKAdvance&0x02) == 0)))
        {
            ui->FCPreviewBtn->hide();
            ui->departTimeBtn->setEnabled(true);
        }
    }

    QHBoxLayout* pLayout = (QHBoxLayout*)ui->bkWidget->layout();
    if(nVewType == 1) //股道视图
    {
        ui->JCTrackBtn->hide();
        ui->FCTrackBtn->hide();
        ui->labelL->hide();
        ui->labelR->hide();

        ui->JCLabelWidget->setFixedWidth(168);
        ui->JCAutoTriggerBtn->hide();

        ui->FCAutoTriggerBtn->hide();
        ui->FCLabelWidget->setFixedWidth(168);
    }
    else if(nVewType == 2)//序列视图
    {
        QLayoutItem* item = pLayout->takeAt(pLayout->indexOf(ui->workLayout));
        pLayout->insertItem(pLayout->indexOf(ui->directionLabel) + 1, item);
        ui->imgLabel->hide();

        QLayoutItem* item2 = pLayout->takeAt(pLayout->indexOf(ui->trainNumLabelR));
        pLayout->insertItem(pLayout->indexOf(ui->workLayout) + 1, item2);

        QMenu* pMenu = new QMenu(this);
        QActionGroup* group = new QActionGroup(pMenu);
        pMenu->setStyleSheet("QMenu{"
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
        for(int i = 0; i < m_pMyStation->DevArray.size(); i++)
        {
            if(m_pMyStation->DevArray[i]->getDevType() == Dev_GD)
            {
                CGD* pGD=(CGD*)(m_pMyStation->DevArray[i]);
                if(pGD->pCenter.x() < 0 || pGD->pCenter.y() < 0)
                {
                    continue;
                }
                if(pGD->m_nGDType == GD_QD)
                {
                    QAction* pAction = new QAction(pGD->m_strName, pMenu);
                    pAction->setCheckable(true);
                    pAction->setChecked(false);

                    if(pGD->m_strName == m_pTrafficLog->m_strRecvTrainTrack ||
                        pGD->m_strName == m_pTrafficLog->m_strDepartTrainTrack)
                    {
                       pAction->setChecked(true);
                    }
                    pMenu->addAction(group->addAction(pAction));

                    connect(pAction, &QAction::triggered, [=](){
                        emit SendTextAnalysisSignal(168, QCursor::pos());

                        if(m_pMyStation->IsReadOnly())
                        {
                            QMessageBox::information(nullptr, "注意-STPC/CTC3.0 通用车站操作终端","列车进路更新失败:\n没有权限","关闭");
                            return;
                        }

                        bool bOperate = false;
                        if(m_pRouteOrderJC)
                        {
                            if(m_pRouteOrderJC->m_bZHJL)
                            {
                                for(int j = 0; j < m_pRouteOrderJC->m_vecSonTrainRouteOrder.size(); j++)
                                {
                                    bOperate |= (m_pRouteOrderJC->m_vecSonTrainRouteOrder.at(j)->m_nOldRouteState != 0 && m_pRouteOrderJC->m_vecSonTrainRouteOrder.at(j)->m_nOldRouteState != 5);
                                }
                            }
                            else bOperate |= (m_pRouteOrderJC->m_nOldRouteState != 0 && m_pRouteOrderJC->m_nOldRouteState != 5);
                        }
                        if(m_pRouteOrderFC)
                        {
                            if(m_pRouteOrderFC->m_bZHJL)
                            {
                                for(int j = 0; j < m_pRouteOrderFC->m_vecSonTrainRouteOrder.size(); j++)
                                {
                                    bOperate |= (m_pRouteOrderFC->m_vecSonTrainRouteOrder.at(j)->m_nOldRouteState != 0 && m_pRouteOrderFC->m_vecSonTrainRouteOrder.at(j)->m_nOldRouteState != 5);
                                }
                            }
                            else bOperate |= (m_pRouteOrderFC->m_nOldRouteState != 0 && m_pRouteOrderFC->m_nOldRouteState != 5);
                        }
                        if(bOperate)
                        {
                            QMessageBox::information(this, tr("STPC"), QString("进路状态不允许修改股道!").arg(pGD->m_strName), tr("确定"),  "", 0);
                            return;
                        }

                        pAction->setChecked(true);
                        ui->JCTrackBtn->setText(pAction->text());
                        ui->FCTrackBtn->setText(pAction->text());
                        emit sendRouteOrderMsg(MSGTYPE_TRACKINFO, m_RouteOrderJCBackup.route_id, pGD->m_nCode);
                        emit sendRouteOrderMsg(MSGTYPE_TRACKINFO, m_RouteOrderFCBackup.route_id, pGD->m_nCode);
                    });
                }
            }
        }
        ui->JCTrackBtn->setMenu(pMenu);
        ui->FCTrackBtn->setMenu(pMenu);
    }
}

void TrackLabel::InitWorkState(QPushButton* pPushBotton, int state)
{
    switch (state) {
    case 1: pPushBotton->setStyleSheet(bkColorRed); break;
    case 2: pPushBotton->setStyleSheet(bkColorYellow); break;
    case 3: pPushBotton->setStyleSheet(bkColorGreen); break;
    default: pPushBotton->setStyleSheet(""); break;
    }
}

void TrackLabel::InitBtnConnect()
{
    //接预
    connect(ui->JCPreviewBtn, &QPushButton::clicked, this, [=](){
        ShowWorkTaskWidget(WNDTYPE_PREVIEW, PREVIEW_ARRIVE);
        emit SendTextAnalysisSignal(16, QCursor::pos());
    });
    //发预
    connect(ui->FCPreviewBtn, &QPushButton::clicked, this, [=](){
        ShowWorkTaskWidget(WNDTYPE_PREVIEW, PREVIEW_DEPART);
        emit SendTextAnalysisSignal(17, QCursor::pos());
    });

    //到点
    connect(ui->arriveTimeBtn, &QPushButton::clicked, this, [=](){
        ShowWorkTaskWidget(WNDTYPE_REPORT, REPORT_ARRIVE);
        emit SendTextAnalysisSignal(18, QCursor::pos());
    });
    //发点
    connect(ui->departTimeBtn, &QPushButton::clicked, this, [=](){
        ShowWorkTaskWidget(WNDTYPE_REPORT, REPORT_DEPART);
        emit SendTextAnalysisSignal(19, QCursor::pos());
    });

    //接车进路
    connect(ui->JCRouteBtn, &QPushButton::clicked, this, [=](){
        nWndType = 0;
        ShowRouteHandWidget();
        emit SendTextAnalysisSignal(20, QCursor::pos());
    });

    //发车进路
    connect(ui->FCRouteBtn, &QPushButton::clicked, this, [=](){
        nWndType = 1;
        ShowRouteHandWidget();
        emit SendTextAnalysisSignal(21, QCursor::pos());
    });

    //交令
    connect(ui->FunBtnJL, &QPushButton::clicked, this, [=](){
        ShowWorkTaskWidget(WNDTYPE_WORK, DEF_FLOW_JL);
        emit SendTextAnalysisSignal(22, QCursor::pos());
    });
    //列检
    connect(ui->FunBtnLJ, &QPushButton::clicked, this, [=](){
        ShowWorkTaskWidget(WNDTYPE_WORK, DEF_FLOW_LJ);
        emit SendTextAnalysisSignal(23, QCursor::pos());
    });
    //上水
    connect(ui->FunBtnSS, &QPushButton::clicked, this, [=](){
        ShowWorkTaskWidget(WNDTYPE_WORK, DEF_FLOW_SS);
        emit SendTextAnalysisSignal(24, QCursor::pos());
    });
    //吸污
    connect(ui->FunBtnXW, &QPushButton::clicked, this, [=](){
        ShowWorkTaskWidget(WNDTYPE_WORK, DEF_FLOW_XW);
        emit SendTextAnalysisSignal(25, QCursor::pos());
    });
    //交票
    connect(ui->FunBtnJP, &QPushButton::clicked, this, [=](){
        ShowWorkTaskWidget(WNDTYPE_WORK, DEF_FLOW_JP);
        emit SendTextAnalysisSignal(26, QCursor::pos());
    });
    //乘降
    connect(ui->FunBtnCJ, &QPushButton::clicked, this, [=](){
        ShowWorkTaskWidget(WNDTYPE_WORK, DEF_FLOW_CJ);
        emit SendTextAnalysisSignal(27, QCursor::pos());
    });
    //摘挂
    connect(ui->FunBtnZG, &QPushButton::clicked, this, [=](){
        ShowWorkTaskWidget(WNDTYPE_WORK, DEF_FLOW_ZG);
        emit SendTextAnalysisSignal(28, QCursor::pos());
    });
    //列尾
    connect(ui->FunBtnLW, &QPushButton::clicked, this, [=](){
        ShowWorkTaskWidget(WNDTYPE_WORK, DEF_FLOW_LW);
        emit SendTextAnalysisSignal(29, QCursor::pos());
    });
    //货检
    connect(ui->FunBtnHJ, &QPushButton::clicked, this, [=](){
        ShowWorkTaskWidget(WNDTYPE_WORK, DEF_FLOW_HJ);
        emit SendTextAnalysisSignal(30, QCursor::pos());
    });
    //换乘
    connect(ui->FunBtnHC, &QPushButton::clicked, this, [=](){
        ShowWorkTaskWidget(WNDTYPE_WORK, DEF_FLOW_HC);
        emit SendTextAnalysisSignal(31, QCursor::pos());
    });
    //装卸
    connect(ui->FunBtnZX, &QPushButton::clicked, this, [=](){
        ShowWorkTaskWidget(WNDTYPE_WORK, DEF_FLOW_ZX);
        emit SendTextAnalysisSignal(32, QCursor::pos());
    });
    //机车
    connect(ui->FunBtnJC, &QPushButton::clicked, this, [=](){
        emit SendTextAnalysisSignal(33, QCursor::pos());
        if(m_pTrafficLog->m_strTrainPosStatus == STATUS_LCYWZJRGD)
        {
            QMenu* pMenu = new QMenu(this);
            pMenu->setStyleSheet("QMenu{"
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

            QAction* pAction1 = new QAction();
            pAction1->setText("列车摘头安排");
            pMenu->addAction(pAction1);
            QAction* pAction2 = new QAction();
            pAction2->setText("摘头办理");
            pMenu->addAction(pAction2);
            pMenu->addSeparator();
            QAction* pAction3 = new QAction();
            pAction3->setText("列车挂头安排");
            pMenu->addAction(pAction3);
            QAction* pAction4 = new QAction();
            pAction4->setText("挂头办理");
            pMenu->addAction(pAction4);
            pMenu->addSeparator();

            QAction* pAction5 = new QAction();
            pAction5->setText("设置作业流程状态");
            pMenu->addAction(pAction5);

            connect(pAction5, &QAction::triggered, this, [=](bool checked){
                ShowWorkTaskWidget(WNDTYPE_WORK, DEF_FLOW_JC);
            });

            pMenu->exec(QCursor::pos());
        }
        else ShowWorkTaskWidget(WNDTYPE_WORK, DEF_FLOW_JC);
    });
    //道口
    connect(ui->FunBtnDK, &QPushButton::clicked, this, [=](){
        ShowWorkTaskWidget(WNDTYPE_WORK, DEF_FLOW_DK);
        emit SendTextAnalysisSignal(34, QCursor::pos());
    });
    //车号
    connect(ui->FunBtnCH, &QPushButton::clicked, this, [=](){
        ShowWorkTaskWidget(WNDTYPE_WORK, DEF_FLOW_CH);
        emit SendTextAnalysisSignal(35, QCursor::pos());
    });
    //综控
    connect(ui->FunBtnZK, &QPushButton::clicked, this, [=](){
        ShowWorkTaskWidget(WNDTYPE_WORK, DEF_FLOW_ZK);
        emit SendTextAnalysisSignal(36, QCursor::pos());
    });
    //站务
    connect(ui->FunBtnZW, &QPushButton::clicked, this, [=](){
        ShowWorkTaskWidget(WNDTYPE_WORK, DEF_FLOW_ZW);
        emit SendTextAnalysisSignal(37, QCursor::pos());
    });
    //接车进路自触
    connect(ui->JCAutoTriggerBtn, &QPushButton::clicked, this, [=](){
//        emit SendTextAnalysisSignal(38, QCursor::pos());
        //SetAutoTrigger(m_pMyStation->getRouteOrderByTrainNum(m_pTrafficLog->m_strTrainNum, 0));
        SetAutoTrigger_LeftButton(m_pMyStation->getRouteOrderByTrainNum(m_pTrafficLog->m_strTrainNum, 0));

    });
    //发车进路自触
    connect(ui->FCAutoTriggerBtn, &QPushButton::clicked, this, [=](){
//        emit SendTextAnalysisSignal(39, QCursor::pos());
        //SetAutoTrigger(m_pMyStation->getRouteOrderByTrainNum(m_pTrafficLog->m_strTrainNum, 1));
        SetAutoTrigger_LeftButton(m_pMyStation->getRouteOrderByTrainNum(m_pTrafficLog->m_strTrainNum, 1));

    });

}

//设置背景颜色
void TrackLabel::SetWidgetBk(bool b)
{
    if(b)
    {
        setStyleSheet("QWidget#bkWidget{background-color: rgb(210, 216, 219);}");
    }
    else
    {
        setStyleSheet("QWidget#bkWidget{background-color: rgb(186, 196, 207);}");
    }
}

void TrackLabel::UpdataBtnColor()
{
    if(m_pTrafficLog->m_timAgrFromAdjtStaDepaTrain.isValid())
    {
        ui->JCPreviewBtn->setStyleSheet(bkColorGreen);
        ui->arriveTimeBtn->setEnabled(true);
    }
    else
    {
        ui->JCPreviewBtn->setStyleSheet("");
        ui->arriveTimeBtn->setEnabled(false);
    }

    if(m_pTrafficLog->m_timToAdjtStaAgrDepaTrain.isValid())
    {
        ui->FCPreviewBtn->setStyleSheet(bkColorGreen);
        ui->departTimeBtn->setEnabled(true);
    }
    else
    {
        ui->FCPreviewBtn->setStyleSheet("");
        ui->departTimeBtn->setEnabled(false);
    }

    if(m_pTrafficLog->m_timRealReachStation.isValid())
    {
        ui->arriveTimeBtn->setStyleSheet(bkColorGreen);
        if(m_pTrafficLog->m_btBeginOrEndFlg == 0xCC)
        {
            m_pTrafficLog->m_bFinish = true;
            emit workFlowFinish(5000);
        }
        if(!m_pRouteOrderJC)
            ui->JCRouteBtn->setStyleSheet(bkColorGreen);
    }
    else ui->arriveTimeBtn->setStyleSheet("");

    if(m_pTrafficLog->m_timRealDepaTrain.isValid())
    {
        ui->departTimeBtn->setStyleSheet(bkColorGreen);
        if(!m_pRouteOrderFC)
            ui->FCRouteBtn->setStyleSheet(bkColorGreen);
    }
    else ui->departTimeBtn->setStyleSheet("");

    InitWorkState(ui->FunBtnLJ, m_pTrafficLog->m_btLJStatus); //列检
    InitWorkState(ui->FunBtnJL, m_pTrafficLog->m_btJLStatus); //交令
    InitWorkState(ui->FunBtnJP, m_pTrafficLog->m_btJPStatus); //交票
    InitWorkState(ui->FunBtnLW, m_pTrafficLog->m_btLWStatus); //列尾
    InitWorkState(ui->FunBtnJC, m_pTrafficLog->m_btJCStatus); //机车
    InitWorkState(ui->FunBtnHJ, m_pTrafficLog->m_btHJStatus); //货检
    InitWorkState(ui->FunBtnCJ, m_pTrafficLog->m_btCJStatus); //乘降
    InitWorkState(ui->FunBtnSS, m_pTrafficLog->m_btSSStatus); //上水
    InitWorkState(ui->FunBtnZG, m_pTrafficLog->m_btZGStatus); //摘挂
    InitWorkState(ui->FunBtnHC, m_pTrafficLog->m_btHCStatus); //换乘
    InitWorkState(ui->FunBtnZX, m_pTrafficLog->m_btZXStatus); //装卸
    InitWorkState(ui->FunBtnXW, m_pTrafficLog->m_btXWStatus); //吸污
    InitWorkState(ui->FunBtnDK, m_pTrafficLog->m_btDKStatus); //道口
    InitWorkState(ui->FunBtnCH, m_pTrafficLog->m_btCHStatus); //车号
    InitWorkState(ui->FunBtnZW, m_pTrafficLog->m_btZWStatus); //站务
    InitWorkState(ui->FunBtnZK, m_pTrafficLog->m_btZKStatus); //综控
}

void TrackLabel::UpdataTrackView()
{
    if(!m_pTrafficLog)
        return;

    UpdataBtnColor();
    //车次号
    ui->trainNumLabelL->setText(m_pTrafficLog->m_strTrainNum);
    ui->trainNumLabelR->setText(m_pTrafficLog->m_strTrainNum);
    //车次信息
    QHBoxLayout* pLayout = (QHBoxLayout*)ui->trainTypeLabel->layout();
    QLayoutItem* item = nullptr;
    while (pLayout->count() > 1) {
        item = pLayout->takeAt(0);
        if(item->widget() && item != ui->horizontalSpacer_7)
        {
            item->widget()->setParent(nullptr);
            delete item;
        }
    }
    //超限
    if(m_pTrafficLog->m_nLevelCX != 0)
    {
        QLabel* picLable = new QLabel;
        picLable->setFixedSize(24, 24);
        switch (m_pTrafficLog->m_nLevelCX) {
        case 1: picLable->setPixmap(QPixmap("://res/一级超限.png")); break;
        case 2: picLable->setPixmap(QPixmap("://res/二级超限.png")); break;
        case 3: picLable->setPixmap(QPixmap("://res/三级超限.png")); break;
        case 4: picLable->setPixmap(QPixmap("://res/超级超限.png")); break;
        default: picLable->setPixmap(QPixmap("://res/一级超限.png")); break;
        }
        picLable->setScaledContents(true);
        pLayout->insertWidget(pLayout->indexOf(ui->horizontalSpacer_7), picLable);
    }
    //重点
    if(m_pTrafficLog->m_bImportant)
    {
        QLabel* picLable = new QLabel;
        picLable->setFixedSize(24, 24);
        picLable->setPixmap(QPixmap("://res/重.png"));
        picLable->setScaledContents(true);
        pLayout->insertWidget(pLayout->indexOf(ui->horizontalSpacer_7), picLable);
    }
    //军运
    if(m_pTrafficLog->m_bArmy)
    {
        QLabel* picLable = new QLabel;
        picLable->setFixedSize(24, 24);
        picLable->setPixmap(QPixmap("://res/军.png"));
        picLable->setScaledContents(true);
        pLayout->insertWidget(pLayout->indexOf(ui->horizontalSpacer_7), picLable);
    }
    //电力
    if(m_pTrafficLog->m_bElectric == 0)
    {
        QLabel* picLable = new QLabel;
        picLable->setFixedSize(24, 24);
        picLable->setPixmap(QPixmap("://res/内.png"));
        picLable->setScaledContents(true);
        pLayout->insertWidget(pLayout->indexOf(ui->horizontalSpacer_7), picLable);
    }
    //办理客运
    if(m_pTrafficLog->m_bBLKY)
    {
        QLabel* picLable = new QLabel;
        picLable->setFixedSize(24, 24);
        picLable->setPixmap(QPixmap("://res/客.png"));
        picLable->setScaledContents(true);
        pLayout->insertWidget(pLayout->indexOf(ui->horizontalSpacer_7), picLable);
    }

    //列车接近状态
    ui->trainPosistatusLabel->setText(m_pTrafficLog->m_strTrainPosStatus);
    if((m_pTrafficLog->m_strTrainPosStatus == STATUS_LCYWZJRGD && m_pTrafficLog->m_btBeginOrEndFlg == 0xCC) ||
            m_pTrafficLog->m_strTrainPosStatus == STATUS_LCYCZ)
    {
        m_pTrafficLog->m_bFinish = true;
        emit workFlowFinish(10000);
    }


    //列车下一步操作流程
    ui->trainProcBtn->setText(m_pTrafficLog->m_strProc);
    QIcon icon;
    if (m_pTrafficLog->m_strProc == PROCESS_JCYG)
        icon = QIcon();
    else if(m_pTrafficLog->m_strProc == PROCESS_BLJL)
        icon.addFile("://res/接.png");
    else if(m_pTrafficLog->m_strProc == PROCESS_LCDD)
        icon.addFile("://res/列车到达通过报点.png");
    else if(m_pTrafficLog->m_strProc == PROCESS_FCYG)
        icon = QIcon();
    else if(m_pTrafficLog->m_strProc == PROCESS_BLFL)
        icon.addFile("://res/发.png");
    else if(m_pTrafficLog->m_strProc == PROCESS_LCCF)
        icon.addFile("://res/发.png");
    else if(m_pTrafficLog->m_strProc == PROCESS_FINISH)
        icon.addFile("://res/流程终止.png");

    ui->trainProcBtn->setIcon(icon);
    //接发车股道
    ui->JCTrackBtn->setText(m_pTrafficLog->m_strRecvTrainTrack);
    ui->FCTrackBtn->setText(m_pTrafficLog->m_strDepartTrainTrack);

    //进路序列
    UpdataRouteOrder();
    //接车按钮,计划/实际时间
    QString strReachTime;
    QString strDepaTime;
    if(m_pTrafficLog->m_timRealReachStation.isValid())
    {
        QFont font = ui->timeLabel->font();
        font.setBold(true);
        font.setItalic(true);
        ui->timeLabel->setFont(font);
    }
    if(m_pTrafficLog->m_timRealDepaTrain.isValid())
    {
        QFont font = ui->timeLabel2->font();
        font.setBold(true);
        font.setItalic(true);
        ui->timeLabel2->setFont(font);
    }

    if(m_pTrafficLog->m_btBeginOrEndFlg == 0xAA) //接发
    {

        strReachTime = QString("%1").arg(m_pTrafficLog->m_timRealReachStation.isValid() ?
                                         m_pTrafficLog->m_timRealReachStation.toString("HH:mm") :
                                         m_pTrafficLog->m_timProvReachStation.toString("HH:mm"));

        strDepaTime = QString("%1").arg(m_pTrafficLog->m_timRealDepaTrain.isValid() ?
                                           m_pTrafficLog->m_timRealDepaTrain.toString("HH:mm") :
                                           m_pTrafficLog->m_timProvDepaTrain.toString("HH:mm"));

        if(m_pTrafficLog->m_timRealReachStation.isValid()
                && m_pTrafficLog->m_timRealDepaTrain.isValid()
                && m_pTrafficLog->m_timRealReachStation == m_pTrafficLog->m_timRealDepaTrain)
        {
            strReachTime = QString("%1").arg("通过");
        }

        //方向
        ui->directionLabel->setText(QString("%1->\n%2").arg(m_pTrafficLog->m_strXHD_JZk).arg(m_pTrafficLog->m_strXHD_CZk));
    }
    else if(m_pTrafficLog->m_btBeginOrEndFlg == 0xBB) //始发
    {
        ui->JCTrackBtn->hide();
        ui->JCPreviewBtn->hide();
        ui->JCAutoTriggerBtn->hide();
        ui->JCRouteBtn->hide();
        ui->arriveTimeBtn->hide();

        strReachTime = QString("始发");
        strDepaTime = QString("%1").arg(m_pTrafficLog->m_timRealDepaTrain.isValid() ?
                                           m_pTrafficLog->m_timRealDepaTrain.toString("HH:mm") :
                                           m_pTrafficLog->m_timProvDepaTrain.toString("HH:mm"));
        //方向
        ui->directionLabel->setText(QString("-->\n%1").arg(m_pTrafficLog->m_strXHD_CZk));
    }
    else if(m_pTrafficLog->m_btBeginOrEndFlg == 0xCC) //终到
    {
        //ui->trainNumLabelR->setText("");
        ui->FCTrackBtn->hide();
        ui->FCPreviewBtn->hide();
        ui->FCAutoTriggerBtn->hide();
        ui->FCRouteBtn->hide();
        ui->departTimeBtn->hide();

        strReachTime = QString("%1").arg(m_pTrafficLog->m_timRealReachStation.isValid() ?
                                         m_pTrafficLog->m_timRealReachStation.toString("HH:mm") :
                                         m_pTrafficLog->m_timProvReachStation.toString("HH:mm"));
        strDepaTime = QString("终到");

        //方向
        ui->directionLabel->setText(QString("%1->\n-").arg(m_pTrafficLog->m_strXHD_JZk));

    }
    else if(m_pTrafficLog->m_btBeginOrEndFlg == 0xDD) //通过
    {
        //strReachTime = QString("通过");
        strReachTime = QString("%1").arg(m_pTrafficLog->m_timRealReachStation.isValid() ?
                                         m_pTrafficLog->m_timRealReachStation.toString("HH:mm") : "通过");

        strDepaTime = QString("%1").arg(m_pTrafficLog->m_timRealDepaTrain.isValid() ?
                                           m_pTrafficLog->m_timRealDepaTrain.toString("HH:mm") :
                                           m_pTrafficLog->m_timProvDepaTrain.toString("HH:mm"));

        if(m_pTrafficLog->m_timRealReachStation.isValid()
                && m_pTrafficLog->m_timRealDepaTrain.isValid()
                && m_pTrafficLog->m_timRealReachStation == m_pTrafficLog->m_timRealDepaTrain)
        {
            strReachTime = QString("%1").arg("通过");
        }

        //方向
        ui->directionLabel->setText(QString("%1->\n%2").arg(m_pTrafficLog->m_strXHD_JZk).arg(m_pTrafficLog->m_strXHD_CZk));
    }
    ui->timeLabel->setText(strReachTime);
    ui->timeLabel2->setText(strDepaTime);

    //作业
    if(!(m_pMyStation->nFunBtnState & 0x8000)) //上水
        ui->FunBtnSS->hide();
    if(!(m_pMyStation->nFunBtnState & 0x4000)) //吸污
        ui->FunBtnXW->hide();
    if(!(m_pMyStation->nFunBtnState & 0x2000)) //机车
        ui->FunBtnJC->hide();
    if(!(m_pMyStation->nFunBtnState & 0x1000)) //列检
        ui->FunBtnLJ->hide();
    if(!(m_pMyStation->nFunBtnState & 0x0800)) //交令
        ui->FunBtnJL->hide();
    if(!(m_pMyStation->nFunBtnState & 0x0400)) //交票
        ui->FunBtnJP->hide();
    if(!(m_pMyStation->nFunBtnState & 0x0200)) //列尾
        ui->FunBtnLW->hide();
    if(!(m_pMyStation->nFunBtnState & 0x0100)) //货检
        ui->FunBtnHJ->hide();
    if(!(m_pMyStation->nFunBtnState & 0x80)) //乘降
        ui->FunBtnCJ->hide();
    if(!(m_pMyStation->nFunBtnState & 0x40)) //摘挂
        ui->FunBtnZG->hide();
    if(!(m_pMyStation->nFunBtnState & 0x20)) //换乘
        ui->FunBtnHC->hide();
    if(!(m_pMyStation->nFunBtnState & 0x10)) //装卸
        ui->FunBtnZX->hide();
    if(!(m_pMyStation->nFunBtnState & 0x08)) //道口
        ui->FunBtnDK->hide();
    if(!(m_pMyStation->nFunBtnState & 0x04)) //车号
        ui->FunBtnCH->hide();
    if(!(m_pMyStation->nFunBtnState & 0x02)) //站务
        ui->FunBtnZW->hide();
    if(!(m_pMyStation->nFunBtnState & 0x01)) //综控
        ui->FunBtnZK->hide();

    if(nVewType == 1)
    {
        QString strStyle = QString("color: %1;")
                .arg(m_pTrafficLog->m_nLHFlg?"rgb(255, 0, 0)":"rgb(0, 0, 0)");

        if(m_pTrafficLog->m_bTrackChenge)
            ui->trainNumLabelR->setStyleSheet(strStyle + "background-color: rgb(255, 170, 0);");
        else ui->trainNumLabelR->setStyleSheet(strStyle + "");
    }
}

void TrackLabel::UpdataTrackIndex(int index)
{
    if(index != 0)
        ui->imgLabel->setPixmap(QPixmap(QString("://res/index_%1.png").arg(index)));
    else ui->imgLabel->setPixmap(QPixmap());
}

void TrackLabel::UpdataRouteOrder()
{
    QIcon icon;
    icon.addFile("://res/checked.png");
    m_pRouteOrderJC = m_pMyStation->getRouteOrderByTrainNum(m_pTrafficLog->m_strTrainNum, 0);
    if(!m_pRouteOrderJC)
        m_pRouteOrderJC = m_pMyStation->getRouteOrderByTrainNum(m_pTrafficLog->m_strTrainNumOld, 0);
    if(m_pRouteOrderJC)
    {
        m_RouteOrderJCBackup = *m_pRouteOrderJC;
        if(m_pRouteOrderJC->m_nAutoTouch)
            ui->JCAutoTriggerBtn->setIcon(icon);
        else ui->JCAutoTriggerBtn->setIcon(QIcon());
        ui->JCRouteBtn->setText(QString("接路:%1").arg(m_pRouteOrderJC->m_strRouteDescrip));

        if(m_pRouteOrderJC->m_nOldRouteState < 2)
            ui->JCRouteBtn->setStyleSheet("");
        if(m_pRouteOrderJC->m_nAutoTouch)
            ui->JCRouteBtn->setStyleSheet(bkColorYellow);
        if(m_pRouteOrderJC->m_nOldRouteState >= 2)
            ui->JCRouteBtn->setStyleSheet(bkColorGreen);

        if(m_pRouteOrderJC->m_bZHJL)
        {
            bool bTouch = true;
            bool bAutoTouch = true;
            for(int i = 0; i < m_pRouteOrderJC->m_vecSonTrainRouteOrder.size(); i++)
            {
                bTouch &= (m_pRouteOrderJC->m_vecSonTrainRouteOrder.at(i)->m_nOldRouteState >= 2);
                bAutoTouch &= m_pRouteOrderJC->m_vecSonTrainRouteOrder.at(i)->m_nAutoTouch;
            }
            ui->JCRouteBtn->setStyleSheet("");
            ui->JCAutoTriggerBtn->setIcon(QIcon());

            if(bAutoTouch)
            {
                ui->JCAutoTriggerBtn->setIcon(icon);
                ui->JCRouteBtn->setStyleSheet(bkColorYellow);
            }


            if(bTouch)
                ui->JCRouteBtn->setStyleSheet(bkColorGreen);
        }
    }

    m_pRouteOrderFC = m_pMyStation->getRouteOrderByTrainNum(m_pTrafficLog->m_strTrainNum, 1);
    if(!m_pRouteOrderFC)
        m_pRouteOrderFC = m_pMyStation->getRouteOrderByTrainNum(m_pTrafficLog->m_strTrainNumOld, 1);
    if(m_pRouteOrderFC)
    {
        m_RouteOrderFCBackup = *m_pRouteOrderFC;
        if(m_pRouteOrderFC->m_nAutoTouch)
            ui->FCAutoTriggerBtn->setIcon(icon);
        else ui->FCAutoTriggerBtn->setIcon(QIcon());
        ui->FCRouteBtn->setText(QString("发路:%1").arg(m_pRouteOrderFC->m_strRouteDescrip));

        if(m_pRouteOrderFC->m_nOldRouteState < 2)
            ui->FCRouteBtn->setStyleSheet("");
        if(m_pRouteOrderFC->m_nAutoTouch)
            ui->FCRouteBtn->setStyleSheet(bkColorYellow);
        if(m_pRouteOrderFC->m_nOldRouteState >= 2)
            ui->FCRouteBtn->setStyleSheet(bkColorGreen);
        if(m_pRouteOrderFC->m_bZHJL)
        {
            bool bTouch = true;
            bool bAutoTouch = true;
            for(int i = 0; i < m_pRouteOrderFC->m_vecSonTrainRouteOrder.size(); i++)
            {
                bTouch &= (m_pRouteOrderFC->m_vecSonTrainRouteOrder.at(i)->m_nOldRouteState >= 2);
                bAutoTouch &= m_pRouteOrderFC->m_vecSonTrainRouteOrder.at(i)->m_nAutoTouch;
            }
            ui->FCRouteBtn->setStyleSheet("");
            ui->FCAutoTriggerBtn->setIcon(QIcon());
            if(bAutoTouch)
            {
                ui->FCAutoTriggerBtn->setIcon(icon);
                ui->FCRouteBtn->setStyleSheet(bkColorYellow);
            }


            if(bTouch)
                ui->FCRouteBtn->setStyleSheet(bkColorGreen);
        }
    }
    bool bJCState = !m_pRouteOrderJC;
    if(bJCState == false)
        bJCState |= m_pRouteOrderJC->m_nOldRouteState == 4;

    bool bFCState = !m_pRouteOrderFC;
    if(bFCState == false)
        bFCState |= m_pRouteOrderFC->m_nOldRouteState == 4;

    if(bJCState && bFCState)
    {
        m_pTrafficLog->m_bFinish = true;
        if(m_pTrafficLog->m_btBeginOrEndFlg != 0xCC)
        {
            emit workFlowFinish(500);
        }
    }
    else m_pTrafficLog->m_bFinish = false;

    if(pRouteHandWidget)
    {
        if(nWndType ? pRouteHandWidget->getRouteOrder() == &m_RouteOrderFCBackup
                    : pRouteHandWidget->getRouteOrder() == &m_RouteOrderJCBackup)
        {
            pRouteHandWidget->RouteUpData(
                        nWndType ? m_pTrafficLog->m_timToAdjtStaAgrDepaTrain.isValid()
                                 : m_pTrafficLog->m_timAgrFromAdjtStaDepaTrain.isValid(),
                        !(nWndType ? m_pRouteOrderFC : m_pRouteOrderJC));
        }
    }
}

void TrackLabel::ShowWorkTaskWidget(int wndType, int workType)
{
    WorkTaskWidget* pWorkTaskWidget = new WorkTaskWidget;

    connect(pWorkTaskWidget, &WorkTaskWidget::SendTextAnalysisSignal,[=](int index, QPoint pt, QString dev){
        emit this->SendTextAnalysisSignal(index, /*mapFromGlobal*/(pt), dev);
    });
    pWorkTaskWidget->setWindowModality(Qt::WindowModal);//设置模态
    pWorkTaskWidget->setAttribute(Qt::WA_DeleteOnClose);//关闭后自动释放窗口
    pWorkTaskWidget->Init(m_pTrafficLog, wndType, workType);

    connect(pWorkTaskWidget, &WorkTaskWidget::sendTrafficLogReportingMsg, [=](int type, QDateTime time){
        emit sendTrafficLogReportingMsg(m_pTrafficLog->log_id, type, time);
    });
    connect(pWorkTaskWidget, &WorkTaskWidget::sendTrafficLogWorkMsg, [=](int type, int flag, QDateTime time){
        emit sendTrafficLogWorkMsg(m_pTrafficLog->log_id, type, flag, time);
    });

    pWorkTaskWidget->show();
}

void TrackLabel::ShowRouteHandWidget()
{
    pRouteHandWidget = new RouteHandWidget;
    bool bAdvance = true;
    //if(nWndType == 0)
    //    bAdvance = !ui->JCPreviewBtn->isVisible();
    //else bAdvance = !ui->FCPreviewBtn->isVisible();

    if(nWndType ? !ui->FCPreviewBtn->isVisible() : !ui->JCPreviewBtn->isVisible())
        bAdvance = true;
    else bAdvance = nWndType ? m_pTrafficLog->m_timToAdjtStaAgrDepaTrain.isValid()
                             : m_pTrafficLog->m_timAgrFromAdjtStaDepaTrain.isValid();
    m_pMyStation->WndType = 1;
    pRouteHandWidget->Init(m_pMyStation, nWndType ? &m_RouteOrderFCBackup : &m_RouteOrderJCBackup,
                           bAdvance, !(nWndType ? m_pRouteOrderFC : m_pRouteOrderJC));

    if(m_pTrafficLog->m_btBeginOrEndFlg == 0xdd
        && m_pTrafficLog->m_timAgrFromAdjtStaDepaTrain.isValid()
        && m_pTrafficLog->m_timToAdjtStaAgrDepaTrain.isValid())
    {
        QVector<TrainRouteOrder*> vecTempTrainRouteOrder;
        vecTempTrainRouteOrder.append(&m_RouteOrderJCBackup);
        vecTempTrainRouteOrder.append(&m_RouteOrderFCBackup);
        pRouteHandWidget->SetThroughRouteOrder(vecTempTrainRouteOrder);
    }
    connect(pRouteHandWidget, &RouteHandWidget::SendTextAnalysisSignal,[=](int index, QPoint pt, QString dev){
        emit this->SendTextAnalysisSignal(index, /*mapFromGlobal*/(pt), dev);
    });
    connect(pRouteHandWidget, &RouteHandWidget::sendRouteOrderMsg, [=](int type, int routeId, int nData, QString strData){
        emit sendRouteOrderMsg(type, routeId, nData, strData);
    });
    connect(pRouteHandWidget, &RouteHandWidget::destroyed, [=](){
        pRouteHandWidget = nullptr;
    });
    pRouteHandWidget->show();
}

bool TrackLabel::eventFilter(QObject* o, QEvent* e)
{
    if(e->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(e);
        if(mouseEvent->button() == Qt::RightButton)
        {
            if(nVewType == 1)
            {
                if(o == ui->JCRouteBtn)
                    SetAutoTrigger(m_pMyStation->getRouteOrderByTrainNum(m_pTrafficLog->m_strTrainNum, 0));
                else if(o == ui->FCRouteBtn)
                    SetAutoTrigger(m_pMyStation->getRouteOrderByTrainNum(m_pTrafficLog->m_strTrainNum, 1));
            }
        }
    }
    return QWidget::eventFilter(o, e);
}

void TrackLabel::SetAutoTrigger(TrainRouteOrder* pRouteOrder)
{
    if(!pRouteOrder)
        return;

    QMenu* pMenu = new QMenu(this);
    pMenu->setStyleSheet("QMenu{"
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
                          "}"
                          "QMenu::item:!enabled {"
                              "color: rgb(180, 180, 180);"
                              "background-color: rgb(229, 241, 251);"
                          "}");

    //办理接车预告
    QAction* pAction1 = new QAction();
    pAction1->setText("设置自触");
    pAction1->setCheckable(true);
    pMenu->addAction(pAction1);
    QAction* pAction2 = new QAction();
    pAction2->setText("取消自触");
    pAction2->setCheckable(true);
    pMenu->addAction(pAction2);

    bool bAutoTouch = false;

    if(pRouteOrder->m_bZHJL == 0)
        bAutoTouch = pRouteOrder->m_nAutoTouch;
    else
    {
        for(int i = 0; i < pRouteOrder->m_vecSonTrainRouteOrder.size(); i++)
        {
            bAutoTouch |= pRouteOrder->m_vecSonTrainRouteOrder.at(i)->m_nAutoTouch;
        }
    }

    if(bAutoTouch)
    {
        pAction1->setChecked(true);
        pAction1->setEnabled(false);
        pAction2->setChecked(false);
    }
    else
    {
        pAction1->setChecked(false);
        pAction2->setEnabled(false);
        pAction2->setChecked(true);
    }
    connect(pAction1, &QAction::triggered, this, [=](bool checked){
        emit SendTextAnalysisSignal(154, QCursor::pos());
        if(m_pMyStation->IsReadOnly())
        {
            QMessageBox::information(nullptr, "注意-STPC/CTC3.0 通用车站操作终端","列车进路更新失败:\n没有权限","关闭");
            return;
        }
        if(pRouteOrder->m_bZHJL)
        {
            for(int i = 0; i < pRouteOrder->m_vecSonTrainRouteOrder.size(); i++)
            {
                emit sendRouteOrderMsg(MSGTYPE_SETTRIGGERTYPE, pRouteOrder->m_vecSonTrainRouteOrder.at(i)->route_id, 0xaa);
            }
        }
        else emit sendRouteOrderMsg(MSGTYPE_SETTRIGGERTYPE, pRouteOrder->route_id, 0xaa);
    });
    connect(pAction2, &QAction::triggered, this, [=](bool checked){
        emit SendTextAnalysisSignal(155, QCursor::pos());
        if(m_pMyStation->IsReadOnly())
        {
            QMessageBox::information(nullptr, "注意-STPC/CTC3.0 通用车站操作终端","列车进路更新失败:\n没有权限","关闭");
            return;
        }
        if(pRouteOrder->m_bZHJL)
        {
            for(int i = 0; i < pRouteOrder->m_vecSonTrainRouteOrder.size(); i++)
            {
                emit sendRouteOrderMsg(MSGTYPE_SETTRIGGERTYPE, pRouteOrder->m_vecSonTrainRouteOrder.at(i)->route_id, 0xbb);
            }
        }
        else emit sendRouteOrderMsg(MSGTYPE_SETTRIGGERTYPE, pRouteOrder->route_id, 0xbb);
    });
    pMenu->exec(QCursor::pos());
}
void TrackLabel::SetAutoTrigger_LeftButton(TrainRouteOrder* pRouteOrder)
{
    if(!pRouteOrder)
        return;

    QMenu* pMenu = new QMenu(this);
    pMenu->setStyleSheet("QMenu{"
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
                          "}"
                          "QMenu::item:!enabled {"
                              "color: rgb(180, 180, 180);"
                              "background-color: rgb(229, 241, 251);"
                          "}");

    //办理接车预告
    QAction* pAction1 = new QAction();
    pAction1->setText("设置自触");
    pAction1->setCheckable(true);
    pMenu->addAction(pAction1);
    QAction* pAction2 = new QAction();
    pAction2->setText("取消自触");
    pAction2->setCheckable(true);
    pMenu->addAction(pAction2);

    bool bAutoTouch = false;

    if(pRouteOrder->m_bZHJL == 0)
        bAutoTouch = pRouteOrder->m_nAutoTouch;
    else
    {
        for(int i = 0; i < pRouteOrder->m_vecSonTrainRouteOrder.size(); i++)
        {
            bAutoTouch |= pRouteOrder->m_vecSonTrainRouteOrder.at(i)->m_nAutoTouch;
        }
    }

    if(bAutoTouch)
    {
        pAction1->setChecked(true);
        pAction1->setEnabled(false);
        pAction2->setChecked(false);
    }
    else
    {
        pAction1->setChecked(false);
        pAction2->setEnabled(false);
        pAction2->setChecked(true);
    }
    connect(pAction1, &QAction::triggered, this, [=](bool checked){
        emit SendTextAnalysisSignal(47, QCursor::pos());
        if(m_pMyStation->IsReadOnly())
        {
            QMessageBox::information(nullptr, "注意-STPC/CTC3.0 通用车站操作终端","列车进路更新失败:\n没有权限","关闭");
            return;
        }
        if(pRouteOrder->m_bZHJL)
        {
            for(int i = 0; i < pRouteOrder->m_vecSonTrainRouteOrder.size(); i++)
            {
                emit sendRouteOrderMsg(MSGTYPE_SETTRIGGERTYPE, pRouteOrder->m_vecSonTrainRouteOrder.at(i)->route_id, 0xaa);
            }
        }
        else emit sendRouteOrderMsg(MSGTYPE_SETTRIGGERTYPE, pRouteOrder->route_id, 0xaa);
    });
    connect(pAction2, &QAction::triggered, this, [=](bool checked){
        emit SendTextAnalysisSignal(48, QCursor::pos());
        if(m_pMyStation->IsReadOnly())
        {
            QMessageBox::information(nullptr, "注意-STPC/CTC3.0 通用车站操作终端","列车进路更新失败:\n没有权限","关闭");
            return;
        }
        if(pRouteOrder->m_bZHJL)
        {
            for(int i = 0; i < pRouteOrder->m_vecSonTrainRouteOrder.size(); i++)
            {
                emit sendRouteOrderMsg(MSGTYPE_SETTRIGGERTYPE, pRouteOrder->m_vecSonTrainRouteOrder.at(i)->route_id, 0xbb);
            }
        }
        else emit sendRouteOrderMsg(MSGTYPE_SETTRIGGERTYPE, pRouteOrder->route_id, 0xbb);
    });
    pMenu->exec(QCursor::pos());
}

int TrackLabel::getMinimumWidth()
{
    int nFunBtnNum = 0;
    if(m_pMyStation->nFunBtnState & 0x8000) //上水
        nFunBtnNum++;
    if(m_pMyStation->nFunBtnState & 0x4000) //吸污
        nFunBtnNum++;
    if(m_pMyStation->nFunBtnState & 0x2000) //机车
        nFunBtnNum++;
    if(m_pMyStation->nFunBtnState & 0x1000) //列检
        nFunBtnNum++;
    if(m_pMyStation->nFunBtnState & 0x0800) //交令
        nFunBtnNum++;
    if(m_pMyStation->nFunBtnState & 0x0400) //交票
        nFunBtnNum++;
    if(m_pMyStation->nFunBtnState & 0x0200) //列尾
        nFunBtnNum++;
    if(m_pMyStation->nFunBtnState & 0x0100) //货检
        nFunBtnNum++;
    if(m_pMyStation->nFunBtnState & 0x80) //乘降
        nFunBtnNum++;
    if(m_pMyStation->nFunBtnState & 0x40) //摘挂
        nFunBtnNum++;
    if(m_pMyStation->nFunBtnState & 0x20) //换乘
        nFunBtnNum++;
    if(m_pMyStation->nFunBtnState & 0x10) //装卸
      nFunBtnNum++;
    if(m_pMyStation->nFunBtnState & 0x08) //道口
        nFunBtnNum++;
    if(m_pMyStation->nFunBtnState & 0x04) //车号
        nFunBtnNum++;
    if(m_pMyStation->nFunBtnState & 0x02) //站务
        nFunBtnNum++;
    if(m_pMyStation->nFunBtnState & 0x01) //综控
        nFunBtnNum++;

    return (nVewType == 1) ? (1052 + 36 * nFunBtnNum) : (1320 + 36 * nFunBtnNum);
}
