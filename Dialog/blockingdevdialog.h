#ifndef BLOCKINGDEVDIALOG_H
#define BLOCKINGDEVDIALOG_H

#include <QDialog>
#include <QTreeView>
#include <QBoxLayout>
#include "GlobalHeaders/Global.h"
#include "GlobalHeaders/GlobalFuntion.h"
#include "GlobalHeaders/GlobalStructure.h"
#include "MyStation/mystation.h"
/*封锁设备操作*/
namespace Ui {
class BlockingDevDialog;
}
typedef struct
{
    QString strItemCheckName="";
    QString strStationName="";
    QList<int> nDevCode={};
}TreeStandardItemCheckBlock;
class BlockingDevDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BlockingDevDialog(QWidget *parent = nullptr);
    ~BlockingDevDialog();
    void Init(QVector<MyStation*> vectMyStation);
    void InitTree(MyStation* pMyStation,int n);
    Qt::CheckState UpadataTree(MyStation* pMyStation,int ntype,int n);
    int GetStationIDByName(QString strStationName);
    QList<int> GetDevCodeByName(QString strStationName,QString strType,QString strDevName);
    void UpdatePowerData();
signals:
    void SendBlockingDevDataToSeverSignal(int nStaIndex,QList<int> nDevCodelist,bool bset);
    void SendTextAnalysisSignal(int index, QPoint pt, QString dev = "");
private slots:
    void on_pushButton_pressed();

    void on_pushButton_2_pressed();

    void on_pushButton_4_pressed();
    void treeItemChanged(QStandardItem * item);
private:
    Ui::BlockingDevDialog *ui;
    QStandardItemModel *TreeModel=nullptr;
    QTreeView* pQTreeView=nullptr;
    QVector<MyStation*> pVectMyStation;
    QBoxLayout *pTreelayout = nullptr;
    //被选中子节点序列
    QVector<TreeStandardItemCheckBlock *>pVectTreeItemCheck;
    QList<QString>strTextShowList;
    //定时器
    int Timer_ID_1000;
private:
    virtual void timerEvent(QTimerEvent *event) override;//定时器
};

#endif // BLOCKINGDEVDIALOG_H
