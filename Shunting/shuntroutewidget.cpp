#include "shuntroutewidget.h"
#include "ui_shuntroutewidget.h"

ShuntRouteWidget::ShuntRouteWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShuntRouteWidget)
{
    ui->setupUi(this);
}

ShuntRouteWidget::~ShuntRouteWidget()
{
    delete ui;
}
