#include "mainwindow.h"

#include <QApplication>
#include "MyDoc/mydoc.h"
#include <QCoreApplication>
#include <QLockFile>
#include <QMessageBox>

bool processIsRunning()
{
    bool isRunning = false;
    //方法：文件锁判定程序是否已启动
    //判定同目录、同名称的程序是否在运行
    QString appFilePath = QCoreApplication::applicationFilePath();//qApp->applicationFilePath();
    QString appName = qApp->applicationName();//+".exe";
    QString lockFilePath;
    lockFilePath = appFilePath+".singleApp.lock";
    QLockFile *lockFile = new QLockFile(lockFilePath);
    if(!lockFile->tryLock(1000))
    {
        QMessageBox::warning(NULL, "警告", "程序已经在运行！"+appName);
        isRunning = true;
    }
    else
    {
        isRunning = false;
    }
    return isRunning;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //重复启动判断
    if(processIsRunning())
    {
        return -1;
    }

    MyDoc* pDoc = new MyDoc();
    if(pDoc->vectMyStation.size()<=0)
    {
        QString strMsg = "没有读取到正确的车站信息！";
        //QLOG_ERROR() << strMsg;
        QMessageBox::warning(NULL, "错误", strMsg);
        //没有车站数据则退出
        a.exit();
        return 0;
    }

    QIcon icon("T3.ico");
    QApplication::setWindowIcon(icon);
    //界面初始化
    MainWindow w;
    w.setWindowFlags(Qt::Widget|Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
    w.InitStationList(pDoc->vectMyStation);
    w.InitZoneBatchDevOperate();
    w.move(800, 0);
    w.show();

    ZXBWindow* pZXBWindow = w.InitZXBWindow(&(pDoc->mZXBWindowCon),pDoc->vectMyStation);
    pDoc->InitMainWnd(&w);
    pDoc->InitZXBMainWnd(pZXBWindow);
    //初始化供电臂设置窗口
    pDoc->InitPowerSupplyArmDialog();
    //初始化封锁设备设置窗口
    pDoc->InitBlockingDevDialog();




    return a.exec();
}
