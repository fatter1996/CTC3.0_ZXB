#ifndef TRAINROUTEORDERWIDGET_H
#define TRAINROUTEORDERWIDGET_H

#include <QWidget>
#include "TableView/tableview.h"
#include "routeorderdetail.h"
typedef struct   //进路序列表格显示配置
{
  int nDirection = 0; //上下行
  int nJCXHDCode = 0; //接车口
  int nFCXHDCode = 0;  //发车口
}RouteOrderTableSHow;
namespace Ui {
class TrainRouteOrderWidget;
}


class TrainRouteOrderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TrainRouteOrderWidget(QWidget *parent = nullptr);
    ~TrainRouteOrderWidget();

    RouteOrderTableSHow *mRouteOrderTableSHow= nullptr;
    void InitTableViewTrain();
    void SetTableStyleSheet();
    void InitTableViewMenu(MyStation* pStation);
    //进路序列数据更新
    void RouteOrderUpData();
    void RouteOrderTableDataUpData(TrainRouteOrder* order, QStringList& strList, int& row);
    void RouteOrderTableColorUpData(TrainRouteOrder* order);
    void FontReSize(int fontSize, int height = 24, double diploid = 1.0f);
    void ClearData();

    void RouteOrderSort(QVector<TrainRouteOrder*>::iterator low, QVector<TrainRouteOrder*>::iterator high);
    bool RouteOrderCompare(TrainRouteOrder* order1, TrainRouteOrder* order2);
    TrainRouteOrder* getRouteOrderByRowIndex(int row);

    //设置进路序列上下行、接发车口表格显示
    void SetRouteOrderTableShowSet(int nType, int nData);
    //获取进路序列上下行、接发车口表格显示设置
    RouteOrderTableSHow* GetRouteOrderTableShowSet();
    //判断该进路序列是否显示
    bool GetRouteOrderTableShowState(TrainRouteOrder* pTrainRouteOrder);

public slots:
    void TableViewLeftMenuInit(const QModelIndex &index);
    void TableViewRightMenuInit(const QModelIndex &index);

signals:
    void sendRouteOrderMsg(MyStation* pMyStation, int type, int routeId, int nData = 0, QString strData = "");
signals:
    void SendTextAnalysisSignal(int index, QPoint pt, QString dev = "");
private:
    Ui::TrainRouteOrderWidget *ui;
    TableView* mCTableViewTrain = nullptr;
    RouteOrderDetail* pRouteOrderDetail = nullptr;
    MyStation* pMyStation = nullptr;
    int nFontSize = 9;

};

#endif // TRAINROUTEORDER_H
