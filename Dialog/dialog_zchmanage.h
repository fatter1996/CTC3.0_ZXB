#ifndef DIALOG_ZCHMANAGE_H
#define DIALOG_ZCHMANAGE_H

#include <QDialog>
#include <QDateTime>
#include <QStandardItemModel>
/*站存车管理*/
namespace Ui {
class Dialog_ZCHManage;
}

class Dialog_ZCHManage : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_ZCHManage(QWidget *parent = nullptr);
    ~Dialog_ZCHManage();
    QStandardItemModel *m_pModel_ZCC1;
    QStandardItemModel *m_pModel_ZCC2;
    QStandardItemModel *m_pModel_ZCC3;
    QStringList ListGD;
    QStringList ListTrain;
    QStringList m_List_ZCC1;
    QStringList m_List_ZCC2;
    QStringList m_List_ZCC3;
    //表格数据
    QStringList m_List_table_ZCC1;
    QStringList m_List_table_ZCC2;
    QStringList m_List_table_ZCC3;
    int nTableCheck1=-1;
    void Init();
    void InitTable();
    void UpDataTable1(QStringList strlist);
    void UpDataTable2();
    void UpDataTable3();
    void SetTime(int ntype,QDateTime time);
    void SetTableData(QStandardItemModel *m_pModel,QStringList strlist,int nColumnCount);
//signals:
//    void itemLBtnClicked(const QModelIndex &index);
private slots:
    void on_unlockButton_2_clicked();

    void on_unlockButton_3_clicked();
     //void itemLBtnClicked(const QModelIndex &index);
     void on_lockButton_2_clicked();

private:
    Ui::Dialog_ZCHManage *ui;
};

#endif // DIALOG_ZCHMANAGE_H
