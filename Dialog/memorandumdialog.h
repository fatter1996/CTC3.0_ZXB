#ifndef MEMORANDUMDIALOG_H
#define MEMORANDUMDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QMenu>
#include <QTreeWidgetItem>
#include <QDateTime>
#include "TableView/tableview.h"
/*备忘录*/
namespace Ui {
class MemorandumDialog;
}
typedef struct
{
   //标题
    QString strTitel;
    //标志
    QString strFlag;
    //信息
    QString strInfo;
    //备忘内容
    QString strMemorandumm;
    //建立时间
    QDateTime timeNew;
     QDateTime timetree;
    //生效时间
    QDateTime timeStart;
    //过期时间
    QDateTime timeEnd;
    //用户编号
    QString strUserNum;
    //用户名称
    QString strUserName;
    //终端
    QString strUserTerminal;
    //确认时间
    QDateTime timeConfrim;
    void Init()
    {
        strTitel="";
        strFlag="";
        strInfo="";
        strMemorandumm="";
        strUserNum="";
        strUserName="";
        strUserTerminal="";
        timeNew=QDateTime::currentDateTime();
        timeStart=QDateTime::currentDateTime();
        timeEnd=QDateTime::currentDateTime();
        timeConfrim=QDateTime::currentDateTime();
        timetree=QDateTime::currentDateTime();

    }
}TreeShowData;
class MemorandumDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MemorandumDialog(QWidget *parent = nullptr);
    ~MemorandumDialog();
    typedef struct
    {
        TreeShowData *pTreeShowData;
        QTreeWidgetItem *mItem;
        bool bdz=false;//得知
        int bActivation=0;//备忘信息激活状态 0：未激活未过期 1：已激活未过期 2：过期
    }TreeShow;
    QVector<TreeShow*>mVectTreeShowData;
    TableView* mCTableView=nullptr;
    TableView* mCTableView2=nullptr;
    void Init();
    void SetTimeEn(bool b);
    //初始化表格
    void InitTable();
    //初始化表格数据
    void InitTableView();
    //设置按钮使能状态
    void SetBtnEn(bool b);
    void InitMenu();
    void InitTree();
    TreeShowData* GetSaveData();
    void UpdateTabWidgetShow(QString str);
    int GetTabItemAndViewInfo1(QTreeWidgetItem *mItem);
    void SetviewInfo(TreeShowData *pTreeShowData);
    bool GetOrderActivationState();
    //站场重置
    void ResetStation();
    void ClearEditInfo();
    //刷新树形目录
    void UpDataTreeShow();
signals:
     void SendBtnIncoChangeSignal(bool b);
     void SendTextAnalysisSignal(int index, QPoint pt, QString dev = "");
private slots:
      //
      void MenuPopSignal(QAction *action);
    //新建
    void on_pushButton_2_pressed();
    //备忘
    void on_pushButton_3_pressed();
    //结束
    void on_pushButton_4_pressed();
    //得知
    void on_pushButton_5_pressed();
    //执行
    void on_pushButton_6_pressed();
    //关闭
    void on_pushButton_7_pressed();
    void onItemSelected1();

private:
    Ui::MemorandumDialog *ui;
    QMenu *m_menu;
    QTreeWidgetItem *topItem1=nullptr;
    int indexcheck=-1;
    int  timerId = -1;
    bool bnew=false;
    virtual void timerEvent(QTimerEvent *event) override;//定时器
};

#endif // MEMORANDUMDIALOG_H
