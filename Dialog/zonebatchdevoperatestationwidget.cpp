#include "zonebatchdevoperatestationwidget.h"
#include "ui_zonebatchdevoperatestationwidget.h"
//#include <QPaintEvent>
ZoneBatchDevOperateStationWidget::ZoneBatchDevOperateStationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZoneBatchDevOperateStationWidget)
{
    ui->setupUi(this);
    installEventFilter(this);
    setMouseTracking(true);
    startTimer(500);
    InitMenu();
}

ZoneBatchDevOperateStationWidget::~ZoneBatchDevOperateStationWidget()
{
    delete ui;
}
void ZoneBatchDevOperateStationWidget::Init(MyStation* myStation)
{
    pMyStation = myStation;
    int width = pMyStation->StaConfigInfo.initialSize.width() * DIPLOID;
    int height = pMyStation->StaConfigInfo.initialSize.height() * DIPLOID;

    setFixedSize(1068 > width ? 1068 : width,
                 704 > height ? 704 : height);
}
void ZoneBatchDevOperateStationWidget::InitMenu()
{
    pMenu = new QMenu(this);
    pMenu->setStyleSheet("background-color: rgb(174,216,230);");

    QAction* Action1 = new QAction(pMenu);
    QAction* Action2 = new QAction(pMenu);
    QAction* Action3 = new QAction(pMenu);

    Action1->setText(QString("隐藏区段名称"));
    Action2->setText(QString("隐藏信号机名称"));
    Action3->setText(QString("隐藏道岔名称"));

    pMenu->addAction(Action1);
    pMenu->addAction(Action2);
    pMenu->addAction(Action3);

    connect(pMenu, &QMenu::triggered,this, [=](QAction *action){
        DevNameShowOrHide(action);
        if(action->text().contains("显示"))
        {
            action->setText(action->text().replace(QRegularExpression("显示"),"隐藏"));
        }
        else if(action->text().contains("隐藏"))
        {
            action->setText(action->text().replace(QRegularExpression("隐藏"),"显示"));
        }
    });
}
void ZoneBatchDevOperateStationWidget::magicTime()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen; //画笔
    pen.setColor(QColor(0, 0, 0));
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(QColor(255, 0, 0, 0)); //画刷
    painter.setPen(pen); //添加画笔
    painter.setBrush(brush); //添加画刷
    //显示站场
    if(pMyStation)
    {
        brush.setStyle(Qt::NoBrush);
        BaseRectDraw(&painter,1,QColor(220, 220, 220),QColor(5, 5, 120), SelectRect);
        brush.setStyle(Qt::SolidPattern);
        pMyStation->drawStation(&painter, nElapsed, DIPLOID, 0x55);
    }
    painter.end();
}
//初始化选中区域起始点坐标
void ZoneBatchDevOperateStationWidget::InitRectPoint()
{
     pointStart=QPoint(-1,-1);
     pointEnd=QPoint(-1,-1);
     pointEnd2=QPoint(-1,-1);
     SelectFlag=false;
     SelectRect=CountRectFun();
     this->update();
}
QRect ZoneBatchDevOperateStationWidget::CountRectFun()
{
    QTransform m_Transform;
    QRect mQRect;
    mQRect=m_Transform.inverted().mapRect(QRect(pointStart.x(),pointStart.y(),(pointEnd.x()-pointStart.x()),(pointEnd.y()-pointStart.y())));//坐标返回为缩小前的坐标

    return mQRect;
}
QRect ZoneBatchDevOperateStationWidget::CountRectFun(QPoint mpointEnd)
{
    QTransform m_Transform;
    QRect mQRect;
    mQRect=m_Transform.inverted().mapRect(QRect(pointStart.x(),pointStart.y(),(mpointEnd.x()-pointStart.x()),(mpointEnd.y()-pointStart.y())));//坐标返回为缩小前的坐标

    return mQRect;
}
void ZoneBatchDevOperateStationWidget::SelectDevFun()
{
    SelectRect = CountRectFun();
    QVector<DeviceInfo> vecDevData;
    GetSelectDev(SelectRect, &vecDevData);
    emit SendSelectDevsignal(vecDevData);
}
void ZoneBatchDevOperateStationWidget::GetSelectDev(QRect Rect, QVector<DeviceInfo>* vecDevData)
{
    CXHD *pXHD=nullptr;
    CGDDC *pCGDDC=nullptr;
    CGD *pGD=nullptr;
    for (int i=0;i<pMyStation->DevArray.size();i++)
    {
        if(pMyStation->DevArray[i]->getDevType() == Dev_XH)
        {
            pXHD=(CXHD*)(pMyStation->DevArray[i]);
            if((pXHD->pCenter.x()>Rect.x())&&(pXHD->pCenter.y()>Rect.y())&&(pXHD->pCenter.x()<(Rect.x()+Rect.width()))&&(pXHD->pCenter.y()<(Rect.y()+Rect.height())))
            {
                if(pXHD->isPLCZ == false)
                {
                    vecDevData->push_back(DeviceInfo(DEVTYPE_XH, pXHD->m_nCode, pXHD->m_strName));
                    pXHD->isPLCZ = true;
                }
            }
        }
        else if(pMyStation->DevArray[i]->getDevType() == Dev_DC)
        {
            pCGDDC=(CGDDC*)(pMyStation->DevArray[i]);
            if((pCGDDC->pCenter.x()>Rect.x())&&(pCGDDC->pCenter.y()>Rect.y())&&(pCGDDC->pCenter.x()<(Rect.x()+Rect.width()))&&(pCGDDC->pCenter.y()<(Rect.y()+Rect.height())))
            {
                if(pCGDDC->isPLCZ == false)
                {
                    vecDevData->push_back(DeviceInfo(DEVTYPE_DC, pCGDDC->m_nCode, pCGDDC->m_strName));
                    pCGDDC->isPLCZ = true;
                }
            }
        }
        else if(pMyStation->DevArray[i]->getDevType() == Dev_GD)
        {
             pGD=(CGD*)(pMyStation->DevArray[i]);
             if((pGD->pCenter.x()>Rect.x())&&(pGD->pCenter.y()>Rect.y())&&(pGD->pCenter.x()<(Rect.x()+Rect.width()))&&(pGD->pCenter.y()<(Rect.y()+Rect.height())))
             {
                 if(pGD->m_nGDType==JJ_QD)
                 {
                     if(pGD->isPLCZ == false)
                     {
                         vecDevData->push_back(DeviceInfo(DEVTYPE_QJ, pGD->m_nCode, pGD->m_strName));
                         pGD->isPLCZ = true;
                     }
                 }
                 else
                 {
                     if(pGD->isPLCZ == false)
                     {
                         vecDevData->push_back(DeviceInfo(DEVTYPE_QD, pGD->m_nCode, pGD->m_strName));
                         pGD->isPLCZ = true;
                     }
                 }
             }
        }
    }

}
void ZoneBatchDevOperateStationWidget::BaseRectDraw(QPainter *painter, int LineType,QColor LineColor,QColor BlackColor, QRect rect)
{
    QPen pen;
    QBrush brush;
    pen.setColor(LineColor);
    pen.setWidth(LineType);
    pen.setStyle(Qt::DashLine);

    painter->setPen(pen);//设置画笔形式
    //painter->setBrush(BlackColor);//设置画刷bai，如果不画实现的直du接把Brush设置为setBrush(Qt::NoBrush);
    painter->drawRect(rect);
}
void ZoneBatchDevOperateStationWidget::ClearSelectFun()
{
       this->update();
}
void ZoneBatchDevOperateStationWidget::setZCRefresh()
{
    CXHD *pXHD=nullptr;
    CGDDC *pGDDC=nullptr;
    CGD *pGD=nullptr;
    for (int i=0;i<pMyStation->DevArray.size();i++)
    {
        if(pMyStation->DevArray[i]->getDevType() == Dev_XH)
        {
            pXHD=(CXHD*)(pMyStation->DevArray[i]);
            pXHD->isPLCZ=false;
        }
        else if(pMyStation->DevArray[i]->getDevType() == Dev_DC)
        {
            pGDDC=(CGDDC*)(pMyStation->DevArray[i]);
            pGDDC->isPLCZ=false;
        }
        else if(pMyStation->DevArray[i]->getDevType() == Dev_GD)
        {
            pGD=(CGD*)(pMyStation->DevArray[i]);
            pGD->isPLCZ=false;
        }
    }

    this->update();
}
void ZoneBatchDevOperateStationWidget::timerEvent(QTimerEvent *event)//定时器
{
    if(pMyStation)
    {

    }

    update();
}
bool ZoneBatchDevOperateStationWidget::eventFilter(QObject *obj,QEvent *event)
{
    if(event->type() == QEvent::Paint)
    {
        magicTime();
    }

    if(event->type() == QEvent::MouseButtonPress)
    {
        if(((QMouseEvent*)event)->button() == Qt::LeftButton) //鼠标左键按下处理
        {
            if(SelectFlag)
            {
                 pointEnd= this->mapFromGlobal(QCursor::pos());
                 SelectRect=CountRectFun();
            }
            else
            {
                pointStart= this->mapFromGlobal(QCursor::pos());
                SelectFlag=true;
            }
        }
        if(((QMouseEvent*)event)->button() == Qt::RightButton) //鼠标右键按下处理
        {
            pMenu->exec(QCursor::pos());
            this->update();
        }
    }
    if(event->type() == QEvent::MouseButtonRelease)
    {
        if(((QMouseEvent*)event)->button() == Qt::LeftButton) //鼠标左键按下处理
        {
            pointEnd= this->mapFromGlobal(QCursor::pos());
            SelectDevFun();
            InitRectPoint();
            //update();
        }
    }
    if(event->type() == QEvent::MouseMove)
    {
        if(SelectFlag)
        {
            pointEnd= this->mapFromGlobal(QCursor::pos());
            SelectRect = CountRectFun();
            update();
        }
    }
    return true/*QWidget::eventFilter(obj, event)*/;
}
//鼠标按下
void ZoneBatchDevOperateStationWidget::mousePressEvent(QMouseEvent * event)
{
    if(event->button() == Qt::LeftButton) //鼠标左键按下处理
    {
        pointStart= this->mapFromGlobal(QCursor::pos());

    }
    else if(event->button() == Qt::RightButton)//鼠标右键按下处理
    {

    }
    return QWidget::mousePressEvent(event);
}

void ZoneBatchDevOperateStationWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        pointEnd= this->mapFromGlobal(QCursor::pos());
        SelectDevFun();
        InitRectPoint();
    }
    else if (event->button() == Qt::RightButton)
    {
     //   TextAnnotationcheck = nullptr;
    }
    return QWidget::mouseReleaseEvent(event);
}

void ZoneBatchDevOperateStationWidget::DevNameShowOrHide(QAction *action)
{
    if(action->text().contains("区段"))
    {
        bQDNameShowFlag=!bQDNameShowFlag;
        pMyStation->SetXHJANShow(10,bQDNameShowFlag);
        pMyStation->SetXHJANShow(11,bQDNameShowFlag);
        pMyStation->SetXHJANShow(12,bQDNameShowFlag);
        pMyStation->SetXHJANShow(14,bQDNameShowFlag);
    }
    else if(action->text().contains("信号"))
    {
        bXHNameShowFlag=!bXHNameShowFlag;
        pMyStation->SetXHJANShow(3,bXHNameShowFlag);
        pMyStation->SetXHJANShow(4,bXHNameShowFlag);
        pMyStation->SetXHJANShow(5,bXHNameShowFlag);
        pMyStation->SetXHJANShow(6,bXHNameShowFlag);
    }
    else if(action->text().contains("道岔"))
    {
        bDCNameShowFlag=!bDCNameShowFlag;
        pMyStation->SetXHJANShow(13,bDCNameShowFlag);
    }
}
