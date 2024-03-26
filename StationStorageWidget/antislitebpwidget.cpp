#include "antislitebpwidget.h"
#include "ui_antislitebpwidget.h"

AntisliTebpWidget::AntisliTebpWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AntisliTebpWidget)
{
    ui->setupUi(this);
}

AntisliTebpWidget::~AntisliTebpWidget()
{
    delete ui;
}
