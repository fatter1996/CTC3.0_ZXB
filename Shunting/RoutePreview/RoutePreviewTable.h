#ifndef ROUTEPREVIEWTABLE_H
#define ROUTEPREVIEWTABLE_H

#pragma execution_character_set("utf-8")
#include<QStandardItemModel>
#include<QTableView>

class RoutePreviewTable : public QTableView
{
public:
    explicit RoutePreviewTable(QObject *parent = nullptr);
    ~RoutePreviewTable();

    void Init();

private:
    QStandardItemModel* m_pItemModel = NULL;
};

#endif // ROUTEPREVIEWTABLE_H
