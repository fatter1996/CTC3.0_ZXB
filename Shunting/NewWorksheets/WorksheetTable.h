#ifndef WORKSHEETTABLE_H
#define WORKSHEETTABLE_H

#pragma execution_character_set("utf-8")
#include<QStandardItemModel>
#include<QTableView>
class WorksheetTable : public QTableView
{
public:
    explicit WorksheetTable(QObject *parent = nullptr);
    ~WorksheetTable();

    void Init();

private:
    QStandardItemModel* m_pItemModel = NULL;
};

#endif // WORKSHEETTABLE_H
