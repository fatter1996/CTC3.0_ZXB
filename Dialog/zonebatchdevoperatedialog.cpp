#include "zonebatchdevoperatedialog.h"
#include "ui_zonebatchdevoperatedialog.h"

ZoneBatchDevOperateDialog::ZoneBatchDevOperateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ZoneBatchDevOperateDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("区域批量设备操作 - TDCS/CTC3.0 通用车站操作终端");
    installEventFilter(this);
    ui->stationweight->installEventFilter(this);
    MeauInite();
    connect(ui->stationweight, &ZoneBatchDevOperateStationWidget::SendSelectDevsignal, this, &ZoneBatchDevOperateDialog::SelectDevslot);
    connect(this, &ZoneBatchDevOperateDialog::ClearStationState, ui->stationweight, &ZoneBatchDevOperateStationWidget::ClearSelectFun);
    connect(this, &ZoneBatchDevOperateDialog::Signal_Refresh_ZC, ui->stationweight, &ZoneBatchDevOperateStationWidget::setZCRefresh);
    Timer_ID_500 = startTimer(500);
}

ZoneBatchDevOperateDialog::~ZoneBatchDevOperateDialog()
{
    //for(int i = 0; i < m_vecZoneData->size(); i++)
    //{
    //    ZoneData* zone = (ZoneData*)&(m_vecZoneData->at(i));
    //    zone->Clear();
    //}
    delete ui;
}

void ZoneBatchDevOperateDialog::Init(MyStation* pMyStation)
{
    pCurrentStation = pMyStation;
    m_vecZoneData = &pCurrentStation->m_vecZoneData;
    ui->stationweight->Init(pCurrentStation);

    ReadZoneData();
}


void ZoneBatchDevOperateDialog::ReadZoneData()
{
    QMutexLocker locker(&mutex);
    for(int i = 0; i < m_vecZoneData->size(); i++)
    {
        strlZoneNames << m_vecZoneData->at(i).zoneName;
    }
    ui->comboBox->addItems(strlZoneNames);
    ui->comboBox->setCurrentIndex(0);
    pCurrectZone = (ZoneData*)&(m_vecZoneData->at(0));
    IniteTreeData();
}

void ZoneBatchDevOperateDialog::IniteTreeData()
{
    TreeModel = new QStandardItemModel(4, 3);
    TreeModel->setHeaderData(0, Qt::Horizontal, QString("设备"));
    TreeModel->setHeaderData(1, Qt::Horizontal, QString("操作"));
    TreeModel->setHeaderData(2, Qt::Horizontal, QString("状态"));

    ui->treeView->setModel(TreeModel);
    ui->treeView->setColumnWidth(0, 150);
    ui->treeView->setColumnWidth(1, 100);

    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(TreeModel, &QStandardItemModel::itemChanged, [=](QStandardItem *item){
        if(!item->hasChildren())
        {
            if(item->parent() && item->parent()->text() == "道岔")
            {
                for(int i = 0; i < pCurrectZone->vecDcDev.size(); i++)
                {
                    if(pCurrectZone->vecDcDev.at(i).devName == item->text())
                    {
                        CGDDC* pDC = pCurrentStation->GetDCByCode(pCurrectZone->vecDcDev.at(i).m_nCode);
                        pDC->isPLCZ = item->checkState() == Qt::Checked;
                        ui->stationweight->update();
                        return;
                    }
                }
            }
            else
            {
                for(int i = 0; i < pCurrectZone->vecQdDev.size(); i++)
                {
                    if(pCurrectZone->vecQdDev.at(i).devName == item->text())
                    {
                        CGD* pGD = pCurrentStation->GetGDByCode(pCurrectZone->vecQdDev.at(i).m_nCode);
                        pGD->isPLCZ = item->checkState() == Qt::Checked;
                        ui->stationweight->update();
                        return;
                    }
                }
                for(int i = 0; i < pCurrectZone->vecQjDev.size(); i++)
                {
                    if(pCurrectZone->vecQjDev.at(i).devName == item->text())
                    {
                        CGD* pGD = pCurrentStation->GetGDByCode(pCurrectZone->vecQjDev.at(i).m_nCode);
                        pGD->isPLCZ = item->checkState() == Qt::Checked;
                        ui->stationweight->update();
                        return;
                    }
                }
            }

        }
    });
    connect(ui->treeView, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onCustomContextMenuRequested(const QPoint&)));
    UpdataTreelist();
}

