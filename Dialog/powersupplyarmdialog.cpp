#include "powersupplyarmdialog.h"
#include "ui_powersupplyarmdialog.h"
#include <QStyleFactory>
#include <QMessageBox>
#include <QScrollBar>
/*设置供电臂状态*/
#pragma execution_character_set("utf-8")
PowerSupplyArmDialog::PowerSupplyArmDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PowerSupplyArmDialog)
{
    ui->setupUi(this);    
    this->setWindowTitle("电力臂状态操作 - TDCS/CTC3.0 通用车站操作终端");
    this->setFixedSize(this->width(),this->height());

    pQTreeView = new QTreeView(ui->widget);
    ui->widget->layout()->addWidget(pQTreeView);
    pQTreeView->setStyle(QStyleFactory::create("windows"));
    pQTreeView->verticalScrollBar()->setStyle(QStyleFactory::create("fusion"));
    pQTreeView->setStyleSheet("QHeaderView::section{border:none; border-right:1px solid #E0DDDC; background-color:rgb(198, 210, 218); padding: 2px 8px 2px 8px;}");

}

PowerSupplyArmDialog::~PowerSupplyArmDialog()
{
    delete ui;
}

void PowerSupplyArmDialog::Init(QVector<MyStation*> vectMyStation)
{
    pVectMyStation=vectMyStation;
    pQTreeView->setHeaderHidden(true);
    pQTreeView->setFrameShape(QFrame::NoFrame);
    pQTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    pQTreeView->setRootIsDecorated(true); // 开启根节点的装饰

    QList<QStandardItem*> list;
    TreeModel = new QStandardItemModel(pVectMyStation.size(), 2);
    for (int i=0;i<pVectMyStation.size();i++)
    {
        pVectMyStation[i]->SetPowerSupplyQJState();
        InitTree(pVectMyStation[i],i);
    }
    // 连接信号槽
    connect(TreeModel, &QStandardItemModel::itemChanged, this, &PowerSupplyArmDialog::treeItemChanged);
    pQTreeView->setModel(TreeModel);
    pQTreeView->expandAll();
    pQTreeView->setColumnWidth(0, 350);
    pQTreeView->setColumnWidth(1, 50);
    //mVectTree.append(pQTreeView);
}
void PowerSupplyArmDialog::InitTree(MyStation* pMyStation,int n)
{
    PowerStr *power=nullptr;
    QList<QStandardItem*> list;
    {
        QStandardItem *item1 = new QStandardItem(pMyStation->getStationName());
        QStandardItem *item2 = new QStandardItem("状态");
        item1->setSizeHint(QSize(item1->sizeHint().width(), 32));
        item2->setSizeHint(QSize(item2->sizeHint().width(), 32));
        item1->setCheckable(true);
        for (int j = 0; j < pMyStation->v_PowerArray.size(); j++)
        {
            pMyStation->v_PowerArray[j]->Item = new QStandardItem(pMyStation->v_PowerArray[j]->PowerName);
            pMyStation->v_PowerArray[j]->Item->setCheckable(true);
            list.append(pMyStation->v_PowerArray[j]->Item);
            QString Value;
            power = pMyStation->v_PowerArray[j];
            //判断股道设备是否有电
            Value=pMyStation->v_PowerArray[j]->PowerFlag==true?"有电":"停电";
            pMyStation->v_PowerArray[j]->Item1 = new QStandardItem(Value);
            list.append(pMyStation->v_PowerArray[j]->Item1);
            item1->appendRow(list);
            list.clear();
            TreeModel->setItem(n, 0, item1);
            TreeModel->setItem(n, 1, item2);
        }
    }
}

