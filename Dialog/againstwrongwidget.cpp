#include "againstwrongwidget.h"
#include "ui_againstwrongwidget.h"
#pragma execution_character_set("utf-8")

AgainstWrongWidget::AgainstWrongWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AgainstWrongWidget)
{
    setWindowFlags(Qt::Dialog);
    ui->setupUi(this);
    ui->textEdit->setReadOnly(true);
}

AgainstWrongWidget::~AgainstWrongWidget()
{
    delete ui;
}

void AgainstWrongWidget::ShowCheckMsg(CheckResult* checkResult)
{
    pCheckResult = checkResult;
    vecCheckResult.append(pCheckResult);
    ui->textEdit->setText(pCheckResult->checkMsg);
    ui->buttonForce->setVisible(pCheckResult->bEnforced);

    QSettings* settings = new QSettings("Data/Global.ini", QSettings::IniFormat);
    // 指定为中文
    settings->setIniCodec("UTF-8");
    //读取配置-通信信息
    int time = settings->value("CONFIG/AutoCloseTime").toInt();
    if(time == 0)
        time = 5000;
    timerId = startTimer(time);
    this->show();
}

void AgainstWrongWidget::ShowCheckMsg(QString strCheckMsg)
{
    ui->textEdit->setText(strCheckMsg);
    ui->buttonForce->setVisible(false);
    this->show();
}

void AgainstWrongWidget::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == timerId)
    {
        vecCheckResult.clear();
        this->close();
    }
}

void AgainstWrongWidget::on_buttonForce_clicked()
{
    emit SendTextAnalysisSignal(152, QCursor::pos());
    ui->textEdit->clear();
    for (int i = 0; i < vecCheckResult.size(); i++)
    {
        if(vecCheckResult.at(i)->stationId == pCheckResult->stationId)
            emit sendAgainstWrongMsg(vecCheckResult.at(i)->stationId, vecCheckResult.at(i)->id, 0x01);
    }
    vecCheckResult.clear();
    this->close();
}

void AgainstWrongWidget::on_buttonCancel_clicked()
{
    emit SendTextAnalysisSignal(8, QCursor::pos());
    ui->textEdit->clear();
    for (int i = 0; i < vecCheckResult.size(); i++)
    {
        if(vecCheckResult.at(i)->stationId == pCheckResult->stationId)
            emit sendAgainstWrongMsg(vecCheckResult.at(i)->stationId, vecCheckResult.at(i)->id, 0x02);
    }
    vecCheckResult.clear();
    this->close();
}
