#include "CompletedWorksheetTable.h"

CompletedWorksheetTable::CompletedWorksheetTable(QObject *parent)
{
    m_pItemModel = new QStandardItemModel;
    m_pItemModel->setHorizontalHeaderLabels(
                QStringList() << "序号" << "类型" << "进路"
                << "所属勾"<< "办理时间" << "状态" << "备注");
    setModel(m_pItemModel);
    setColumnWidth(0, 32);
    setColumnWidth(1, 48);
    setColumnWidth(2, 120);
    setColumnWidth(3, 100);
    setColumnWidth(4, 96);
    setColumnWidth(5, 96);
    setColumnWidth(6, 100);
}

CompletedWorksheetTable::~CompletedWorksheetTable()
{
    delete m_pItemModel;
}

void CompletedWorksheetTable::Init()
{

}