void ZoneBatchDevOperateDialog::UpdataTreelist()
{
    if(!TreeModel)
        return;

    TreeModel->removeRows(0, TreeModel->rowCount());
    QStandardItem *buffitem  = new QStandardItem(ui->comboBox->currentText());
    buffitem->setCheckable(true);
    buffitem->setEditable(false);

    int checkedCount = 0;
    Qt::CheckState state;
    //道岔
    {
        QStandardItem *buffdcitem = new QStandardItem(QString("道岔"));
        buffdcitem->setCheckable(true);
        buffitem->appendRow(buffdcitem);

        int checkedCount1 = 0;
        Qt::CheckState state1;
        for(int i = 0; i < pCurrectZone->vecDcDev.size(); i++)
        {
            QList<QStandardItem*> list;
            //第一列-设备
            QStandardItem *buffchiditem  = new QStandardItem(pCurrectZone->vecDcDev[i].devName);
            buffchiditem->setCheckable(true);
            buffchiditem->setEditable(false);
            buffchiditem->setCheckState(pCurrectZone->vecDcDev[i].isSelectFlag);
            list.append(buffchiditem);
            if(pCurrectZone->vecDcDev[i].isSelectFlag)
                checkedCount1++;

            //第二列-操作
            QStandardItem *buffOperitem  = new QStandardItem(pCurrectZone->vecDcDev[i].getOperStr());
            buffOperitem->setForeground(QBrush(Qt::red));
            list.append(buffOperitem);

            //第二列-状态
            QStandardItem *buffstateitem  = new QStandardItem(pCurrectZone->vecDcDev[i].getStateStr());
            buffstateitem->setForeground(QBrush(Qt::green));
            list.append(buffstateitem);

            buffdcitem->appendRow(list);
        }

        if(checkedCount1 == pCurrectZone->vecDcDev.size())
        {
            state1 = Qt::Checked;
            checkedCount++;
        }
        else if(checkedCount1 == 0)
            state1 = Qt::Unchecked;
        else
        {
            state1 = Qt::PartiallyChecked;
            checkedCount = -10;
        }
        buffdcitem->setCheckState(state1);
    }
    //股道/无岔
    {
        QStandardItem *buffqditem  = new QStandardItem(QString("股道/无岔"));
        buffqditem->setCheckable(true);
        buffitem->appendRow(buffqditem);

        int checkedCount2 = 0;
        Qt::CheckState state2;
        for(int i = 0; i < pCurrectZone->vecQdDev.size(); i++)
        {
            QList<QStandardItem*> list;
            //第一列-设备
            QStandardItem *buffchiditem  = new QStandardItem(pCurrectZone->vecQdDev[i].devName);
            buffchiditem->setCheckable(true);
            buffchiditem->setEditable(false);
            buffchiditem->setCheckState(pCurrectZone->vecQdDev[i].isSelectFlag);
            list.append(buffchiditem);
            if(pCurrectZone->vecQdDev[i].isSelectFlag)
                checkedCount2++;

            //第二列-操作
            QStandardItem *buffOperitem  = new QStandardItem(pCurrectZone->vecQdDev[i].getOperStr());
            buffOperitem->setForeground(QBrush(Qt::red));
            list.append(buffOperitem);

            //第二列-状态
            QStandardItem *buffstateitem  = new QStandardItem(pCurrectZone->vecQdDev[i].getStateStr());
            buffstateitem->setForeground(QBrush(Qt::green));
            list.append(buffstateitem);

            buffqditem->appendRow(list);
        }

        if(checkedCount2 == pCurrectZone->vecQdDev.size())
        {
            state2 = Qt::Checked;
            checkedCount++;
        }
        else if(checkedCount2 == 0)
            state2 = Qt::Unchecked;
        else
        {
            state2 = Qt::PartiallyChecked;
            checkedCount = -10;
        }
        buffqditem->setCheckState(state2);
    }
    //区间
    {
        QStandardItem *buffqjitem  = new QStandardItem(QString("区间"));
        buffqjitem->setCheckable(true);
        buffitem->appendRow(buffqjitem);

        int checkedCount3 = 0;
        Qt::CheckState state3;
        for(int i = 0; i < pCurrectZone->vecQjDev.size(); i++)
        {
            QList<QStandardItem*> list;
            //第一列-设备
            QStandardItem *buffchiditem  = new QStandardItem(pCurrectZone->vecQjDev[i].devName);
            buffchiditem->setCheckable(true);
            buffchiditem->setEditable(false);
            buffchiditem->setCheckState(pCurrectZone->vecQjDev[i].isSelectFlag);
            list.append(buffchiditem);
            if(pCurrectZone->vecQjDev[i].isSelectFlag)
                checkedCount3++;

            //第二列-操作
            QStandardItem *buffOperitem  = new QStandardItem(pCurrectZone->vecQjDev[i].getOperStr());
            buffOperitem->setForeground(QBrush(Qt::red));
            list.append(buffOperitem);

            //第二列-状态
            QStandardItem *buffstateitem  = new QStandardItem(pCurrectZone->vecQjDev[i].getStateStr());
            buffstateitem->setForeground(QBrush(Qt::green));
            list.append(buffstateitem);

            buffqjitem->appendRow(list);
        }
        if(checkedCount3 == pCurrectZone->vecQjDev.size())
        {
            state3 = Qt::Checked;
            checkedCount++;
        }
        else if(checkedCount3 == 0)
            state3 = Qt::Unchecked;
        else
        {
            state3 = Qt::PartiallyChecked;
            checkedCount = -10;
        }
        buffqjitem->setCheckState(state3);
    }
    /*
    //信号机
    {
        QStandardItem *buffqditem  = new QStandardItem(QString("信号机"));
        buffqditem->setCheckable(true);
        buffqditem->setCheckState(pCurrectZone->isXhSelect);
        buffitem->appendRow(buffqditem);

        for(int i = 0; i < pCurrectZone->vecXhDev.size(); i++)
        {
            QList<QStandardItem*> list;
            //第一列-设备
            QStandardItem *buffchiditem  = new QStandardItem(pCurrectZone->vecXhDev[i].devName);
            buffchiditem->setCheckable(true);
            buffchiditem->setEditable(false);
            buffchiditem->setCheckState(pCurrectZone->vecXhDev[i].isSelectFlag);
            list.append(buffchiditem);

            //第二列-操作
            QStandardItem *buffOperitem  = new QStandardItem(pCurrectZone->vecXhDev[i].getOperStr());
            buffOperitem->setForeground(QBrush(Qt::red));
            list.append(buffOperitem);

            //第二列-状态
            QStandardItem *buffstateitem  = new QStandardItem(pCurrectZone->vecXhDev[i].getStateStr());
            buffstateitem->setForeground(QBrush(Qt::green));
            list.append(buffstateitem);

            buffqditem->appendRow(list);
        }
    }*/
    if(checkedCount == 0)
        state = Qt::Unchecked;
    else if(checkedCount == 3)
        state = Qt::Checked;
    else state = Qt::PartiallyChecked;
    buffitem->setCheckState(state);
    TreeModel->setItem(0, 0,buffitem);
    //buffitem->setCheckState((GetChildTreeCheckedByParent(buffitem)));
    ui->treeView->expandAll(); //默认展开所有节点
}

