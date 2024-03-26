#include "RoutePreviewWnd.h"
#include "ui_RoutePreviewWnd.h"

RoutePreviewWnd::RoutePreviewWnd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoutePreviewWnd)
{
    ui->setupUi(this);
}

RoutePreviewWnd::~RoutePreviewWnd()
{
    delete ui;
}
