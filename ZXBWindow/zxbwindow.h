#ifndef ZXBWINDOW_H
#define ZXBWINDOW_H

#include <QMainWindow>

#include "GlobalHeaders/Global.h"
#include "GlobalHeaders/GlobalFuntion.h"
#include "GlobalHeaders/GlobalStructure.h"
#include "RouteOrder/routeorderwidget.h"
#include <QVBoxLayout>
#include <QVector>
#include <QSplitter>
#include "zxbflwidget.h"
#include "zxbroutewidget.h"
#include "zxbtrackwidget.h"
#include "zxbshuntwidget.h"

namespace Ui {
class ZXBWindow;
}

class ZXBWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ZXBWindow(QWidget *parent = nullptr);
    ~ZXBWindow();
    void Init();
    void InitWindow(ZXBWindowCon* pZXBWindowCon,QVector<MyStation*>mMyStationArray);
    void CreatWindow();
private:
    virtual bool eventFilter(QObject *obj,QEvent *event) override;
    virtual bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;

public:

    ZXBWindowCon* mZXBWindowCon=nullptr;
    //股道窗口
    ZXBTrackWidget* pTrackWidget = nullptr;
    //进路窗口
    ZXBRouteWidget *pRouteWidget = nullptr;
    //防溜与站存车
    ZXBFLWidget *pFLWidget = nullptr;
    //调车窗口
    ZXBShuntWidget *pShuntWidget = nullptr;
    //主界面布局 默认股道窗口与进路窗口为垂直布局
    QVBoxLayout *pMainlayout=nullptr;

    QHBoxLayout *pRouteAndFLlayout=nullptr;

    QBoxLayout *pJLlayout=nullptr;
    QBoxLayout *pFLlayout=nullptr;
    //股道窗口
    QVector<QWidget*> pVectGD;
    //进路窗口
    QVector<QWidget*> pVectJL;
    //防溜与站存车窗口
    QVector<QWidget*> pVectFL;


//    TextAnalysis* pTextAnalysis = nullptr;
//    TextAnalysis* GetTextAnalysis() { return pTextAnalysis; }
//signals:
//    void SendTextAnalysisSignal(int index, QPoint pt, QString dev = "");
private slots:
    void on_closeButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::ZXBWindow *ui;
    QPoint startPt = { 0,0 };
    QPoint startWndPt = { 0,0 };
};

#endif // ZXBWINDOW_H
