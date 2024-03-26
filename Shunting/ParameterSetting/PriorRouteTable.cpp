#include "PriorRouteTable.h"

PriorRouteTable::PriorRouteTable(QObject *parent)
{
    m_pItemModel = new QStandardItemModel;
    m_pItemModel->setHorizontalHeaderLabels(QStringList() << "源区段" << "目标区段" << "下一勾股道"<< "进路");

    setModel(m_pItemModel);
    setColumnWidth(0, 60);
    setColumnWidth(1, 64);
    setColumnWidth(2, 72);
    setColumnWidth(3, 120);
}

PriorRouteTable::~PriorRouteTable()
{
    delete m_pItemModel;
}

void PriorRouteTable::Init()
{

}

