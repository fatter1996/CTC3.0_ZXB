#include "PriorRouteSetting.h"
#include "ui_PriorRouteSetting.h"

PriorRouteSetting::PriorRouteSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PriorRouteSetting)
{
    ui->setupUi(this);
}

PriorRouteSetting::~PriorRouteSetting()
{
    delete ui;
}
