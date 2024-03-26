#include "stationstoragesetdialog.h"
#include "ui_stationstoragesetdialog.h"
#include <QChar>
#include <QMessageBox>
#pragma execution_character_set("utf-8")
StationStorageSetDialog::StationStorageSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StationStorageSetDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("股道状态设置 - TDCS/CTC3.0 通用车站操作终端"));
    this->setFixedSize(this->width(),this->height());
    Init();
    InitTable();
}

StationStorageSetDialog::~StationStorageSetDialog()
{
    delete ui;
}
void StationStorageSetDialog::Init()
{
    //股道
    ui->lineEdit_2->clear();
    //股道存车占用信息
    ui->textBrowser->clear();
    //类别
    ui->comboBox->addItem("空车");
    ui->comboBox->addItem("重车");
    //车种
    ui->comboBox_2->addItem("棚车");
    ui->comboBox_2->addItem("敞车");
    ui->comboBox_2->addItem("平板车");
    ui->comboBox_2->addItem("罐车");
    ui->comboBox_2->addItem("保温车");
    ui->comboBox_2->addItem("集装箱车");
    ui->comboBox_2->addItem("长大货车");
    ui->comboBox_2->addItem("矿石车");
    ui->comboBox_2->addItem("特种车");
    ui->comboBox_2->addItem("水泥车");
    ui->comboBox_2->addItem("家畜车");
    ui->comboBox_2->addItem("毒车");
    ui->comboBox_2->addItem("守车");
    ui->comboBox_2->addItem("罐轻车");
    ui->comboBox_2->addItem("罐粘车");
    ui->comboBox_2->addItem("罐滑车");
    ui->comboBox_2->addItem("机保车");
    ui->comboBox_2->addItem("其他车种");
    ui->comboBox_2->addItem("冰保车");
    ui->comboBox_2->addItem("客车");
    ui->comboBox_2->addItem("罐其他");
    ui->comboBox_2->addItem("机车");
    //作业说明
    ui->comboBox_3->addItem("");
    ui->comboBox_3->addItem("卸");
    ui->comboBox_3->addItem("装");
    ui->comboBox_3->addItem("保留");
    ui->comboBox_3->addItem("待卸");
    ui->comboBox_3->addItem("待装");
    ui->comboBox_3->addItem("检修");
    ui->comboBox_3->addItem("洗刷");
    ui->comboBox_3->addItem("整理");
    //运用标记
    ui->comboBox_5->addItem("");
    ui->comboBox_5->addItem("运用");
    ui->comboBox_5->addItem("厂修");
    ui->comboBox_5->addItem("段修");
    ui->comboBox_5->addItem("部备");
    ui->comboBox_5->addItem("发电");
    ui->comboBox_5->addItem("供应");
    ui->comboBox_5->addItem("回检");
    ui->comboBox_5->addItem("回送");
    ui->comboBox_5->addItem("军用");
    ui->comboBox_5->addItem("水罐");
    ui->comboBox_5->addItem("行包");
    ui->comboBox_5->addItem("整备");
    ui->comboBox_5->addItem("租用");
    ui->comboBox_5->addItem("待报废");
    ui->comboBox_5->addItem("工作车");
    ui->comboBox_5->addItem("检衡车");
    ui->comboBox_5->addItem("检修车");
    ui->comboBox_5->addItem("路用车");
    ui->comboBox_5->addItem("生活车");
    ui->comboBox_5->addItem("洗罐车");
    ui->comboBox_5->addItem("代客货车");
    ui->comboBox_5->addItem("局管空车");
    ui->comboBox_5->addItem("整备罐车");
    ui->comboBox_5->addItem("租用空车");
    ui->comboBox_5->addItem("改装试验车");
    //说明/去向
    ui->lineEdit->clear();
}
void StationStorageSetDialog::InitEditZoneShow()
{
    ui->comboBox->setCurrentIndex(0);
    ui->comboBox_2->setCurrentIndex(0);
    ui->comboBox_3->setCurrentIndex(0);
    ui->comboBox_5->setCurrentIndex(0);
    ui->lineEdit->clear();
    ui->spinBox->setValue(0);
}
void StationStorageSetDialog::InitTable()
{
    InitTableView();
    mCTableView->SetTableHeadHideV(false);
    mCTableView->SetTableHeadHideH(true);
    mCTableView->CreatTableView(false);
    mCTableView->SetEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->widget_5->layout()->addWidget(mCTableView);
    //鼠标左键点击响应
    connect(mCTableView,SIGNAL(itemLBtnClicked(QModelIndex)),this,SLOT(itemLBtnClicked(QModelIndex)));
}
//初始化股道信息
void StationStorageSetDialog::InitGD(GDData* mGDData)
{
   pGDData= mGDData;
   ui->lineEdit_2->setText(pGDData->pGD->getName());
   ui->groupBox_2->setTitle(pGDData->pGD->getName()+" 存车信息");
   mVectTableShowData=pGDData->mVectTableShowData;
   //存车信息
   //if(bclear)
   {
       UpDateTableView();
       UpDateView();
       InitEditZoneShow();
   }

}
void StationStorageSetDialog::SetTableData(QStandardItemModel *m_pModel,QStringList strlist,int nColumnCount)
{
    int nlen = strlist.size();
    for(int i = 0; i < nlen / nColumnCount; i++)
    {
        QString str="";
        for(int j=0;j<nColumnCount;j++)
        {
            if(nlen>((i*nColumnCount)+j))
                str=strlist[(i*nColumnCount)+j];
            else
                str="";
            QStandardItem* item = new QStandardItem(str);
            item->setText(str);
            item->setTextAlignment(Qt::AlignLeft);
            m_pModel->setItem(i,j,item);
        }
    }
}
void StationStorageSetDialog::SetTextBrowser(QString str)
{
    ui->textBrowser->setText(str);
}
void StationStorageSetDialog::SetTextBrowser()
{
    QString str="";
    QString strnull=" ";
    for(int i=0;i<mVectTableShowData.size();i++)
    {
        str=str+mVectTableShowData[i].strGDTrain+strnull;
    }
    ui->textBrowser->setText(str);
}
QString StationStorageSetDialog::GetTextBrowser()
{
    return ui->textBrowser->toPlainText();
}
QString StationStorageSetDialog::GetTextBrowserAll()
{
    QString str="";
    for(int i=0;i<mVectTableShowData.size();i++)
    {
        if(i==mVectTableShowData.size()-1)
        {
            str=str+mVectTableShowData[i].strGDTrain;
        }
        else
        {
            str=str+mVectTableShowData[i].strGDTrain+" ";
        }
    }
    return str;
}
QString StationStorageSetDialog::convertChineseToPinyin(QString chinese)
{
    QString pinyin = chinese;
    if(chinese.contains("棚车"))
    {
          pinyin="P";
    }
    else if(chinese.contains("敞车"))
    {
        pinyin="C";
    }
    else if(chinese.contains("平板车"))
    {
        pinyin="N";
    }
    else if(chinese.contains("罐车"))
    {
        pinyin="G";
    }
    else if(chinese.contains("保温车"))
    {
        pinyin="B";
    }
    else if(chinese.contains("集装箱车"))
    {
        pinyin="X";
    }
    else if(chinese.contains("长大货车"))
    {
        pinyin="D";
    }
    else if(chinese.contains("矿石车"))
    {
        pinyin="K";
    }
    else if(chinese.contains("特种车"))
    {
        pinyin="T";
    }
    else if(chinese.contains("水泥车"))
    {
        pinyin="U";
    }
    else if(chinese.contains("家畜车"))
    {
        pinyin="J";
    }
    else if(chinese.contains("毒车"))
    {
        pinyin="W";
    }
    else if(chinese.contains("守车"))
    {
        pinyin="S";
    }
    else if(chinese.contains("罐轻车"))
    {
        pinyin="GQ";
    }
    else if(chinese.contains("罐粘车"))
    {
        pinyin="GN";
    }
    else if(chinese.contains("罐滑车"))
    {
        pinyin="GH";
    }
    else if(chinese.contains("机保车"))
    {
        pinyin="BJ";
    }
    else if(chinese.contains("其他车种"))
    {
        pinyin="O";
    }
    else if(chinese.contains("冰保车"))
    {
        pinyin="BB";
    }
    else if(chinese.contains("客车"))
    {
        pinyin="KC";
    }
    else if(chinese.contains("罐其他"))
    {
        pinyin="GT";
    }
    else if(chinese.contains("机车"))
    {
        pinyin="JC";
    }
    else
    {
        pinyin="";
    }
    return pinyin;
}
void StationStorageSetDialog::InitTableView()
{
    mCTableView= new TableView();
    mCTableView->mTableViewInfo.Init();
    mCTableView->mTableViewInfo.Vect_HeaderData.append("车厢类型");
    mCTableView->mTableViewInfo.Vect_HeaderDataWidth.append(80);
    mCTableView->mTableViewInfo.Vect_HeaderData.append("辆数");
    mCTableView->mTableViewInfo.Vect_HeaderDataWidth.append(50);
    mCTableView->mTableViewInfo.Vect_HeaderData.append("说明/去向");
    mCTableView->mTableViewInfo.Vect_HeaderDataWidth.append(100);
    mCTableView->mTableViewInfo.Vect_HeaderData.append("运用标记");
    mCTableView->mTableViewInfo.Vect_HeaderDataWidth.append(80);

    mCTableView->SetTableHeadHeight(0, 24);
    mCTableView->SetTableHeadHeight(1, 16);
    mCTableView->IintTableColAndRow(0, 4);
    mCTableView->SetTableHeadsetStyleSheet(1,
        "QHeaderView{"
            "background:rgb(240, 240, 240);"
        "}"
        "QHeaderView::section:horizontal {"
            "background:rgb(240, 240, 240);"
            "font-size: 12px;"
            "font-weight: normal;"
        "}");
}

