#ifndef STATIONSTORAGESETDIALOG_H
#define STATIONSTORAGESETDIALOG_H

#include <QDialog>
#include "GDData/gddata.h"
#include <QStandardItemModel>
#include "TableView/tableview.h"
namespace Ui {
class StationStorageSetDialog;
}

class StationStorageSetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StationStorageSetDialog(QWidget *parent = nullptr);
    ~StationStorageSetDialog();
    QStandardItemModel *m_pModel_ZCC3;
    //表头
    QStringList m_List_ZCC3;
    //表格数据
    QStringList m_List_table_ZCC3;
    TableView* mCTableView;
    bool bok=false;
    bool bclear=false;
//    typedef struct
//    {
//       //表格显示信息
//        QStringList strlist;
//        //股道占用信息
//        QString strGDTrain;
//        //类别
//        QString strlb;
//        //车种
//        QString strcz;
//        //
//        //作业说明
//        QString strzysm;
//        //辆数
//        QString strnum;
//        //运用标记
//        QString strbj;
//        //去向
//        QString strqx;
//        void Init()
//        {
//            strlist.clear();
//            strGDTrain="";
//            strlb="";
//            strcz="";
//            strzysm="";
//            strnum="";
//            strbj="";
//            strqx="";
//        }
//    }TableShowData;
    QVector<TableShowData>mVectTableShowData;
    void Init();
    void InitEditZoneShow();
    void InitTable();
    //初始化股道信息
    void InitGD(GDData* mGDData);
    void SetTableData(QStandardItemModel *m_pModel,QStringList strlist,int nColumnCount);
    void SetTextBrowser(QString str);
    void SetTextBrowser();
    QString GetTextBrowser();
    QString GetTextBrowserAll();
    QString convertChineseToPinyin(QString chinese);
    //初始化表格数据
    void InitTableView();
    //更新表格数据
    void UpDateTableView();
    //插入、增加操作
    void AddGDSet();
    //设置表行背景颜色
    void SetBacklackRow(TableView* mCTableView,int index);
    //更新界面显示
    void UpDateView();
signals:
    void SendTextAnalysisSignal(int index, QPoint pt, QString dev = "");
private slots:
    //插入
    void on_unlockButton_3_pressed();
    //增加
    void on_unlockButton_6_pressed();
    //修改
    void on_unlockButton_5_pressed();
    //删除
    void on_unlockButton_4_pressed();
    //清除
    void on_pushButton_pressed();
    //确认设置
    void on_pushButton_2_pressed();
    //关闭
    void on_pushButton_3_pressed();
    //  表格左键点击
    void itemLBtnClicked(const QModelIndex &index);

private:
    Ui::StationStorageSetDialog *ui;
    GDData* pGDData=nullptr;
    int nCheckRowLeft=-1;
};

#endif // STATIONSTORAGESETDIALOG_H
