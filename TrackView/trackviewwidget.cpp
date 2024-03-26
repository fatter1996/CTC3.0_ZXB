#include "trackviewwidget.h"
#include "ui_trackviewwidget.h"
#include <QMessageBox>
#include <windows.h>

TrackViewWidget::TrackViewWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrackViewWidget)
{
    ui->setupUi(this);
    m_pPermissionsWidget = ui->tittleWidget;
    Timer_ID_500 = startTimer(500);
}

TrackViewWidget::~TrackViewWidget()
{
    delete ui;
}

void TrackViewWidget::Init(MyStation* pStation, int type, int maxRow, bool bMuit)
{
    m_nType = type;
    m_nMaxRow = maxRow;
    m_pMyStation = pStation;
    m_bMuit = bMuit;
    if(m_nType == 1)
    {
        m_pPermissionsWidget->hide();
        for(int i = 0; i < m_pMyStation->m_ArrayGDData.size(); i++)
        {
            StationStorageWidgetGD* pStorageWidget = new StationStorageWidgetGD();
            pStorageWidget->SetStationID(m_pMyStation->getStationID(), m_pMyStation->strAntislip, m_pMyStation->bSleepers);
            pStorageWidget->Init(false);
            pStorageWidget->InitMenu(false);
            pStorageWidget->InitGD(m_pMyStation->m_ArrayGDData.at(i));

            connect(pStorageWidget, &StationStorageWidgetGD::SendAntiSlipSignal, pStorageWidget, [=](GDData* pGDData,bool bSet, int nstationid){
                emit SendAntiSlipSignal(pGDData, bSet, nstationid);
            });
            connect(this, &TrackViewWidget::UpDataStationStorage, pStorageWidget, &StationStorageWidgetGD::onUpDataStationStorage);
            connect(pStorageWidget, &StationStorageWidgetGD::StationStorageChange, pStorageWidget, [=](){
               TrackLabelRefresh();
            });
            connect(pStorageWidget, &StationStorageWidgetGD::SendTextAnalysisSignal,[=](int index, QPoint pt, QString dev){
                emit this->SendTextAnalysisSignal(index, /*mapFromGlobal*/(pt), dev);
            });
            QVBoxLayout* pLayout = (QVBoxLayout*)ui->widget->layout();
            pLayout->insertWidget(i, pStorageWidget);
            pStorageWidget->SetWidgetBk(bColor);

            if(m_bMuit)
            {
                if(i == 0)
                    pStorageWidget->InsertStationName(m_pMyStation->m_strStationName);
                else pStorageWidget->InsertStationName("");
            }

            TrackLabelInfo info;
            info.nType = type;
            info.pGDData = m_pMyStation->m_ArrayGDData.at(i);
            info.pStorageWidget = pStorageWidget;
            info.bColor = bColor;
            m_vecTrackLabelInfo.append(info);
            bColor = !bColor;
        }

        bColor = !bColor;
    }
    else if(m_nType == 2)
    {
        m_pPermissionsWidget->Init(m_pMyStation, true, true);

        connect(m_pPermissionsWidget, &PermissionsWidget::applyForPermission, this, &TrackViewWidget::onApplyForPermission);
        connect(m_pPermissionsWidget, &PermissionsWidget::releasePermission, this, &TrackViewWidget::onReleasePermission);

        connect(m_pPermissionsWidget, &PermissionsWidget::DirectionDisplay, this, &TrackViewWidget::onDirectionDisplay);
        connect(m_pPermissionsWidget, &PermissionsWidget::ArriveDisplay, this, &TrackViewWidget::onArriveDisplay);
        connect(m_pPermissionsWidget, &PermissionsWidget::DepartureDisplay, this, &TrackViewWidget::onDepartureDisplay);

        connect(m_pPermissionsWidget, &PermissionsWidget::TurnToTrackView, [=](){
            bShowType = true;
            TrackWidget->show();
            RouteOrderWidget->hide();
        });
        connect(m_pPermissionsWidget, &PermissionsWidget::TurnToRouteOrderView, [=](){
            bShowType = false;
            TrackWidget->hide();
            RouteOrderWidget->show();
        });

        connect(this, &TrackViewWidget::SetLableSignal,m_pPermissionsWidget, &PermissionsWidget::SetLableSolt);
        connect(m_pPermissionsWidget, &PermissionsWidget::SendTextAnalysisSignal,[=](int index, QPoint pt, QString dev){
            emit this->SendTextAnalysisSignal(index, /*mapFromGlobal*/(pt), dev);
        });

        TrackWidget = new QWidget;
        QVBoxLayout* pLayout = new QVBoxLayout;
        pLayout->setMargin(0);
        pLayout->setSpacing(2);
        pSpacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Expanding);
        pLayout->addItem(pSpacer);

        TrackWidget->setLayout(pLayout);
        //TrackWidget->setFixedHeight(280);
        TrackWidget->setMinimumHeight(320);
        RouteOrderWidget = new TrainRouteOrderWidget;
        //RouteOrderWidget->setFixedHeight(280);
        RouteOrderWidget->setMinimumHeight(320);
        RouteOrderWidget->InitTableViewMenu(m_pMyStation);

        QVBoxLayout* pLayout2 = (QVBoxLayout*)ui->widget->layout();
        pLayout2->removeItem(ui->verticalSpacer);
        pLayout2->insertWidget(0, RouteOrderWidget);
        pLayout2->insertWidget(0, TrackWidget);
        //ui->widget->layout()->addWidget(RouteOrderWidget);
        //ui->widget->layout()->addWidget(TrackWidget);

        if(bShowType)
        {
            TrackWidget->show();
            RouteOrderWidget->hide();
        }
        else
        {
            TrackWidget->hide();
            RouteOrderWidget->show();
        }
        RouteOrderWidget->RouteOrderUpData();
        connect(RouteOrderWidget, &TrainRouteOrderWidget::SendTextAnalysisSignal,[=](int index, QPoint pt, QString dev){
            emit this->SendTextAnalysisSignal(index, /*mapFromGlobal*/(pt), dev);
        });

    }

    for(int i = 0; i < m_pMyStation->m_ArrayTrafficLog.size(); i++)
    {
        if(m_pMyStation->m_ArrayTrafficLog.at(i)->m_nExecuteFlag || m_pMyStation->m_ArrayTrafficLog.at(i)->m_bFinish)
            continue;
        Insert(TrackLabel::Create(m_pMyStation, m_pMyStation->m_ArrayTrafficLog.at(i), m_nType));
    }
}

