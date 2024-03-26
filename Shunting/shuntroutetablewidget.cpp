#include "shuntroutetablewidget.h"
#include "ui_shuntroutetablewidget.h"
#include <QLayout>
ShuntRouteTableWidget::ShuntRouteTableWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShuntRouteTableWidget)
{
    ui->setupUi(this);
    mCTableViewTrain = new TableView();

    InitTableView();
    mCTableViewTrain->SetTableHeadHideV(false);
    mCTableViewTrain->SetTableHeadHideH(true);
    mCTableViewTrain->CreatTableView(false);
    this->layout()->addWidget(mCTableViewTrain);
}

ShuntRouteTableWidget::~ShuntRouteTableWidget()
{
    delete ui;
}

void ShuntRouteTableWidget::InitTableView()
{
    mCTableViewTrain->mTableViewInfo.Init();

    //初始化表头
    mCTableViewTrain->mTableViewInfo.Vect_HeaderData.append("勾");
    mCTableViewTrain->mTableViewInfo.Vect_HeaderDataWidth.append(32);
    mCTableViewTrain->mTableViewInfo.Vect_HeaderData.append("线路");
    mCTableViewTrain->mTableViewInfo.Vect_HeaderDataWidth.append(80);
    mCTableViewTrain->mTableViewInfo.Vect_HeaderData.append("方法");
    mCTableViewTrain->mTableViewInfo.Vect_HeaderDataWidth.append(48);
    mCTableViewTrain->mTableViewInfo.Vect_HeaderData.append("辆数");
    mCTableViewTrain->mTableViewInfo.Vect_HeaderDataWidth.append(48);
    mCTableViewTrain->mTableViewInfo.Vect_HeaderData.append("方向");
    mCTableViewTrain->mTableViewInfo.Vect_HeaderDataWidth.append(48);

    mCTableViewTrain->mTableViewInfo.Vect_HeaderData.append("牵出线");
    mCTableViewTrain->mTableViewInfo.Vect_HeaderDataWidth.append(120);

    mCTableViewTrain->mTableViewInfo.Vect_HeaderData.append("状态");
    mCTableViewTrain->mTableViewInfo.Vect_HeaderDataWidth.append(64);

    mCTableViewTrain->mTableViewInfo.Vect_HeaderData.append("牵出进路");
    mCTableViewTrain->mTableViewInfo.Vect_HeaderDataWidth.append(80);
    mCTableViewTrain->mTableViewInfo.Vect_HeaderData.append("推入进路");
    mCTableViewTrain->mTableViewInfo.Vect_HeaderDataWidth.append(80);

    //mCTableViewTrain->mTableViewInfo.Vect_HeaderData.append("方向");
    //mCTableViewTrain->mTableViewInfo.Vect_HeaderDataWidth.append(108);
    mCTableViewTrain->mTableViewInfo.Vect_HeaderData.append("记事");
    mCTableViewTrain->mTableViewInfo.Vect_HeaderDataWidth.append(180);
    //mCTableViewTrain->SetSectionText("序", 32);
    mCTableViewTrain->SetTableHeadHeight(0, 24);
    mCTableViewTrain->SetTableHeadHeight(1, 24);


    //初始化第一行为空行
    QStringList strList;
    strList<<"1"<<""<< ""<< ""<< ""<< ""<< ""<< ""<< ""<< "";
    mCTableViewTrain->IintTableColAndRow(1, 11);
    mCTableViewTrain->SetTableData(strList);
    mCTableViewTrain->SetRowItemsBKColor(0, QColor(240,240,240), 0, 1);
    mCTableViewTrain->SetRowItemsBKColor(0, QColor(250,250,250), 1);

    //设置单元格不可编辑
    mCTableViewTrain->SetEditTriggers(QAbstractItemView::NoEditTriggers);

    SetTableStyleSheet();
}

void ShuntRouteTableWidget::SetTableStyleSheet()
{
    mCTableViewTrain->SetTableHeadsetStyleSheet(0, QString(
        "QTableView{"
            "outline:none;"
            "gridline-color: rgb(200, 200, 200);"
            "border:1px solid rgb(160, 160, 160);"
//          "font-size:10pt;"
        "}"
        "QHeaderView::section:first{"
            "border:0px solid rgb(0, 0, 0);"
            "border-right:1px solid rgb(250, 250, 250);"
        "}"
        "QHeaderView::section:middle {"
            "border:0px solid rgb(0, 0, 0);"
            "border-right:1px solid rgb(250, 250, 250);"
        "}"
        "QHeaderView::section:last {"
            "border:0px solid rgb(0, 0, 0);"
        "}"
        "QTableView QTableCornerButton::section {"
            "background:rgb(240, 240, 240);"
            "font-size:10pt;"
            "font-family:微软雅黑;"
            "border-top:2px outset rgb(240, 240, 240);"
            "border-left:2px outset rgb(240, 240, 240);"
            "border-bottom:2px outset rgb(80, 80, 80);"
            "border-right:2px outset rgb(80, 80, 80);"
        "}"
        "QTableView QTableCornerButton::section:Pressed {"
            "background:rgb(240, 240, 240);"
            "border-bottom:2px outset rgb(240, 240, 240);"
            "border-right:2px outset rgb(240, 240, 240);"
            "border-top:2px outset rgb(80, 80, 80);"
            "border-left:2px outset rgb(80, 80, 80);"
        "}"
        "QTableView::item:selected {"
            "outline:none;"
            "background:rgb(5, 125, 255);"
        "}"));

    mCTableViewTrain->SetTableHeadsetStyleSheet(1, QString(
        "QHeaderView{"
            "font-size: 10pt;"
            "background:rgba(225, 225, 225, 220);"
        "}"
        "QHeaderView::section:horizontal {"
            "background:rgba(225, 225, 225, 220);"
            "font-size: 10pt;"
            "font-weight: normal;"
        "}"));


//    mCTableViewTrain->SetTableHeadsetStyleSheet(2, QString(
//        "QHeaderView{"
//            "font-size: %1pt;"
//            "background:rgb(240, 240, 240);"
//        "}"
//        "QHeaderView::section:vartical {"
//            "background:rgb(240, 240, 240);"
//            "font-size: %1pt;"
//            "font-weight: normal;"
//        "}").arg(nFontSize));
}
