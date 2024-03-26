#include "LeedWireTable.h"

LeedWireTable::LeedWireTable(QObject *parent)
{
    m_pItemModel = new QStandardItemModel;
    m_pItemModel->setHorizontalHeaderLabels(QStringList() << "源股道" << "目标股道" << "牵出方向"<< "牵出线");

    setModel(m_pItemModel);
    setColumnWidth(0, 234);
    setColumnWidth(1, 234);
    setColumnWidth(2, 234);
    setColumnWidth(3, 234);
}

LeedWireTable::~LeedWireTable()
{
    delete m_pItemModel;
}

void LeedWireTable::Init()
{

}