void ZoneBatchDevOperateDialog::UpdataTreeItemState()
{
    QStandardItem *buffitem = TreeModel->item(0, 0);
    //道岔
    QStandardItem *buffdcitem = buffitem->child(0);
    for(int i = 0; i < buffdcitem->rowCount(); i++)
    {
        for(int j = 0; j < pCurrectZone->vecDcDev.size(); j++)
        {
            if(pCurrectZone->vecDcDev.at(j).devName == buffdcitem->child(i)->text())
            {
                int state = pCurrectZone->vecDcDev[j].state & 0x17;
                if(state > 0)
                    buffdcitem->child(i, 2)->setText(pCurrectZone->vecDcDev[j].getStateStr());
                else buffdcitem->child(i, 2)->setText("");
            }
        }
    }

    //股道/无岔
    QStandardItem *buffqditem = buffitem->child(1);
    for(int i = 0; i < buffqditem->rowCount(); i++)
    {
        for(int j = 0; j < pCurrectZone->vecQdDev.size(); j++)
        {
            if(pCurrectZone->vecQdDev.at(j).devName == buffqditem->child(i)->text())
            {
                int state = pCurrectZone->vecQdDev[j].state & 0x17;
                if(state > 0)
                    buffqditem->child(i, 2)->setText(pCurrectZone->vecQdDev[j].getStateStr());
                else buffqditem->child(i, 2)->setText("");
            }

        }
    }

    //区间
    QStandardItem *buffqjitem = buffitem->child(2);
    for(int i = 0; i < buffqjitem->rowCount(); i++)
    {
        for(int j = 0; j < pCurrectZone->vecQjDev.size(); j++)
        {
            if(pCurrectZone->vecQjDev.at(j).devName == buffqjitem->child(i)->text())
            {
                int state = pCurrectZone->vecQjDev[j].state & 0x17;
                if(state > 0)
                    buffqjitem->child(i, 2)->setText(pCurrectZone->vecQjDev[j].getStateStr());
                else buffqjitem->child(i, 2)->setText("");
            }
        }
    }

    //TreeModel->itemFromIndex(TreeModel.ind);
}

int ZoneBatchDevOperateDialog::UpdataTreelist(QString devName)
{
    for(int i = 0; i < pCurrectZone->vecDcDev.size(); i++)
    {
        if(pCurrectZone->vecDcDev[i].devName == devName)
        {
            pCurrectDev = &pCurrectZone->vecDcDev[i];
            return DCDev;
        }
    }
    for(int i = 0; i < pCurrectZone->vecQdDev.size(); i++)
    {
        if(pCurrectZone->vecQdDev[i].devName == devName)
        {
            pCurrectDev = &pCurrectZone->vecQdDev[i];
            return QDDev;
        }
    }
    for(int i = 0; i < pCurrectZone->vecQjDev.size(); i++)
    {
        if(pCurrectZone->vecQjDev[i].devName == devName)
        {
            pCurrectDev = &pCurrectZone->vecQjDev[i];
            return QJDev;
        }
    }
    for(int i = 0; i < pCurrectZone->vecXhDev.size(); i++)
    {
        if(pCurrectZone->vecXhDev[i].devName == devName)
        {
            pCurrectDev = &pCurrectZone->vecXhDev[i];
            return XHDev;
        }
    }
    return 0;
}

