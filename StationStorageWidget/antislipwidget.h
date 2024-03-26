#ifndef ANTISLIPWIDGET_H
#define ANTISLIPWIDGET_H

#include <QWidget>
#include "GDData/gddata.h"

/**防溜设置窗口
*/
namespace Ui {
class AntiSlipWidget;
}

class AntiSlipWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AntiSlipWidget(QWidget *parent = nullptr);
    ~AntiSlipWidget();
    void Init(GDData* mGDData);
    //获取设置参数
    GDData* GetSetData();
    //控件文字显示转换 某些数据需要显示为空，并不是0
    QString TextShowChange(QString str);

    void SetStationID(int nID, QString strAntislip, int bSleepers) {
        nStationID = nID;
        m_strAntislip = strAntislip;
        m_bSleepers = bSleepers;
    }

signals:
    //撤销 设置股道防溜信号 bSet:设置-true 取消-false
    void SendAntiSlipSignal(GDData* pGDData,bool bSet,int nstationid);
    void SendTextAnalysisSignal(int index, QPoint pt, QString dev = "");
private slots:
    //撤销按钮
    void on_pushButton_3_pressed();
    //确认按钮
    void on_pushButton_4_pressed();
    //取消按钮
    void on_pushButton_5_pressed();

    void on_radioButton_4_clicked(bool checked);

    void on_radioButton_6_clicked(bool checked);

    void on_radioButton_3_clicked(bool checked);

    void on_radioButton_5_clicked(bool checked);

    void on_radioButton_clicked(bool checked);

    void on_radioButton_2_clicked(bool checked);

    void on_pushButton_4_released();

    void on_radioButton_19_clicked(bool checked);

    void on_radioButton_20_clicked(bool checked);

    void on_pushButton_2_clicked();

private:
    Ui::AntiSlipWidget *ui;
    GDData* pGDData=nullptr;
    bool bset=true;
    int nStationID = 0;
    QString m_strAntislip;
    bool m_bSleepers = true;
};

#endif // ANTISLIPWIDGET_H
