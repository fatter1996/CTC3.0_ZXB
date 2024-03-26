#include "stationstoragewidgettitle.h"
#include "ui_stationstoragewidgettitle.h"

StationStorageWidgetTitle::StationStorageWidgetTitle(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StationStorageWidgetTitle)
{
    ui->setupUi(this);
}

StationStorageWidgetTitle::~StationStorageWidgetTitle()
{
    delete ui;
}

void StationStorageWidgetTitle::on_pushButton_clicked()
{

}
