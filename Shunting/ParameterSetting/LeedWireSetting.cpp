#include "LeedWireSetting.h"
#include "ui_LeedWireSetting.h"

LeedWireSetting::LeedWireSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LeedWireSetting)
{
    ui->setupUi(this);
}

LeedWireSetting::~LeedWireSetting()
{
    delete ui;
}