void StationStorageSetDialog::UpDateTableView()
{
    QStringList tableshow;
    for(int i=0;i<mVectTableShowData.size();i++)
    {
        tableshow=tableshow+mVectTableShowData[i].strlist;
    }
     mCTableView->SetTableData(tableshow);
}
void StationStorageSetDialog::AddGDSet()
{
    //类别
    QString strlb=ui->comboBox->currentText();
    //车种
    QString strcz=ui->comboBox_2->currentText();
    //作业说明
    QString strzysm=ui->comboBox_3->currentText();
    //辆数
    QString strnum=ui->spinBox->text();
    //运用标记
    QString strbj=ui->comboBox_5->currentText();
    //去向
    QString strqx=ui->lineEdit->text();
    //股道占用信息
    QString strGDTrain=ui->textBrowser->toPlainText();
    //车辆数目不能为0
    if(strnum.toInt()>0)
    {
        QStringList tableshow;
        tableshow<<strlb.left(1)+strzysm+strcz<<strnum<<strqx<<strbj;
        TableShowData mTableShowData;
        mTableShowData.Init();
        mTableShowData.strlist<<strlb.left(1)+strzysm+strcz<<strnum<<strqx<<strbj;
        mTableShowData.strGDTrain=strGDTrain;
        QString pinyin = convertChineseToPinyin(strcz);
        if(strqx!="")
        {
            strqx="("+strqx+")";
        }
        mTableShowData.strGDTrain=strlb.left(1)+strzysm+pinyin+strnum+strqx;
        //SetTextBrowser(mTableShowData.strGDTrain);


        //类别
        mTableShowData.strlb=strlb;
        //车种
        mTableShowData.strcz=strcz;
        //作业说明
        mTableShowData.strzysm=strzysm;
        //辆数
        mTableShowData.strnum=strnum;
        //运用标记
        mTableShowData.strbj=strbj;
        //说明
        mTableShowData.strqx=strqx;
        mVectTableShowData.append(mTableShowData);
        SetTextBrowser();
        UpDateTableView();
    }
    else
    {
        QMessageBox::information(this,tr("提示"),"辆数值无效",tr("确定"),"",0);
    }
}
void StationStorageSetDialog::SetBacklackRow(TableView* mCTableView,int index)
{
    QColor color,color2;
    color = QColor(5, 125, 255);
    color2= QColor(198, 210, 218);
    for(int i=0;i<mCTableView->mTableViewInfo.Vect_RowItem.size();i++)
    {
        if(i==index)
        {
            mCTableView->SetRowItemsBKColor(i, color);
        }
        else
        {
            mCTableView->SetRowItemsBKColor(i, color2);
        }
    }
}
//更新界面显示
void StationStorageSetDialog::UpDateView()
{
    if(nCheckRowLeft>=0&&nCheckRowLeft<mVectTableShowData.size())
    {
        //SetTextBrowser(mVectTableShowData[nCheckRowLeft].strGDTrain);
        //SetTextBrowser();
        //类别
        ui->comboBox->setCurrentText(mVectTableShowData[nCheckRowLeft].strlist[0].left(1)+"车");
        //车种
        ui->comboBox_2->setCurrentText(mVectTableShowData[nCheckRowLeft].strcz);
        //作业说明
        ui->comboBox_3->setCurrentText(mVectTableShowData[nCheckRowLeft].strzysm);
        //辆数
        ui->spinBox->setValue(mVectTableShowData[nCheckRowLeft].strlist[1].toInt());
        //去向
        ui->lineEdit->setText(mVectTableShowData[nCheckRowLeft].strlist[2]);
        //运用标记
        ui->comboBox_5->setCurrentText(mVectTableShowData[nCheckRowLeft].strlist[3]);
    }
    else
    {
        //类别
        ui->comboBox->setCurrentText("空车");
        //车种
        ui->comboBox_2->setCurrentText("棚车");
        //作业说明
        ui->comboBox_3->setCurrentText("");
        //辆数
        ui->spinBox->setValue(0);
        //去向
        ui->lineEdit->setText("");
        //运用标记
        ui->comboBox_5->setCurrentText("");
    }
    SetTextBrowser();
}
//插入
void StationStorageSetDialog::on_unlockButton_3_pressed()
{
    emit SendTextAnalysisSignal(65, QCursor::pos());
    AddGDSet();
}
//增加
void StationStorageSetDialog::on_unlockButton_6_pressed()
{
    emit SendTextAnalysisSignal(66, QCursor::pos());
    AddGDSet();
}
//修改
void StationStorageSetDialog::on_unlockButton_5_pressed()
{
    emit SendTextAnalysisSignal(67, QCursor::pos());
    if(nCheckRowLeft>=0&&nCheckRowLeft<mVectTableShowData.size())
    {
        //类别
        QString strlb=ui->comboBox->currentText();
        //车种
        QString strcz=ui->comboBox_2->currentText();
        //作业说明
        QString strzysm=ui->comboBox_3->currentText();
        //辆数
        QString strnum=ui->spinBox->text();
        //运用标记
        QString strbj=ui->comboBox_5->currentText();
        //去向
        QString strqx=ui->lineEdit->text();
        //股道占用信息
        QString strGDTrain=ui->textBrowser->toPlainText();
        //车辆数目不能为0
        if(strnum.toInt()>0)
        {
            QStringList tableshow;
            tableshow<<strlb.left(1)+strzysm+strcz<<strnum<<strqx<<strbj;
            TableShowData mTableShowData;
            mTableShowData.Init();
            mTableShowData.strlist<<strlb.left(1)+strzysm+strcz<<strnum<<strqx<<strbj;
            mTableShowData.strGDTrain=strGDTrain;
            QString pinyin = convertChineseToPinyin(strcz);
            if(strqx!="")
            {
                strqx="("+strqx+")";
            }
            mTableShowData.strGDTrain=strlb.left(1)+strzysm+pinyin+strnum+strqx;
            //SetTextBrowser(mTableShowData.strGDTrain);
            mTableShowData.strcz=strcz;
            mTableShowData.strzysm=strzysm;


            //类别
            mTableShowData.strlb=strlb;
            //车种
            mTableShowData.strcz=strcz;
            //作业说明
            mTableShowData.strzysm=strzysm;
            //辆数
            mTableShowData.strnum=strnum;
            //运用标记
            mTableShowData.strbj=strbj;
            //说明
            mTableShowData.strqx=strqx;
            mVectTableShowData.replace(nCheckRowLeft,mTableShowData);
            SetTextBrowser();
            UpDateTableView();
        }
        else
        {
            QMessageBox::information(this,tr("提示"),"辆数值无效",tr("确定"),"",0);
        }
    }
    else
    {
       QMessageBox::information(this,tr("提示"),"选中行无效",tr("确定"),"",0);
    }
}
//删除
void StationStorageSetDialog::on_unlockButton_4_pressed()
{
    emit SendTextAnalysisSignal(68, QCursor::pos());
    if(nCheckRowLeft>=0&&nCheckRowLeft<mVectTableShowData.size())
    {
        mVectTableShowData.erase(mVectTableShowData.begin()+nCheckRowLeft);
        UpDateTableView();
        SetTextBrowser();
        InitEditZoneShow();
    }
    else
    {
       QMessageBox::information(this,tr("提示"),"选中行无效",tr("确定"),"",0);
    }
}
//清除
void StationStorageSetDialog::on_pushButton_pressed()
{
    emit SendTextAnalysisSignal(69, QCursor::pos());
    bclear=true;
    InitEditZoneShow();
    ui->textBrowser->clear();
    //mVectTableShowData.clear();
    mCTableView->Clear();
}
//确认设置
void StationStorageSetDialog::on_pushButton_2_pressed()
{
    emit SendTextAnalysisSignal(70, QCursor::pos());
    if(bclear)
    {
        InitEditZoneShow();
        ui->textBrowser->clear();
        mVectTableShowData.clear();
        mCTableView->Clear();
    }
    pGDData->mVectTableShowData=mVectTableShowData;
    bclear=false;
    bok=true;
    close();
}
//关闭
void StationStorageSetDialog::on_pushButton_3_pressed()
{
    emit SendTextAnalysisSignal(54, QCursor::pos());
   close();
}
void StationStorageSetDialog::itemLBtnClicked(const QModelIndex &index)
{
    QColor color,color2;
    color = Qt::blue;
    color2= Qt::white;
    nCheckRowLeft=index.row();
    UpDateView();
    if(index.row()<mCTableView->mTableViewInfo.Vect_RowItem.size())
    {
        SetBacklackRow(mCTableView,nCheckRowLeft);
    }
}
