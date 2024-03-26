#include "CancelRouteWnd.h"
#include "ui_CancelRouteWnd.h"

CancelRouteWnd::CancelRouteWnd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CancelRouteWnd)
{
    ui->setupUi(this);
}

CancelRouteWnd::~CancelRouteWnd()
{
    delete ui;
}
