#include "RouteHandlingWnd.h"
#include "ui_RouteHandlingWnd.h"

RouteHandlingWnd::RouteHandlingWnd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RouteHandlingWnd)
{
    ui->setupUi(this);
}

RouteHandlingWnd::~RouteHandlingWnd()
{
    delete ui;
}