void ZoneBatchDevOperateDialog::MeauInite()
{
    DCMenu = new QMenu();
    DCMenu->setStyleSheet("background-color: rgb(174,216,230);");
    QDMenu = new QMenu();
    XHMenu = new QMenu();
    QJMenu = new QMenu();
    QDMenu->setStyleSheet("background-color: rgb(174,216,230);");
    XHMenu->setStyleSheet("background-color: rgb(174,216,230);");
    QJMenu->setStyleSheet("background-color: rgb(174,216,230);");

    connect(DCMenu,SIGNAL(triggered(QAction *)),this,SLOT(onDCMenuTriggered(QAction *)));
    connect(QDMenu,SIGNAL(triggered(QAction *)),this,SLOT(onQDMenuTriggered(QAction *)));
    connect(XHMenu,SIGNAL(triggered(QAction *)),this,SLOT(onXHMenuTriggered(QAction *)));
    connect(QJMenu,SIGNAL(triggered(QAction *)),this,SLOT(onQJMenuTriggered(QAction *)));

    DCDSaction = new QAction(QString("单锁"),this);
    DCDSaction->setCheckable(true);
    DCMenu->addAction(DCDSaction);

    DCDSDaction = new QAction(QString("单锁至定位"),this);
    DCDSDaction->setCheckable(true);
    DCMenu->addAction(DCDSDaction);

    DCDSFaction = new QAction(QString("单锁至反位"),this);
    DCDSFaction->setCheckable(true);
    DCMenu->addAction(DCDSFaction);

    DCFSaction = new QAction(QString("封锁"),this);
    DCFSaction->setCheckable(true);
    DCMenu->addAction(DCFSaction);

    DCMenu->addSeparator();

    DCDeleteaction = new QAction(QString("删除设备"),this);
    DCMenu->addAction(DCDeleteaction);

    //QDFSaction = new QAction(QString("封"),this);
    //QDFSaction->setCheckable(true);
    //QDMenu->addAction(QDFSaction);
    QDDeleteaction = new QAction(QString("删除设备"),this);
    QDMenu->addAction(QDDeleteaction);

    //QJFSaction = new QAction(QString("封"),this);
    //QJFSaction->setCheckable(true);
    //QJMenu->addAction(QJFSaction);
    QJDeleteaction = new QAction(QString("删除设备"),this);
    QJMenu->addAction(QJDeleteaction);

    //XHFSaction = new QAction(QString("封(列)封(调)"),this);
    //XHFSaction->setCheckable(true);
    //XHMenu->addAction(XHFSaction);
    XHDeleteaction = new QAction(QString("删除设备"),this);
    XHMenu->addAction(XHDeleteaction);
}

void ZoneBatchDevOperateDialog::DevOperateByBtn(bool isUnlock)
{
    if(!isEdit) return;

    for(unsigned int i = 0 ;i<pCurrectZone->vecDcDev.size();i++)
    {
        if(pCurrectZone->vecDcDev[i].isSelectFlag == Qt::Unchecked)
            continue;

        if(isUnlock)
        {
            pCurrectZone->vecDcDev[i].state = pCurrectZone->vecDcDev[i].state | 0x10 ;

            if(pCurrectZone->vecDcDev[i].operType & 0x03)
                SendData(FUNTYPE_JS,pCurrectZone->vecDcDev[i].m_nCode);
            if(pCurrectZone->vecDcDev[i].operType & 0x04)
                SendData(FUNTYPE_JF,pCurrectZone->vecDcDev[i].m_nCode);
        }
        else
        {
            pCurrectZone->vecDcDev[i].state = pCurrectZone->vecDcDev[i].state | 0x10;
            if(pCurrectZone->vecDcDev[i].operType & 0x03)
                SendData(FUNTYPE_DS,pCurrectZone->vecDcDev[i].m_nCode);
            if(pCurrectZone->vecDcDev[i].operType & 0x04)
                SendData(FUNTYPE_FS,pCurrectZone->vecDcDev[i].m_nCode);
        }
    }
    for(unsigned int i = 0 ;i<pCurrectZone->vecQdDev.size();i++)
    {
        if(pCurrectZone->vecQdDev[i].isSelectFlag == Qt::Unchecked)
            continue;

        if(isUnlock)
        {
            pCurrectZone->vecQdDev[i].state = pCurrectZone->vecQdDev[i].state | 0x10 ;

            if(pCurrectZone->vecQdDev[i].operType & 0x04)
                SendData(FUNTYPE_JF,pCurrectZone->vecQdDev[i].m_nCode);
        }
        else
        {
            pCurrectZone->vecQdDev[i].state = pCurrectZone->vecQdDev[i].state | 0x10;

            if(pCurrectZone->vecQdDev[i].operType & 0x04)
                SendData(FUNTYPE_FS,pCurrectZone->vecQdDev[i].m_nCode);
        }
    }
    for(unsigned int i = 0 ;i<pCurrectZone->vecQjDev.size();i++)
    {
        if(pCurrectZone->vecQjDev[i].isSelectFlag == Qt::Unchecked)
            continue;

        if(isUnlock)
        {
            pCurrectZone->vecQjDev[i].state = pCurrectZone->vecQjDev[i].state | 0x10 ;

            if(pCurrectZone->vecQjDev[i].operType & 0x04)
                SendData(FUNTYPE_JF,pCurrectZone->vecQjDev[i].m_nCode);
        }
        else
        {
            pCurrectZone->vecQjDev[i].state = pCurrectZone->vecQjDev[i].state | 0x10;

            if(pCurrectZone->vecQjDev[i].operType & 0x04)
                SendData(FUNTYPE_FS,pCurrectZone->vecQjDev[i].m_nCode);
        }
    }
    /*
    for(unsigned int i = 0 ;i<pCurrectZone->vecXhDev.size();i++)
    {
        if(pCurrectZone->vecQdDev[i].isSelectFlag == Qt::Unchecked)
            continue;

        if(isUnlock)
        {
            pCurrectZone->vecXhDev[i].state = pCurrectZone->vecXhDev[i].operType | 0x08 ;

            if(pCurrectZone->vecXhDev[i].state & 0x04)
                SendData(FUNTYPE_JF,pCurrectZone->vecXhDev[i].m_nCode);
        }
        else
        {
            pCurrectZone->vecXhDev[i].state = pCurrectZone->vecXhDev[i].operType;

            if(pCurrectZone->vecXhDev[i].state & 0x04)
                SendData(FUNTYPE_FS,pCurrectZone->vecXhDev[i].m_nCode);
        }
    }*/
}