void TrackViewWidget::Insert(TrackLabel* pTrackLabel, bool bUpData)
{
    if(!pTrackLabel)
        return;

    pTrackLabel->disconnect(); //先解绑所有关联槽,再重新绑定
    pTrackLabel->UpdataRouteOrder();

    TrafficLog* pTrafficLog = pTrackLabel->GetTrafficLog();
    if(!pTrafficLog)
        return;
    if(pTrafficLog->m_bFinish)
        return;

    if(!TrafficLogFilter(pTrafficLog))
        return;

    connect(pTrackLabel, &TrackLabel::SendTextAnalysisSignal,[=](int index, QPoint pt, QString dev){
        emit this->SendTextAnalysisSignal(index, /*mapFromGlobal*/(pt), dev);
    });

    connect(pTrackLabel, &TrackLabel::sendTrafficLogReportingMsg, [=](int logId, int type, QDateTime time){
        emit sendTrafficLogMsg(m_pMyStation->m_nStationID, logId, type, time);
    });
    connect(pTrackLabel, &TrackLabel::sendTrafficLogWorkMsg, [=](int logId, int type, int flag, QDateTime time){
        emit updataTrafficLogFlow(m_pMyStation->m_nStationID, logId, type, flag, time);
    });

    connect(pTrackLabel, &TrackLabel::workFlowFinish, [=](int time){
        killTimer(Timer_ID_5000);
        Timer_ID_5000 = startTimer(time);
    });

    connect(pTrackLabel, &TrackLabel::sendRouteOrderMsg, [=](int type, int routeId, int nData, QString strData){
        TrainRouteOrder* pTrainRouteOrder = m_pMyStation->GetTrainRouteOrderById(routeId);
        if((type == MSGTYPE_MANTRIGGER || type == MSGTYPE_SETTRIGGERTYPE) && pTrainRouteOrder->m_btRecvOrDepart == 0x01)
        {
            int ret = m_pMyStation->WorkFlowControl(type, pTrafficLog);
            if(ret == 0)
                emit sendRouteOrderMsg(m_pMyStation, type, routeId, nData, strData);
        }
        else emit sendRouteOrderMsg(m_pMyStation, type, routeId, nData, strData);
    });

    if(m_nType == 1)
    {
        QVBoxLayout* pLayout = (QVBoxLayout*)ui->widget->layout();
        TrackLabelInfo* pInfo = nullptr;
        for(int i = 0; i < m_vecTrackLabelInfo.size(); i++)
        {
            pInfo = &m_vecTrackLabelInfo[i];
            if(pInfo->pGDData->pGD->m_strName == pTrafficLog->m_strRecvTrainTrack ||
                pInfo->pGDData->pGD->m_strName == pTrafficLog->m_strDepartTrainTrack)
            {
                pInfo->nOrderNum++;
                if(pInfo->nOrderNum > m_nMaxRow)
                    break;

                pTrackLabel->UpdataTrackView();
                if((pInfo->nOrderNum == 1 || pInfo->pStorageWidget->GetTrackLabel() == nullptr) && !pInfo->pStorageWidget->IsHaveTrainMsg()) //第一条
                {
                    connect(pInfo->pStorageWidget, &StationStorageWidgetGD::FinishWorkFlow, pTrackLabel, [=](){
                        TrackLabelDelete(pTrackLabel);
                    });
                    pInfo->pStorageWidget->InitMenu(true);
                    pInfo->pStorageWidget->InsertTrackLabel(pTrackLabel);
                    pInfo->vecSubTrackLabel.push_back(pTrackLabel);
                }
                else
                {
                    StationStorageWidgetGD* pTempStorageWidget = new StationStorageWidgetGD();
                    pTempStorageWidget->Init(false);
                    pTempStorageWidget->InitGD(nullptr);
                    pTempStorageWidget->SetWidgetBk(pInfo->bColor);
                    connect(pTempStorageWidget, &StationStorageWidgetGD::FinishWorkFlow, pTrackLabel, [=](){
                        TrackLabelDelete(pTrackLabel);
                    });
                    bColor = !bColor;
                    pTempStorageWidget->InsertTrackLabel(pTrackLabel);
                    if(m_bMuit)
                        pTempStorageWidget->InsertStationName("");

                    pInfo->vecSubTrackLabel.push_back(pTrackLabel);

                    if(pInfo->pStorageWidget->IsHaveTrainMsg())
                        pLayout->insertWidget(pLayout->indexOf(pInfo->pStorageWidget) + pInfo->nOrderNum, pTempStorageWidget);
                    else pLayout->insertWidget(pLayout->indexOf(pInfo->pStorageWidget) + pInfo->nOrderNum - 1, pTempStorageWidget);

                    pInfo->m_vecSubStorage.push_back(pTempStorageWidget);
                }
                break;
            }
        }
    }
    else
    {
        QVBoxLayout* pLayout = (QVBoxLayout*)TrackWidget->layout();
        pTrackLabel->SetWidgetBk(bColor);
        pTrackLabel->UpdataTrackView();
        m_vecTrackLabel.push_back(pTrackLabel);
        pLayout->insertWidget(pLayout->indexOf(pSpacer), pTrackLabel);
        bColor = !bColor;
    }

    //pTrackLabel->UpdataRouteOrder();
    if(bUpData)
        return;

    for (int i = 0; i < m_pMyStation->StaConfigInfo.JFCKInfo.size(); i++)
    {
        MyStation::JFCK_T info = m_pMyStation->StaConfigInfo.JFCKInfo.at(i);

        //接车预告
        if((info.strJCKName == pTrafficLog->m_strXHD_JZk && ((info.bJCKAdvance&0x01) == 0)) ||
           (info.strFCKName == pTrafficLog->m_strXHD_JZk && ((info.bFCKAdvance&0x01) == 0)))
        {
            if(pTrafficLog->m_timAgrFromAdjtStaDepaTrain.isNull() && pTrafficLog->m_btBeginOrEndFlg != 0xBB)
                emit sendTrafficLogMsg(m_pMyStation->m_nStationID, pTrafficLog->log_id, 0x02, QDateTime::currentDateTime());
        }
        //发车预告
        if((info.strJCKName == pTrafficLog->m_strXHD_CZk && ((info.bJCKAdvance&0x02) == 0)) ||
           (info.strFCKName == pTrafficLog->m_strXHD_CZk && ((info.bFCKAdvance&0x02) == 0)))
        {
            if(pTrafficLog->m_timToAdjtStaAgrDepaTrain.isNull() && pTrafficLog->m_btBeginOrEndFlg != 0xCC)
                emit sendTrafficLogMsg(m_pMyStation->m_nStationID, pTrafficLog->log_id, 0x01, QDateTime::currentDateTime());
        }
    }

    //onTrainRouteOrderUpData(0, 0);
    //UpdataTrack();
}

