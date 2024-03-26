#include "WorksheetsSendTable.h"

WorksheetsSendTable::WorksheetsSendTable(QObject *parent)
{
    m_pItemModel = new QStandardItemModel;
    m_pItemModel->setHorizontalHeaderLabels(QStringList() << "勾" << "线路" << "作业"<< "方向");

    setModel(m_pItemModel);
    setColumnWidth(0, 92);
    setColumnWidth(1, 120);
    setColumnWidth(2, 120);
    setColumnWidth(3, 120);
}

WorksheetsSendTable::~WorksheetsSendTable()
{
    delete m_pItemModel;
}

void WorksheetsSendTable::Init()
{

}
