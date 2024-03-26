#include "RoutePreviewTable.h"

RoutePreviewTable::RoutePreviewTable(QObject *parent)
{
    m_pItemModel = new QStandardItemModel;
    m_pItemModel->setHorizontalHeaderLabels(QStringList() << "勾" << "线路" << "方向" << "牵出线"
                                            << "状态" << "牵出进路" << "推入进路");

    setModel(m_pItemModel);
    setColumnWidth(0, 32);
    setColumnWidth(1, 64);
    setColumnWidth(2, 48);
    setColumnWidth(3, 72);
    setColumnWidth(4, 56);
    setColumnWidth(5, 120);
    setColumnWidth(6, 120);

}

RoutePreviewTable::~RoutePreviewTable()
{
    delete m_pItemModel;
}

void RoutePreviewTable::Init()
{

}


