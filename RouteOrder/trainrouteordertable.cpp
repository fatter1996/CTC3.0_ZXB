#include "trainrouteordertable.h"
#include "ui_trainrouteordertable.h"

TrainRouteOrderTable::TrainRouteOrderTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrainRouteOrderTable)
{
    ui->setupUi(this);
}

TrainRouteOrderTable::~TrainRouteOrderTable()
{
    delete ui;
}
