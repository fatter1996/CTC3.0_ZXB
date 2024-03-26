#include "Worksheets.h"
#include "ui_Worksheets.h"
#include <QMenu>

Worksheets::Worksheets(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Worksheets)
{
    ui->setupUi(this);
}

Worksheets::~Worksheets()
{
    delete ui;
}

void Worksheets::setTitle(QString strTitle)
{
    ui->groupBox->setTitle(strTitle);
}

void  Worksheets::addSheet(QStringList strMune)
{
    QMenu* menu= new QMenu(this);
    for(int i = 0; i < strMune.size(); i++)
    {
        QAction* action= new QAction(menu);
        action->setText(strMune.at(i));
        menu->addAction(action);
    }
    ui->pushButton->setMenu(menu);
}



