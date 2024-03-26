#include "WorksheetTable.h"

WorksheetTable::WorksheetTable(QObject *parent)
{
    m_pItemModel = new QStandardItemModel;
    m_pItemModel->setHorizontalHeaderLabels(
                QStringList() << "勾" << "线路" << "类型"
                << "辆数" << "方向" << "状态" << "记事");
    setModel(m_pItemModel);
    setColumnWidth(0, 32);
    setColumnWidth(1, 72);
    setColumnWidth(2, 36);
    setColumnWidth(3, 36);
    setColumnWidth(4, 36);
    setColumnWidth(5, 60);
    setColumnWidth(6, 96);
}

WorksheetTable::~WorksheetTable()
{
    delete m_pItemModel;
}

void WorksheetTable::Init()
{

}
