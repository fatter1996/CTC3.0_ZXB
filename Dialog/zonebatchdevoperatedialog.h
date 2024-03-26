#ifndef ZONEBATCHDEVOPERATEDIALOG_H
#define ZONEBATCHDEVOPERATEDIALOG_H

#include <QDialog>
#include <QTreeView>
#include <QBoxLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QInputDialog>
#include "GlobalHeaders/Global.h"
#include "GlobalHeaders/GlobalFuntion.h"
#include "GlobalHeaders/GlobalStructure.h"
#include "MyStation/mystation.h"
#include "zonebatchdevoperatestationwidget.h"
//区域批量设备操作
#define FUNTYPE_DS 0x08
#define FUNTYPE_JS 0x09
#define FUNTYPE_FS 0x10
#define FUNTYPE_JF 0x11

namespace Ui {
class ZoneBatchDevOperateDialog;
}

class ZoneBatchDevOperateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ZoneBatchDevOperateDialog(QWidget *parent = nullptr);
    ~ZoneBatchDevOperateDialog();
    void Init(MyStation* pMyStation);
    void ReadZoneData(); //读取分区信息
    void IniteTreeData(); //初始化Tree

    void UpdataTreelist(); //更新Tree数据
    void UpdataTreeItemState(); //更新Tree数据
    int UpdataTreelist(QString DevName);
    void MeauInite(); //右键菜单初始化
    void DevOperateByBtn(bool isUnlock); //设备操作-按钮
    void DeleteCurrectDev(); //删除设备
    bool IsZoneNameRepeat(QString name); //检查分区名称是否重复
    void SendData(unsigned char FUNType/*功能类型*/,unsigned int FUNSet_DeviceNum/*功能设置_设备编号*/);
    void CheckAllChildTreeItem(QStandardItem* item, Qt::CheckState check);
    void CheckChildTreeItemChanged(QStandardItem* item);
    Qt::CheckState GetChildTreeCheckedByChild(QStandardItem* item);
    Qt::CheckState GetChildTreeCheckedByParent(QStandardItem* item);

protected:
    void timerEvent(QTimerEvent* enent);

signals:
    void ClearStationState();
    void Signal_Refresh_ZC();
    void SendZoneBatchDevOperateToSeverSignal(int nStationID,unsigned char FUNType,unsigned int FUNSet_DeviceNum);
    void SendTextAnalysisSignal(int index, QPoint pt, QString dev = "");
public slots:
     void SelectDevslot(QVector<DeviceInfo> vecDevData);
     void treeItemChanged(QStandardItem * item);
     void onCustomContextMenuRequested(const QPoint &pos);
     void onDCMenuTriggered(QAction *action);
     void onQDMenuTriggered(QAction *action);
     void onQJMenuTriggered(QAction *action);
     void onXHMenuTriggered(QAction *action);

private slots:
    void on_comboBox_currentIndexChanged(int index);
    void on_radioButton_clicked(bool checked);
    void on_rename_btn_clicked();
    void on_AddZone_btn_pressed();
    void on_detelezone_btn_pressed();
    void on_savezone_btn_pressed();
    void on_loadzone_btn_pressed();
    void on_FSDev_btn_pressed();
    void on_JFDev_btn_pressed();
    void on_refresh__btn_pressed();
    void on_close_btn_pressed();

    void on_treeView_clicked(const QModelIndex &index);

private:
    Ui::ZoneBatchDevOperateDialog *ui;
    //当前选中显示车站
    MyStation *pCurrentStation = nullptr;

    ZoneData* pCurrectZone = nullptr;
    ZoneDev* pCurrectDev = nullptr;

    QVector<ZoneData>* m_vecZoneData = nullptr;
    QStringList strlZoneNames;
    QStandardItemModel* TreeModel = nullptr;
    QStandardItem* pSelectitem = nullptr;
    bool isEdit = true;
    int Timer_ID_500 = 0;
    //bool isStateCheng = false;
    QMutex mutex;

    QMenu *DCMenu = nullptr;
    QMenu *QDMenu = nullptr;
    QMenu *XHMenu = nullptr;
    QMenu *QJMenu = nullptr;

    QAction* DCDSaction = nullptr;
    QAction* DCDSDaction = nullptr;
    QAction* DCDSFaction = nullptr;
    QAction* DCFSaction = nullptr;
    QAction* DCDeleteaction = nullptr;

    //QAction* QDFSaction = nullptr;
    QAction* QDDeleteaction = nullptr;

    //QAction* QJFSaction = nullptr;
    QAction* QJDeleteaction = nullptr;

    QAction* XHFSaction = nullptr;
    QAction* XHDeleteaction = nullptr;
};

#endif // ZONEBATCHDEVOPERATEDIALOG_H
