#ifndef ZXBTRACKWIDGET_H
#define ZXBTRACKWIDGET_H

#include <QWidget>
#include "TrackView/trackviewwidget.h"
#include <QBoxLayout>

namespace Ui {
class ZXBTrackWidget;
}

class ZXBTrackWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ZXBTrackWidget(QWidget *parent = nullptr);
    ~ZXBTrackWidget();

    void SetWindowCon(ZXBWindowCon* mZXBWindowCon, QVector<MyStation*>mMyStationArray);
    TrackViewWidget* GetTrackViewWidgetByStationID(int nID);
    void UpdataMinimumWidth();
protected:
    virtual void paintEvent(QPaintEvent *event) override;

signals:
    void SendAntiSlipSignal(GDData* pGDData,bool bSet, int nstationid);
    //void UpDataStationStorageWidgetGDSignal();

private:
    Ui::ZXBTrackWidget *ui;
    QVector<TrackViewWidget*> m_vecTableView;
    QVBoxLayout *pGDlayout = nullptr;
    QTabWidget* tabWidget = nullptr;
};

#endif // ZXBTRACKWIDGET_H
