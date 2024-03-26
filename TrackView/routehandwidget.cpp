#include "routehandwidget.h"
#include "ui_routehandwidget.h"
#include <windows.h>
#include <QMessageBox>

#define DIPLOID_JLYL 0.6

RouteHandWidget::RouteHandWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RouteHandWidget)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("进路办理 - TDCS/CTC3.0 通用车站操作终端"));
    this->setWindowFlags(Qt::Dialog);
    this->setWindowModality(Qt::ApplicationModal);//设置模态
    this->setAttribute(Qt::WA_DeleteOnClose);//关闭后自动释放窗口
    ui->handThroughBtn->hide();
    ui->tableWidget->hide();
    ui->radioButton->setEnabled(false);
    ui->handRouteBtn->installEventFilter(this);
    ui->StationView->installEventFilter(this);
    ui->handThroughBtn->installEventFilter(this);
    ui->handThroughBtn->setMouseTracking(true);
    m_nTimerId_500 = startTimer(500);
}

RouteHandWidget::~RouteHandWidget()
{
    //emit sendRouteOrderMsg(MSGTYPE_DJKX, m_pRouteOrder->route_id, 0xbb);
    for(int i = 0; i < m_pMyStation->DevArray.size(); i++)
    {
        if(m_pMyStation->DevArray.at(i)->getDevType() == Dev_GD)
        {
            m_pMyStation->DevArray.at(i)->setState(DCDW);
        }
    }
    killTimer(m_nTimerId_500);
    delete ui;
}

