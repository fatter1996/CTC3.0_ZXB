#include "DirectionSetting.h"
#include "ui_DirectionSetting.h"

DirectionSetting::DirectionSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DirectionSetting)
{
    ui->setupUi(this);
}

DirectionSetting::~DirectionSetting()
{
    delete ui;
}
