#ifndef AGAINSTWRONGWIDGET_H
#define AGAINSTWRONGWIDGET_H

#include <QWidget>
#include "GlobalHeaders/GlobalStructure.h"
namespace Ui {
class AgainstWrongWidget;
}

class AgainstWrongWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AgainstWrongWidget(QWidget *parent = nullptr);
    ~AgainstWrongWidget();
    void ShowCheckMsg(CheckResult* checkResult);
    void ShowCheckMsg(QString strCheckMsg);

protected:
    void timerEvent(QTimerEvent *event) override;
private slots:
    void on_buttonForce_clicked();
    void on_buttonCancel_clicked();
signals:
    void sendAgainstWrongMsg(int stationId, int id, int type);
    void SendTextAnalysisSignal(int index, QPoint pt, QString dev = "");
private:
    Ui::AgainstWrongWidget *ui;
    CheckResult* pCheckResult = nullptr;
    QVector<CheckResult*> vecCheckResult;
    int  timerId = -1;
};

#endif // AGAINSTWRONGWIDGET_H