void ZoneBatchDevOperateDialog::DeleteCurrectDev()
{
    if(!isEdit) return;

    for(int i = 0;i < pCurrectZone->vecDcDev.size();i++)
    {
        if(pCurrectZone->vecDcDev[i].devName == pCurrectDev->devName)
        {
           pCurrectZone->vecDcDev[i].isSelectFlag = Qt::Unchecked;
           pCurrectZone->vecDcDev.erase(pCurrectZone->vecDcDev.begin() + i);
           return ;
        }
    }
    for(int i = 0; i < pCurrectZone->vecQdDev.size(); i++)
    {
        if(pCurrectZone->vecQdDev[i].devName == pCurrectDev->devName)
        {
            pCurrectZone->vecQdDev[i].isSelectFlag = Qt::Unchecked;
            pCurrectZone->vecQdDev.erase(pCurrectZone->vecQdDev.begin() + i);
            return ;
        }
    }
    for(int i = 0; i < pCurrectZone->vecQjDev.size(); i++)
    {
        if(pCurrectZone->vecQjDev[i].devName == pCurrectDev->devName)
        {
            pCurrectZone->vecQjDev[i].isSelectFlag = Qt::Unchecked;
            pCurrectZone->vecQjDev.erase(pCurrectZone->vecQjDev.begin() + i);
            return ;
        }
    }
    for(unsigned int i = 0;i<pCurrectZone->vecXhDev.size();i++)
    {
        if(pCurrectZone->vecXhDev[i].devName == pCurrectDev->devName)
        {
            pCurrectZone->vecXhDev[i].isSelectFlag = Qt::Unchecked;
            pCurrectZone->vecXhDev.erase(pCurrectZone->vecXhDev.begin() + i);
            return ;
        }
    }
}

bool ZoneBatchDevOperateDialog::IsZoneNameRepeat(QString name)
{
    for (int i = 0; i < m_vecZoneData->size(); i++)
    {
        if(name == m_vecZoneData->at(i).zoneName)
        {
            return true;
        }
    }
    return false;
}

void ZoneBatchDevOperateDialog::SendData(unsigned char FUNType, unsigned int FUNSet_DeviceNum)
{
     emit SendZoneBatchDevOperateToSeverSignal(pCurrentStation->getStationID(),FUNType,FUNSet_DeviceNum);
}

void ZoneBatchDevOperateDialog::CheckAllChildTreeItem(QStandardItem* item, Qt::CheckState check)
{
    if (item == nullptr)
        return;
    item->setCheckState(check);

    int rowCount = item->rowCount();
    for (int i = 0; i < rowCount; ++i)
    {
        CheckAllChildTreeItem(item->child(i), check);
    }
}

void ZoneBatchDevOperateDialog::CheckChildTreeItemChanged(QStandardItem* item)
{
    if(!item)
        return;
    Qt::CheckState siblingState = GetChildTreeCheckedByChild(item);
    QStandardItem * parentItem = item->parent();
    if (!parentItem)
        return;
    parentItem->setCheckState(siblingState);
    CheckChildTreeItemChanged(parentItem);
}

Qt::CheckState ZoneBatchDevOperateDialog::GetChildTreeCheckedByChild(QStandardItem* item)
{
    //先通过父节点获取兄弟节点
    QStandardItem * parent = item->parent();
    if (nullptr == parent)
        return item->checkState();

    int brotherCount = parent->rowCount();
    int checkedCount = 0;
    Qt::CheckState state;
    for (int i = 0; i < brotherCount; ++i)
    {
        QStandardItem* siblingItem = parent->child(i);
        state = siblingItem->checkState();

        if (state == Qt::PartiallyChecked)
            return Qt::PartiallyChecked;

        if (state == Qt::Checked)
            checkedCount++;
    }

    if(checkedCount == 0)
        return Qt::Unchecked;
    else if(checkedCount == brotherCount)
        return Qt::Checked;
    else return Qt::PartiallyChecked;
}