//刷新显示
void PowerSupplyArmDialog::UpdatePowerData()
{
    for (int i=0;i<pVectMyStation.size();i++)
    {
        pVectMyStation[i]->SetPowerSupplyQJState();
        //InitTree(pVectMyStation[i],i);
        for (int j = 0; j < pVectMyStation[i]->v_PowerArray.size(); j++)
        {
            pVectMyStation[i]->v_PowerArray[j]->Item1->setText(pVectMyStation[i]->v_PowerArray[j]->PowerFlag==true?"有电":"停电");
        }
    }

    //pQTreeView->setModel(TreeModel);
    //pQTreeView->expandAll();
    //pQTreeView->setColumnWidth(0, 350);
    //pQTreeView->setColumnWidth(1, 50);
    //pQTreeView->update();
}
void PowerSupplyArmDialog::selectAllChildren(QStandardItem *item)
{
    for (int i = 0; i < item->rowCount(); ++i)
    {
        //selectAllChildren(item->child(i));
        item->child(i)->setCheckState(item->checkState());
    }
}
int PowerSupplyArmDialog::GetStationIDByName(QString strStationName)
{
    for(int i=0;i<pVectMyStation.size();i++)
    {
        if(pVectMyStation[i]->getStationName()==strStationName)
        {
            return i;
        }
    }
    return -1;
}
//设置有电
void PowerSupplyArmDialog::on_pushButton_pressed()
{
    emit SendTextAnalysisSignal(52, QCursor::pos());
    int nStationIndex=-1;
    int ret = QMessageBox::information(nullptr, "TDCS/CTC 3.0 通用车站操作窗口",
                                       "确认设置为有电吗?", "是", "否");
    if(ret != 0)
        return;

    if(pVectTreeItemCheck.size()>0)
    {
        for(int i=0;i<pVectTreeItemCheck.size();i++)
        {
            nStationIndex=GetStationIDByName(pVectTreeItemCheck[i]->strStationName);
            if(nStationIndex!=-1)
            {
                emit SendPowerSupplyArmDataToSeverSignal(nStationIndex,pVectTreeItemCheck[i]->strItemCheckName,false);
            }
        }
    }
    Timer_ID_500 = startTimer(500);

}
//设置停电
void PowerSupplyArmDialog::on_pushButton_2_pressed()
{
    emit SendTextAnalysisSignal(53, QCursor::pos());
    int nStationIndex=-1;

    int ret = QMessageBox::information(nullptr, "TDCS/CTC 3.0 通用车站操作窗口",
                                       "确认设置为无电吗?", "是", "否");
    if(ret != 0)
        return;

    if(pVectTreeItemCheck.size()>0)
    {
        for(int i=0;i<pVectTreeItemCheck.size();i++)
        {
            nStationIndex=GetStationIDByName(pVectTreeItemCheck[i]->strStationName);
            if(nStationIndex!=-1)
            {
                emit SendPowerSupplyArmDataToSeverSignal(nStationIndex,pVectTreeItemCheck[i]->strItemCheckName,true);
            }
        }
    }
    Timer_ID_500 = startTimer(500);

}
//刷新状态
void PowerSupplyArmDialog::on_pushButton_3_pressed()
{
    emit SendTextAnalysisSignal(55, QCursor::pos());
    UpdatePowerData();
}
//关闭
void PowerSupplyArmDialog::on_pushButton_4_pressed()
{
    close();
    emit SendTextAnalysisSignal(54, QCursor::pos());
}
void PowerSupplyArmDialog::treeItemChanged(QStandardItem * item)
{
    if (item == nullptr)
        return ;
    if (!item->isCheckable())
        return ;
    QStandardItem *parentItem=nullptr;
    bool isChecked = item->checkState() == Qt::Checked;
    parentItem=item->parent();
    QString strparentItem="";

    //父节点被选中，即全站选中
    if(parentItem == nullptr)
    {
        selectAllChildren(item);
    }
    else//子节点被选中
    {
        if (isChecked)
        {
            TreeStandardItemCheck* pTreeStandardItemCheck=new TreeStandardItemCheck();
            strparentItem=parentItem->text();
            pTreeStandardItemCheck->strItemCheckName=item->text();
            pTreeStandardItemCheck->strStationName=parentItem->text();
            //保存被选中子节点
            pVectTreeItemCheck.append(pTreeStandardItemCheck);
        }
        else
        {
            //清除被选中子节点
            for(int i=0;i<pVectTreeItemCheck.size();i++)
            {
                if(pVectTreeItemCheck[i]->strStationName == parentItem->text()
                        && pVectTreeItemCheck[i]->strItemCheckName == item->text())
                    pVectTreeItemCheck.remove(i);
            }
        }
    }
}

void PowerSupplyArmDialog::timerEvent(QTimerEvent* enent)
{
    if(enent->timerId() == Timer_ID_500)
    {
        UpdatePowerData();
        killTimer(Timer_ID_500);
    }
}