void TrackViewWidget::TrackLabelClear()
{
    bColor = true;
    m_vecTrackLabelTemp[0] = nullptr;
    m_vecTrackLabelTemp[1] = nullptr;
    m_vecTrackLabelTemp[2] = nullptr;
    if(m_nType == 1)
    {
        QHBoxLayout* pLayout = (QHBoxLayout*)ui->widget->layout();
        TrackLabelInfo* pInfo = nullptr;
        for(int i = 0; i < m_vecTrackLabelInfo.size(); i++)
        {
            pInfo = &m_vecTrackLabelInfo[i];

            for(int j = 0; j < pInfo->m_vecSubStorage.size(); j++)
            {
                pInfo->m_vecSubStorage.at(j)->ClearTrackLabel();
                pLayout->removeWidget(pInfo->m_vecSubStorage.at(j));
                delete pInfo->m_vecSubStorage.at(j);
            }
            pInfo->nOrderNum = 0;
            nIndex = 1;
            pInfo->vecSubTrackLabel.clear();
            pInfo->m_vecSubStorage.clear();
            pInfo->pStorageWidget->ClearTrackLabel();
            pInfo->pStorageWidget->InitMenu(false);
        }
    }
    else
    {
        QVBoxLayout* pLayout = (QVBoxLayout*)TrackWidget->layout();
        for(int i = 0; i < m_vecTrackLabel.size(); i++)
        {
            pLayout->removeWidget(m_vecTrackLabel.at(i));
            delete m_vecTrackLabel.at(i);
        }
        m_vecTrackLabel.clear();
    }
}

