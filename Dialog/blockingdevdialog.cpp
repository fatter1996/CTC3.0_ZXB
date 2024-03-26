#include "blockingdevdialog.h"
#include "ui_blockingdevdialog.h"
#include <QStyleFactory>
#include <QMessageBox>
#include <QScrollBar>
/*封锁设备操作*/
#pragma execution_character_set("utf-8")
BlockingDevDialog::BlockingDevDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BlockingDevDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("封锁操作对话框 - TDCS/CTC3.0 通用车站操作终端");
    this->setFixedSize(this->width(),this->height());
    pTreelayout=new QVBoxLayout(ui->widget);
    pTreelayout->setMargin(0);
    pQTreeView=new QTreeView(ui->widget);
    pQTreeView->setStyle(QStyleFactory::create("windows"));
    pQTreeView->verticalScrollBar()->setStyle(QStyleFactory::create("fusion"));
    pQTreeView->setStyleSheet("QHeaderView::section{border:none; border-right:1px solid #E0DDDC; background-color:rgb(198, 210, 218); padding: 2px 8px 2px 8px;}");
}

BlockingDevDialog::~BlockingDevDialog()
{
    delete ui;
}

void BlockingDevDialog::Init(QVector<MyStation*> vectMyStation)
{
    pVectMyStation=vectMyStation;
    //pQTreeView->setHeaderHidden(true);
    pQTreeView->setFrameShape(QFrame::NoFrame);
    pQTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QList<QStandardItem*> list;
    TreeModel = new QStandardItemModel(pVectMyStation.size(), 2);
    TreeModel->setHeaderData(0, Qt::Horizontal, tr("设备"));
    TreeModel->setHeaderData(1, Qt::Horizontal, tr("封锁状态"));
    for (int i=0;i<pVectMyStation.size();i++)
    {
        pVectMyStation[i]->SetBlockingDevState();
        InitTree(pVectMyStation[i],i);
    }
    // 连接信号槽
    connect(TreeModel, &QStandardItemModel::itemChanged, this, &BlockingDevDialog::treeItemChanged);
    pQTreeView->setModel(TreeModel);
    pQTreeView->expandAll();
    pQTreeView->setColumnWidth(0, 350);
    pQTreeView->setColumnWidth(1, 50);
    pTreelayout->addWidget(pQTreeView);
    ui->widget->setLayout(pTreelayout);
}
void BlockingDevDialog::InitTree(MyStation* pMyStation,int n)
{
    qdData *power=nullptr;
    QList<QStandardItem*> list;
    {
        QStandardItem *item1 = new QStandardItem(pMyStation->getStationName());
        QStandardItem *subItemGD = new QStandardItem("股道");
        //创建股道设备节点
        for (int j = 0; j < pMyStation->v_GDArray.size(); j++)
        {
            pMyStation->v_GDArray[j]->item = new QStandardItem(pMyStation->v_GDArray[j]->qdName);
            pMyStation->v_GDArray[j]->item->setCheckable(true);
            pMyStation->v_GDArray[j]->item->setCheckState(UpadataTree(pMyStation,0,j));
            list.append(pMyStation->v_GDArray[j]->item);
            QString Value;
            power = pMyStation->v_GDArray[j];
            //判断股道设备是否有电
            Value=pMyStation->v_GDArray[j]->Blocked==true?"√":"";
            pMyStation->v_GDArray[j]->item1 = new QStandardItem(Value);
            list.append(pMyStation->v_GDArray[j]->item1);
            subItemGD->appendRow(list);
            list.clear();
        }
        item1->appendRow(subItemGD);
        //创建道岔设备节点
        QStandardItem *subItemQJ = new QStandardItem("区间");
        for (int j = 0; j < pMyStation->v_QJArray.size(); j++)
        {
            pMyStation->v_QJArray[j]->item = new QStandardItem(pMyStation->v_QJArray[j]->qdName);
            pMyStation->v_QJArray[j]->item->setCheckable(true);
            pMyStation->v_QJArray[j]->item->setCheckState(UpadataTree(pMyStation,1,j));

            list.append(pMyStation->v_QJArray[j]->item);
            QString Value;
            power = pMyStation->v_QJArray[j];
            //判断股道设备是否有电
            Value=pMyStation->v_QJArray[j]->Blocked==true?"√":"";
            pMyStation->v_QJArray[j]->item1 = new QStandardItem(Value);
            list.append(pMyStation->v_QJArray[j]->item1);
            subItemQJ->appendRow(list);
            list.clear();
        }
        item1->appendRow(subItemQJ);
        TreeModel->setItem(n, 0, item1);
    }
}
Qt::CheckState BlockingDevDialog::UpadataTree(MyStation* pMyStation,int ntype,int n)
{
//    /

    for(int i=0;i<pVectTreeItemCheck.size();i++)
    {
        if(ntype==0)//区间
        {
             if(pVectTreeItemCheck[i]->strStationName==pMyStation->getStationName()&&
                     pVectTreeItemCheck[i]->strItemCheckName==pMyStation->v_GDArray[n]->qdName)
             {
                 return Qt::Checked;
             }
        }
        else//区间
        {
            if(pVectTreeItemCheck[i]->strStationName==pMyStation->getStationName()&&
                    pVectTreeItemCheck[i]->strItemCheckName==pMyStation->v_QJArray[n]->qdName)
            {
                 return Qt::Checked;
            }
        }

    }
    return Qt::Unchecked;
}
int BlockingDevDialog::GetStationIDByName(QString strStationName)
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
 QList<int> BlockingDevDialog::GetDevCodeByName(QString strStationName,QString strType,QString strDevName)
 {
     QList<int> ncodelist={};
     if(strType=="股道")//股道
     {
          for (int i=0;i<pVectMyStation.size();i++)
          {
              if(pVectMyStation[i]->getStationName()==strStationName)
              {
                  for (int j = 0; j < pVectMyStation[i]->v_GDArray.size(); j++)
                  {
                      if(pVectMyStation[i]->v_GDArray[j]->qdName==strDevName)
                      {
                          ncodelist.append(pVectMyStation[i]->v_GDArray[j]->qdnode);
                          return ncodelist;
                      }
                  }
              }
          }
     }
     else if(strType=="区间")//区间
     {
         for (int i=0;i<pVectMyStation.size();i++)
         {
             if(pVectMyStation[i]->getStationName()==strStationName)
             {
                 for (int j = 0; j < pVectMyStation[i]->v_QJArray.size(); j++)
                 {
                     if(pVectMyStation[i]->v_QJArray[j]->qdName==strDevName)
                     {
                         ncodelist=pVectMyStation[i]->v_QJArray[j]->FSDEVCode;
                         return ncodelist;
                     }
                 }
             }
         }
     }
     return ncodelist;
 }

