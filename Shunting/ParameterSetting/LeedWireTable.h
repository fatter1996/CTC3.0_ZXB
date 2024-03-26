#ifndef LEEDWIRETABLE_H
#define LEEDWIRETABLE_H

#pragma execution_character_set("utf-8")
#include<QStandardItemModel>
#include<QTableView>

class LeedWireTable : public QTableView
{
public:
    explicit LeedWireTable(QObject *parent = nullptr);
    ~LeedWireTable();

    void Init();

private:
    QStandardItemModel* m_pItemModel = NULL;
};
#endif // LEEDWIRETABLE_H
