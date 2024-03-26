#include "dialog_zchmanage.h"
#include "ui_dialog_zchmanage.h"
#include <QMessageBox>
#pragma execution_character_set("utf-8")
Dialog_ZCHManage::Dialog_ZCHManage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_ZCHManage)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("站存车信息 - TDCS/CTC3.0 通用车站操作终端"));
    this->setFixedSize(this->width(),this->height());
    this->move(600,250);
    Init();
    InitTable();
    UpDataTable2();
}

Dialog_ZCHManage::~Dialog_ZCHManage()
{
    delete ui;
}
void Dialog_ZCHManage::Init()
{
    //修改时间
    ui->label_2->setText(QDateTime::currentDateTime().toString("MM月dd日hh时mm分"));
    //发送时间
    ui->label_4->setText("           ");
    //接收时间
    ui->label_6->setText("           ");
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
    ui->comboBox_2->addItem("长大火车");
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
void Dialog_ZCHManage::InitTable()
{
    //QStringList m_List_ZCC1;
    m_List_ZCC1 << "股道名称";
    m_List_ZCC1 << "现在存车信息";
    m_List_ZCC1 << "小计";

    m_pModel_ZCC1 = new QStandardItemModel();
    m_pModel_ZCC1->setHorizontalHeaderLabels(m_List_ZCC1);

    ui->tableView_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_2->setModel(m_pModel_ZCC1);
    ui->tableView_2->setColumnWidth(0,80);
    ui->tableView_2->setColumnWidth(1,300);
    ui->tableView_2->setColumnWidth(2,60);
    ui->tableView_2->horizontalHeader()->setHighlightSections(false);
    ui->tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_2->verticalHeader()->setVisible(false);
    connect(ui->tableView_2,&QTableView::clicked,[=](const QModelIndex &index){
         //itemLBtnClicked(index);
        nTableCheck1=index.row();
    });
    //QStringList m_List_ZCC2;
    m_List_ZCC2 << "车种";
    m_List_ZCC2 << "空车";
    m_List_ZCC2 << "重车";
    m_pModel_ZCC2 = new QStandardItemModel();
    m_pModel_ZCC2->setHorizontalHeaderLabels(m_List_ZCC2);
    ListTrain<<"棚车"<<"敞车"<<"平板车"<<"保温车"<<"集装箱车"<<"长大货车";

    ui->tableView_3->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_3->setModel(m_pModel_ZCC2);
    ui->tableView_3->setColumnWidth(0,60);
    ui->tableView_3->setColumnWidth(1,45);
    ui->tableView_3->setColumnWidth(2,45);
    ui->tableView_3->horizontalHeader()->setHighlightSections(false);
    ui->tableView_3->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_3->verticalHeader()->setVisible(false);
    //QStringList m_List_ZCC3;
    m_List_ZCC3 << "车厢类型";
    m_List_ZCC3 << "辆数";
    m_List_ZCC3 << "说明/去向";
    m_List_ZCC3 << "运用标记";
    m_pModel_ZCC3 = new QStandardItemModel();
    m_pModel_ZCC3->setHorizontalHeaderLabels(m_List_ZCC3);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setModel(m_pModel_ZCC3);
    ui->tableView->setColumnWidth(0,80);
    ui->tableView->setColumnWidth(1,50);
    ui->tableView->setColumnWidth(2,100);
    ui->tableView->setColumnWidth(3,80);
    ui->tableView->horizontalHeader()->setHighlightSections(false);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->verticalHeader()->setVisible(false);
}
void Dialog_ZCHManage::UpDataTable1(QStringList strlisr)
{
    ListGD=strlisr;
    int col=m_List_ZCC1.size();
    int row=strlisr.size();
    //QStringList tableshow;
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<col;j++)
        {
            if(j==0)
            {
                m_List_table_ZCC1<<ListGD[i];
            }
            else if(j==col-1)
            {
                 m_List_table_ZCC1<<"0";
            }
            else
            {
                m_List_table_ZCC1<<"";
            }
        }
    }
    SetTableData(m_pModel_ZCC1,m_List_table_ZCC1,col);
}
void Dialog_ZCHManage::UpDataTable2()
{
    int col=m_List_ZCC2.size();
    int row=ListTrain.size();
    QStringList tableshow;
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<col;j++)
        {
            if(j==0)
            {
                m_List_table_ZCC2<<ListTrain[i];
            }
            else
            {
                 m_List_table_ZCC2<<"0";
            }
        }
    }
    SetTableData(m_pModel_ZCC2,m_List_table_ZCC2,col);
}
void Dialog_ZCHManage::UpDataTable3()
{

}
void Dialog_ZCHManage::SetTime(int ntype,QDateTime time)
{
    if(ntype==0)
    {
        ui->label_2->setText(time.toString("MM月dd日hh时mm分"));
    }
    else if(ntype==1)
    {
        ui->label_4->setText(time.toString("MM月dd日hh时mm分"));
    }
    else if(ntype==2)
    {
        ui->label_6->setText(time.toString("MM月dd日hh时mm分"));
    }
}
void Dialog_ZCHManage::SetTableData(QStandardItemModel *m_pModel,QStringList strlist,int nColumnCount)
{
    //int nColumnCount = mTableViewInfo.Vect_HeaderData.size();
    int nlen = strlist.size();
    //this->Clear();
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

void Dialog_ZCHManage::on_unlockButton_2_clicked()
{
    close();
}
//插入
void Dialog_ZCHManage::on_unlockButton_3_clicked()
{
    //先判断股道有无选中
    if(nTableCheck1>=0)
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

        //更新表1数据
        {
            int col=m_List_ZCC1.size();
            int row=ListGD.size();
            QStringList tableshow;
            for(int i=0;i<row;i++)
            {
                if(i==nTableCheck1)
                {
                    for(int j=0;j<col;j++)
                    {
                        if(j==0)
                        {
                            //tableshow<<ListGD[i];
                            m_List_table_ZCC1[i*col+j]=ListGD[i];
                        }
                        else if(j==col-1)
                        {
                            //tableshow<<strnum;
                            m_List_table_ZCC1[i*col+j]=strnum;
                        }
                        else
                        {
                            //tableshow<<strlb.left(1)+strzysm+strcz.left(1)+"("+strqx+")";
                            m_List_table_ZCC1[i*col+j]=strlb.left(1)+strzysm+strcz.left(1)+"("+strqx+")";
                        }
                    }
                }

            }
            SetTableData(m_pModel_ZCC1,tableshow,m_List_ZCC1.size());
        }

        //更新表2数据
        {
            int col=m_List_ZCC2.size();
            int row=ListGD.size();
            QStringList tableshow;
            for(int i=0;i<row;i++)
            {
                if(ListTrain[i]==strcz)
                {
                    for(int j=0;j<col;j++)
                    {
                        if(j==0)
                        {
                            m_List_table_ZCC2[i*col+j]=ListTrain[i];
                        }
                        if(j==1)
                        {
                            if(strlb=="空车")
                            {
                                m_List_table_ZCC2[i*col+j]=strnum;
                            }
                            else
                            {
                                m_List_table_ZCC2[i*col+j]="0";
                            }
                        }
                        if(j==2)
                        {
                            if(strlb=="重车")
                            {
                                m_List_table_ZCC2[i*col+j]=strnum;
                            }
                            else
                            {
                                m_List_table_ZCC2[i*col+j]="0";
                            }
                        }
                    }
                }

            }
            SetTableData(m_pModel_ZCC2,m_List_table_ZCC2,m_List_ZCC2.size());
        }

        //更新表3数据
        {
            QStringList tableshow;
            tableshow<<strlb.left(1)+strzysm+strcz<<strnum<<strqx<<strbj;
            SetTableData(m_pModel_ZCC3,tableshow,m_List_ZCC3.size());
        }
    }
    else
    {
        QMessageBox::information(this, tr("提示"), tr("未选中股道"),tr("确定"),"",0);
    }
}
//上报调度所
void Dialog_ZCHManage::on_lockButton_2_clicked()
{

}
