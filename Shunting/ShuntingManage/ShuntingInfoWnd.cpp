#include "ShuntingInfoWnd.h"
#include "ui_ShuntingInfoWnd.h"
#include <QMenu>
#include <QDebug>

ShuntingInfoWnd::ShuntingInfoWnd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShuntingInfoWnd)
{
    ui->setupUi(this);

    m_pTrainTypeMune = addSheet(QStringList() << "本务机" << "专用调机");
    ui->trainTypeBtn->setMenu(m_pTrainTypeMune);
    connect(m_pTrainTypeMune, &QMenu::triggered, this, &ShuntingInfoWnd::onTrainTypeChange);

    m_pPrintMune = addSheet(QStringList() << "单份" << "纵向双份" << "横向三份");
    ui->printBtn->setMenu(m_pPrintMune);
    connect(m_pPrintMune, &QMenu::triggered, this, &ShuntingInfoWnd::onPrintChoice);

}

ShuntingInfoWnd::~ShuntingInfoWnd()
{
    m_pTrainTypeMune->clear();
    m_pPrintMune->clear();
    delete m_pTrainTypeMune;
    delete m_pPrintMune;
    delete ui;
}

QMenu* ShuntingInfoWnd::addSheet(QStringList strMune)
{
    QMenu* menu= new QMenu(this);
    for(int i = 0; i < strMune.size(); i++)
    {
        QAction* action= new QAction(menu);
        action->setText(strMune.at(i));
        menu->addAction(action);
    }
    return menu;
}

void ShuntingInfoWnd::onTrainTypeChange(QAction *action)
{
    ui->trainTypeBtn->setText(QString(action->text()));
}

void ShuntingInfoWnd::onPrintChoice(QAction *action)
{
    if(action->text() == QString("单份"))
    {

    }
    else if(action->text() == QString("纵向双份"))
    {

    }
    else if(action->text() == QString("横向三份"))
    {

    }
}