void RouteHandWidget::InitTableWidget()
{
    ui->tableWidget->verticalHeader()->setMinimumWidth(32);
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableWidget->horizontalHeader()->setVisible(true);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->tableWidget->setSelectionBehavior(QTableWidget::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //更新表格行数
    ui->tableWidget->setColumnWidth(0, 92);
    ui->tableWidget->setColumnWidth(1, 48);
    ui->tableWidget->setColumnWidth(2, 56);
    ui->tableWidget->viewport()->installEventFilter(this);
    ui->tableWidget->viewport()->setMouseTracking(true);
}

void RouteHandWidget::Init(MyStation* pMyStation, TrainRouteOrder* pRouteOrder, bool bAdvance, bool bFinished)
{
    m_pMyStation = pMyStation;
    m_bZHJL = pRouteOrder->m_bZHJL;
    m_pRouteOrder = pRouteOrder;

    int width = m_pMyStation->StaConfigInfo.initialSize.width() * DIPLOID_JLYL;
    int height = m_pMyStation->StaConfigInfo.initialSize.height() * DIPLOID_JLYL;

    ui->StationView->setFixedSize(1068 > width ? 1068 : width,
                 704 > height ? 704 : height);

    if(!bAdvance)
        ui->handRouteBtn->setEnabled(false);

    if(bFinished)
    {
        ui->handRouteBtn->setEnabled(false);
        ui->cancelBtn->setEnabled(false);
        ui->radioButton->setChecked(m_bZHJL);
        ui->tableWidget->setVisible(m_bZHJL);
        ui->radioButton->setEnabled(false);
        return;
    }

    //判断进路是否触发
    if(m_pRouteOrder->m_nOldRouteState != 0)
    {
        ui->handRouteBtn->setEnabled(false);
        ui->radioButton->setChecked(m_bZHJL);
        ui->tableWidget->setVisible(m_bZHJL);
        ui->radioButton->setEnabled(false);
        if(m_pRouteOrder->m_nOldRouteState != 4)
            ui->cancelBtn->setEnabled(true);
    }
    if(m_bZHJL)
    {
        InitTableWidget();
        ui->tableWidget->setRowCount(m_pRouteOrder->m_vecSonTrainRouteOrder.size());
        ui->radioButton->setChecked(false);
        ui->tableWidget->setVisible(false);
        ui->radioButton->setEnabled(true);
        TableWidgetUpData();
        connect(ui->tableWidget, &QTableWidget::itemClicked, this, &RouteHandWidget::onItemClicked);
    }


    //变通进路
    if(m_pRouteOrder->m_bZHJL != 1)
    {
        QMenu* pMenu = new QMenu();
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
        QActionGroup* group = new QActionGroup(pMenu);
        group->setExclusive(true);
        for(int i = 0; i < m_pRouteOrder->strArrayBTJL.size(); i++)
        {
            QString str = m_pRouteOrder->strArrayBTJL.at(i);
            str.replace(',', '-');
            QAction* pAction = new QAction(str, group);
            pAction->setCheckable(true);
            pAction->setChecked(false);
            pMenu->addAction(pAction);
            if(str == m_pRouteOrder->m_strRouteDescrip)
                pAction->setChecked(true);
            else pAction->setChecked(false);

            connect(pAction, &QAction::triggered, [=](){
                emit SendTextAnalysisSignal(151, QCursor::pos());
                if(pMyStation->IsReadOnly())
                {
                    QMessageBox::information(nullptr, "注意-STPC/CTC3.0 通用车站操作终端","列车进路更新失败:\n没有权限","关闭");
                    pAction->setChecked(false);
                    for(auto action : pMenu->actions())
                    {
                        if(action->text() == m_pRouteOrder->m_strRouteDescrip)
                            action->setChecked(true);
                    }
                    return;
                }
                QString str = m_pRouteOrder->strArrayBTJL.at(i);
                emit sendRouteOrderMsg(MSGTYPE_ROUTEDESCRIP, m_pRouteOrder->route_id, str.length(), str);
            });
        }
        pMenu->installEventFilter(this);
        ui->resetBtn->setMenu(pMenu);
    }
    else ui->resetBtn->hide();

    RouteOrderInfoUpData(bAdvance);
}

void RouteHandWidget::RouteOrderInfoUpData(bool bAdvance)
{
    QString strType;
    QString direction;
    QString descrip;
    if(m_pRouteOrder->m_btRecvOrDepart == 1)
    {
        strType = "发车";
        mouseActionNum_banli=169;
        mouseActionNum_quxiao=170;
        ui->radioButton_3->show();
    }
    else
    {
        strType = "接车";
        mouseActionNum_banli=147;
        mouseActionNum_quxiao=148;
        ui->radioButton_3->hide();
    }
    direction = m_pRouteOrder->m_strDirection;
    direction.replace("->", "");
    direction.replace("<-", "");

    if(!m_bZHJL)
        descrip = m_pRouteOrder->m_strRouteDescrip;
    else
    {
        TrainRouteOrder* pSubRouteOrder = nullptr;
        for(int i = 0; i < m_pRouteOrder->m_vecSonTrainRouteOrder.size(); i++)
        {
            pSubRouteOrder = m_pRouteOrder->m_vecSonTrainRouteOrder.at(i);
            if(pSubRouteOrder->m_bZHJL == 0)
            {
                descrip += pSubRouteOrder->m_strRouteDescrip;
                if(i < m_pRouteOrder->m_vecSonTrainRouteOrder.size() - 1)
                    descrip += ";";
            }
        }
    }

    ui->trainNumLabel->setText(m_pRouteOrder->m_strTrainNum);
    ui->routeInfoEdit->setText(QString("%1 车次:%2 股道:%3 %1方向:%4 进路按钮:%5")
                               .arg(strType).arg(m_pRouteOrder->m_strTrainNum).arg(m_pRouteOrder->m_strTrack)
                               .arg(direction).arg(descrip));
    ui->resetBtn->setText(QString("修改%1进路").arg(strType));
    ui->handRouteBtn->setText(QString("办理%1").arg(strType));

    ui->radioButton_4->setChecked(m_pRouteOrder->m_nAutoTouch);
    ui->radioButton_4->setEnabled(m_pRouteOrder->m_nOldRouteState == 0 || m_pRouteOrder->m_nOldRouteState == 5);

    if(m_bZHJL)
    {
        bool bTouch = true;
        bool bAutoTouch = true;
        bool bAutoTouch2 = false;
        bool bOperate = false;
        for(int i = 0; i < m_pRouteOrder->m_vecSonTrainRouteOrder.size(); i++)
        {
            bTouch &= (m_pRouteOrder->m_vecSonTrainRouteOrder.at(i)->m_nOldRouteState != 0 && m_pRouteOrder->m_vecSonTrainRouteOrder.at(i)->m_nOldRouteState != 5);
            bOperate |= (m_pRouteOrder->m_vecSonTrainRouteOrder.at(i)->m_nOldRouteState != 0 && m_pRouteOrder->m_vecSonTrainRouteOrder.at(i)->m_nOldRouteState != 5);
            bAutoTouch &= m_pRouteOrder->m_vecSonTrainRouteOrder.at(i)->m_nAutoTouch;
            bAutoTouch2 |= m_pRouteOrder->m_vecSonTrainRouteOrder.at(i)->m_nAutoTouch;
        }
        if(bAutoTouch)
        {
            ui->handRouteBtn->setStyleSheet("QPushButton{background-color: rgb(255, 170, 0);}"
                                            "QPushButton:hover{background-color: rgb(240, 170, 20);}"
                                            "QPushButton:pressed{background-color: rgb(240, 160, 25);}");
            ui->radioButton_4->setStyleSheet("background-color: rgb(255, 170, 0);");
        }
        else
        {
            ui->handRouteBtn->setStyleSheet("");
            ui->radioButton_4->setStyleSheet("");
        }

        ui->radioButton_4->setChecked(bAutoTouch2);
        ui->radioButton_4->setEnabled(!bTouch);
        ui->handRouteBtn->setEnabled(!bTouch && bAdvance);
        if(bOperate)
        {
            ui->radioButton->setChecked(true);
            ui->radioButton->setEnabled(false);
            ui->handRouteBtn->setEnabled(false);
            ui->tableWidget->setVisible(true);
        }
    }
    else
    {
        if(m_pRouteOrder->m_nAutoTouch)
        {
            ui->handRouteBtn->setStyleSheet("QPushButton{background-color: rgb(255, 170, 0);}"
                                            "QPushButton:hover{background-color: rgb(240, 170, 20);}"
                                            "QPushButton:pressed{background-color: rgb(240, 160, 25);}");
            ui->radioButton_4->setStyleSheet("background-color: rgb(255, 170, 0);");
        }
        else
        {
            ui->handRouteBtn->setStyleSheet("");
            ui->radioButton_4->setStyleSheet("");
        }
        ui->handRouteBtn->setEnabled((m_pRouteOrder->m_nOldRouteState == 0 || m_pRouteOrder->m_nOldRouteState == 5) && bAdvance);
    }

    if(m_pMyStation->IsReadOnly())
        ui->radioButton_4->setEnabled(false);

    ui->cancelBtn->setText(QString("取消%1").arg(strType));

    QString path;
    if(m_pRouteOrder->m_bZHJL && ui->radioButton->isChecked() && pSelectRouteOrder)
    {
        path = QString("border-image:url(./data/bmp/%1/%2.bmp)").arg(m_pRouteOrder->station_id).arg(pSelectRouteOrder->m_strRouteDescrip);
        m_strCurRouteDescrip = pSelectRouteOrder->m_strRouteDescripReal;
        m_strShowRouteDescrip = pSelectRouteOrder->m_strRouteDescripReal;
    }
    else
    {
        path = QString("border-image:url(./data/bmp/%1/%2.bmp)").arg(m_pRouteOrder->station_id).arg(m_pRouteOrder->m_strRouteDescrip);
        m_strCurRouteDescrip = m_pRouteOrder->m_strRouteDescripReal;
        m_strShowRouteDescrip = m_pRouteOrder->m_strRouteDescripReal;
    }
    //ui->frame->setStyleSheet(path);

}

void RouteHandWidget::TableWidgetUpData()
{
    int isAutotouch = 0;
    TrainRouteOrder* pSubRouteOrder = nullptr;
    for(int i = 0; i < m_pRouteOrder->m_vecSonTrainRouteOrder.size(); i++)
    {
        pSubRouteOrder = m_pRouteOrder->m_vecSonTrainRouteOrder.at(i);
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(pSubRouteOrder->m_strRouteDescrip));
        if(pSubRouteOrder->m_nAutoTouch)
            ui->tableWidget->setItem(i, 1, new QTableWidgetItem(" √ "));
        else ui->tableWidget->setItem(i, 1, new QTableWidgetItem("  "));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(pSubRouteOrder->m_strRouteState));
        isAutotouch += pSubRouteOrder->m_nAutoTouch;
    }
    if(isAutotouch == m_pRouteOrder->m_vecSonTrainRouteOrder.size())
    {
        ui->handRouteBtn->setStyleSheet("QPushButton{background-color: rgb(255, 170, 0);}"
                                        "QPushButton:hover{background-color: rgb(240, 170, 20);}"
                                        "QPushButton:pressed{background-color: rgb(240, 160, 25);}");
        ui->radioButton_4->setStyleSheet("background-color: rgb(255, 170, 0);");
    }
    else
    {
        ui->handRouteBtn->setStyleSheet("");
        ui->radioButton_4->setStyleSheet("");
    }
}

