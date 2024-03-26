#include "worktaskwidget.h"
#include "ui_worktaskwidget.h"
#include <windows.h>

#pragma execution_character_set("utf-8")

WorkTaskWidget::WorkTaskWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WorkTaskWidget)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("作业窗口 - TDCS/CTC3.0"));
    this->setWindowFlags(Qt::Dialog);
    Timer_ID_500 = startTimer(500);
}

WorkTaskWidget::~WorkTaskWidget()
{
    delete ui;
}

void WorkTaskWidget::Init(TrafficLog* pTrafficLog, int wndType, int workType)
{
    QString strWorkTasks = QString("%1:: 车次:<%2> 股道:<%3>\n方向:<%4->%5>")
            .arg(pTrafficLog->m_strStaName).arg(pTrafficLog->m_strTrainNum).arg(pTrafficLog->m_strTrainTrack)
            .arg(pTrafficLog->m_strFromAdjtStation).arg(pTrafficLog->m_strToAdjtStation);
    if(wndType == 1) //预告
    {
        InitJFCPreview(pTrafficLog, workType);
    }
    else if(wndType == 2) //报点
    {
        InitReporting(pTrafficLog, workType);
    }
    else if(wndType == 3) //作业
    {
        InitWorkTask(pTrafficLog, workType);
    }

    ui->workTaskLabel->setText(strWorkTasks);
    ui->operateTimeEdit->setDateTime(QDateTime::currentDateTime());
}

void WorkTaskWidget::InitJFCPreview(TrafficLog* pTrafficLog, int workType)
{
    QPushButton* completeBtn = new QPushButton;
    completeBtn->setFixedHeight(24);

    QPushButton* cancelBtn = new QPushButton;
    cancelBtn->setFixedHeight(24);
    int workTypeCode = 0;
    int MouseActionCode = 0;
    if(workType == 1) //接车预告
    {
        ui->operateMsgLabel->setText("下达接车预告命令");
        completeBtn->setText("完成接预");
        cancelBtn->setText("取消:接预");

        if(!pTrafficLog->m_timAgrFromAdjtStaDepaTrain.isValid()) //完成接预
        {
            workTypeCode = 0x02;
            cancelBtn->setEnabled(false);
            MouseActionCode=78;
        }
        else //取消:接预
        {
            workTypeCode = 0x0b;
            completeBtn->setEnabled(false);
            MouseActionCode=79;
        }
    }
    else if(workType == 2) //发车预告
    {
        ui->operateMsgLabel->setText("下达发车预告命令");
        completeBtn->setText("完成发预");
        cancelBtn->setText("取消:发预");

        if(!pTrafficLog->m_timToAdjtStaAgrDepaTrain.isValid()) //完成发预
        {
            workTypeCode = 0x01;
            cancelBtn->setEnabled(false);
            MouseActionCode=80;
        }
        else //取消:发预
        {
            workTypeCode = 0x0a;
            completeBtn->setEnabled(false);
            MouseActionCode=81;
        }
    }

    ui->btnLayout->addWidget(completeBtn);
    ui->btnLayout->addWidget(cancelBtn);
    connect(completeBtn, &QPushButton::clicked, [=](){
        emit SendTextAnalysisSignal(MouseActionCode, QCursor::pos());
    });
    connect(cancelBtn, &QPushButton::clicked, [=](){
        emit SendTextAnalysisSignal(MouseActionCode, QCursor::pos());
    });

    connect(completeBtn, &QPushButton::clicked, [=](){
        emit sendTrafficLogReportingMsg(workTypeCode, ui->operateTimeEdit->dateTime());
        //Sleep(1000);
        //this->close();
        timer_on=true;
    });
    connect(cancelBtn, &QPushButton::clicked, [=](){
        emit sendTrafficLogReportingMsg(workTypeCode, ui->operateTimeEdit->dateTime());
        //Sleep(1000);
        //this->close();
        timer_on=true;
    });
}