void BlockingDevDialog::UpdatePowerData()
{
    for (int i=0;i<pVectMyStation.size();i++)
    {
        pVectMyStation[i]->SetBlockingDevState();

        for (int j = 0; j < pVectMyStation[i]->v_GDArray.size(); j++)
        {
            pVectMyStation[i]->v_GDArray[j]->item1->setText(pVectMyStation[i]->v_GDArray[j]->Blocked==true?"√":"");
        }
        for (int j = 0; j < pVectMyStation[i]->v_QJArray.size(); j++)
        {
            pVectMyStation[i]->v_QJArray[j]->item1->setText(pVectMyStation[i]->v_QJArray[j]->Blocked==true?"√":"");
        }
    }
}
//设置封锁
void BlockingDevDialog::on_pushButton_pressed()
{
    emit SendTextAnalysisSignal(56, QCursor::pos());
    int nStationIndex=-1;
    int ret = QMessageBox::information(nullptr, "TDCS/CTC 3.0 通用车站操作窗口", "确认封锁操作?", "是", "否");
    if(ret != 0)
        return;
    MyStation* pStation = nullptr;
    CGD* pGd = nullptr;
    bool bLock = false;
    bool bFS = false;
    if(pVectTreeItemCheck.size()>0)
    {
        for(int i=0;i<pVectTreeItemCheck.size();i++)
        {
            nStationIndex=GetStationIDByName(pVectTreeItemCheck[i]->strStationName);
            pStation = pVectMyStation[nStationIndex];
            bLock = false;
            bFS = false;
            if(nStationIndex!=-1)
            {
                for(int j=0; j < pVectTreeItemCheck[i]->nDevCode.size(); j++)
                {
                    pGd = pStation->GetGDByCode(pVectTreeItemCheck[i]->nDevCode[j]);
                    if(pGd)
                    {
                        bLock |= pGd->isLock;
                        bFS |= pGd->isFS;
                    }
                }
                if(bLock)
                    ui->textBrowser->append(QDateTime::currentDateTime().toString("hh:mm:ss")+"  区段有锁闭,无法进行封锁操作");
                else if(bFS)
                    ui->textBrowser->append(QDateTime::currentDateTime().toString("hh:mm:ss")+"  区段已封锁,无法再封锁");
                else ui->textBrowser->append(QDateTime::currentDateTime().toString("hh:mm:ss")+"  操作成功");
                emit SendBlockingDevDataToSeverSignal(nStationIndex,pVectTreeItemCheck[i]->nDevCode,true);
            }
        }

         Timer_ID_1000 = startTimer(1000);
    }

}
//解除封锁
void BlockingDevDialog::on_pushButton_2_pressed()
{
    emit SendTextAnalysisSignal(57, QCursor::pos());
    int nStationIndex=-1;
    int ret = QMessageBox::information(nullptr, "TDCS/CTC 3.0 通用车站操作窗口", "确认解除封锁操作?", "是", "否");
    if(ret != 0)
        return;
    MyStation* pStation = nullptr;
    CGD* pGd = nullptr;
    bool bLock = false;
    bool bFS = false;
    if(pVectTreeItemCheck.size()>0)
    {
        for(int i=0;i<pVectTreeItemCheck.size();i++)
        {
            nStationIndex=GetStationIDByName(pVectTreeItemCheck[i]->strStationName);
            pStation = pVectMyStation[nStationIndex];
            bLock = false;
            bFS = false;
            if(nStationIndex != -1)
            {
                for(int j=0; j < pVectTreeItemCheck[i]->nDevCode.size(); j++)
                {
                    pGd = pStation->GetGDByCode(pVectTreeItemCheck[i]->nDevCode[j]);
                    if(pGd)
                    {
                        bLock |= pGd->isLock;
                        bFS |= pGd->isFS;
                    }
                }
                if(!pGd->isFS)
                    ui->textBrowser->append(QDateTime::currentDateTime().toString("hh:mm:ss")+"  区段未封锁,无法解除封锁");
                else ui->textBrowser->append(QDateTime::currentDateTime().toString("hh:mm:ss")+"  操作成功");
                emit SendBlockingDevDataToSeverSignal(nStationIndex,pVectTreeItemCheck[i]->nDevCode,false);
            }
        }

        Timer_ID_1000 = startTimer(1000);
    }

}
//关闭
void BlockingDevDialog::on_pushButton_4_pressed()
{
    close();
    emit SendTextAnalysisSignal(54, QCursor::pos());
}
void BlockingDevDialog::treeItemChanged(QStandardItem * item)
{
    if (item == nullptr)
        return ;
    if (!item->isCheckable())
        return ;
    QStandardItem *parentItem = nullptr;
    bool isChecked = item->checkState() == Qt::Checked;
    parentItem = item->parent();
    QString strparentItem="";
    TreeStandardItemCheckBlock* pTreeStandardItemCheck=new TreeStandardItemCheckBlock();
    if (isChecked)
    {
        strparentItem=parentItem->text();
        pTreeStandardItemCheck->strItemCheckName=item->text();
        pTreeStandardItemCheck->strStationName=parentItem->parent()->text();
        pTreeStandardItemCheck->nDevCode=GetDevCodeByName(parentItem->parent()->text(),parentItem->text(),item->text());
        //保存被选中子节点
        pVectTreeItemCheck.append(pTreeStandardItemCheck);
    }
    else
    {
        //清除被选中子节点
        for(int i=0;i<pVectTreeItemCheck.size();i++)
        {
            if(pVectTreeItemCheck[i]->strItemCheckName == item->text())
                pVectTreeItemCheck.remove(i);
        }
    }
}
void BlockingDevDialog::timerEvent(QTimerEvent *event)//定时器
{
    if(event->timerId() == Timer_ID_1000/*&&this->isHidden()*/)
    {
        UpdatePowerData();
        killTimer(Timer_ID_1000);
    }

}