void RouteHandWidget::RouteUpData(bool bAdvance, bool bFinished)
{
    if(!m_pRouteOrder)
        return;

    RouteOrderInfoUpData(bAdvance);

    if(m_bZHJL)
    {
        if(pSelectRouteOrder && pSelectRouteOrder->m_nOldRouteState != 0 && pSelectRouteOrder->m_nOldRouteState != 5)
        {
            ui->handRouteBtn->setEnabled(false);
            ui->radioButton->setEnabled(false);
        }
        TableWidgetUpData();
    }
}

void RouteHandWidget::SetThroughRouteOrder(QVector<TrainRouteOrder*> vecThroughRouteOrder)
{
    if(m_pRouteOrder->m_btRecvOrDepart == 1)
        return;

    ui->handThroughBtn->show();
    m_vecThroughRouteOrder.swap(vecThroughRouteOrder);
}

void RouteHandWidget::onItemClicked(QTableWidgetItem *item)
{
    TrainRouteOrder* pSubRouteOrder = nullptr;
    for(int i = 0; i < m_pRouteOrder->m_vecSonTrainRouteOrder.size(); i++)
    {
        pSubRouteOrder = m_pRouteOrder->m_vecSonTrainRouteOrder.at(i);
        if(pSubRouteOrder->m_strRouteDescrip == ui->tableWidget->item(item->row(), 0)->text())
        {
            pSelectRouteOrder = pSubRouteOrder;
            break;
        }
    }

    if(pSelectRouteOrder->m_nOldRouteState != 0 && pSelectRouteOrder->m_nOldRouteState != 5)
    {
        ui->radioButton->setEnabled(false);
        ui->cancelBtn->setEnabled(true);
        ui->handRouteBtn->setEnabled(false);
    }
    else
    {
        ui->radioButton->setEnabled(true);
        ui->cancelBtn->setEnabled(false);
        ui->handRouteBtn->setEnabled(true);
    }

    QString str = QString("border-image:url(./data/bmp/%1/%2.bmp)").arg(m_pMyStation->m_nStationID).arg(ui->tableWidget->item(item->row(), 0)->text());
    //ui->frame->setStyleSheet(QString("border-image:url(./data/bmp/%1/%2.bmp)").arg(m_pMyStation->m_nStationID).arg(ui->tableWidget->item(item->row(), 0)->text()));
    m_strCurRouteDescrip = pSelectRouteOrder->m_strRouteDescripReal;
    m_strShowRouteDescrip = pSelectRouteOrder->m_strRouteDescripReal;
}