Qt::CheckState ZoneBatchDevOperateDialog::GetChildTreeCheckedByParent(QStandardItem* item)
{
    if(!item)
        return Qt::Unchecked;

    //if (!item->hasChildren())
    //    return item->checkState();

    int brotherCount = item->rowCount();
    int checkedCount = 0;
    Qt::CheckState state;
    for (int i = 0; i < brotherCount; ++i)
    {
        state = GetChildTreeCheckedByParent(item);

        if (state == Qt::PartiallyChecked)
            return Qt::PartiallyChecked;

        if (state == Qt::Checked)
            checkedCount++;
    }
    if(checkedCount == 0)
        return Qt::Unchecked;
    else if(checkedCount == brotherCount)
        return Qt::Checked;
    else return Qt::PartiallyChecked;
}

void ZoneBatchDevOperateDialog::timerEvent(QTimerEvent* enent)
{
    if(enent->timerId() == Timer_ID_500)
    {
        if(pCurrentStation)
        {
            pCurrentStation->UpdataZoneDataState();
            UpdataTreeItemState();
            ui->stationweight->update();
            ui->treeView->update();
        }
    }
}

void ZoneBatchDevOperateDialog::SelectDevslot(QVector<DeviceInfo> vecDevData)
{
    if(pCurrectZone == nullptr)
        return ;
    pCurrectZone->AddDevList(vecDevData);
    UpdataTreelist();
}

void ZoneBatchDevOperateDialog::treeItemChanged(QStandardItem* item)
{
    if (!pCurrectZone || !item)
        return ;

    if (item->isCheckable()/* && isStateCheng*/)
    {
        //如果条目是存在复选框的，那么就进行下面的操作
        //pSelectitem = item;
        CheckChildTreeItemChanged(item);
        if (item->hasChildren())
        {

            CheckAllChildTreeItem(item, item->checkState());
        }
        else
        {
            //pSelectitem = item;
            //CheckChildTreeItemChanged(item);
        }
    }

    if(pCurrectZone)
    {
        //isStateCheng = true;
        pCurrectZone->SetCheck(item->text(), item->checkState());
    }
}

void ZoneBatchDevOperateDialog::onCustomContextMenuRequested(const QPoint &pos)
{
    if(pCurrectZone == nullptr || isEdit == false)
        return ;
    QModelIndex index = ui->treeView->model()->index(
                ui->treeView->indexAt(pos).row(), 0, ui->treeView->indexAt(pos).parent());
    int Type = UpdataTreelist(index.data().toString());

    if(pCurrectDev == nullptr)
        return;

    if(Type == DCDev)
    {
        DCDSaction->setChecked((pCurrectDev->operType & 0x03) == 3);
        DCDSDaction->setChecked((pCurrectDev->operType & 0x03) == 1);
        DCDSFaction->setChecked((pCurrectDev->operType & 0x03) == 2);
        DCFSaction->setChecked(pCurrectDev->operType & 0x04);
        DCMenu->popup(ui->treeView->mapToGlobal(QPoint(pos.x(), pos.y() + 25)));
    }
    else if(Type == QDDev)
    {
        QDMenu->popup(ui->treeView->mapToGlobal(QPoint(pos.x(), pos.y() + 25)));
    }
    else if(Type == QJDev)
    {
        QJMenu->popup(ui->treeView->mapToGlobal(QPoint(pos.x(), pos.y() + 25)));
    }
    else if(Type == XHDev)
    {
        XHMenu->popup(ui->treeView->mapToGlobal(QPoint(pos.x(), pos.y() + 25)));
    }
}

void ZoneBatchDevOperateDialog::onDCMenuTriggered(QAction *action)
{
    if(pCurrectDev == nullptr || pCurrectZone == nullptr || !isEdit)
        return;

    if (action == DCDSaction) //单锁
    {
        pCurrectDev->operType = pCurrectDev->operType & 0xFC;
        if(DCDSaction->isChecked())
            pCurrectDev->operType = pCurrectDev->operType | 0x03;

        DCDSaction->setChecked(Qt::Checked);
        DCDSDaction->setChecked(Qt::Unchecked);
        DCDSFaction->setChecked(Qt::Unchecked);

    }
    else if (action == DCDSDaction) //单锁至定位
    {
        pCurrectDev->operType = pCurrectDev->operType & 0xFC;
        if(DCDSDaction->isChecked())
            pCurrectDev->operType = pCurrectDev->operType | 0x01;
    }
    else if(action == DCDSFaction) //单锁至反位
    {
        pCurrectDev->operType = pCurrectDev->operType & 0xFC;
        if(DCDSFaction->isChecked())
            pCurrectDev->operType = pCurrectDev->operType | 0x02;
    }
    else if(action == DCFSaction) //封锁
    {
        pCurrectDev->operType = pCurrectDev->operType & 0xFB;
        if(DCFSaction->isChecked())
            pCurrectDev->operType = pCurrectDev->operType | 0x04;
    }
    else if(action == DCDeleteaction) //删除设备
       DeleteCurrectDev();

    UpdataTreelist();
}

void ZoneBatchDevOperateDialog::onQDMenuTriggered(QAction *action)
{
    if(pCurrectDev == nullptr || pCurrectZone == nullptr || !isEdit)
        return;

    if (action == QDDeleteaction)
    {
       DeleteCurrectDev();
    }
    //isStateCheng = false;
    UpdataTreelist();
}

