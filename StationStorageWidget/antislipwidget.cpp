#include "antislipwidget.h"
#include "ui_antislipwidget.h"

AntiSlipWidget::AntiSlipWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AntiSlipWidget)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window|Qt::WindowCloseButtonHint);
    this->setFixedSize(this->width(),this->height());
    ui->tabWidget->setCurrentIndex(0);
    //限制只能输入数字
    // 设置输入范围为0到255
    QIntValidator *validator = new QIntValidator(0, 255, this);
    ui->lineEdit->setValidator(validator);
    ui->lineEdit_2->setValidator(validator);

//    ui->lineEdit->setValidator(new QRegExpValidator(QRegExp("0|[1-9][0-9]{0,}")));
//    ui->lineEdit_2->setValidator(new QRegExpValidator(QRegExp("[0-9]")));
    //TODO:：屏蔽tabwidget选项第二组，第三组铁鞋设置界面切换
    connect(ui->tabWidget,&QTabWidget::currentChanged,[this](int index){
        if(index!=0)
        {
            ui->tabWidget->setCurrentIndex(0);
        }
    });
}

AntiSlipWidget::~AntiSlipWidget()
{
    delete ui;
}
void AntiSlipWidget::Init(GDData* mGDData)
{
    pGDData=mGDData;
    ui->radioButton->setText(m_strAntislip.left(m_strAntislip.indexOf('/')));
    ui->radioButton_2->setText(m_strAntislip.mid(m_strAntislip.indexOf('/') + 1));
    if(!m_bSleepers)
    {
        ui->radioButton_19->hide();
        ui->radioButton_20->hide();
        ui->lineEdit_23->hide();
        ui->lineEdit_24->hide();
        this->setFixedWidth(684);
        ui->tabWidget->setFixedWidth(668);
    }

    ui->tabWidget->setCurrentIndex(0);
    //股道
    ui->label_2->setText(" "+pGDData->pGD->getName());
    //东部铁鞋
    ui->radioButton->setChecked(pGDData->pGD->bIronShoesSetS);
    //东部铁鞋号
    if(pGDData->pGD->bIronShoesSetS)//铁鞋设置选中
    {
        ui->lineEdit->setText(TextShowChange(QString::number(pGDData->pGD->m_nLTxNum)));
        ui->lineEdit->setEnabled(true);
    }
    else//未选中
    {
        ui->lineEdit->setText(TextShowChange(""));
        ui->lineEdit->setEnabled(false);
    }

    //东部人力制动机 人力制动机紧固器
    if(pGDData->pGD->nManualBrakeS==0)//三者皆无
    {
        ui->radioButton_4->setChecked(false);
        ui->radioButton_6->setChecked(false);
        ui->radioButton_19->setChecked(false);
        ui->lineEdit_22->clear();
        ui->lineEdit_4->clear();
        ui->lineEdit_46->clear();
        ui->lineEdit_23->clear();
        ui->lineEdit_22->setEnabled(false);
        ui->lineEdit_4->setEnabled(false);
        ui->lineEdit_46->setEnabled(false);
        ui->lineEdit_23->setEnabled(false);
    }
    else if(pGDData->pGD->nManualBrakeS==1)//人力制动机
    {
        ui->radioButton_4->setChecked(true);
        ui->radioButton_6->setChecked(false);
        ui->radioButton_19->setChecked(false);
        ui->lineEdit_22->setEnabled(true);
        ui->lineEdit_4->clear();
        ui->lineEdit_46->clear();
        ui->lineEdit_23->clear();
        ui->lineEdit_4->setEnabled(false);
        ui->lineEdit_46->setEnabled(false);
        ui->lineEdit_23->setEnabled(false);
    }
    else if(pGDData->pGD->nManualBrakeS==2)//人力制动机紧固器
    {
        ui->radioButton_4->setChecked(false);
        ui->radioButton_6->setChecked(true);
        ui->radioButton_19->setChecked(false);
        ui->lineEdit_22->clear();
        ui->lineEdit_23->clear();
        ui->lineEdit_22->setEnabled(false);
        ui->lineEdit_4->setEnabled(true);
        ui->lineEdit_46->setEnabled(true);
        ui->lineEdit_23->setEnabled(false);
        //东部上行位置
        ui->lineEdit_4->setText(TextShowChange(QString::number(pGDData->pGD->m_nLJgqNum)));
    }
    else if(pGDData->pGD->nManualBrakeS==3)//防溜枕木
    {
        ui->radioButton_4->setChecked(false);
        ui->radioButton_6->setChecked(false);
        ui->radioButton_19->setChecked(true);
        ui->lineEdit_22->clear();
        ui->lineEdit_4->clear();
        ui->lineEdit_46->clear();
        ui->lineEdit_22->setEnabled(false);
        ui->lineEdit_4->setEnabled(false);
        ui->lineEdit_46->setEnabled(false);
        ui->lineEdit_23->setEnabled(true);
        //东部上行位置
        ui->lineEdit_4->setText(TextShowChange(QString::number(pGDData->pGD->m_nLJgqNum)));
    }

//    //东部上行位置
//    ui->lineEdit_4->setText(TextShowChange(QString::number(pGDData->pGD->m_nLJgqNum)));
    //东部警内位置
    ui->lineEdit_6->setText(TextShowChange(QString::number(pGDData->pGD->m_nLJnMeters)));

    //西部铁鞋
    ui->radioButton_2->setChecked(pGDData->pGD->bIronShoesSetX);
    //西部铁鞋号
    //ui->lineEdit_2->setText(TextShowChange(QString::number(pGDData->pGD->m_nRJgqNum)));
    if(pGDData->pGD->bIronShoesSetX)//铁鞋设置选中
    {
        ui->lineEdit_2->setText(TextShowChange(QString::number(pGDData->pGD->m_nRTxNum)));
        ui->lineEdit_2->setEnabled(true);
    }
    else//未选中
    {
        ui->lineEdit_2->setText(TextShowChange(""));
        ui->lineEdit_2->setEnabled(false);
    }
    //西部人力制动机 人力制动机紧固器
    if(pGDData->pGD->nManualBrakeX==0)//两者皆无
    {
        ui->radioButton_3->setChecked(false);
        ui->radioButton_5->setChecked(false);
        ui->radioButton_20->setChecked(false);

        ui->lineEdit_45->clear();
        ui->lineEdit_5->clear();
        ui->lineEdit_47->clear();
        ui->lineEdit_24->clear();

        ui->lineEdit_45->setEnabled(false);
        ui->lineEdit_5->setEnabled(false);
        ui->lineEdit_47->setEnabled(false);
        ui->lineEdit_24->setEnabled(false);
    }
    else if(pGDData->pGD->nManualBrakeX==1)//人力制动机
    {
        ui->radioButton_3->setChecked(true);
        ui->radioButton_5->setChecked(false);
        ui->radioButton_20->setChecked(false);
        ui->lineEdit_5->clear();
        ui->lineEdit_47->clear();
        ui->lineEdit_24->clear();
        ui->lineEdit_5->setEnabled(false);
        ui->lineEdit_47->setEnabled(false);
        ui->lineEdit_24->setEnabled(false);
    }
    else if(pGDData->pGD->nManualBrakeX==2)//人力制动机紧固器
    {
         ui->radioButton_3->setChecked(false);
         ui->radioButton_5->setChecked(true);
         ui->radioButton_20->setChecked(false);
         ui->lineEdit_24->clear();
         ui->lineEdit_5->setEnabled(true);
         ui->lineEdit_47->setEnabled(true);
         ui->lineEdit_24->setEnabled(false);
         ui->lineEdit_5->setText(TextShowChange(QString::number(pGDData->pGD->m_nRJgqNum)));
    }
    else if(pGDData->pGD->nManualBrakeX==3)//防溜枕木
    {
         ui->radioButton_3->setChecked(false);
         ui->radioButton_5->setChecked(false);
         ui->radioButton_20->setChecked(true);
         ui->lineEdit_5->setEnabled(false);
         ui->lineEdit_47->setEnabled(false);
         ui->lineEdit_24->setEnabled(true);
    }
    //西部上行位置
    //ui->lineEdit_5->setText(TextShowChange(QString::number(pGDData->pGD->m_nRJgqNum)));
    //西部警内位置
    ui->lineEdit_7->setText(TextShowChange(QString::number(pGDData->pGD->m_nRJnMeters)));
    //存车信息
    ui->lineEdit_3->setText(pGDData->pGD->m_sTrainInfoFL);
}
GDData* AntiSlipWidget::GetSetData()
{
    if(ui->tabWidget->currentIndex()==0)
    {
        //东部铁鞋
            pGDData->pGD->bIronShoesSetS=ui->radioButton->isChecked();
            //东部铁鞋号
            if(pGDData->pGD->bIronShoesSetS)
            {
                pGDData->pGD->m_nLTxNum=ui->lineEdit->text().toInt();
                if(pGDData->pGD->m_nLTxNum>255)
                {
                    pGDData->pGD->m_nLTxNum=0xff;
                }
            }
            else
            {
                pGDData->pGD->m_nLTxNum=0;
            }


            //东部人力制动机 人力制动机紧固器
            if(ui->radioButton_4->isChecked())
            {
                pGDData->pGD->nManualBrakeS=1;
            }
            else if(ui->radioButton_6->isChecked())
            {
                pGDData->pGD->nManualBrakeS=2;
            }
            else if(ui->radioButton_19->isChecked())
            {
                pGDData->pGD->nManualBrakeS=3;
            }
            else
            {
                pGDData->pGD->nManualBrakeS=0;
            }

            //东部上行位置
            pGDData->pGD->m_nLJgqNum= ui->lineEdit_4->text().toInt();
            //东部警内位置
            pGDData->pGD->m_nLJnMeters=ui->lineEdit_6->text().toInt();
            //西部铁鞋
             pGDData->pGD->bIronShoesSetX=ui->radioButton_2->isChecked();
            //西部铁鞋号
             if(pGDData->pGD->bIronShoesSetX)
             {
                 pGDData->pGD->m_nRTxNum=ui->lineEdit_2->text().toInt();
                 if(pGDData->pGD->m_nRTxNum>255)
                 {
                     pGDData->pGD->m_nRTxNum=0xff;
                 }
             }
             else
             {
              pGDData->pGD->m_nRTxNum=0;
             }

            //西部人力制动机 人力制动机紧固器
            if(ui->radioButton_3->isChecked())
            {
                pGDData->pGD->nManualBrakeX=1;
            }
            else if(ui->radioButton_5->isChecked())
            {
                pGDData->pGD->nManualBrakeX=2;
            }
            else if(ui->radioButton_20->isChecked())
            {
                pGDData->pGD->nManualBrakeX=3;
            }
            else
            {
                pGDData->pGD->nManualBrakeX=0;
            }
            //西部下行位置
             ui->lineEdit->setText(QString::number(pGDData->pGD->m_nRJgqNum));
             pGDData->pGD->m_nRJgqNum=ui->lineEdit_5->text().toInt();
            //西部警内位置
             ui->lineEdit->setText(QString::number(pGDData->pGD->m_nRJnMeters));
             pGDData->pGD->m_nRJnMeters=ui->lineEdit_7->text().toInt();
             //存车信息
             //pGDData->pGD->m_nTrainNums=ui->lineEdit_3->text().toInt();
             //存车信息 防溜
             if((!pGDData->pGD->bIronShoesSetS&&pGDData->pGD->nManualBrakeS==0)&&(!pGDData->pGD->bIronShoesSetX&&pGDData->pGD->nManualBrakeX==0))
             {
                 ui->lineEdit_3->clear();
             }
             pGDData->pGD->m_sTrainInfoFL=ui->lineEdit_3->text();
    }
    return pGDData;
}