void RouteHandWidget::on_handRouteBtn_clicked() //办理进路
{
    emit SendTextAnalysisSignal(mouseActionNum_banli, QCursor::pos());
    if(ui->radioButton->isChecked() && pSelectRouteOrder)
        emit sendRouteOrderMsg(MSGTYPE_MANTRIGGER, pSelectRouteOrder->route_id, 0xaa);
    else if(m_bZHJL)
    {
        for(int i = 0; i < m_pRouteOrder->m_vecSonTrainRouteOrder.size(); i++)
        {
            emit sendRouteOrderMsg(MSGTYPE_MANTRIGGER, m_pRouteOrder->m_vecSonTrainRouteOrder.at(i)->route_id, 0xaa);
        }
        //emit sendRouteOrderMsg(MSGTYPE_MANTRIGGER, m_pRouteOrder->route_id, 0xaa);
    }
    else emit sendRouteOrderMsg(MSGTYPE_MANTRIGGER, m_pRouteOrder->route_id, 0xaa);

    Sleep(500);
    this->close();
}

void RouteHandWidget::on_cancelBtn_clicked() //取消进路
{
    emit SendTextAnalysisSignal(mouseActionNum_quxiao, QCursor::pos());
    QString descrip;
    if(m_bZHJL)
    {
        if(ui->radioButton->isChecked() && pSelectRouteOrder)
            descrip = pSelectRouteOrder->m_strRouteDescrip;
        else descrip = m_pRouteOrder->m_strRouteDescrip;
    }
    else descrip = m_pRouteOrder->m_strRouteDescrip;
    int ret = QMessageBox::information(
                nullptr, "注意-TDCS/CTC3.0",
                QString("确认取消:%1次进路(%2)").arg(m_pRouteOrder->m_strTrainNum).arg(descrip),
                "是", "否", "", 0);
    if(ret == 0)
    {
        if(ui->radioButton->isChecked() && pSelectRouteOrder)
        {

            emit sendRouteOrderMsg(MSGTYPE_CANCEL, pSelectRouteOrder->route_id, m_pMyStation->GetXHCodeByName(pSelectRouteOrder->m_strXHDBegin));
        }
        else if(m_bZHJL)
        {
            for(int i = 0; i < m_pRouteOrder->m_vecSonTrainRouteOrder.size(); i++)
            {
                emit sendRouteOrderMsg(MSGTYPE_CANCEL, m_pRouteOrder->m_vecSonTrainRouteOrder.at(i)->route_id, m_pMyStation->GetXHCodeByName(m_pRouteOrder->m_vecSonTrainRouteOrder.at(i)->m_strXHDBegin));
            }
           // emit sendRouteOrderMsg(MSGTYPE_MANTRIGGER, m_pRouteOrder->route_id, 0xbb);
        }
        else
        {
            emit sendRouteOrderMsg(MSGTYPE_CANCEL, m_pRouteOrder->route_id, m_pMyStation->GetXHCodeByName(m_pRouteOrder->m_strXHDBegin));
        }

        Sleep(500);
        this->close();
    }
}