void ZoneBatchDevOperateDialog::onQJMenuTriggered(QAction *action)
{
    if(!isEdit) return;

    if(action == QJDeleteaction)
       DeleteCurrectDev();

    //isStateCheng = false;
    UpdataTreelist();
}

void ZoneBatchDevOperateDialog::onXHMenuTriggered(QAction *action)
{
    if(!isEdit)
        return;

    if (action == XHDeleteaction)
        DeleteCurrectDev();

    //isStateCheng = false;
    UpdataTreelist();
}
//工作区切换
void ZoneBatchDevOperateDialog::on_comboBox_currentIndexChanged(int index)
{
    if(index >= m_vecZoneData->size())
        return;
    pCurrectZone = (ZoneData*)&(m_vecZoneData->at(index));
    UpdataTreelist();

    if(!TreeModel)
        return;
    QStandardItem *buffitem = TreeModel->item(0, 0);

    QStandardItem *buffdcitem = buffitem->child(0);
    QStandardItem *buffqditem = buffitem->child(1);
    QStandardItem *buffqjitem = buffitem->child(2);

    QStandardItem *item = nullptr;
    CGDDC* pDC = nullptr;
    CGD* pGD = nullptr;
    for(int i = 0; i < pCurrentStation->DevArray.size(); i++)
    {
        if(pCurrentStation->DevArray.at(i)->getDevType() == Dev_DC)
        {
            pDC = (CGDDC*)pCurrentStation->DevArray.at(i);
            pDC->isPLCZ = false;
            for(int j = 0; j < pCurrectZone->vecDcDev.size(); j++)
            {
                if(pDC->m_nCode == pCurrectZone->vecDcDev.at(j).m_nCode)
                {
                    item = buffdcitem->child(j);
                    pDC->isPLCZ = item->checkState() == Qt::Checked;
                    ui->stationweight->update();
                }
            }
        }
        if(pCurrentStation->DevArray.at(i)->getDevType() == Dev_GD)
        {
            pGD = (CGD*)pCurrentStation->DevArray.at(i);
            pGD->isPLCZ = false;
            if(pGD->m_nGDType == JJ_QD)
            {
                for(int j = 0; j < pCurrectZone->vecQjDev.size(); j++)
                {
                    if(pGD->m_nCode == pCurrectZone->vecQjDev.at(j).m_nCode)
                    {
                        item = buffqjitem->child(j);
                        pGD->isPLCZ = item->checkState() == Qt::Checked;
                        ui->stationweight->update();
                    }
                }
            }
            else
            {
                for(int j = 0; j < pCurrectZone->vecQdDev.size(); j++)
                {
                    if(pGD->m_nCode == pCurrectZone->vecQdDev.at(j).m_nCode)
                    {
                        item = buffqditem->child(j);
                        pGD->isPLCZ = item->checkState() == Qt::Checked;
                        ui->stationweight->update();
                    }
                }
            }
        }
    }
}
//编辑
void ZoneBatchDevOperateDialog::on_radioButton_clicked(bool checked)
{
    isEdit = checked;
    emit ClearStationState();
}
//重命名
void ZoneBatchDevOperateDialog::on_rename_btn_clicked()
{
    if(!pCurrectZone || !isEdit)
        return ;

    QLineEdit::EchoMode echoMode=QLineEdit::Normal;//正常文字输入
    bool ok = false;
    QString text = QInputDialog::getText(this, "重命名分区", "请输入分区名称", echoMode, pCurrectZone->zoneName, &ok);
    if (ok && !text.isEmpty())
    {
        if(IsZoneNameRepeat(text))
        {
            QMessageBox::information(this, "注意-TDCS/CTC 3.0 通用车站操作窗口", "重命名失败,分区名已存在",
                                     QMessageBox::Ok,QMessageBox::NoButton);
            return;
        }
        int index = ui->comboBox->currentIndex();
        pCurrectZone->zoneName = text;
        ui->comboBox->clear();
        ui->comboBox->removeItem(index);
        ui->comboBox->insertItem(index, text);
        ui->comboBox->setCurrentIndex(index);
        UpdataTreelist();
    }
}
//增加分区
void ZoneBatchDevOperateDialog::on_AddZone_btn_pressed()
{
    emit SendTextAnalysisSignal(58, QCursor::pos());
    if(!isEdit)
        return;

    QString NewName;
    for(int i = 1;;i++)
    {
        NewName = QString("自定义区域%1").arg(i);
        if(!IsZoneNameRepeat(NewName))
            break;
    }

    bool ok = false;
    QLineEdit::EchoMode echoMode = QLineEdit::Normal;//正常文字输入

    QString text = QInputDialog::getText(this, "新增分区", "请输入分区名称", echoMode, NewName, &ok);
    if (ok && !text.isEmpty())
    {
        if(IsZoneNameRepeat(text))
        {
            QMessageBox::information(this, "注意-TDCS/CTC 3.0 通用车站操作窗口", "命名失败,分区名已存在",
                                     QMessageBox::Ok,QMessageBox::NoButton);
            return;
        }
    }

    ZoneData zone;
    zone.stationId = pCurrentStation->getStationID();
    zone.zoneName = text;
    m_vecZoneData->push_back(zone);
    pCurrectZone = &zone;
    ui->comboBox->addItem(zone.zoneName);
    ui->comboBox->setCurrentText(zone.zoneName);

    UpdataTreelist();
    emit ClearStationState();
}
//删除分区
void ZoneBatchDevOperateDialog::on_detelezone_btn_pressed()
{
    emit SendTextAnalysisSignal(59, QCursor::pos());
    if(!pCurrectZone || !isEdit)
        return ;

    QMessageBox::StandardButton result;//返回选择的按钮
    result=QMessageBox::warning(this, "注意-TDCS/CTC 3.0 通用车站操作窗口",
                                QString("确定要删除：%1").arg(ui->comboBox->currentText()),
                                QMessageBox::Yes|QMessageBox::No);
    if (result==QMessageBox::Yes)
    {
        if(ui->comboBox->currentIndex() <= 5)
        {
            QMessageBox::warning(this, "注意-TDCS/CTC 3.0 通用车站操作窗口", "该分区不能删除，只能删除新增分区", QMessageBox::Cancel);
            return;
        }

        int index = ui->comboBox->currentIndex();
        ui->comboBox->removeItem(index);
        m_vecZoneData->remove(index);
        ui->comboBox->setCurrentIndex((index > 0) ? (index - 1) : 0);
        pCurrectZone = nullptr;
    }
}
//保存分区
void ZoneBatchDevOperateDialog::on_savezone_btn_pressed()
{
    emit SendTextAnalysisSignal(60, QCursor::pos());
}
//上传自定义分区
void ZoneBatchDevOperateDialog::on_loadzone_btn_pressed()
{
    emit SendTextAnalysisSignal(61, QCursor::pos());
}
//封锁/单锁
void ZoneBatchDevOperateDialog::on_FSDev_btn_pressed()
{
    emit SendTextAnalysisSignal(62, QCursor::pos());
    if(!pCurrectZone || !isEdit)
        return ;
    int ret = QMessageBox::information(this, "TDCS/CTC 3.0 通用车站操作窗口",
                                       "确定要进行封锁/单锁批量操作码?", "是", "否");
    if(ret == 0)
    {
        DevOperateByBtn(false);
        //DevOperateByBtn(false);
        //isStateCheng = false;
        UpdataTreeItemState();
        //global.SenddataFun();
    }
}
//解封/解锁
void ZoneBatchDevOperateDialog::on_JFDev_btn_pressed()
{
    emit SendTextAnalysisSignal(63, QCursor::pos());
    if(!pCurrectZone || !isEdit)
        return ;

    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("输入密码 - TDCS");
    dialog->setGeometry(800, 500, 200, 120);
    dialog->setStyleSheet("background-color: rgb(198, 210, 218);");
    QLabel *label = new QLabel(dialog);
    label->setFont(QFont("微软雅黑", 11));
    label->setGeometry(12, 4, 96, 32);
    label->setText("确认批量解锁：");
    label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    QLineEdit *lineEdit = new QLineEdit(dialog);
    lineEdit->setGeometry(12, 40, 176, 32);
    lineEdit->setStyleSheet("background-color: rgb(240, 240, 240);");
    lineEdit->setEchoMode(QLineEdit::Password);

    QPushButton *pushbutton_OK = new QPushButton(dialog);
    pushbutton_OK->setText("OK");
    pushbutton_OK->setFont(QFont("微软雅黑", 11));
    pushbutton_OK->setGeometry(88, 76, 32, 32);
    pushbutton_OK->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
                                 "QPushButton{border:1px groove gray;border-radius:4px;padding:2px 4px;border-style:outset;}"
                                 "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
                                 "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
                                 "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}");

    QPushButton *pushbutton = new QPushButton(dialog);
    pushbutton->setText("Cancle");
    pushbutton->setFont(QFont("微软雅黑", 11));
    pushbutton->setGeometry(132, 76, 56, 32);
    pushbutton->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
                              "QPushButton{border:1px groove gray;border-radius:4px;padding:2px 4px;border-style:outset;}"
                              "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
                              "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
                              "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}");

    connect(pushbutton_OK,&QPushButton::clicked,[=](){
        QString Str=lineEdit->text();
        if(Str=="123")
        {
            DevOperateByBtn(true);
            UpdataTreeItemState();
        }
    });
    connect(pushbutton_OK, &QPushButton::clicked, dialog, &QDialog::close);
    connect(pushbutton, &QPushButton::clicked, dialog, &QDialog::close);
    dialog->exec();
}
//刷新状态
void ZoneBatchDevOperateDialog::on_refresh__btn_pressed()
{
    emit SendTextAnalysisSignal(64, QCursor::pos());
}
//关闭
void ZoneBatchDevOperateDialog::on_close_btn_pressed()
{
    emit SendTextAnalysisSignal(54, QCursor::pos());
    this->close();
}

void ZoneBatchDevOperateDialog::on_treeView_clicked(const QModelIndex &index)
{
    pSelectitem = TreeModel->itemFromIndex(index);
    treeItemChanged(pSelectitem);
}
