#include "WorksheetsDetailsWnd.h"
#include "ui_WorksheetsDetailsWnd.h"

WorksheetsDetailsWnd::WorksheetsDetailsWnd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WorksheetsDetailsWnd)
{
    ui->setupUi(this);
}

WorksheetsDetailsWnd::~WorksheetsDetailsWnd()
{
    delete ui;
}
