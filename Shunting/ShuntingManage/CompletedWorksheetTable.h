#ifndef COMPLETEDWORKSHEETTABLE_H
#define COMPLETEDWORKSHEETTABLE_H

#pragma execution_character_set("utf-8")
#include<QStandardItemModel>
#include<QTableView>

class CompletedWorksheetTable : public QTableView
{
public:
    explicit CompletedWorksheetTable(QObject *parent = nullptr);
    ~CompletedWorksheetTable();

    void Init();

private:
    QStandardItemModel* m_pItemModel = NULL;
};

#endif // COMPLETEDWORKSHEETTABLE_H
