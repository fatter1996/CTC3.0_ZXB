#include "NewWorksheetsWnd.h"
#include "ui_NewWorksheetsWnd.h"

NewWorksheetsWnd::NewWorksheetsWnd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewWorksheetsWnd)
{
    ui->setupUi(this);
    ui->receiveTable->setTitle(QString("收到的作业单"));
    ui->currentTable->setTitle(QString("当前作业单"));
    ui->mergeTable->setTitle(QString("合并后的作业单"));

    ui->receiveTable->addSheet(QStringList() << "J101（调机）" << "J102" << "J103");
}

NewWorksheetsWnd::~NewWorksheetsWnd()
{
    delete ui;
}
