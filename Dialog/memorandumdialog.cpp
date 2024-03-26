#include "memorandumdialog.h"
#include "ui_memorandumdialog.h"
#include <QDateTime>
#include <QDebug>
#include <QMessageBox>
/*备忘录*/
#pragma execution_character_set("utf-8")
MemorandumDialog::MemorandumDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MemorandumDialog)
{
    ui->setupUi(this);
    Init();
    InitTable();
    SetBtnEn(false);
    InitMenu();
    InitTree();
    this->setFixedSize(this->width(),this->height());
}
void MemorandumDialog::Init()
{
    QDateTime time=QDateTime::currentDateTime();
    ui->dateTimeEdit_3->setDateTime(time);
    ui->dateTimeEdit_4->setDateTime(time);
    ui->dateTimeEdit_5->setDateTime(time);
    SetTimeEn(false);
}
void MemorandumDialog::SetTimeEn(bool b)
{
    ui->dateTimeEdit_3->setEnabled(b);
    ui->dateTimeEdit_5->setEnabled(b);
}
MemorandumDialog::~MemorandumDialog()
{
    delete ui;
}
void MemorandumDialog::InitTable()
{
    InitTableView();

    mCTableView2->SetTableHeadHideV(false);
    mCTableView2->SetTableHeadHideH(true);
    mCTableView2->CreatTableView(true);
    mCTableView2->SetEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->widget_2->layout()->addWidget(mCTableView2);
    //鼠标左键点击响应
    //connect(mCTableView,SIGNAL(itemLBtnClicked(QModelIndex)),this,SLOT(itemLBtnClicked(QModelIndex)));
}
//初始化表格数据
void MemorandumDialog::InitTableView()
{
    mCTableView2= new TableView();
    mCTableView2->mTableViewInfo.Init();
    mCTableView2->mTableViewInfo.Vect_HeaderData.append("用户编号");
    mCTableView2->mTableViewInfo.Vect_HeaderDataWidth.append(80);
    mCTableView2->mTableViewInfo.Vect_HeaderData.append("用户名称");
    mCTableView2->mTableViewInfo.Vect_HeaderDataWidth.append(80);
    mCTableView2->mTableViewInfo.Vect_HeaderData.append("确认时间");
    mCTableView2->mTableViewInfo.Vect_HeaderDataWidth.append(160);
    mCTableView2->mTableViewInfo.Vect_HeaderData.append("终端");
    mCTableView2->mTableViewInfo.Vect_HeaderDataWidth.append(130);

    mCTableView2->SetTableHeadHeight(0, 24);
    mCTableView2->SetTableHeadHeight(1, 16);
    mCTableView2->IintTableColAndRow(0, 4);
    mCTableView2->SetTableHeadsetStyleSheet(1,
        "QHeaderView{"
            "background:rgb(240, 240, 240);"
        "}"
        "QHeaderView::section:horizontal {"
            "background:rgb(240, 240, 240);"
            "font-size: 12px;"
            "font-weight: normal;"
        "}");
}

