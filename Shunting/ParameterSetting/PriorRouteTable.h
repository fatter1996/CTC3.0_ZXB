#ifndef PRIORROUTETABLE_H
#define PRIORROUTETABLE_H

#pragma execution_character_set("utf-8")
#include<QStandardItemModel>
#include<QTableView>

class PriorRouteTable : public QTableView
{
public:
    explicit PriorRouteTable(QObject *parent = nullptr);
    ~PriorRouteTable();

    void Init();

private:
    QStandardItemModel* m_pItemModel = NULL;
};

#endif // PRIORROUTETABLE_H