void TrackViewWidget::TrackLabelDelete(TrackLabel* pDelTrackLabel)
{
    pDelTrackLabel->GetTrafficLog()->m_bFinish = true;
    TrackLabelRefresh();
}

void TrackViewWidget::TrackLabelRefresh()
{
    QVector<TrackLabel*> vecTempTrackLabel = takeAllTrackLabel();
    //if(vecTempTrackLabel.size() == 0)
    //    return;
    QVector<TrackLabel*> vecTempTrackLabel2;
    TrackLabel* pTrackLabel = nullptr;
    for(int i = 0; i < m_pMyStation->m_ArrayTrafficLog.size(); i++)
    {
        //if(m_pMyStation->m_ArrayTrafficLog.at(i)->m_bFinish || m_pMyStation->m_ArrayTrafficLog.at(i)->m_nExecuteFlag)
        //    continue;
        for(int j = 0; j < vecTempTrackLabel.size(); j++)
        {
            if(vecTempTrackLabel.at(j)->GetTrafficLog() == m_pMyStation->m_ArrayTrafficLog.at(i))
            {
                pTrackLabel = vecTempTrackLabel.at(j);
                //安全删除对应元素
                for(int k = 0; k < vecTempTrackLabel.size(); k++)
                {
                    if(k == j)
                        continue;
                    vecTempTrackLabel2.push_back(vecTempTrackLabel.at(k));
                }
                vecTempTrackLabel.swap(vecTempTrackLabel2);
                QVector<TrackLabel*> vecTemp;
                vecTempTrackLabel2.swap(vecTemp);
                break;
            }
        }
        if(pTrackLabel)
            Insert(pTrackLabel);
        else Insert(TrackLabel::Create(m_pMyStation, m_pMyStation->m_ArrayTrafficLog.at(i), m_nType));
        pTrackLabel = nullptr;
    }

    for(int j = 0; j < vecTempTrackLabel.size(); j++)
    {
        if(vecTempTrackLabel.at(j))
        {
            for(int k = 0; k < 3; k++)
            {
                if(m_vecTrackLabelTemp[k] == vecTempTrackLabel.at(j))
                {
                    m_vecTrackLabelTemp[k] = nullptr;
                    break;
                }
            }

            delete vecTempTrackLabel.at(j);
            vecTempTrackLabel[j] = nullptr;
        }
    }
    UpdataTrack();
}