void RouteHandWidget::on_handThroughBtn_clicked()
{
    emit SendTextAnalysisSignal(149, QCursor::pos());
    for(int i = 0; i < m_vecThroughRouteOrder.size(); i++)
    {
        if(m_vecThroughRouteOrder.at(i)->m_btRecvOrDepart)
        {
            if(m_vecThroughRouteOrder.at(i)->m_bZHJL)
            {
                for(int j = 0; j < m_vecThroughRouteOrder.at(i)->m_vecSonTrainRouteOrder.size(); j++)
                {
                    emit sendRouteOrderMsg(MSGTYPE_MANTRIGGER, m_vecThroughRouteOrder.at(i)->m_vecSonTrainRouteOrder.at(j)->route_id, 0xaa);
                }
            }
            else emit sendRouteOrderMsg(MSGTYPE_MANTRIGGER, m_vecThroughRouteOrder.at(i)->route_id, 0xaa);
        }
    }

    for(int i = 0; i < m_vecThroughRouteOrder.size(); i++)
    {
        if(m_vecThroughRouteOrder.at(i)->m_btRecvOrDepart == 0)
        {
            if(m_vecThroughRouteOrder.at(i)->m_bZHJL)
            {
                for(int j = 0; j < m_vecThroughRouteOrder.at(i)->m_vecSonTrainRouteOrder.size(); j++)
                {
                    emit sendRouteOrderMsg(MSGTYPE_MANTRIGGER, m_vecThroughRouteOrder.at(i)->m_vecSonTrainRouteOrder.at(j)->route_id, 0xaa);
                }
            }
            else emit sendRouteOrderMsg(MSGTYPE_MANTRIGGER, m_vecThroughRouteOrder.at(i)->route_id, 0xaa);
        }
    }

    Sleep(500);
    this->close();
}

void RouteHandWidget::on_closeBtn_clicked() //关闭
{
    emit SendTextAnalysisSignal(54, QCursor::pos());
    this->close();
}

