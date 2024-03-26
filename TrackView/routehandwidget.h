#ifndef ROUTEHANDWIDGET_H
#define ROUTEHANDWIDGET_H

#include <QWidget>
#include <QTableWidgetItem>
#include "MyStation/mystation.h"

namespace Ui {
class RouteHandWidget;
}

class RouteHandWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RouteHandWidget(QWidget *parent = nullptr);
    ~RouteHandWidget();

    void InitTableWidget();
    void Init(MyStation* pMyStation, TrainRouteOrder* pRouteOrder, bool bAdvance, bool bFinished);

    void RouteOrderInfoUpData(bool bAdvance);
    void TableWidgetUpData();
    void RouteUpData(bool bAdvance, bool bFinished);

    void SetThroughRouteOrder(QVector<TrainRouteOrder*> m_vecThroughRouteOrder);
    bool SetAutoTrigger(TrainRouteOrder* pRouteOrder);
    bool SetAutoTrigger_LeftButton(TrainRouteOrder* pRouteOrder);
    TrainRouteOrder* getRouteOrder() { return m_pRouteOrder; }

    int mouseActionNum_banli=0;
    int mouseActionNum_quxiao=0;

private:
    virtual bool eventFilter(QObject* o, QEvent* e);
    virtual void timerEvent(QTimerEvent *event);
private slots:
    void onItemClicked(QTableWidgetItem *item);

    void on_handRouteBtn_clicked();

    void on_cancelBtn_clicked();

    void on_handThroughBtn_clicked();

    void on_closeBtn_clicked();

    void on_radioButton_clicked(bool checked);

    void on_tableWidget_clicked(const QModelIndex &index);

    void on_radioButton_4_clicked(bool checked);

    void on_radioButton_3_clicked(bool checked);

    void on_lockBtn_clicked();

signals:
    void sendRouteOrderMsg(int type, int routeId, int nData = 0, QString strData = "");
    void SendTextAnalysisSignal(int index, QPoint pt, QString dev = "");

private:
    Ui::RouteHandWidget *ui;
    MyStation* m_pMyStation = nullptr;
    TrainRouteOrder* m_pRouteOrder = nullptr;
    TrainRouteOrder* pSelectRouteOrder = nullptr;

    bool m_bPreview = true;
    bool m_bZHJL = false;
    QString m_strCurRouteDescrip;
    QString m_strShowRouteDescrip;
    int m_nTimerId_500 = -1;

    //办理通过进路
    QVector<TrainRouteOrder*> m_vecThroughRouteOrder;
};

#endif // ROUTEHANDWIDGET_H
