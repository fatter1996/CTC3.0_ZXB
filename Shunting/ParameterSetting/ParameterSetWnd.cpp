#include "ParameterSetWnd.h"
#include "ui_ParameterSetWnd.h"
#include <QDebug>

#include "DirectionSetting.h"
#include "PriorRouteSetting.h"
#include "TrackSetting.h"
#include "LeedWireSetting.h"
#include "ResolveRuleSetting.h"
ParameterSetWnd::ParameterSetWnd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParameterSetWnd)
{
    ui->setupUi(this);
}

ParameterSetWnd::~ParameterSetWnd()
{
    delete ui;
}

void ParameterSetWnd::addTabWidget(QWidget* _tebWid, QString wndName)
{
    ui->tabWidget->addTab(_tebWid, wndName);
    this->setFixedSize(_tebWid->width() + 24, _tebWid->height() + 108);
}

void ParameterSetWnd::setDirectionSetting()
{
    DirectionSetting* direction = new DirectionSetting;
    addTabWidget(direction, QString("机车与方向"));
}

void ParameterSetWnd::setPrioritySetting()
{
    PriorRouteSetting* priorRoute = new PriorRouteSetting;
    LeedWireSetting* leedWire = new LeedWireSetting;
    addTabWidget(priorRoute, QString("优先进路"));
    addTabWidget(leedWire, QString("优先牵出线"));
}

void ParameterSetWnd::setRuleSetting()
{
    ResolveRuleSetting* resolveRule = new ResolveRuleSetting;
    TrackSetting* track = new TrackSetting;
    addTabWidget(resolveRule, QString("勾分解规则"));
    addTabWidget(track, QString("股道"));
}
