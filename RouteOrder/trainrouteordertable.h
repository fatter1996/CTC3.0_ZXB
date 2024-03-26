#ifndef TRAINROUTEORDERTABLE_H
#define TRAINROUTEORDERTABLE_H

#include <QWidget>

namespace Ui {
class TrainRouteOrderTable;
}

class TrainRouteOrderTable : public QWidget
{
    Q_OBJECT

public:
    explicit TrainRouteOrderTable(QWidget *parent = nullptr);
    ~TrainRouteOrderTable();

private:
    Ui::TrainRouteOrderTable *ui;
};

#endif // TRAINROUTEORDERTABLE_H
