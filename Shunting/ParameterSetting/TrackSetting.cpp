#include "TrackSetting.h"
#include "ui_TrackSetting.h"

TrackSetting::TrackSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrackSetting)
{
    ui->setupUi(this);
}

TrackSetting::~TrackSetting()
{
    delete ui;
}
