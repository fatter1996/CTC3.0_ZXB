#ifndef SHUNTROUTETABLEWIDGET_H
#define SHUNTROUTETABLEWIDGET_H

#include <QWidget>
#include "TableView/tableview.h"
#include "MyStation/mystation.h"
namespace Ui {
class ShuntRouteTableWidget;
}

class ShuntRouteTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ShuntRouteTableWidget(QWidget *parent = nullptr);
    ~ShuntRouteTableWidget();

    void InitTableView();
    void SetTableStyleSheet();

private:
    Ui::ShuntRouteTableWidget *ui;
    TableView* mCTableViewTrain = nullptr;
    MyStation* pMyStation = nullptr;
};

#endif // SHUNTROUTETABLEWIDGET_H
