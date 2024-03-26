#include "zxbwindow.h"
#include "ui_zxbwindow.h"
#include "windows.h"
#include "windowsx.h"
#include "winUser.h"
#include <QGraphicsDropShadowEffect>
ZXBWindow::ZXBWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ZXBWindow)
{
    ui->setupUi(this);
    //取消窗口图标栏
    this->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
    ui->topWidget->installEventFilter(this);
    Init();
    ui->pushButton->hide();
    ui->pushButton_2->show();
}

ZXBWindow::~ZXBWindow()
{
    delete ui;
}
void ZXBWindow::Init()
{
    //股道视图窗口
    pTrackWidget = ui->widget;
    //进路视图窗口
    pRouteWidget = ui->widget_2;
    //防溜与站存车视图窗口
    pFLWidget = ui->widget_3;
    //调车视图窗口
    pShuntWidget = ui->widget_4;

    QSplitter *splitter = new QSplitter(Qt::Vertical, this);
    //splitter->setOpaqueResize(false);//手动到位并弹出鼠标之后再显示分割条
    splitter->addWidget(ui->scrollArea);
    splitter->addWidget(ui->horizontalWidget);
    splitter->setCollapsible(splitter->indexOf(ui->scrollArea), false);
    splitter->setCollapsible(splitter->indexOf(ui->horizontalWidget), false);
    splitter->setSizes(QList<int>() << 800 << 300);
    ui->centralwidget->layout()->addWidget(splitter);
    //ui->splitter->setSizes(QList<int>() <<800 << 320);
    //ui->splitter->setChildrenCollapsible(false);
}
void ZXBWindow::InitWindow(ZXBWindowCon* pZXBWindowCon,QVector<MyStation*>mMyStationArray)
{
    mZXBWindowCon=pZXBWindowCon;
    //股道视图、传递进路序列、防溜与站存车窗口配置
    if(mZXBWindowCon->nGDView == 0)
        pTrackWidget->hide();

    if(!mZXBWindowCon->bGDHV)
        ui->tittleLabel->hide();

    if(mZXBWindowCon->nJLView == 0)
        pRouteWidget->hide();

    if(mZXBWindowCon->nFLView == 0)
        pFLWidget->hide();

    if(mZXBWindowCon->nDCView == 0)
        pShuntWidget->hide();


    pTrackWidget->SetWindowCon(mZXBWindowCon,mMyStationArray);
    pRouteWidget->SetWindowCon(mZXBWindowCon,mMyStationArray);
    pFLWidget->SetWindowCon(mZXBWindowCon,mMyStationArray);
    pShuntWidget->SetWindowCon(mZXBWindowCon,mMyStationArray);

}

bool ZXBWindow::eventFilter(QObject *obj,QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        startPt = QCursor::pos();//相对于整个界面的全局坐标
        startWndPt = this->pos();
    }
    if(event->type() == QEvent::MouseButtonRelease)
    {
        startPt = { 0,0 };
    }
    if(event->type() == QEvent::MouseMove)
    {
        ui->pushButton_2->hide();
        ui->pushButton->show();
        this->showNormal();
        QPoint pt = QCursor::pos();//相对于整个界面的全局坐标
        this->setGeometry(startWndPt.x() + pt.x() - startPt.x(),
                          startWndPt.y() + pt.y() - startPt.y(),
                          this->geometry().width(), this->geometry().height());
    }
    if(event->type() == QEvent::Resize)
    {
        pTrackWidget->UpdataMinimumWidth();
    }
    return QWidget::eventFilter(obj, event);
}

void ZXBWindow::on_closeButton_clicked()
{
    this->hide();
}

bool ZXBWindow::nativeEvent(const QByteArray& eventType, void* message, long* result)
{
    MSG* msg = static_cast<MSG*>(message);
    if (msg->message == WM_NCHITTEST)
    {
        *result = 0;
        const int BORDER_WIDTH = 8; // 拉伸边框的宽度
        QRect winrect = this->geometry();
        //GetWindowRect(reinterpret_cast<HWND>(winId()), &winrect);
        long x = GET_X_LPARAM(msg->lParam);
        long y = GET_Y_LPARAM(msg->lParam);
        // 判断鼠标位置是否在拉伸区域内
        if (y < winrect.top() + BORDER_WIDTH) {
            *result = HTTOP;
        }
        if (y > winrect.bottom() - BORDER_WIDTH) {
            *result = HTBOTTOM;
        }
        if (x < winrect.left() + BORDER_WIDTH) {
            *result = HTLEFT;
        }
        if (x > winrect.right() - BORDER_WIDTH) {
            *result = HTRIGHT;
        }
        if (y < winrect.top() + BORDER_WIDTH && x < winrect.left() + BORDER_WIDTH) {
            *result = HTTOPLEFT;
        }
        if (y < winrect.top() + BORDER_WIDTH && x > winrect.right() - BORDER_WIDTH) {
            *result = HTTOPRIGHT;
        }
        if (y > winrect.bottom() - BORDER_WIDTH && x < winrect.left() + BORDER_WIDTH) {
            *result = HTBOTTOMLEFT;
        }
        if (y > winrect.bottom() - BORDER_WIDTH && x > winrect.right() - BORDER_WIDTH) {
            *result = HTBOTTOMRIGHT;
        }
        if (*result != 0) {
            return true;
        }
    }
    return QWidget::nativeEvent(eventType, message, result);
}

void ZXBWindow::on_pushButton_2_clicked()
{
    ui->pushButton_2->hide();
    ui->pushButton->show();
    this->showNormal();
}

void ZXBWindow::on_pushButton_clicked()
{
    ui->pushButton->hide();
    ui->pushButton_2->show();
    this->showMaximized();
}
