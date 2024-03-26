#ifndef POWERSUPPLYARMDIALOG_H
#define POWERSUPPLYARMDIALOG_H

#include <QDialog>
#include <QTreeView>
#include <QBoxLayout>
#include "GlobalHeaders/Global.h"
#include "GlobalHeaders/GlobalFuntion.h"
#include "GlobalHeaders/GlobalStructure.h"
#include "MyStation/mystation.h"
/*设置供电臂状态*/
namespace Ui {
class PowerSupplyArmDialog;
}
typedef struct
{
    QString strItemCheckName="";
    QString strStationName="";
}TreeStandardItemCheck;
class PowerSupplyArmDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PowerSupplyArmDialog(QWidget *parent = nullptr);
    ~PowerSupplyArmDialog();
    void Init(QVector<MyStation*> vectMyStation);
    void InitTree(MyStation* pMyStation,int n);
    void UpdatePowerData();
    void selectAllChildren(QStandardItem *item);
    int  GetStationIDByName(QString strStationName);

protected:
    void timerEvent(QTimerEvent* enent);

signals:
    void SendPowerSupplyArmDataToSeverSignal(int nStaIndex,QString strPowerName,bool bset);
    void SendTextAnalysisSignal(int index, QPoint pt, QString dev = "");

private slots:
    void on_pushButton_pressed();

    void on_pushButton_2_pressed();

    void on_pushButton_3_pressed();

    void on_pushButton_4_pressed();
    void treeItemChanged(QStandardItem * item);

private:
    Ui::PowerSupplyArmDialog *ui;
    QStandardItemModel *TreeModel=nullptr;
    QTreeView* pQTreeView=nullptr;
    QVector<MyStation*> pVectMyStation;
    int Timer_ID_500 = 0;
    //被选中子节点序列
    QVector<TreeStandardItemCheck *>pVectTreeItemCheck;
};

#endif // POWERSUPPLYARMDIALOG_H
