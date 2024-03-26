#ifndef WORKSHEETSSENDTABLE_H
#define WORKSHEETSSENDTABLE_H

#pragma execution_character_set("utf-8")
#include<QStandardItemModel>
#include<QTableView>

class WorksheetsSendTable : public QTableView
{
public:
    explicit WorksheetsSendTable(QObject *parent = nullptr);
    ~WorksheetsSendTable();

    void Init();

private:
    QStandardItemModel* m_pItemModel = NULL;
};

#endif // WORKSHEETSSENDTABLE_H
