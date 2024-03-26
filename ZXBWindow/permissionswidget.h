#ifndef PERMISSIONSWIDGET_H
#define PERMISSIONSWIDGET_H

#include <QWidget>
#include "MyStation/mystation.h"

namespace Ui {
class PermissionsWidget;
}

class PermissionsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PermissionsWidget(QWidget *parent = nullptr);
    ~PermissionsWidget();

    void Init(MyStation* pMyStation, bool bShowSearch = true, bool bShowFunBtn = false);
    //设置只读、可修改
public slots:
    void SetLableSolt(bool bPermission);
private slots:
    void on_applyForBtn_clicked();

    void on_releaseBtn_clicked();



    void on_pushButton_7_clicked(bool checked);

    void on_pushButton_8_clicked(bool checked);

    void on_pushButton_3_clicked();

signals:
    void applyForPermission(); //修改申请
    void releasePermission(); //只读申请
    void DirectionDisplay(int type); //上下行 0-全部,1-上行,2-下行
    void ArriveDisplay(int code); //接车口 0-全部
    void DepartureDisplay(int code); //发车口 0-全部
    void TurnToTrackView();
    void TurnToRouteOrderView();
    void SendTextAnalysisSignal(int index, QPoint pt, QString dev = "");
private:
    Ui::PermissionsWidget *ui;
    MyStation* pCurStation = nullptr;
};

#endif // PERMISSIONSWIDGET_H
