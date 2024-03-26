#ifndef WORKTASKWIDGET_H
#define WORKTASKWIDGET_H

#include <QWidget>
#include "MyStation/mystation.h"
//添加定时器
#include <QTimerEvent>

namespace Ui {
class WorkTaskWidget;
}

class WorkTaskWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WorkTaskWidget(QWidget *parent = nullptr);
    ~WorkTaskWidget();

    int Timer_ID_500;
    int timecount=0;
    bool timer_on=false;

    //wndType : 1-预告,2-报点,3-作业
    void Init(TrafficLog* pTrafficLog, int wndType, int workType);
    void InitJFCPreview(TrafficLog* pTrafficLog, int workType);
    void InitReporting(TrafficLog* pTrafficLog, int workType);
    void InitWorkTask(TrafficLog* pTrafficLog, int workType);


signals:
    void sendTrafficLogReportingMsg(int type, QDateTime time); //发送行车日志报点命令
    void sendTrafficLogWorkMsg(int type, int flag, QDateTime time); //发送行车日志更新命令
    void SendTextAnalysisSignal(int index, QPoint pt, QString dev = "");

private slots:
    void on_closeBtn_clicked();

private:
    Ui::WorkTaskWidget *ui;
    virtual void timerEvent(QTimerEvent *event) override;//定时器
};

#endif // WORKTASKWIDGET_H