//控件文字显示转换 某些数据需要显示为空，并不是0
QString AntiSlipWidget::TextShowChange(QString str)
{
    return str == "0" ? "" : str;
}
//撤除
void AntiSlipWidget::on_pushButton_3_pressed()
{
    emit SendTextAnalysisSignal(71, QCursor::pos());
    pGDData->pGD->IntiSlip();
    pGDData->Init();
    Init(pGDData);
    bset=false;
}
//确定
void AntiSlipWidget::on_pushButton_4_pressed()
{
//    GDData *pGDData1=GetSetData();
//    emit SendAntiSlipSignal(pGDData1,bset,GetStationID());
//    bset=true;
//    close();
}
//取消
void AntiSlipWidget::on_pushButton_5_pressed()
{
    emit SendTextAnalysisSignal(72, QCursor::pos());
    close();
}

void AntiSlipWidget::on_radioButton_4_clicked(bool checked)
{
    ui->radioButton_4->setChecked(checked);
    if(ui->radioButton_6->isChecked()&&checked)
    {
        ui->radioButton_6->setChecked(false);
    }
    if(ui->radioButton_19->isChecked()&&checked)
    {
        ui->radioButton_19->setChecked(false);
    }
}

void AntiSlipWidget::on_radioButton_6_clicked(bool checked)
{
    //ui->radioButton_4->setChecked(!checked);
    ui->radioButton_6->setChecked(checked);
    if(ui->radioButton_4->isChecked()&&checked)
    {
        ui->radioButton_4->setChecked(false);
    }
    if(ui->radioButton_19->isChecked()&&checked)
    {
        ui->radioButton_19->setChecked(false);
    }
}