QVector<TrackLabel*> TrackViewWidget::takeAllTrackLabel()
{
    QVector<TrackLabel*> vecTrackLabel;
    if(m_nType == 1)
    {
        TrackLabelInfo* pInfo = nullptr;
        TrackLabel* pTrackLabel = nullptr;
        QVBoxLayout* pLayout = (QVBoxLayout*)ui->widget->layout();

        for(int i = 0; i < m_vecTrackLabelInfo.size(); i++)
        {
            pInfo = &m_vecTrackLabelInfo[i];
            pTrackLabel = pInfo->pStorageWidget->takeTrackLabel();
            pInfo->pStorageWidget->InitMenu(false);
            if(pTrackLabel)
                vecTrackLabel.push_back(pTrackLabel);
            for(int j = 0; j < pInfo->m_vecSubStorage.size(); j++)
            {
                pTrackLabel = pInfo->m_vecSubStorage.at(j)->takeTrackLabel();
                pTrackLabel->setParent(nullptr);
                if(pTrackLabel)
                    vecTrackLabel.push_back(pTrackLabel);
                pLayout->removeWidget(pInfo->m_vecSubStorage.at(j));
                delete pInfo->m_vecSubStorage.at(j);
                pInfo->m_vecSubStorage[j] = nullptr;
            }
            pInfo->nOrderNum = 0;
            nIndex = 1;
            QVector<TrackLabel*> vecTemp;
            pInfo->vecSubTrackLabel.swap(vecTemp);
            QVector<StationStorageWidgetGD*> vecTemp2;
            pInfo->m_vecSubStorage.swap(vecTemp2);
        }
    }
    else
    {
        QVBoxLayout* pLayout = (QVBoxLayout*)TrackWidget->layout();
        for(int i = 0; i < m_vecTrackLabel.size(); i++)
        {
            pLayout->takeAt(pLayout->indexOf(m_vecTrackLabel.at(i)));
            vecTrackLabel.push_back(m_vecTrackLabel.at(i));
        }
        QVector<TrackLabel*> vecTemp;
        m_vecTrackLabel.swap(vecTemp);
    }
    return vecTrackLabel;
}