void RouteHandWidget::on_radioButton_clicked(bool checked) //设置分段办理
{
    emit SendTextAnalysisSignal(150, QCursor::pos());
    ui->tableWidget->setVisible(checked);
    ui->handRouteBtn->setEnabled(!checked);
    if(!checked)
    {
        //ui->frame->setStyleSheet(QString("border-image:url(./data/bmp/%1/%2.bmp);")
        //                         .arg(m_pMyStation->m_nStationID).arg(m_pRouteOrder->m_strRouteDescrip));
        m_strCurRouteDescrip = m_pRouteOrder->m_strRouteDescripReal;
        m_strShowRouteDescrip = m_pRouteOrder->m_strRouteDescripReal;
    }
    else
    {
        if(!pSelectRouteOrder)
            pSelectRouteOrder = m_pRouteOrder->m_vecSonTrainRouteOrder.at(0);

        //ui->frame->setStyleSheet(QString("border-image:url(./data/bmp/%1/%2.bmp);")
        //                                  .arg(m_pMyStation->m_nStationID).arg(m_pRouteOrder->m_vecSonTrainRouteOrder.at(0)->m_strRouteDescrip));
        m_strCurRouteDescrip = pSelectRouteOrder->m_strRouteDescripReal;
        m_strShowRouteDescrip = pSelectRouteOrder->m_strRouteDescripReal;
    }
}

void RouteHandWidget::on_tableWidget_clicked(const QModelIndex &index)
{
    if(index.column() == 1)
        SetAutoTrigger(pSelectRouteOrder);
}

void RouteHandWidget::on_radioButton_4_clicked(bool checked)
{
    ui->radioButton_4->setChecked(!checked);
    //SetAutoTrigger(m_pRouteOrder);
    SetAutoTrigger_LeftButton(m_pRouteOrder);

    //if(m_pRouteOrder->m_bZHJL)
    //{
    //    for(int i = 0; i < m_pRouteOrder->m_vecSonTrainRouteOrder.size(); i++)
    //    {
    //        emit sendRouteOrderMsg(MSGTYPE_SETTRIGGERTYPE, m_pRouteOrder->m_vecSonTrainRouteOrder.at(i)->route_id, checked ? 0xaa : 0xbb);
    //    }
    //}
    //else emit sendRouteOrderMsg(MSGTYPE_SETTRIGGERTYPE, m_pRouteOrder->route_id,  checked ? 0xaa : 0xbb);
}