void AntiSlipWidget::on_radioButton_3_clicked(bool checked)
{
    ui->radioButton_3->setChecked(checked);
    if(ui->radioButton_5->isChecked()&&checked)
    {
        ui->radioButton_5->setChecked(false);
    }
    if(ui->radioButton_20->isChecked()&&checked)
    {
        ui->radioButton_20->setChecked(false);
    }
}

void AntiSlipWidget::on_radioButton_5_clicked(bool checked)
{
    ui->radioButton_5->setChecked(checked);
    if(ui->radioButton_3->isChecked()&&checked)
    {
        ui->radioButton_3->setChecked(false);
    }
    if(ui->radioButton_20->isChecked()&&checked)
    {
        ui->radioButton_20->setChecked(false);
    }
}
//东部铁鞋单选框
void AntiSlipWidget::on_radioButton_clicked(bool checked)
{
    if(checked)
    {
        ui->lineEdit->setText(TextShowChange(QString::number(pGDData->pGD->m_nLTxNum)));
        ui->lineEdit->setEnabled(true);
    }
    else
    {
        ui->lineEdit->setText("");
        ui->lineEdit->setEnabled(false);
    }
}
//西部铁鞋单选框
void AntiSlipWidget::on_radioButton_2_clicked(bool checked)
{
    if(checked)
    {
        ui->lineEdit_2->setText(TextShowChange(QString::number(pGDData->pGD->m_nRTxNum)));
        ui->lineEdit_2->setEnabled(true);
    }
    else
    {
        ui->lineEdit_2->setText("");
        ui->lineEdit_2->setEnabled(false);
    }
}

void AntiSlipWidget::on_pushButton_4_released()
{
    emit SendTextAnalysisSignal(73, QCursor::pos());
    GDData *pGDData1=GetSetData();
    emit SendAntiSlipSignal(pGDData1,bset,nStationID);
    bset=true;
    close();
}

void AntiSlipWidget::on_radioButton_19_clicked(bool checked)
{
    ui->radioButton_19->setChecked(checked);
    if(ui->radioButton_4->isChecked()&&checked)
    {
        ui->radioButton_4->setChecked(false);
    }
    if(ui->radioButton_6->isChecked()&&checked)
    {
        ui->radioButton_6->setChecked(false);
    }
}

void AntiSlipWidget::on_radioButton_20_clicked(bool checked)
{
    ui->radioButton_20->setChecked(checked);
    if(ui->radioButton_3->isChecked()&&checked)
    {
        ui->radioButton_3->setChecked(false);
    }
    if(ui->radioButton_5->isChecked()&&checked)
    {
        ui->radioButton_5->setChecked(false);
    }
}

void AntiSlipWidget::on_pushButton_2_clicked()
{
    emit SendTextAnalysisSignal(165, QCursor::pos());
}