TrackLabel* TrackViewWidget::takeTrackLabelByTrafficLog(int logId)
{
    if(m_nType == 1)
    {
        TrackLabelInfo* pInfo = nullptr;
        TrackLabel* pTrackLabel = nullptr;
        //int index = 0;
        for(int i = 0; i < m_vecTrackLabelInfo.size(); i++)
        {
            pInfo = &m_vecTrackLabelInfo[i];
            for(int j = 0; j < pInfo->vecSubTrackLabel.size(); j++)
            {
                if(pInfo->vecSubTrackLabel.at(j)->GetTrafficLog()->log_id == logId)
                {
                    if(pInfo->pStorageWidget->GetTrackLabel() == pInfo->vecSubTrackLabel.at(j))
                    {
                        if(pInfo->pStorageWidget->IsHaveTrainMsg() && pInfo->m_vecSubStorage.size() > 0)
                        {
                            pTrackLabel = pInfo->m_vecSubStorage.at(0)->takeTrackLabel();
                            if(pTrackLabel)
                                pTrackLabel->setParent(nullptr);
                            delete pInfo->m_vecSubStorage.at(0);
                            pInfo->m_vecSubStorage.remove(0);
                        }
                        else
                        {
                            pTrackLabel = pInfo->pStorageWidget->takeTrackLabel();
                            if(pTrackLabel)
                                pTrackLabel->setParent(nullptr);
                        }
                        pInfo->pStorageWidget->InitMenu(false);
                    }
                    else
                    {
                        for (int k = 0; k < pInfo->m_vecSubStorage.size(); k++)
                        {
                            if(pInfo->m_vecSubStorage.at(k)->GetTrackLabel()->GetTrafficLog()->log_id == logId)
                            {
                                pTrackLabel = pInfo->m_vecSubStorage.at(k)->takeTrackLabel();

                                if(pTrackLabel)
                                    pTrackLabel->setParent(nullptr);
                                delete pInfo->m_vecSubStorage.at(k);
                                pInfo->m_vecSubStorage.remove(k);
                                break;
                            }
                        }
                    }
                    pInfo->vecSubTrackLabel.remove(j);
                    pInfo->nOrderNum -= 1;
                    return pTrackLabel;
                }
            }
        }
    }
    else
    {
        QVBoxLayout* pLayout = (QVBoxLayout*)TrackWidget->layout();
        for(int i = 0; i < m_vecTrackLabel.size(); i++)
        {
            if(m_vecTrackLabel.at(i)->GetTrafficLog()->log_id == logId)
            {
                pLayout->takeAt(pLayout->indexOf(m_vecTrackLabel.at(i)));
                return m_vecTrackLabel.at(i);
            }
        }
    }
    return nullptr;
}

bool TrackViewWidget::TrafficLogFilter(TrafficLog* pTrafficLog)
{
    bool bFilter = true;
    //上下行
    bFilter &= nDirection == 0 ||
            (pTrafficLog->m_bReachTrainNumSX && nDirection == 1) ||
            (!pTrafficLog->m_bReachTrainNumSX && nDirection == 2);

    //接车口
    bFilter &= nJCXHDCode == 0 || (pTrafficLog->m_nCodeReachStaEquip == nJCXHDCode);
    //发车口
    bFilter &= nFCXHDCode == 0 || (pTrafficLog->m_nCodeDepartStaEquip == nFCXHDCode);
    return bFilter;
}

void TrackViewWidget::UpdataTrack()
{
    if(m_nType != 1)
        return;

    QString strTrainNumTop3[3] = { "" };

    for (int i = 0; i < m_pMyStation->m_ArrayRouteOrder.size(); i++)
    {
        if(m_pMyStation->m_ArrayRouteOrder.at(i)->m_nOldRouteState == 4)
            continue;

        if(strTrainNumTop3[0] == "")
        {
            strTrainNumTop3[0] = m_pMyStation->m_ArrayRouteOrder.at(i)->m_strTrainNum;
            continue;
        }

        if(strTrainNumTop3[1] == "")
        {
            if(m_pMyStation->m_ArrayRouteOrder.at(i)->m_strTrainNum != strTrainNumTop3[0])
                strTrainNumTop3[1] = m_pMyStation->m_ArrayRouteOrder.at(i)->m_strTrainNum;
            continue;
        }

        if(strTrainNumTop3[2] == "")
        {
            if(m_pMyStation->m_ArrayRouteOrder.at(i)->m_strTrainNum != strTrainNumTop3[0] &&
               m_pMyStation->m_ArrayRouteOrder.at(i)->m_strTrainNum != strTrainNumTop3[1])
                strTrainNumTop3[2] = m_pMyStation->m_ArrayRouteOrder.at(i)->m_strTrainNum;
            continue;
        }
    }


    for(int i = 0; i < 3; i++)
    {
        if(m_vecTrackLabelTemp[i])
        {
            m_vecTrackLabelTemp[i]->UpdataTrackIndex();
            m_vecTrackLabelTemp[i] = nullptr;
        }

    }

    TrackLabel* pTrackLabel = nullptr;
    for(int i = 0; i < 3; i++)
    {
        pTrackLabel = GetTrackLabelByTrainNum(strTrainNumTop3[i]);
        m_vecTrackLabelTemp[i] = pTrackLabel;
        if(pTrackLabel)
            pTrackLabel->UpdataTrackIndex(i + 1);
    }
}