bool RouteHandWidget::eventFilter(QObject* o, QEvent* e)
{
    if(e->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(e);
        if(mouseEvent->button() == Qt::RightButton)
        {
            if(o == ui->handRouteBtn)
                SetAutoTrigger(m_pRouteOrder);
        }
    }

    if(e->type() == QEvent::Leave)
    {
        m_strShowRouteDescrip = m_strCurRouteDescrip;
    }

    if(e->type() == QEvent::MouseMove)
    {
        if(o == ui->resetBtn->menu())
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(e);
            QAction* pAction = ui->resetBtn->menu()->actionAt(mouseEvent->pos());
            if(pAction)
            {
                for(int i = 0; i < m_pRouteOrder->strArrayBTJL.size(); i++)
                {
                    QString str = m_pRouteOrder->strArrayBTJL.at(i);
                    str.replace(',', '-');
                    if(str == pAction->text())
                        m_strShowRouteDescrip = m_pRouteOrder->strArrayBTJL.at(i);
                }
            }
        }
        if(o == ui->tableWidget->viewport())
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(e);
            QTableWidgetItem* pItem = ui->tableWidget->itemAt(mouseEvent->pos());
            if(pItem)
                m_strShowRouteDescrip = m_pRouteOrder->m_vecSonTrainRouteOrder.at(pItem->row())->m_strRouteDescripReal;
            else m_strShowRouteDescrip = m_strCurRouteDescrip;
        }
        if(o == ui->handThroughBtn)
        {
            m_strShowRouteDescrip = "";
            for(int i = 0; i < m_vecThroughRouteOrder.size(); i++)
            {
                if(m_vecThroughRouteOrder.at(i)->m_bZHJL)
                {
                    for(int j = 0; j < m_vecThroughRouteOrder.at(i)->m_vecSonTrainRouteOrder.size(); j++)
                    {
                        m_strShowRouteDescrip += m_vecThroughRouteOrder.at(i)->m_vecSonTrainRouteOrder.at(j)->m_strRouteDescripReal;
                        m_strShowRouteDescrip += ";";
                    }
                }
                else
                {
                    m_strShowRouteDescrip += m_vecThroughRouteOrder.at(i)->m_strRouteDescripReal;
                    m_strShowRouteDescrip += ";";
                }
            }
            m_strShowRouteDescrip = m_strShowRouteDescrip.left(m_strShowRouteDescrip.length() - 1);
        }
    }

    if(e->type() == QEvent::Paint)
    {
        if(o == ui->StationView)
        {
            for(int i = 0; i < m_pMyStation->DevArray.size(); i++)
            {
                if(m_pMyStation->DevArray.at(i)->getDevType() == Dev_GD)
                {
                    m_pMyStation->DevArray.at(i)->setState(0);
                }
                if(m_pMyStation->DevArray.at(i)->getDevType() == Dev_DC)
                {
                    CGDDC* pGDDC = (CGDDC*)m_pMyStation->DevArray.at(i);
                    pGDDC->setState(DCDW);
                    pGDDC->setQDState(0);
                }
                if(m_pMyStation->DevArray.at(i)->getDevType() == Dev_XH)
                {
                    CXHD* pXHD = (CXHD*)m_pMyStation->DevArray.at(i);
                    pXHD->setXHDState(XHD_HD);
                }
            }

            QStringList strDescrip;
            QStringList strShowDescripList = m_strShowRouteDescrip.split(";");
            for(int i = 0; i < strShowDescripList.size(); i++)
            {
                MyStation::ZHJL* pZHJL = m_pMyStation->GetZHJLByDescrip(strShowDescripList.at(i));
                if(pZHJL)
                {
                    for(int j = 0; j < pZHJL->ZHJL_SonRouteNum; j++)
                    {
                        strDescrip << pZHJL->ZHJL_SDXH_JL[j] + "," + pZHJL->ZHJL_ZDXH_JL[j];
                    }
                }
                else strDescrip << strShowDescripList.at(i);
            }

            InterlockRoute* pInterlock = nullptr;
            CGD* pGD = nullptr;
            CGDDC* pGDDC = nullptr;
            for(int i = 0; i < strDescrip.size(); i++)
            {
                pInterlock = m_pMyStation->GetInterlockRouteByDescrip(strDescrip.at(i));
                if(pInterlock)
                {
                    QVector<CGDDC*> vecGDDC = m_pMyStation->getDCListByInterlock(pInterlock);
                    for(int j = 0; j < pInterlock->QDArr.size(); j++)
                    {
                        pGD = m_pMyStation->GetGDByCode(m_pMyStation->GetCodeByStrName(pInterlock->QDArr.at(j)));
                        if(pGD)
                        {
                            if(pGD->getGDType() != JJ_QD)
                                pGD->setState(QDSB);
                        }
                        else
                        {
                            QString strDC = pInterlock->QDArr.at(j);
                            for(int k = 0; k < vecGDDC.size(); k++)
                            {
                                if(vecGDDC.at(k)->m_DGName == strDC)
                                {
                                    vecGDDC.at(k)->setQDState(QDSB);
                                    qDebug() << "QDSB 321" << vecGDDC.at(k)->m_strName;
                                }
                            }
                            if(strDC.right(2) == "DG") //道岔区段
                            {
                                strDC = strDC.left(strDC.length() - 2);
                                QStringList strList = strDC.split("-");
                                for(int k = 0; k < strList.size(); k++)
                                {
                                    pGDDC = m_pMyStation->GetDCByCode(m_pMyStation->GetCodeByStrName(strList.at(k)));
                                    if(pGDDC)
                                    {
                                        if(vecGDDC.indexOf(pGDDC) >= 0)
                                        {
                                            pGDDC->setQDState(QDSB);
                                            qDebug() << "QDSB 123" << pGDDC->m_strName;
                                            if(pGDDC->getState() & DCDW)
                                            {
                                                if(m_pMyStation->GetDCByCode(pGDDC->getDWdev()))
                                                {
                                                    m_pMyStation->GetDCByCode(pGDDC->getDWdev())->setQDState(QDSB);
                                                    qDebug() << "QDSB 111" << m_pMyStation->GetDCByCode(pGDDC->getDWdev())->m_strName;
                                                }
                                            }
                                            if(pGDDC->getState() & DCFW)
                                            {
                                                if(m_pMyStation->GetDCByCode(pGDDC->getFWdev()))
                                                {
                                                    m_pMyStation->GetDCByCode(pGDDC->getFWdev())->setQDState(QDSB);
                                                    qDebug() << "QDSB 222" << m_pMyStation->GetDCByCode(pGDDC->getFWdev())->m_strName;
                                                }
                                                CGDDC* pGDDCAss = m_pMyStation->GetDCByCode(m_pMyStation->GetCodeByStrName(pGDDC->strAssociation));
                                                if(pGDDCAss)
                                                    pGDDCAss->setQDState(QDSB);

                                            }
                                            if(m_pMyStation->GetDCByCode(pGDDC->getCQdev()))
                                            {
                                                m_pMyStation->GetDCByCode(pGDDC->getCQdev())->setQDState(QDSB);
                                                qDebug() << "QDSB 333" << m_pMyStation->GetDCByCode(pGDDC->getCQdev())->m_strName;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                    if(m_strShowRouteDescrip == m_pRouteOrder->m_strRouteDescripReal && m_pRouteOrder->m_bZHJL)
                    {
                        InterlockRoute* pInterlock2 = nullptr;
                        for(int j = 0; j < m_pRouteOrder->m_vecSonTrainRouteOrder.size(); j++)
                        {
                            pInterlock2 = m_pMyStation->GetInterlockRouteByDescrip(m_pRouteOrder->m_vecSonTrainRouteOrder.at(j)->m_strRouteDescripReal);
                            if(pInterlock2)
                                m_pMyStation->GetXHDByCode(m_pMyStation->GetXHCodeByName(pInterlock2->strXHD))->setXHDState(XHD_LD);
                        }
                    }
                    else m_pMyStation->GetXHDByCode(m_pMyStation->GetXHCodeByName(pInterlock->strXHD))->setXHDState(XHD_LD);
                }
            }
            //显示站场
            if(m_pMyStation)
            {
                QPainter painter(ui->StationView);
                painter.setRenderHint(QPainter::Antialiasing);
                m_pMyStation->drawStation(&painter, false, DIPLOID_JLYL, 0x55);
                painter.end();
            }

        }
    }
    return QWidget::eventFilter(o, e);
}

void RouteHandWidget::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_nTimerId_500)
    {
        ui->StationView->update();
    }
}

bool RouteHandWidget::SetAutoTrigger(TrainRouteOrder* pRouteOrder)
{
    if(!pRouteOrder)
        return false;

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
            //emit sendRouteOrderMsg(MSGTYPE_SETTRIGGERTYPE, pRouteOrder->route_id,  checked ? 0xaa : 0xaa);
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
            //emit sendRouteOrderMsg(MSGTYPE_SETTRIGGERTYPE, pRouteOrder->route_id,  checked ? 0xaa : 0xbb);
        }
        else emit sendRouteOrderMsg(MSGTYPE_SETTRIGGERTYPE, pRouteOrder->route_id, 0xbb);
    });

    return pMenu->exec(QCursor::pos()) == pAction1;
}

bool RouteHandWidget::SetAutoTrigger_LeftButton(TrainRouteOrder* pRouteOrder)
{
    if(!pRouteOrder)
        return false;

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
            //emit sendRouteOrderMsg(MSGTYPE_SETTRIGGERTYPE, pRouteOrder->route_id,  checked ? 0xaa : 0xaa);
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
            //emit sendRouteOrderMsg(MSGTYPE_SETTRIGGERTYPE, pRouteOrder->route_id,  checked ? 0xaa : 0xbb);
        }
        else emit sendRouteOrderMsg(MSGTYPE_SETTRIGGERTYPE, pRouteOrder->route_id, 0xbb);
    });

    return pMenu->exec(QCursor::pos()) == pAction1;
}

void RouteHandWidget::on_radioButton_3_clicked(bool checked)
{
    emit SendTextAnalysisSignal(171, QCursor::pos());
    emit sendRouteOrderMsg(MSGTYPE_DJKX, m_pRouteOrder->route_id,  checked ? 0xaa : 0xbb);
}

void RouteHandWidget::on_lockBtn_clicked()
{
    emit SendTextAnalysisSignal(153, QCursor::pos());
}
