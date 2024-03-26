#include "ShuntingManageWnd.h"
#include "ui_ShuntingManageWnd.h"
#include <QButtonGroup>
#include <QMenu>
#include <QDesktopWidget>
#include <QDebug>
#include <QMainWindow>

ShuntingManageWnd::ShuntingManageWnd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShuntingManageWnd)
{
    ui->setupUi(this);

    addNewTabWidget(QString("tab_1"));
    addNewTabWidget(QString("tab_2"));
    addNewTabWidget(QString("tab_3"));
    QButtonGroup* block1 = new QButtonGroup(this);
    block1->addButton(ui->currentRBtn, 0);
    block1->addButton(ui->historyRBtn, 1);

    ui->currentRBtn->setChecked(true);
    ui->historyLabel->setVisible(false);

    ui->widget->setVisible(false);
}

ShuntingManageWnd::~ShuntingManageWnd()
{
    delete ui;
}

void ShuntingManageWnd::moveEvent(QMoveEvent *event)
{
    if(isPaging)
    {
        QWidget* temp = NULL;
        for(int i = 0; i < m_tabWidgetList.size(); i++)
        {
            temp = m_tabWidgetList.at(i);
            int x = this->x() + 344 + temp->width() * (i % 2) + 12 * (i % 2);
            int y = this->y() + 42 + temp->height() * (i / 2) + 44 * (i / 2);
            temp->move(x, y);
            if(i >= 3) break;
        }
    }
}

void ShuntingManageWnd::addNewTabWidget(QString wndName)
{
    ShuntingInfoWnd* infoWnd = new ShuntingInfoWnd(ui->tabWidget);
    infoWnd->setWindowTitle(wndName);
    ui->tabWidget->addTab(infoWnd,QIcon(":/data/res/train.png"), wndName);
    m_tabWidgetList.append(infoWnd);
}

void ShuntingManageWnd::on_currentRBtn_toggled(bool checked)
{
    if(checked)
    {
        ui->currentLable->setVisible(true);
        ui->historyLabel->setVisible(false);
    }
}

void ShuntingManageWnd::on_historyRBtn_toggled(bool checked)
{
    if(checked)
    {
        ui->currentLable->setVisible(false);
        ui->historyLabel->setVisible(true);
    }
}

void ShuntingManageWnd::on_showSetOutRBtn_toggled(bool checked)
{
    if(checked)
    {

    }
    else
    {

    }
}


void ShuntingManageWnd::on_pagingBtn_clicked()
{
    if(isPaging) return;
    if(m_tabWidgetList.size() == 0) return;

    ui->widget->setVisible(true);
    ui->tabWidget->setVisible(false);

    for(int i = 0; i < m_tabWidgetList.size(); i++)
    {
        ui->tabWidget->removeTab(0);
    }

    if(m_tabWidgetList.size() == 2)
    {
        this->setFixedSize(1608, 412);
    }
    else if(m_tabWidgetList.size() > 2)
    {
        this->setFixedSize(1608, 824);
    }

    QDesktopWidget *pDesk = QApplication::desktop();
    this->move((pDesk->width() - this->width()) / 2,
               (pDesk->height() - this->height()) / 2);

    QWidget* temp = NULL;
    for(int i = 0; i < m_tabWidgetList.size(); i++)
    {
        temp = m_tabWidgetList.at(i);
        temp->setWindowFlags(windowFlags() &~ Qt::WindowMinMaxButtonsHint);
        temp->setWindowIcon(QIcon(":/data/res/train.png"));

        int x = this->x() + 344 + temp->width() * (i % 2) + 12 * (i % 2);
        int y = this->y() + 42 + temp->height() * (i / 2) + 44 * (i / 2);
        temp->move(x, y);
        temp->show();

        if(i >= 3) break;
    }

    isPaging = true;
}



void ShuntingManageWnd::on_columnarBtn_clicked()
{
    if(!isPaging) return;
    ui->widget->setVisible(false);

    this->setFixedSize(980, 412);
    QDesktopWidget *pDesk = QApplication::desktop();
    this->move((pDesk->width() - this->width()) / 2,
               (pDesk->height() - this->height()) / 2);

    QWidget* temp = NULL;
    for(int i = 0; i < m_tabWidgetList.size(); i++)
    {
        temp = m_tabWidgetList.at(i);
        temp->setParent(ui->tabWidget);
        ui->tabWidget->addTab(temp, temp->windowTitle());
    }
    ui->tabWidget->setVisible(true);
    isPaging = false;
}