void TrackViewWidget::timerEvent(QTimerEvent* enent)
{
    if(enent->timerId() == Timer_ID_500) //权限信息监控,更新
    {
        if(m_pMyStation)
        {
            if(m_nType != 1)
            {
                if(curPermission != m_pMyStation->m_nRoutePermission || curFCZKMode != m_pMyStation->m_nFCZKMode
                        || curModeState != m_pMyStation->m_nModeState)
                {
                    curPermission = m_pMyStation->m_nRoutePermission;
                    curFCZKMode = m_pMyStation->m_nFCZKMode;
                    curModeState = m_pMyStation->m_nModeState;
                    emit SetLableSignal(m_pMyStation->m_nRoutePermission == 2 && !m_pMyStation->m_nFCZKMode && m_pMyStation->m_nModeState == 1);
                }
            }
        }
    }
    if(enent->timerId() == Timer_ID_500_2) //重新加载
    {
        TrackLabelRefresh();
        RouteOrderWidget->RouteOrderUpData();
        killTimer(Timer_ID_500_2);
    }
    if(enent->timerId() == Timer_ID_5000)
    {
        TrackLabelRefresh();
        killTimer(Timer_ID_5000);
    }
}

void TrackViewWidget::onTrafficLogUpData(int type, int logId)
{
    if(type == SYNC_FLAG_ADD)
        Insert(TrackLabel::Create(m_pMyStation, m_pMyStation->GetTrafficLogById(logId), m_nType));
    else if(type == SYNC_FLAG_DELALL)
        TrackLabelClear();
    else if(type == SYNC_FLAG_DELETE)
        TrackLabelRefresh();
    else if(type == SYNC_FLAG_UPDATE)
    {
        TrackLabel* pTrackLabel = nullptr;
        if(m_nType == 1)
        {
            pTrackLabel = takeTrackLabelByTrafficLog(logId);
            if(pTrackLabel)
                Insert(pTrackLabel, true);

            if(!pTrackLabel)
                Insert(TrackLabel::Create(m_pMyStation, m_pMyStation->GetTrafficLogById(logId), m_nType));
        }
        else
        {
            RouteOrderWidget->RouteOrderUpData();
            for(int i = 0; i < m_vecTrackLabel.size(); i++)
            {
                if(m_vecTrackLabel.at(i)->GetTrafficLog()->log_id == logId)
                {
                    pTrackLabel = m_vecTrackLabel.at(i);
                    pTrackLabel->UpdataTrackView();
                }
            }
            if(!pTrackLabel)
                Insert(TrackLabel::Create(m_pMyStation, m_pMyStation->GetTrafficLogById(logId), m_nType));
        }
    }

    UpdataTrack();
}

void TrackViewWidget::onTrainRouteOrderUpData(int type, int route_id)
{
    TrackLabel* pTrackLabel = nullptr;
    if(m_nType == 1)
    {
        for(int i = 0; i < m_vecTrackLabelInfo.size(); i++)
        {
            for(int j = 0; j < m_vecTrackLabelInfo.at(i).vecSubTrackLabel.size(); j++)
            {
                pTrackLabel = m_vecTrackLabelInfo.at(i).vecSubTrackLabel.at(j);
                pTrackLabel->UpdataRouteOrder();
            }
        }
    }
    else
    {
        RouteOrderWidget->RouteOrderUpData();
        for(int i = 0; i < m_vecTrackLabel.size(); i++)
        {
            pTrackLabel = m_vecTrackLabel.at(i);
            pTrackLabel->UpdataRouteOrder();
        }
    }
    TrackLabelRefresh();
}

