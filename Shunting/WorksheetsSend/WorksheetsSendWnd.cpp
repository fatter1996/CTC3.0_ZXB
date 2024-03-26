#include "WorksheetsSendWnd.h"
#include "ui_WorksheetsSendWnd.h"

WorksheetsSendWnd::WorksheetsSendWnd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WorksheetsSendWnd)
{
    ui->setupUi(this);
}

WorksheetsSendWnd::~WorksheetsSendWnd()
{
    delete ui;
}