void WorkTaskWidget::InitReporting(TrafficLog* pTrafficLog, int workType)
{
    QPushButton* completeBtn = new QPushButton;
    completeBtn->setFixedHeight(24);

    QPushButton* cancelBtn = new QPushButton;
    cancelBtn->setFixedHeight(24);
    int workTypeCode = 0;
    int MouseActionCode = 0;
    if(workType == 1) //到点
    {
        //ui->operateMsgLabel->setText("可选人工报告通过时间或到达时间");
        completeBtn->setText("完成到点");
        cancelBtn->setText("取消:到点");

        if(!pTrafficLog->m_timRealReachStation.isValid()) //完成到点
        {
            workTypeCode = 0x03;
            MouseActionCode=74;
            cancelBtn->setEnabled(false);
        }
        else
        {
            workTypeCode = 0x07; //取消:到点
            MouseActionCode=75;
            completeBtn->setEnabled(false);
        }
    }
    else if(workType == 2) //发点
    {
        //ui->operateMsgLabel->setText("可选人工报告通过时间或到达时间");
        completeBtn->setText("完成发点");
        cancelBtn->setText("取消:发点");

        if(!pTrafficLog->m_timRealDepaTrain.isValid()) //完成发点
        {
            workTypeCode = 0x04;
            MouseActionCode=76;
            cancelBtn->setEnabled(false);
        }
        else
        {
            workTypeCode = 0x09; //取消:发预
            MouseActionCode=77;
            completeBtn->setEnabled(false);
        }
    }

    ui->btnLayout->addWidget(completeBtn);
    ui->btnLayout->addWidget(cancelBtn);



    connect(completeBtn, &QPushButton::clicked, [=](){
        emit SendTextAnalysisSignal(MouseActionCode, QCursor::pos());
    });
    connect(completeBtn, &QPushButton::clicked, [=](){
        emit sendTrafficLogReportingMsg(workTypeCode, ui->operateTimeEdit->dateTime());
        //Sleep(1000);
        //this->close();
        timer_on=true;
    });

    connect(cancelBtn, &QPushButton::clicked, [=](){
        emit SendTextAnalysisSignal(MouseActionCode, QCursor::pos());
    });
    connect(cancelBtn, &QPushButton::clicked, [=](){
        emit sendTrafficLogReportingMsg(workTypeCode, ui->operateTimeEdit->dateTime());
        //Sleep(1000);
        //this->close();
        timer_on=true;
    });
}

