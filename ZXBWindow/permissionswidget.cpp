#include "permissionswidget.h"
#include "ui_permissionswidget.h"
#include <QMenu>
#pragma execution_character_set("utf-8")

PermissionsWidget::PermissionsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PermissionsWidget)
{
    ui->setupUi(this);
    ui->label_2->hide();
    ui->pushButton_4->hide();
}

PermissionsWidget::~PermissionsWidget()
{
    delete ui;
}

void PermissionsWidget::Init(MyStation* pMyStation, bool bShowSearch, bool bShowFunBtn)
{
    if(!bShowSearch)
    {
        ui->pushButton_3->hide();
        ui->lineEdit->hide();
    }

    if(!bShowFunBtn)
    {
        ui->widget->hide();
        ui->pushButton_7->hide();
        ui->pushButton_8->hide();
    }

    pCurStation = pMyStation;
    SetLableSolt(pCurStation->m_nRoutePermission == 2 && !pCurStation->m_nFCZKMode && pCurStation->m_nModeState == 1);
    //上下行
    {
        QMenu* m_menu = new QMenu(this);
        QAction* pActionAll = new QAction();
        QAction* pActionS = new QAction();
        QAction* pActionX = new QAction();
        pActionAll->setText("全部");
        pActionS->setText("S");
        pActionX->setText("X");
        m_menu->addAction(pActionAll);
        m_menu->addAction(pActionS);
        m_menu->addAction(pActionX);
        ui->pushButton_4->setMenu(m_menu);
        connect(pActionAll, &QAction::triggered, this, [=](bool checked){
            ui->pushButton_4->setText("全部");
            emit DirectionDisplay(0);
        });
        connect(pActionS, &QAction::triggered, this, [=](bool checked){
            ui->pushButton_4->setText("S");
            emit DirectionDisplay(1);
        });
        connect(pActionX, &QAction::triggered, this, [=](bool checked){
            ui->pushButton_4->setText("X");
            emit DirectionDisplay(2);
        });
    }

    //接发车口
    {
        QMenu* pMenuJC = new QMenu(this);
        QAction* pActionAllJC = new QAction();
        pActionAllJC->setText("全部");
        pMenuJC->addAction(pActionAllJC);
        connect(pActionAllJC, &QAction::triggered, this, [=](bool checked){
            ui->pushButton_5->setText("全部");
            emit ArriveDisplay(0);
            emit SendTextAnalysisSignal(40, QCursor::pos());
        });
        ui->pushButton_5->setMenu(pMenuJC);
//
        QMenu* pMenuFC = new QMenu(this);
        QAction* pActionAllFC = new QAction();
        pActionAllFC->setText("全部");
        pMenuFC->addAction(pActionAllFC);
        connect(pActionAllFC, &QAction::triggered, this, [=](bool checked){
            ui->pushButton_6->setText("全部");
            emit DepartureDisplay(0);
            emit SendTextAnalysisSignal(41, QCursor::pos());
        });
        ui->pushButton_6->setMenu(pMenuFC);

        for(int i = 0; i < pCurStation->DevArray.size(); i++)
        {
            if(pCurStation->DevArray[i]->getDevType() == Dev_XH)
            {
                CXHD* pXHD=(CXHD*)(pCurStation->DevArray[i]);
                if(pXHD->pCenter.x() < 0 || pXHD->pCenter.y() < 0)
                {
                    continue;
                }
                if(pXHD->getXHDType() == JZ_XHJ)
                {
                    QAction* pActionJC = new QAction();
                    pActionJC->setText(pXHD->m_strName);
                    pMenuJC->addAction(pActionJC);
                    connect(pActionJC, &QAction::triggered, this, [=](bool checked){
                        ui->pushButton_5->setText(pXHD->m_strName);
                        emit ArriveDisplay(pXHD->m_nCode);
                        emit SendTextAnalysisSignal(40, QCursor::pos());
                    });

                    QAction* pActionFC = new QAction();
                    pActionFC->setText(pXHD->m_strName);
                    pMenuFC->addAction(pActionFC);
                    connect(pActionFC, &QAction::triggered, this, [=](bool checked){
                        ui->pushButton_6->setText(pXHD->m_strName);
                        emit DepartureDisplay(pXHD->m_nCode);
                        emit SendTextAnalysisSignal(41, QCursor::pos());
                    });
                }
            }
        }
    }
}
//设置只读、可修改
void PermissionsWidget::SetLableSolt(bool bPermission)
{
    ui->label->clear();
    if(bPermission)
    {
        ui->label->setText(pCurStation->m_strStationName+"-模式:<可修改>");
        ui->label->setStyleSheet("background-color: rgb(85, 255, 0);"
                                 "padding-left: 4px;"
                                 "padding-top: 4px;"
                                 "padding-right: 4px;"
                                 "padding-bottom: 4px;");
    }
    else
    {
        ui->label->setText(pCurStation->m_strStationName+"-模式:<只读>");
        ui->label->setStyleSheet("background-color: rgb(240, 160, 25);"
                                 "padding-left: 4px;"
                                 "padding-top: 4px;"
                                 "padding-right: 4px;"
                                 "padding-bottom: 4px;");
    }
}

void PermissionsWidget::on_applyForBtn_clicked()
{
    emit SendTextAnalysisSignal(42, QCursor::pos());
    emit applyForPermission();

}

void PermissionsWidget::on_releaseBtn_clicked()
{
    emit SendTextAnalysisSignal(43, QCursor::pos());
    emit releasePermission();

}

void PermissionsWidget::on_pushButton_7_clicked(bool checked)
{
    emit SendTextAnalysisSignal(44, QCursor::pos());
    emit TurnToTrackView();

}

void PermissionsWidget::on_pushButton_8_clicked(bool checked)
{
    emit SendTextAnalysisSignal(45, QCursor::pos());
    emit TurnToRouteOrderView();

}

void PermissionsWidget::on_pushButton_3_clicked()
{
    emit SendTextAnalysisSignal(167, QCursor::pos());
}
