#include "ShuntingOperationTable.h"

ShuntingOperationTable::ShuntingOperationTable(QObject *parent)
{
    m_pItemModel = new QStandardItemModel;
    m_pItemModel->setHorizontalHeaderLabels(
                QStringList() << "勾序" << "线路" << "方法" << "辆数" << "方向"
                << "牵出线"<< "状态" << "牵出线路" << "推入进路" << "记事");
    setModel(m_pItemModel);
    setColumnWidth(0, 32);
    setColumnWidth(1, 48);
    setColumnWidth(2, 36);
    setColumnWidth(3, 36);
    setColumnWidth(4, 36);
    setColumnWidth(5, 100);
    setColumnWidth(6, 48);
    setColumnWidth(7, 80);
    setColumnWidth(8, 80);
    setColumnWidth(9, 96);
}

ShuntingOperationTable::~ShuntingOperationTable()
{
    delete m_pItemModel;
}

void ShuntingOperationTable::Init()
{

}
