#ifndef ZXBSHUNTWIDGET_H
#define ZXBSHUNTWIDGET_H

#include <QWidget>
#include "GlobalHeaders/Global.h"
#include "GlobalHeaders/GlobalFuntion.h"
#include "GlobalHeaders/GlobalStructure.h"
#include "Shunting/shuntroutewidget.h"
#include "MyStation/mystation.h"
#include <QBoxLayout>

namespace Ui {
class ZXBShuntWidget;
}

class ZXBShuntWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ZXBShuntWidget(QWidget *parent = nullptr);
    ~ZXBShuntWidget();
    void SetWindowCon(ZXBWindowCon* mZXBWindowCon,QVector<MyStation*>mMyStationArray);

    //根据车站获取对应窗口指针
    ShuntRouteWidget* GetShuntRouteWidgetByStationID(int nID);

private:
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    Ui::ZXBShuntWidget *ui;
    ZXBWindowCon* pZXBWindowCon=nullptr;

    QLayout* playout=nullptr;
    QTabWidget* tabWidget=nullptr;
    QVector<ShuntRouteWidget*> pVectShuntRouteWidget;
};

#endif // ZXBSHUNTWIDGET_H
