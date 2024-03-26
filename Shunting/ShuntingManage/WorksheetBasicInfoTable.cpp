#include "WorksheetBasicInfoTable.h"

WorksheetBasicInfoTable::WorksheetBasicInfoTable(QObject *parent)
{
    m_pItemModel = new QStandardItemModel;
    m_pItemModel->setHorizontalHeaderLabels(
                QStringList() << "作业单" << "状态" << "编制时间");
    setModel(m_pItemModel);
    setColumnWidth(0, 120);
    setColumnWidth(1, 48);
    setColumnWidth(2, 120);
}

WorksheetBasicInfoTable::~WorksheetBasicInfoTable()
{
    delete m_pItemModel;
}

void WorksheetBasicInfoTable::Init()
{

}