//设置按钮使能状态
void MemorandumDialog::SetBtnEn(bool b)
{
    ui->pushButton_4->setEnabled(b);
    ui->pushButton_5->setEnabled(b);
    ui->pushButton_6->setEnabled(b);
}
void MemorandumDialog::InitMenu()
{
    m_menu = new QMenu(this);
    m_menu->setStyleSheet("background-color: rgb(204, 228, 247);");
    QAction* ZCCSetAction = new QAction(m_menu);//无操作
    QAction* TrainZTAPAction = new QAction(m_menu);//调度命令
    QAction* TrainGTAPAction = new QAction(m_menu);//阶段计划
    QAction* TrainBWAction = new QAction(m_menu);//备忘
    ZCCSetAction->setText(QObject::tr("无操作"));
    TrainZTAPAction->setText(QObject::tr("调度命令"));
    TrainGTAPAction->setText(QObject::tr("阶段计划"));
    TrainBWAction->setText(QObject::tr("备忘"));
    m_menu->addAction(ZCCSetAction);
    m_menu->addAction(TrainZTAPAction);
    m_menu->addAction(TrainGTAPAction);
    m_menu->addAction(TrainBWAction);
    ui->pushButton_8->setMenu(m_menu);
    connect(m_menu, &QMenu::triggered, this, &MemorandumDialog::MenuPopSignal);
}
void MemorandumDialog::InitTree()
{
    ui->treeWidget->setHeaderHidden(true);
    topItem1 = new QTreeWidgetItem(ui->treeWidget);
    topItem1->setHidden(true);
    ui->treeWidget->expandAll();
    // 连接itemSelectionChanged信号到槽函数
    connect(ui->treeWidget, &QTreeWidget::itemSelectionChanged, this, &MemorandumDialog::onItemSelected1);
}
TreeShowData* MemorandumDialog::GetSaveData()
{
    TreeShowData* pTreeShowData=new TreeShowData();
    pTreeShowData->Init();
    pTreeShowData->strFlag=ui->pushButton_8->text();
    pTreeShowData->strTitel=ui->lineEdit->text();
    pTreeShowData->strMemorandumm=ui->textEdit->toPlainText();

    pTreeShowData->timeNew=ui->dateTimeEdit_3->dateTime();
    pTreeShowData->timeStart=QDateTime::currentDateTime()/*ui->dateTimeEdit_4->dateTime()*/;
    pTreeShowData->timeEnd=ui->dateTimeEdit_5->dateTime();

    return pTreeShowData;
}
void MemorandumDialog::UpdateTabWidgetShow(QString str)
{
    QStringList strList;
    strList<<""<<""<<""<<"";
    if(str=="")
    {
        mCTableView2->Clear();
    }
    else
    {
       strList[0]="001";
       strList[1]="001";
       strList[2]=str;
       strList[3]="值班员";
       mCTableView2->SetTableData(strList);
    }

}
int MemorandumDialog::GetTabItemAndViewInfo1(QTreeWidgetItem *mItem)
{
    for (int i=0;i<mVectTreeShowData.size();i++)
    {
        if(mVectTreeShowData[i]->mItem==mItem)
        {
            return i;
        }
    }
    return -1;
}
void MemorandumDialog::SetviewInfo(TreeShowData *pTreeShowData)
{

    ui->pushButton_8->setText(pTreeShowData->strFlag);
    ui->lineEdit->setText(pTreeShowData->strTitel);
    ui->textEdit->setText(pTreeShowData->strMemorandumm);

    ui->dateTimeEdit_3->setDateTime(pTreeShowData->timeNew);
    ui->dateTimeEdit_4->setDateTime(pTreeShowData->timeStart);
    ui->dateTimeEdit_5->setDateTime(pTreeShowData->timeEnd);
}
bool MemorandumDialog::GetOrderActivationState()
{
    for (int i=0;i<mVectTreeShowData.size();i++)
    {
        if(mVectTreeShowData[i]->bActivation==1)
        {
            return true;
        }
    }
    return false;
}
void MemorandumDialog::ResetStation()
{
    mVectTreeShowData.clear();
    mCTableView2->Clear();
    Init();
    while (topItem1->childCount()>0) {
        QTreeWidgetItem* mQTreeWidgetItem=topItem1->child(0);
        topItem1->removeChild(mQTreeWidgetItem);
    }
    topItem1->setHidden(true);
    ui->treeWidget->update();
    ui->lineEdit->clear();
    ui->pushButton_8->setText("");
    update();
}
void MemorandumDialog::ClearEditInfo()
{
    ui->lineEdit->clear();
    ui->textEdit->clear();
    ui->pushButton_8->setText("");
}
//刷新树形目录
void MemorandumDialog::UpDataTreeShow()
{
    for(int i=0;i<mVectTreeShowData.size();i++)
    {
        TreeShow* mTreeShow=mVectTreeShowData[i];
        mTreeShow->pTreeShowData=GetSaveData();
        int index=i+1;
        QString stritem="        "+QString::number(index)+"             "
                +mTreeShow->pTreeShowData->strTitel+"           "
                +mTreeShow->pTreeShowData->timetree.toString("yyyy-MM-dd hh:mm:ss");
        mTreeShow->mItem->setText(0,stritem);
        ui->treeWidget->expandAll();
        ui->treeWidget->update();
    }
}
void MemorandumDialog::MenuPopSignal(QAction *action)
{
    ui->pushButton_8->setText(action->text());
}
//新建
void MemorandumDialog::on_pushButton_2_pressed()
{
    emit SendTextAnalysisSignal(157, QCursor::pos());
    ui->lineEdit->clear();
    ui->textEdit->clear();
    ui->pushButton_8->setText("");
    QDateTime time=QDateTime::currentDateTime();
    ui->dateTimeEdit_4->setDateTime(time);
    ui->dateTimeEdit_3->setDateTime(time);
    ui->dateTimeEdit_5->setDateTime(time);
    SetTimeEn(true);
    bnew=true;
    ui->treeWidget->clearSelection();
    indexcheck=-1;
}
//备忘  保存
void MemorandumDialog::on_pushButton_3_pressed()
{
    emit SendTextAnalysisSignal(158, QCursor::pos());
    if(ui->dateTimeEdit_3->isEnabled())
    {
        if(topItem1->isHidden())
        {
            topItem1->setHidden(false);
            QDateTime date=QDateTime::currentDateTime();
            topItem1->setText(0,date.toString("yyyy-MM-dd"));
        }

        if(bnew)//新建
        {
            bnew=false;
            TreeShow* mTreeShow=new TreeShow();
            mTreeShow->pTreeShowData=GetSaveData();

            mTreeShow->mItem= new QTreeWidgetItem(topItem1);
            mTreeShow->mItem->setForeground(0,QBrush(Qt::red));
            int index=mVectTreeShowData.size()+1;
            mTreeShow->pTreeShowData->timetree=QDateTime::currentDateTime();
            QString stritem="        "+QString::number(index)+"             "
                    +mTreeShow->pTreeShowData->strTitel+"           "
                    +mTreeShow->pTreeShowData->timetree.toString("yyyy-MM-dd hh:mm:ss");
            mTreeShow->mItem->setText(0,stritem);
            ui->treeWidget->addTopLevelItem(topItem1);
            ui->treeWidget->expandAll();
            ui->treeWidget->update();
            mVectTreeShowData.append(mTreeShow);
            SetTimeEn(false);
            timerId =startTimer(1000*1);
        }
        else if(indexcheck>=0&&indexcheck<mVectTreeShowData.size())//修改编辑
        {
            TreeShow* mTreeShow=mVectTreeShowData[indexcheck];
            mTreeShow->pTreeShowData=GetSaveData();
            int index=indexcheck+1;
            QString stritem="        "+QString::number(index)+"             "
                    +mTreeShow->pTreeShowData->strTitel+"           "
                    +mTreeShow->pTreeShowData->timetree.toString("yyyy-MM-dd hh:mm:ss");
            mTreeShow->mItem->setText(0,stritem);
            ui->treeWidget->expandAll();
            ui->treeWidget->update();
            SetTimeEn(false);
        }

    }
    else
    {
        QMessageBox::critical(this, tr("错误"), tr("请输入合法数据!"),tr("确定"),"",0);
    }
    ui->lineEdit->clear();
    ui->textEdit->clear();
    ui->pushButton_8->setText("");
}
//结束 删除
void MemorandumDialog::on_pushButton_4_pressed()
{
    emit SendTextAnalysisSignal(159, QCursor::pos());
    QTreeWidgetItem *item = ui->treeWidget->currentItem();
    if(item!=nullptr)
    {
       int index=GetTabItemAndViewInfo1(item);
       if(index!=-1&&index<mVectTreeShowData.size())
       {
           mVectTreeShowData.remove(index);
           //ui->treeWidget->takeTopLevelItem(ui->treeWidget->indexOfTopLevelItem(item));
           delete item;
           if(mVectTreeShowData.size()==0)
           {
               topItem1->setHidden(true);
               killTimer(timerId);
           }
           //ui->treeWidget->update();
           UpDataTreeShow();
       }
    }
    ClearEditInfo();
     mCTableView2->Clear();
}
//得知 在用户信息表中显示
void MemorandumDialog::on_pushButton_5_pressed()
{
    emit SendTextAnalysisSignal(160, QCursor::pos());
    QTreeWidgetItem *item = ui->treeWidget->currentItem();
    if(item!=nullptr)
    {
       int index=GetTabItemAndViewInfo1(item);
       if(index!=-1&&index<mVectTreeShowData.size())
       {
           mVectTreeShowData[index]->mItem->setForeground(0,QBrush(Qt::black));
           //在有效时间内
           if(mVectTreeShowData[index]->bActivation==0||mVectTreeShowData[index]->bActivation==1)
           {
               mVectTreeShowData[index]->bdz=true;
               UpdateTabWidgetShow(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
           }
           //在有效时间外
           else if(mVectTreeShowData[index]->bActivation==2)
           {
               mVectTreeShowData.remove(index);
               //ui->treeWidget->takeTopLevelItem(ui->treeWidget->indexOfTopLevelItem(item));
               delete item;
               if(mVectTreeShowData.size()==0)
               {
                   topItem1->setHidden(true);
                   killTimer(timerId);
               }
               //ui->treeWidget->update();
                UpDataTreeShow();
           }

       }
    }
    ClearEditInfo();
}
//执行
void MemorandumDialog::on_pushButton_6_pressed()
{
    emit SendTextAnalysisSignal(161, QCursor::pos());
}
//关闭
void MemorandumDialog::on_pushButton_7_pressed()
{
    emit SendTextAnalysisSignal(8, QCursor::pos());
    close();
}
void MemorandumDialog::onItemSelected1()
{
     QTreeWidgetItem *item = ui->treeWidget->currentItem();
     SetBtnEn(true);
     if(item!=nullptr)
     {
        int index=GetTabItemAndViewInfo1(item);
        if(index!=-1&&index<mVectTreeShowData.size())
        {
            indexcheck=index;
            SetTimeEn(true);
            SetviewInfo(mVectTreeShowData[index]->pTreeShowData);
            SetBtnEn(true);
        }
     }
     else
     {
         UpdateTabWidgetShow("");
     }
}
void MemorandumDialog::timerEvent(QTimerEvent *event)//定时器
{
     if(event->timerId() == timerId)
    {
         bool bBtnChange=false;
         for (int i=0;i<mVectTreeShowData.size();i++)
         {
             QDateTime date=QDateTime::currentDateTime();
             qint64 ntimeS=date.toSecsSinceEpoch()-mVectTreeShowData[i]->pTreeShowData->timeNew.toSecsSinceEpoch();
             qint64 ntimeE=mVectTreeShowData[i]->pTreeShowData->timeEnd.toSecsSinceEpoch()-date.toSecsSinceEpoch();
             if(ntimeS>0)
             {
                 mVectTreeShowData[i]->bActivation=1;
                 if(!mVectTreeShowData[i]->mItem->text(0).contains("*"))
                 {
                     TreeShow* mTreeShow=mVectTreeShowData[i];
                     QString stritem="        "+QString::number(i+1)+"             "
                             +"(*)"+mTreeShow->pTreeShowData->strTitel+"           "
                             +mTreeShow->pTreeShowData->timetree.toString("yyyy-MM-dd hh:mm:ss");
                     mTreeShow->mItem->setText(0,stritem);
                     ui->treeWidget->update();
                 }
             }
             if(ntimeE<0)
             {
                 mVectTreeShowData[i]->bActivation=2;
                 if(mVectTreeShowData[i]->mItem->text(0).contains("*"))
                 {
                     TreeShow* mTreeShow=mVectTreeShowData[i];
                     QString stritem="        "+QString::number(i+1)+"             "
                             +mTreeShow->pTreeShowData->strTitel+"           "
                             +mTreeShow->pTreeShowData->timetree.toString("yyyy-MM-dd hh:mm:ss");
                     mTreeShow->mItem->setText(0,stritem);
                     ui->treeWidget->update();
                 }
             }
             if(mVectTreeShowData[i]->bActivation==1&&!mVectTreeShowData[i]->bdz)
             {
                 bBtnChange=true;
             }
         }
         emit SendBtnIncoChangeSignal(bBtnChange);
    }

    update();
}
