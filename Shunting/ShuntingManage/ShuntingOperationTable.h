#ifndef SHUNTINGOPERATIONTABLE_H
#define SHUNTINGOPERATIONTABLE_H

#pragma execution_character_set("utf-8")
#include<QStandardItemModel>
#include<QTableView>

class ShuntingOperationTable : public QTableView
{
public:
    explicit ShuntingOperationTable(QObject *parent = nullptr);
    ~ShuntingOperationTable();

    void Init();

private:
    QStandardItemModel* m_pItemModel = NULL;
};

#endif // SHUNTINGOPERATIONTABLE_H