void WorkTaskWidget::InitWorkTask(TrafficLog* pTrafficLog, int workType)
{
    ui->operateMsgLabel->setText("下达安排作业命令");

    int workState = 0;
    QString strWorkType;
    int MouseActionCode_anpai = 0;
    int MouseActionCode_wanc = 0;
    int MouseActionCode_quxiao = 0;
    int MouseActionCode_quxiaoanpai = 0;
    switch (workType) {
    case DEF_FLOW_JL: workState = pTrafficLog->m_btJLStatus; strWorkType = "交令"; break;
    case DEF_FLOW_LJ: workState = pTrafficLog->m_btLJStatus; strWorkType = "列检"; break;
    case DEF_FLOW_SS: workState = pTrafficLog->m_btSSStatus; strWorkType = "上水"; break;
    case DEF_FLOW_XW: workState = pTrafficLog->m_btXWStatus; strWorkType = "吸污"; break;
    case DEF_FLOW_JP: workState = pTrafficLog->m_btJPStatus; strWorkType = "交票"; break;
    case DEF_FLOW_CJ: workState = pTrafficLog->m_btCJStatus; strWorkType = "乘降"; break;
    case DEF_FLOW_ZG: workState = pTrafficLog->m_btZGStatus; strWorkType = "摘挂"; break;
    case DEF_FLOW_LW: workState = pTrafficLog->m_btLWStatus; strWorkType = "列尾"; break;
    case DEF_FLOW_HJ: workState = pTrafficLog->m_btHJStatus; strWorkType = "货检"; break;
    case DEF_FLOW_HC: workState = pTrafficLog->m_btHCStatus; strWorkType = "换乘"; break;
    case DEF_FLOW_ZX: workState = pTrafficLog->m_btZXStatus; strWorkType = "装卸"; break;
    case DEF_FLOW_JC: workState = pTrafficLog->m_btJCStatus; strWorkType = "机车"; break;
    case DEF_FLOW_DK: workState = pTrafficLog->m_btDKStatus; strWorkType = "道口"; break;
    case DEF_FLOW_CH: workState = pTrafficLog->m_btCHStatus; strWorkType = "车号"; break;
    case DEF_FLOW_ZK: workState = pTrafficLog->m_btZKStatus; strWorkType = "综控"; break;
    case DEF_FLOW_ZW: workState = pTrafficLog->m_btZWStatus; strWorkType = "站务"; break;
    }
    switch (workType) {
    case DEF_FLOW_JL: MouseActionCode_anpai = 82; MouseActionCode_wanc = 99;  MouseActionCode_quxiao = 115; MouseActionCode_quxiaoanpai = 131; break;
    case DEF_FLOW_LJ: MouseActionCode_anpai = 83; MouseActionCode_wanc = 100; MouseActionCode_quxiao = 116; MouseActionCode_quxiaoanpai = 132; break;
    case DEF_FLOW_SS: MouseActionCode_anpai = 84; MouseActionCode_wanc = 101; MouseActionCode_quxiao = 117; MouseActionCode_quxiaoanpai = 133; break;
    case DEF_FLOW_XW: MouseActionCode_anpai = 85; MouseActionCode_wanc = 102; MouseActionCode_quxiao = 118; MouseActionCode_quxiaoanpai = 134; break;
    case DEF_FLOW_JP: MouseActionCode_anpai = 86; MouseActionCode_wanc = 103; MouseActionCode_quxiao = 119; MouseActionCode_quxiaoanpai = 135; break;
    case DEF_FLOW_CJ: MouseActionCode_anpai = 87; MouseActionCode_wanc = 104; MouseActionCode_quxiao = 120; MouseActionCode_quxiaoanpai = 136; break;
    case DEF_FLOW_ZG: MouseActionCode_anpai = 88; MouseActionCode_wanc = 105; MouseActionCode_quxiao = 121; MouseActionCode_quxiaoanpai = 137; break;
    case DEF_FLOW_LW: MouseActionCode_anpai = 89; MouseActionCode_wanc = 106; MouseActionCode_quxiao = 122; MouseActionCode_quxiaoanpai = 138; break;
    case DEF_FLOW_HJ: MouseActionCode_anpai = 90; MouseActionCode_wanc = 107; MouseActionCode_quxiao = 123; MouseActionCode_quxiaoanpai = 139; break;
    case DEF_FLOW_HC: MouseActionCode_anpai = 91; MouseActionCode_wanc = 108; MouseActionCode_quxiao = 124; MouseActionCode_quxiaoanpai = 140; break;
    case DEF_FLOW_ZX: MouseActionCode_anpai = 92; MouseActionCode_wanc = 109; MouseActionCode_quxiao = 125; MouseActionCode_quxiaoanpai = 141; break;
    case DEF_FLOW_JC: MouseActionCode_anpai = 93; MouseActionCode_wanc = 110; MouseActionCode_quxiao = 126; MouseActionCode_quxiaoanpai = 142; break;
    case DEF_FLOW_DK: MouseActionCode_anpai = 94; MouseActionCode_wanc = 111; MouseActionCode_quxiao = 127; MouseActionCode_quxiaoanpai = 143; break;
    case DEF_FLOW_CH: MouseActionCode_anpai = 95; MouseActionCode_wanc = 112; MouseActionCode_quxiao = 128; MouseActionCode_quxiaoanpai = 144; break;
    case DEF_FLOW_ZK: MouseActionCode_anpai = 96; MouseActionCode_wanc = 113; MouseActionCode_quxiao = 129; MouseActionCode_quxiaoanpai = 145; break;
    case DEF_FLOW_ZW: MouseActionCode_anpai = 97; MouseActionCode_wanc = 114; MouseActionCode_quxiao = 130; MouseActionCode_quxiaoanpai = 146; break;
    }
    //0未安排(取消安排)，1安排，2得知(取消完成)，3完成
    QPushButton* cancelBen = new QPushButton;
    cancelBen->setFixedHeight(24);
    cancelBen->setText(QString("取消:*安排 %1").arg(strWorkType));

    if(workState == 0)
    {
        QPushButton* completeBtn = new QPushButton;
        completeBtn->setFixedHeight(24);
        completeBtn->setText(QString("*安排 %1").arg(strWorkType));

        ui->btnLayout->addWidget(completeBtn);
        connect(completeBtn, &QPushButton::clicked, [=](){
            emit SendTextAnalysisSignal(MouseActionCode_anpai, QCursor::pos());
        });
        connect(completeBtn, &QPushButton::clicked, [=](){
            emit sendTrafficLogWorkMsg(workType, 0xaa, ui->operateTimeEdit->dateTime());
            //Sleep(1000);
            //this->close();
            timer_on=true;
        });
    }
    else
    {
        QPushButton* informedBen = new QPushButton;
        informedBen->setFixedHeight(24);
        informedBen->setText("得知");

        QPushButton* completeWorkBtn = new QPushButton;
        completeWorkBtn->setFixedHeight(24);
        completeWorkBtn->setText(QString("完成%1").arg(strWorkType));

        QPushButton* cancelWorkBtn = new QPushButton;
        cancelWorkBtn->setFixedHeight(24);
        cancelWorkBtn->setText(QString("取消:%1").arg(strWorkType));

        ui->btnLayout->addWidget(informedBen);
        ui->btnLayout->addWidget(completeWorkBtn);
        ui->btnLayout->addWidget(cancelWorkBtn);

        connect(informedBen, &QPushButton::clicked, [=](){
            emit SendTextAnalysisSignal(98, QCursor::pos());
        });
        connect(informedBen, &QPushButton::clicked, [=](){
            emit sendTrafficLogWorkMsg(workType, 0xbb, ui->operateTimeEdit->dateTime());
            //Sleep(1000);
            //this->close();
            timer_on=true;
        });

        connect(completeWorkBtn, &QPushButton::clicked, [=](){
            emit SendTextAnalysisSignal(MouseActionCode_wanc, QCursor::pos());
        });
        connect(completeWorkBtn, &QPushButton::clicked, [=](){
            emit sendTrafficLogWorkMsg(workType, 0xcc, ui->operateTimeEdit->dateTime());
            //Sleep(1000);
            //this->close();
            timer_on=true;
        });

        connect(cancelWorkBtn, &QPushButton::clicked, [=](){
            emit SendTextAnalysisSignal(MouseActionCode_quxiao, QCursor::pos());
        });
        connect(cancelWorkBtn, &QPushButton::clicked, [=](){
            emit sendTrafficLogWorkMsg(workType, 0xdd, ui->operateTimeEdit->dateTime());
            //Sleep(1000);
            //this->close();
            timer_on=true;
        });
    }
    ui->btnLayout->addWidget(cancelBen);
    connect(cancelBen, &QPushButton::clicked, [=](){
        emit SendTextAnalysisSignal(MouseActionCode_quxiaoanpai, QCursor::pos());
    });
    connect(cancelBen, &QPushButton::clicked, [=](){
        emit sendTrafficLogWorkMsg(workType, 0xee, ui->operateTimeEdit->dateTime());
        //Sleep(1000);
        //this->close();
        timer_on=true;
    });
}

void WorkTaskWidget::on_closeBtn_clicked()
{
    emit SendTextAnalysisSignal(54, QCursor::pos());
    this->close();
}

void WorkTaskWidget::timerEvent(QTimerEvent *event)//定时器
{
    if(event->timerId()==Timer_ID_500)
    {
        if(timer_on)
        {
            timecount++;
        }

        if(timecount>=2)
        {
            this->close();
        }
    }
}
