#include "ResolveRuleSetting.h"
#include "ui_ResolveRuleSetting.h"

ResolveRuleSetting::ResolveRuleSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResolveRuleSetting)
{
    ui->setupUi(this);
}

ResolveRuleSetting::~ResolveRuleSetting()
{
    delete ui;
}
