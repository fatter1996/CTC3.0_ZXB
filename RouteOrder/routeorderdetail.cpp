#include "routeorderdetail.h"
#include "ui_routeorderdetail.h"

RouteOrderDetail::RouteOrderDetail(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RouteOrderDetail)
{
    setWindowFlags(Qt::Window|Qt::WindowCloseButtonHint);
    ui->setupUi(this);
    QList<QLineEdit*> editList = this->findChildren<QLineEdit*>();
    for(int i = 0; i < editList.size(); i++)
    {
        editList.at(i)->setEnabled(false);
    }
    QList<QComboBox*> comboList = this->findChildren<QComboBox*>();
    for(int i = 0; i < comboList.size(); i++)
    {
        comboList.at(i)->setEnabled(false);
    }
    ui->pushButton->setEnabled(false);
}

RouteOrderDetail::~RouteOrderDetail()
{
    delete ui;
}

void RouteOrderDetail::RouteOrderDataInit(MyStation* pStation, TrainRouteOrder* pOrder)
{
    if(pOrder->m_nLHFlg == 1)
        ui->lineEdit_2->setText("列车进路");
    else ui->lineEdit_2->setText("货车进路");
    ui->lineEdit_3->setText(QString("%1(%2)").arg(pStation->m_strStationName).arg(pStation->m_nStationID));
    ui->lineEdit_4->setText(QString("%1").arg(pOrder->route_id));
    ui->lineEdit_6->setText(QString("%1").arg(pOrder->m_strTrainNum));
    QString state;
    switch (pOrder->m_nOldRouteState)
    {
    case 0: state = "等待"; break;
    case 1: state = "正在触发"; break;
    case 2: state = "触发完成"; break;
    case 3: state = "占用"; break;
    case 4: state = "出清"; break;
    case 5: state = "取消"; break;
    }
    ui->lineEdit_7->setText(state);

    QStringList strList;
    for(int i = 0; i < pStation->DevArray.size(); i++)
    {
        if(pStation->DevArray[i]->getDevType() == Dev_GD)
        {
            CGD* pGD=(CGD*)(pStation->DevArray[i]);
            if(pGD->m_nGDType == GD_QD)
            {
                strList.append(pGD->m_strName);
            }
        }
    }

    ui->comboBox_5->addItems(strList);
    ui->comboBox_5->setCurrentText(pOrder->m_strTrack);

    ui->lineEdit_15->setText(pOrder->m_timBegin.toString("hh:mm"));
    ui->lineEdit_16->setText(pOrder->m_timPlanned.toString("hh:mm"));
    ui->lineEdit->setText(pOrder->m_strRouteDescrip);
    QStringList strRouteDescripList = pOrder->m_strRouteDescripReal.split(',');

    if(strRouteDescripList.size() >= 2)
    {
        ui->comboBox->addItem(strRouteDescripList.at(0));
        ui->comboBox_4->addItem(strRouteDescripList.at(strRouteDescripList.size() - 1));
    }
    if(strRouteDescripList.size() >= 3)
    {
        ui->comboBox_2->addItem(strRouteDescripList.at(1));
    }
    if(strRouteDescripList.size() >= 4)
    {
        ui->comboBox_3->addItem(strRouteDescripList.at(2));
    }
}

void RouteOrderDetail::on_pushButton_2_clicked()
{
    this->close();
}

void RouteOrderDetail::on_pushButton_clicked()
{
}