void TrackViewWidget::onGDSignalUpData()
{
    if(m_nType != 1)
        return;
    emit UpDataStationStorage();
}

//修改申请（刷新）
void TrackViewWidget::onApplyForPermission()
{
    if(m_pMyStation)
    {
        //申请权限
        emit sendRouteOrderMsg(m_pMyStation, MSGTYPE_PERMISSION, 0, 0xaa);
        TrackLabelClear();
        Timer_ID_500_2 = startTimer(500);
    }
}
//只读申请（释放权限）
void TrackViewWidget::onReleasePermission()
{
    if(m_pMyStation && (m_pMyStation->m_nRoutePermission == 2))
    {
        //只读
        int ret = QMessageBox::information(nullptr, tr("注意-STPC/CTC3.0 通用车站操作终端"), tr("只读申请后,将无法对进路序列进行修改,确定要只读申请吗?"), tr("确定"), tr("取消"), "", 0);
        if(ret == 0)
        {
            emit sendRouteOrderMsg(m_pMyStation, MSGTYPE_PERMISSION, 0, 0xbb);
            TrackLabelRefresh();
            RouteOrderWidget->RouteOrderUpData();
        }
    }
}

//上下行
void TrackViewWidget::onDirectionDisplay(int type)
{
    nDirection = type; //上下行
    RouteOrderWidget->SetRouteOrderTableShowSet(0, type);
    TrackLabelRefresh();
    RouteOrderWidget->RouteOrderUpData();
}
//接车口
void TrackViewWidget::onArriveDisplay(int code)
{
    nJCXHDCode = code; //接车口
    RouteOrderWidget->SetRouteOrderTableShowSet(1, code);
    TrackLabelRefresh();
    RouteOrderWidget->RouteOrderUpData();
}
//发车口
void TrackViewWidget::onDepartureDisplay(int code)
{
    nFCXHDCode = code;  //发车口
    RouteOrderWidget->SetRouteOrderTableShowSet(2, code);
    TrackLabelRefresh();
    RouteOrderWidget->RouteOrderUpData();
}

TrackLabel* TrackViewWidget::GetTrackLabelByTrainNum(QString strTrainNum)
{
    if(m_nType == 1)
    {
        TrackLabelInfo* pInfo = nullptr;
        for(int i = 0; i < m_vecTrackLabelInfo.size(); i++)
        {
            pInfo = &m_vecTrackLabelInfo[i];
            for(int j = 0; j < pInfo->vecSubTrackLabel.size(); j++)
            {
                if(pInfo->vecSubTrackLabel.at(j)->GetTrafficLog()->m_strTrainNum == strTrainNum)
                    return pInfo->vecSubTrackLabel.at(j);
            }
        }
        return nullptr;
    }
    else
    {
        for(int i = 0; i < m_vecTrackLabel.size(); i++)
        {
            if(m_vecTrackLabel.at(i)->GetTrafficLog()->m_strTrainNum == strTrainNum)
                return m_vecTrackLabel.at(i);
        }
        return nullptr;
    }
}

int TrackViewWidget::getMinimumWidth()
{
    int maxMinimumWidth = 0;
    if(m_nType == 1)
    {
        TrackLabelInfo* pInfo = nullptr;
        for(int i = 0; i < m_vecTrackLabelInfo.size(); i++)
        {
            pInfo = &m_vecTrackLabelInfo[i];
            for(int j = 0; j < pInfo->vecSubTrackLabel.size(); j++)
            {
                maxMinimumWidth = ((pInfo->vecSubTrackLabel.at(j)->getMinimumWidth() + 232) > maxMinimumWidth) ?
                                    (pInfo->vecSubTrackLabel.at(j)->getMinimumWidth() + 232) : maxMinimumWidth;
            }
        }
    }
    else
    {
        for(int i = 0; i < m_vecTrackLabel.size(); i++)
        {
            maxMinimumWidth = (m_vecTrackLabel.at(i)->getMinimumWidth() > maxMinimumWidth) ?
                                m_vecTrackLabel.at(i)->getMinimumWidth() : maxMinimumWidth;
        }
    }
    return maxMinimumWidth;
}
