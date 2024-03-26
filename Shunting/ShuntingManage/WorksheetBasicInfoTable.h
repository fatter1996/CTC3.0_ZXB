#ifndef WORKSHEETBASICINFOTABLE_H
#define WORKSHEETBASICINFOTABLE_H

#pragma execution_character_set("utf-8")
#include<QStandardItemModel>
#include<QTableView>

class WorksheetBasicInfoTable : public QTableView
{
public:
    explicit WorksheetBasicInfoTable(QObject *parent = nullptr);
    ~WorksheetBasicInfoTable();

    void Init();

private:
    QStandardItemModel* m_pItemModel = NULL;
};

#endif // WORKSHEETBASICINFOTABLE_H
