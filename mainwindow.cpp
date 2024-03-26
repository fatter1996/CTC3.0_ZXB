#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Dialog/zonebatchdevoperatedialog.h"
//#include "comm.h"
//#include "Global.h"

//添加线程
#include <QThread>
#include <QDebug>
//添加定时器
#include <QTimerEvent>
//添加对话框组
#include <QDialog>
#include <QMessageBox>
#include <QPainter>
#include <complex>
#include "math.h"
#include <QDateTime>

#include <QSplitter>//分割窗口类头文件
#include <QTextEdit>
#include <QDockWidget>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <Windows.h>
#include <QPushButton>
#include <QLabel>
#include <QToolBar>
#include <QLineEdit>
#include <QMenuBar>
#include <QMenu>
#include <QScrollArea>
#include <QStandardItemModel>
#include <QIcon>
#include <QLayout>
#include <QTextToSpeech>
#include <QTableWidget>
#include <QHeaderView>
#include <QStringList>
#include <QComboBox>

//#define Version  V1.2.1
#define COMMUNICATION_TIPS "未检测到有效的服务端和联锁通信，无法操作！"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Init();
    Menu_SetButton();
    Menu_T3Button();
    SplitWindow();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::Init()
{
    LoginID="";
    LoginName="";
    TrackBMP_JLNum=0;
    LoginState=1;
    for (int i=0;i<300;i++)
    {
        BMPArray[i].JLIndex=0;
        BMPArray[i].JLType="";
        BMPArray[i].JLPushButton="";
        BMPArray[i].JLBMPPath="";
    }
    m_nStationGDNode=0;
    for (int i=0;i<50;i++)
    {
        StationGDNode[i].nCode=0;
        StationGDNode[i].strGDName="";
        StationGDNode[i].strLeftNode="";
        StationGDNode[i].strRightNode="";
    }

    JDJHQSState=false;
    JDJHQSState_Flag=false;
    DDMLQSState=false;
    DDMLQSState_Flag=false;
    pMemorandumDialog=new MemorandumDialog();
    connect(pMemorandumDialog, SIGNAL(SendBtnIncoChangeSignal(bool)), this, SLOT(SendBtnIncoChangeSolt(bool)));
    connect(pMemorandumDialog, &MemorandumDialog::SendTextAnalysisSignal,[=](int index, QPoint pt, QString dev){
        this->SendTextAnalysisSignal(index, pt, dev);
    });

}

/***************************************************************/
ZXBWindow* MainWindow::InitZXBWindow(ZXBWindowCon* pZXBWindowCon, QVector<MyStation*>mMyStationArray)
{
    pZXBWindow = new ZXBWindow();
    pZXBWindow->InitWindow(pZXBWindowCon, mMyStationArray);
    if(GetServiceCommFlag())
    {
        pZXBWindow->showMaximized();
        pZXBWindow->move(0, 0);
        pZXBWindow->show();
        pZXBWindow->activateWindow();
    }
    else timerId_500 = startTimer(500);
    return pZXBWindow;
}
//按钮窗口
void MainWindow::SplitWindow()
{
    int ButtonLenth=0;
    int ButtonSide=0;

    this->setStyleSheet("QWidget#centralwidget{ background-color: rgb(150, 150, 150);}");

    //按钮2：
    ButtonSide=ButtonSide+4+ButtonLenth;
    ButtonLenth=34;
    QPushButton *btn2 = new QPushButton();
    btn2->setParent(this);
    //this->layout()->addWidget(btn2);
    btn2->setGeometry(ButtonSide,2,ButtonLenth,34);
    QIcon icon2;
    icon2.addFile("://res/T3.png");
    btn2->setIcon(icon2);
    btn2->setIconSize(QSize(25,25));
    btn2->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
                        "QPushButton{border:1px groove gray;border-radius:4px;padding:1px 2px;border-style:outset;}"
                        "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
                        "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
                        "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}"
                        "QPushButton::menu-indicator{image:none;width:0px;}");
    btn2->setMenu(m_menu_T3);
    connect(btn2, &QPushButton::clicked, this, &MainWindow::clicked_T3);


    //按钮5：占线版
    ButtonSide=ButtonSide+6+ButtonLenth;
    ButtonLenth=34;
    QPushButton *btn5 = new QPushButton();
    btn5->setParent(this);
    //this->layout()->addWidget(btn5);
    btn5->setGeometry(ButtonSide,2,ButtonLenth,34);
    QIcon icon5;
    icon5.addFile("://res/1.png");
    btn5->setIcon(icon5);
    btn5->setIconSize(QSize(25,25));
    btn5->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
                        "QPushButton{border:1px groove gray;border-radius:4px;padding:1px 2px;border-style:outset;}"
                        "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
                        "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
                        "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}"
                        );

    connect(btn5, &QPushButton::clicked, this, &MainWindow::clicked_btn5_openmainWindowForStationTrack);

    //按钮3：调车计划
    ButtonSide=ButtonSide+4+ButtonLenth;
    ButtonLenth=34;
    QPushButton *btn3 = new QPushButton();
    btn3->setParent(this);
    //this->layout()->addWidget(btn3);
    btn3->setGeometry(ButtonSide,2,ButtonLenth,34);
    QIcon icon3;
    icon3.addFile("://res/火车.png");
    btn3->setIcon(icon3);
    btn3->setIconSize(QSize(25,25));
    btn3->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
                        "QPushButton{border:1px groove gray;border-radius:4px;padding:1px 2px;border-style:outset;}"
                        "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
                        "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
                        "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}"
                        "QPushButton::menu-indicator{image:none;width:0px;}");


    //按钮6：网络连接状态
    ButtonSide=ButtonSide+6+ButtonLenth;
    ButtonLenth=34;
    //if(GetServiceCommFlag())
    {
        btn_Net = new QPushButton();
        btn_Net->setParent(this);
        //this->layout()->addWidget(btn_Net);
        btn_Net->setGeometry(ButtonSide,2,ButtonLenth,34);        
        QIcon icon6;
        icon6.addFile("://res/断开.png");
        btn_Net->setIcon(icon6);
        btn_Net->setIconSize(QSize(25,25));
        btn_Net->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
                            "QPushButton{border:1px groove gray;border-radius:4px;padding:1px 2px;border-style:outset;}"
                            "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
                            "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
                            "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}"
                            );
        connect(btn_Net, &QPushButton::clicked, this, &MainWindow::Dialog_NetStatButton);
    }

    //按钮7：备忘提醒功能
    ButtonSide=ButtonSide+6+ButtonLenth;
    ButtonLenth=34;

    btn_Memorandum = new QPushButton();
    btn_Memorandum->setParent(this);
    //this->layout()->addWidget(btn_Memorandum);
    btn_Memorandum->move(185,5);
    btn_Memorandum->setGeometry(ButtonSide,2,ButtonLenth,34);

//    QPushButton *btn7 = new QPushButton();
//    btn7->setParent(this);
//    //this->layout()->addWidget(btn7);
//    btn7->move(185,5);
//    btn7->setGeometry(ButtonSide,2,ButtonLenth,34);

    QIcon icon7;
    icon7.addFile("://res/9.png");
    btn_Memorandum->setIcon(icon7);
    btn_Memorandum->setIconSize(QSize(25,25));
    btn_Memorandum->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
                        "QPushButton{border:1px groove gray;border-radius:4px;padding:1px 2px;border-style:outset;}"
                        "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
                        "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
                        "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}"
                        );
    connect(btn_Memorandum, &QPushButton::clicked, this, &MainWindow::clicked_btn7_openBWL);

    //登录按钮：
    ButtonSide=ButtonSide+6+ButtonLenth;
    ButtonLenth=400;
    btn_Enter = new QPushButton(this);//登录按钮
    //this->layout()->addWidget(btn_Enter);
    btn_Enter->setGeometry(ButtonSide,4,ButtonLenth,30);
    btn_Enter->setText("**值班员A - 未登录");
    btn_Enter->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
                             "QPushButton{border:1px groove gray;border-radius:4px;padding:2px 4px;border-style:outset;}"
                             "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
                             "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
                             "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}");
    connect(btn_Enter, &QPushButton::clicked, this, &MainWindow::Dialog_LoginButton);

    if(0)
    {
        //按钮9：
        ButtonSide=ButtonSide+6+ButtonLenth;
        ButtonLenth=34;
        btn9 = new QPushButton();
        btn9->setParent(this);
        btn9->setGeometry(ButtonSide,2,ButtonLenth,34);
        QIcon icon9;
        icon9.addFile("://res/签.png");
        btn9->setIcon(icon9);
        btn9->setIconSize(QSize(25,25));
        btn9->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
                            "QPushButton{border:1px groove gray;border-radius:4px;padding:1px 2px;border-style:outset;}"
                            "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
                            "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
                            "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}"
                            );
        connect(btn9, &QPushButton::clicked, this, &MainWindow::clicked_btn9_openJSJDJH);

        //按钮10：
        ButtonSide=ButtonSide+6+ButtonLenth;
        ButtonLenth=34;
        btn10 = new QPushButton();
        btn10->setParent(this);
        btn10->setGeometry(ButtonSide,2,ButtonLenth,34);
        QIcon icon10;
        icon10.addFile("://res/令.png");
        btn10->setIcon(icon10);
        btn10->setIconSize(QSize(25,25));
        btn10->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
                             "QPushButton{border:1px groove gray;border-radius:4px;padding:1px 2px;border-style:outset;}"
                             "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
                             "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
                             "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}"
                             );
        connect(btn10, &QPushButton::clicked, this, &MainWindow::clicked_btn10_openDDMLCL);
    }
    //按钮11：设置
    ButtonSide=ButtonSide+6+ButtonLenth;
    ButtonLenth=34;
    QPushButton *btn11 = new QPushButton();
    btn11->setParent(this);
    //this->layout()->addWidget(btn11);
    btn11->setGeometry(ButtonSide,2,ButtonLenth,34);
    QIcon icon11;
    icon11.addFile("://res/设置.png");
    btn11->setIcon(icon11);
    btn11->setIconSize(QSize(25,25));
    btn11->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
                         "QPushButton{border:1px groove gray;border-radius:4px;padding:1px 2px;border-style:outset;}"
                         "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
                         "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
                         "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}"
                         "QPushButton::menu-indicator{image:none;width:0px;}");
    btn11->setMenu(m_menu_Set);


    //按钮12：关闭
    ButtonSide=ButtonSide+6+ButtonLenth;
    ButtonLenth=34;
    QPushButton *btn12 = new QPushButton();
    btn12->setParent(this);
    //this->layout()->addWidget(btn12);
    btn12->setGeometry(ButtonSide,2,ButtonLenth,34);
    QIcon icon12;
    icon12.addFile("://res/关机.png");
    btn12->setIcon(icon12);
    btn12->setIconSize(QSize(25,25));
    btn12->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
                         "QPushButton{border:1px groove gray;border-radius:4px;padding:1px 2px;border-style:outset;}"
                         "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
                         "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
                         "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}"
                         );
    connect(btn12, &QPushButton::clicked, this, &MainWindow::clicked_btn12_KeyDialogbox);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)//移动工具条窗口//QT5实战项目视频
{
    QWidget::mouseMoveEvent(event);
    QPoint y=event->globalPos();//鼠标相对于桌面左上角的位置，鼠标的全局位置
    QPoint x=y-this->z;
    this->move(x);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    QPoint x=this->geometry().topLeft();//窗口左上角相对于桌面左上角的位置，窗口位置
    QPoint y=event->globalPos();//鼠标相对于桌面左上角的位置，鼠标的全局位置
    this->z=y-x;//定值//z是鼠标相对于窗口的位置
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    this->z=QPoint();
}
/***************************************************************/

/***************************************************************/
void MainWindow::KeyDialogbox(QString str)//弹出对话框
{
    QMessageBox *msgBox = new QMessageBox(this);
    msgBox->setWindowTitle("提示：");
    msgBox->setText(str);
    msgBox->setStyleSheet("background-color: rgb(240,240,240);");
    QPushButton *btn_sure = msgBox->addButton("确定", QMessageBox::AcceptRole);
    QPushButton *bun_cancel = msgBox->addButton("取消", QMessageBox::RejectRole);
    btn_sure->setStyleSheet("background-color: rgb(220,220,220);");
    bun_cancel->setStyleSheet("background-color: rgb(220,220,220);");
    msgBox->move(800,300);
    msgBox->show();
    connect(btn_sure, &QPushButton::clicked, this, &MainWindow::clicked_btn12_Close);

}

void MainWindow::AlarmSpeech(QString str)//语音告警
{
    if(GetServiceCommFlag())
    {
        QTextToSpeech *tts = new QTextToSpeech(this);
        tts->setLocale(QLocale::Chinese);//设置语言环境
        tts->setRate(0.0);//设置语速-1.0到1.0
        tts->setPitch(1.0);//设置音高-1.0到1.0
        tts->setVolume(1.0);//设置音量0.0-1.0
        if(tts->state()==QTextToSpeech::Ready)
        {
            tts->say("警告!"+str);
        }
    }
    else
    {
        QMessageBox::critical(this, tr("错误"), tr("未检测到有效的服务端和联锁通信，无法操作！"),tr("确定"),"",0);
    }
}
/***************************************************************/

/***************************************************************/
void MainWindow::Menu_SetButton()
{
    m_menu_Set=new QMenu(this);
    m_menu_Set->setStyleSheet("background-color: rgb(204, 228, 247);");
    m_ZXBST_Set=new QAction(m_menu_Set);//占线板视图设置
    m_GDSTXS_Set=new QAction(m_menu_Set);//股道视图显示设置
    m_FXXS_Set=new QAction(m_menu_Set);//方向显示设置
    m_DCCS_Set=new QAction(m_menu_Set);//调车参数配置
    m_DCYXJ_Set=new QAction(m_menu_Set);//调车优先级配置
    m_DCGZ_Set=new QAction(m_menu_Set);//调车规则配置
    m_GNAN_Set=new QAction(m_menu_Set);//功能按钮设置
    m_ZCJMXS_Set=new QAction(m_menu_Set);//站场界面显示设置
    m_BJTS_Set=new QAction(m_menu_Set);//报警提示配置
    m_Version=new QAction(m_menu_Set);//版本信息

    m_ZXBST_Set->setText(QObject::tr("占线板视图设置"));
    m_GDSTXS_Set->setText(QObject::tr("股道视图显示设置"));
    m_FXXS_Set->setText(QObject::tr("方向显示设置"));
    m_DCCS_Set->setText(QObject::tr("调车参数配置"));
    m_DCYXJ_Set->setText(QObject::tr("调车优先级配置"));
    m_DCGZ_Set->setText(QObject::tr("调车规则配置"));
    m_GNAN_Set->setText(QObject::tr("功能按钮设置"));
    m_ZCJMXS_Set->setText(QObject::tr("站场界面显示设置"));
    m_BJTS_Set->setText(QObject::tr("报警提示配置"));
    m_Version->setText(QObject::tr("版本信息"));

    m_menu_Set->addAction(m_ZXBST_Set);
    m_menu_Set->addAction(m_GDSTXS_Set);
    m_menu_Set->addAction(m_FXXS_Set);
    m_menu_Set->addAction(m_DCCS_Set);
    m_menu_Set->addAction(m_DCYXJ_Set);
    m_menu_Set->addAction(m_DCGZ_Set);
    m_menu_Set->addAction(m_GNAN_Set);
    m_menu_Set->addAction(m_ZCJMXS_Set);
    m_menu_Set->addAction(m_BJTS_Set);
    m_menu_Set->addAction(m_Version);
    connect(m_ZXBST_Set, &QAction::triggered, this, &MainWindow::ZXBViewSetFun);
    connect(m_GDSTXS_Set, &QAction::triggered, this, &MainWindow::GDSTViewSetFun);
    connect(m_BJTS_Set, &QAction::triggered, this, &MainWindow::BJTSSetFun);

//    QMenu *m_menu_FX;//方向下拉菜单
//    QAction *m_addAction_Station1;
//    QAction *m_addAction_Station2;
//    m_menu_FX = new QMenu(this);
//    m_menu_FX->setStyleSheet("background-color: rgb(204, 228, 247);");
//    m_addAction_Station1 = new QAction(m_menu_FX);
//    m_addAction_Station2 = new QAction(m_menu_FX);
//    m_addAction_Station1->setText(QObject::tr("111"));
//    m_addAction_Station2->setText(QObject::tr("222"));
//    m_menu_FX->addAction(m_addAction_Station1);
//    m_menu_FX->addAction(m_addAction_Station2);
//    connect(m_addAction_Station1, &QAction::triggered, this, &MainWindow::StationDirectionFun);
//    connect(m_addAction_Station2, &QAction::triggered, this, &MainWindow::StationDirectionFun);
//    m_FXXS_Set->setMenu(m_menu_FX);

//    QMenu *m_menu_DCCSPZ;//调车参数配置下拉菜单
//    QAction *m_addAction_1;
//    QAction *m_addAction_2;
//    m_menu_DCCSPZ = new QMenu(this);
//    m_menu_DCCSPZ->setStyleSheet("background-color: rgb(204, 228, 247);");
//    m_addAction_1 = new QAction(m_menu_DCCSPZ);
//    m_addAction_2 = new QAction(m_menu_DCCSPZ);
//    m_addAction_1->setText(QObject::tr("1"));
//    m_addAction_2->setText(QObject::tr("2"));
//    m_menu_DCCSPZ->addAction(m_addAction_1);
//    m_menu_DCCSPZ->addAction(m_addAction_2);
////    connect(m_addAction, &QAction::triggered, this, &ButtonMenu::addFunc);
////    connect(m_delAction, &QAction::triggered, this, &ButtonMenu::delFunc);
//    m_DCCS_Set->setMenu(m_menu_DCCSPZ);

//    QMenu *m_menu_DCYXJPZ;//调车优先级配置下拉菜单
//    QAction *m_addAction_11;
//    QAction *m_addAction_22;
//    m_menu_DCYXJPZ = new QMenu(this);
//    m_menu_DCYXJPZ->setStyleSheet("background-color: rgb(204, 228, 247);");
//    m_addAction_11 = new QAction(m_menu_DCYXJPZ);
//    m_addAction_22 = new QAction(m_menu_DCYXJPZ);
//    m_addAction_11->setText(QObject::tr("11"));
//    m_addAction_22->setText(QObject::tr("22"));
//    m_menu_DCYXJPZ->addAction(m_addAction_11);
//    m_menu_DCYXJPZ->addAction(m_addAction_22);
////    connect(m_addAction, &QAction::triggered, this, &ButtonMenu::addFunc);
////    connect(m_delAction, &QAction::triggered, this, &ButtonMenu::delFunc);
//    m_DCYXJ_Set->setMenu(m_menu_DCYXJPZ);

//    QMenu *m_menu_DCGZPZ;//调车规则配置下拉菜单
//    QAction *m_addAction_111;
//    QAction *m_addAction_222;
//    m_menu_DCGZPZ = new QMenu(this);
//    m_menu_DCGZPZ->setStyleSheet("background-color: rgb(204, 228, 247);");
//    m_addAction_111 = new QAction(m_menu_DCGZPZ);
//    m_addAction_222 = new QAction(m_menu_DCGZPZ);
//    m_addAction_111->setText(QObject::tr("111"));
//    m_addAction_222->setText(QObject::tr("222"));
//    m_menu_DCGZPZ->addAction(m_addAction_111);
//    m_menu_DCGZPZ->addAction(m_addAction_222);
////    connect(m_addAction, &QAction::triggered, this, &ButtonMenu::addFunc);
////    connect(m_delAction, &QAction::triggered, this, &ButtonMenu::delFunc);
//    m_DCGZ_Set->setMenu(m_menu_DCGZPZ);

//    QMenu *m_menu_GNAN;//功能按钮设置下拉菜单
//    QAction *m_addAction_FunSetStation1;
//    QAction *m_addAction_FunSetStation2;
//    m_menu_GNAN = new QMenu(this);
//    m_menu_GNAN->setStyleSheet("background-color: rgb(204, 228, 247);");
//    m_addAction_FunSetStation1 = new QAction(m_menu_GNAN);
//    m_addAction_FunSetStation2 = new QAction(m_menu_GNAN);
//    m_addAction_FunSetStation1->setText(QObject::tr("1111"));
//    m_addAction_FunSetStation2->setText(QObject::tr("2222"));
//    m_menu_GNAN->addAction(m_addAction_FunSetStation1);
//    m_menu_GNAN->addAction(m_addAction_FunSetStation2);
//    connect(m_addAction_FunSetStation1, &QAction::triggered, this, &MainWindow::FunctionPushButtonFun);
//    connect(m_addAction_FunSetStation2, &QAction::triggered, this, &MainWindow::FunctionPushButtonFun);
//    m_GNAN_Set->setMenu(m_menu_GNAN);
}
/***************************************************************/


/***************************************************************/
void MainWindow::Menu_T3Button()
{
    m_menu_T3=new QMenu(this);//T3按钮下拉菜单
    m_menu_T3->setStyleSheet("background-color: rgb(204, 228, 247);");

    m_GDBState_Set=new QAction(m_menu_T3);//设置供电臂状态
    m_FSSBCZ=new QAction(m_menu_T3);//封锁设备操作
    m_QYPLSBCZ=new QAction(m_menu_T3);//区域批量设备操作
    //m_ZCCGL=new QAction(m_menu_T3);//站存车管理
    //m_SBYXFX=new QAction(m_menu_T3);//设备影响分析
    m_CZRZCX=new QAction(m_menu_T3);//操作日志查询
    m_FLSZCX=new QAction(m_menu_T3);//防溜设置查询
    m_XXTS=new QAction(m_menu_T3);//信息提示

    m_GDBState_Set->setText(QObject::tr("设置供电臂状态"));
    m_FSSBCZ->setText(QObject::tr("封锁设备操作"));
    m_QYPLSBCZ->setText(QObject::tr("区域批量设备操作"));
    //m_ZCCGL->setText(QObject::tr("站存车管理"));
    //m_SBYXFX->setText(QObject::tr("设备影响分析"));
    m_CZRZCX->setText(QObject::tr("操作日志查询"));
    m_FLSZCX->setText(QObject::tr("防溜设置查询"));
    m_XXTS->setText(QObject::tr("信息提示"));

    m_menu_T3->addAction(m_GDBState_Set);
    m_menu_T3->addAction(m_FSSBCZ);
    m_menu_T3->addAction(m_QYPLSBCZ);
    //m_menu_T3->addAction(m_ZCCGL);
    //m_menu_T3->addAction(m_SBYXFX);
    m_menu_T3->addAction(m_CZRZCX);
    m_menu_T3->addAction(m_FLSZCX);
    m_menu_T3->addAction(m_XXTS);
    //绑定菜单选项对应信号槽
    //设置供电臂状态
    connect(m_GDBState_Set, &QAction::triggered, this, &MainWindow::PowerSupplyArmDialogSolt);
    //封锁设备操作
    connect(m_FSSBCZ, &QAction::triggered, this, &MainWindow::BlockingDevDialogSolt);
//    //区域批量设备操作
//    connect(m_QYPLSBCZ, &QAction::triggered, this, &MainWindow::ZoneBatchDevOperateDialogSolt);

    QMenu *m_menu_ZCCGL;//站存车管理下拉菜单
    QAction *m_addAction_33;
    QAction *m_addAction_44;
    m_menu_ZCCGL = new QMenu(this);
    m_menu_ZCCGL->setStyleSheet("background-color: rgb(204, 228, 247);");

    QMenu *m_menu_SBYXFX;//设备影响分析下拉菜单
    QAction *m_addAction_333;
    QAction *m_addAction_444;
    m_menu_SBYXFX = new QMenu(this);
    m_menu_SBYXFX->setStyleSheet("background-color: rgb(204, 228, 247);");


}
/***************************************************************/

/***************************************************************/
void MainWindow::Dialog_NetStatButton()
{
    emit SendTextAnalysisSignal(5, QCursor::pos());
    if(GetServiceCommFlag())
    {
        QDialog *dlg=new QDialog(this);
        dlg->setGeometry(700,300,400,250);
        //dlg->setStyleSheet("background-color: rgb(220, 220, 220);");
        dlg->setMinimumSize(QSize(400,250));
        dlg->setMaximumSize(QSize(400,250));
        dlg->move(760,250);
        dlg->setAttribute(Qt::WA_DeleteOnClose);
        QTableWidget *tableWidget = new QTableWidget(dlg);
        tableWidget->setGeometry(5,10,389,200);
        tableWidget->setColumnCount(3);
        tableWidget->setRowCount(6);
        tableWidget->setStyleSheet("background-color: rgb(220, 220, 220);");
        tableWidget->setFont(QFont("宋体",9,50));
        tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: rgb(200,200,200);}");//设置表头颜色
        tableWidget->setAlternatingRowColors(true);//设置相邻行颜色交替显示
        tableWidget->verticalHeader()->setVisible(false);//垂直表头不显示
        tableWidget->verticalHeader()->setDefaultSectionSize(10);//设置行高
        tableWidget->horizontalHeader()->setDefaultSectionSize(129);//设置列宽

        QStringList m_List_State;
        m_List_State << "名称";
        m_List_State << "状态";
        m_List_State << "状态描述";
        tableWidget->setHorizontalHeaderLabels(m_List_State);
        QStringList m_List_Item;
        m_List_Item << "Commtask0"<<"1"<<"正常";
        m_List_Item << "Commtask1"<<"1"<<"正常";
        m_List_Item << "**站变量服务"<<"1"<<"正常";
        m_List_Item << "**站自律机"<<"1"<<"正常";
        m_List_Item << "**站RT"<<"1"<<"正常";
        m_List_Item << "中心shuntsvr"<<"1"<<"正常";
        for (int i=0;i<6;i++)
        {
            int col=0;
            tableWidget->setItem(i,col++,new QTableWidgetItem(m_List_Item[i*3+0]));//记住使用中括号
            tableWidget->setItem(i,col++,new QTableWidgetItem(m_List_Item[i*3+1]));//记住使用中括号
            tableWidget->setItem(i,col++,new QTableWidgetItem(m_List_Item[i*3+2]));//记住使用中括号
        }


        QPushButton *colseButton = new QPushButton(dlg);
        colseButton->setGeometry(250,215,50,25);
        colseButton->setText("关闭");
        colseButton->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
                                   "QPushButton{border:1px groove gray;border-radius:4px;padding:1px 2px;border-style:outset;}"
                                   "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
                                   "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
                                   "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}"
                                   );
        connect(colseButton, SIGNAL(clicked()), dlg, SLOT(close()));
        connect(colseButton, &QPushButton::clicked, [=](){
            emit SendTextAnalysisSignal(8, QCursor::pos());
        });

        QPushButton *refreshButton = new QPushButton(dlg);
        refreshButton->setGeometry(100,215,50,25);
        refreshButton->setText("刷新");
        refreshButton->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
                                     "QPushButton{border:1px groove gray;border-radius:4px;padding:1px 2px;border-style:outset;}"
                                     "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
                                     "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
                                     "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}"
                                     );
        connect(refreshButton, &QPushButton::clicked, [=](){
            emit SendTextAnalysisSignal(156, QCursor::pos());
        });
        dlg->exec();
    }
    else
    {
        QMessageBox::critical(this, tr("错误"), tr(COMMUNICATION_TIPS),tr("确定"),"",0);
    }
}
/***************************************************************/

/***************************************************************/
void MainWindow::Dialog_LoginButton()
{
    emit SendTextAnalysisSignal(7, QCursor::pos());
    if(GetServiceCommFlag())
    {
        QDialog *dlg=new QDialog(this);
        dlg->setGeometry(850,580,250,150);
        dlg->setMinimumSize(QSize(250,150));
        dlg->setMaximumSize(QSize(250,150));
        dlg->setAttribute(Qt::WA_DeleteOnClose);
        dlg->move(800,300);

        QLabel *label_ID=new QLabel(dlg);
        QLabel *label_Name=new QLabel(dlg);
        label_ID->setGeometry(20,30,50,25);
        label_ID->setText("用户 ID");
        label_Name->setGeometry(20,65,50,25);
        label_Name->setText("用户名称");

        comboBox_ID=new QComboBox(dlg);
        comboBox_ID->setGeometry(80,30,160,25);
        comboBox_ID->setStyleSheet("background-color: rgb(240, 240, 240);");
        comboBox_ID->clear();
        comboBox_ID->setEditable(true);
        QStringList strList_ID;
        strList_ID<<"1";
        comboBox_ID->addItems(strList_ID);

        comboBox_Name=new QComboBox(dlg);
        comboBox_Name->setGeometry(80,65,160,25);
        comboBox_Name->setStyleSheet("background-color: rgb(240, 240, 240);");
        comboBox_Name->clear();
        comboBox_Name->setEditable(true);
        QStringList strList_Name;
        strList_Name<<"用户1";
        comboBox_Name->addItems(strList_Name);


        QPushButton *LoginButton = new QPushButton(dlg);
        LoginButton->setGeometry(75,110,50,25);
        LoginButton->setText("登录");
        LoginButton->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
                                   "QPushButton{border:1px groove gray;border-radius:4px;padding:1px 2px;border-style:outset;}"
                                   "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
                                   "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
                                   "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}"
                                   );

        connect(LoginButton, &QPushButton::clicked, this, &MainWindow::clicked_LoginButton);
        connect(LoginButton, SIGNAL(clicked()), dlg, SLOT(close()));

        QPushButton *LogoutButton = new QPushButton(dlg);
        LogoutButton->setGeometry(130,110,50,25);
        LogoutButton->setText("注销");
        LogoutButton->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
                                    "QPushButton{border:1px groove gray;border-radius:4px;padding:1px 2px;border-style:outset;}"
                                    "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
                                    "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
                                    "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}"
                                    );

        connect(LogoutButton, &QPushButton::clicked, this, &MainWindow::clicked_LogoutButton);
        connect(LogoutButton, SIGNAL(clicked()), dlg, SLOT(close()));

        QPushButton *colseButton = new QPushButton(dlg);
        colseButton->setGeometry(185,110,50,25);
        colseButton->setText("关闭");
        colseButton->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
                                   "QPushButton{border:1px groove gray;border-radius:4px;padding:1px 2px;border-style:outset;}"
                                   "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
                                   "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
                                   "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}"
                                   );

        connect(colseButton, &QPushButton::clicked, [=](){
            emit SendTextAnalysisSignal(8, QCursor::pos());
        });
        connect(colseButton, SIGNAL(clicked()), dlg, SLOT(close()));

        dlg->exec();
    }
    else
    {
        QMessageBox::critical(this, tr("错误"), tr(COMMUNICATION_TIPS),tr("确定"),"",0);
    }
}
/***************************************************************/

/***************************************************************/
//占线板视图设置功能：
void MainWindow::ZXBViewSetFun()
{
    if(GetServiceCommFlag())
    {
//        ZXBViewSet *zxbViewSet = new ZXBViewSet(this);
//        zxbViewSet->setWindowModality(Qt::WindowModal);//设置模态
//        zxbViewSet->setAttribute(Qt::WA_DeleteOnClose);//关闭后自动释放窗口
//        zxbViewSet->show();
    }
    else
    {
        QMessageBox::critical(this, tr("错误"), tr(COMMUNICATION_TIPS),tr("确定"),"",0);
    }
}
/***************************************************************/

/***************************************************************/
//股道视图显示设置功能：
void MainWindow::GDSTViewSetFun()
{
    if(GetServiceCommFlag())
    {
//        GDSTViewSet *gdstViewSet = new GDSTViewSet(this);
//        gdstViewSet->setWindowModality(Qt::WindowModal);//设置模态
//        gdstViewSet->setAttribute(Qt::WA_DeleteOnClose);//关闭后自动释放窗口
//        gdstViewSet->show();
    }
    else
    {
        QMessageBox::critical(this, tr("错误"), tr(COMMUNICATION_TIPS),tr("确定"),"",0);
    }
}
/***************************************************************/

/***************************************************************/
//方向显示设置：
void MainWindow::StationDirectionFun()
{
    if(GetServiceCommFlag())
    {
//        StationDirection *stationDirection = new StationDirection(this);
//        stationDirection->setWindowModality(Qt::WindowModal);//设置模态
//        stationDirection->setAttribute(Qt::WA_DeleteOnClose);//关闭后自动释放窗口
//        stationDirection->show();
    }
    else
    {
        QMessageBox::critical(this, tr("错误"), tr(COMMUNICATION_TIPS),tr("确定"),"",0);
    }
}
/***************************************************************/

/***************************************************************/
//功能按钮设置：
void MainWindow::FunctionPushButtonFun()
{
    if(GetServiceCommFlag())
    {
//        FunctionPushButtonSet *functionPushButtonSet = new FunctionPushButtonSet(this);
//        functionPushButtonSet->setWindowModality(Qt::WindowModal);//设置模态
//        functionPushButtonSet->setAttribute(Qt::WA_DeleteOnClose);//关闭后自动释放窗口
//        functionPushButtonSet->show();
    }
    else
    {
        QMessageBox::critical(this, tr("错误"), tr("未检测到有效的服务端和联锁通信，无法操作！"),tr("确定"),"",0);
    }
}
/***************************************************************/

/***************************************************************/
//报警提示配置：
void MainWindow::BJTSSetFun()
{
    if(GetServiceCommFlag())
    {
//        BJTSSet *bjtsSet = new BJTSSet(this);
//        bjtsSet->setWindowModality(Qt::WindowModal);//设置模态
//        bjtsSet->setAttribute(Qt::WA_DeleteOnClose);//关闭后自动释放窗口
//        bjtsSet->show();
    }
    else
    {
        QMessageBox::critical(this, tr("错误"), tr(COMMUNICATION_TIPS),tr("确定"),"",0);
    }
}
//获取占线版与服务端通信状态
void MainWindow::SetServiceCommFlag(bool b)
{
    if(commState!=b)
    {
        SetNetBtnPNG(b);
    }
    commState=b;
}
bool MainWindow::GetServiceCommFlag()
{
    return commState;
}
void MainWindow::SetNetBtnPNG(bool b)
{
    QIcon icon;
    if(b)
    {
       icon.addFile("://res/连接.png");
    }
    else
    {
        icon.addFile("://res/断开.png");
    }
    btn_Net->setIcon(icon);
}
//初始化区域批量设备操作菜单
void MainWindow::InitZoneBatchDevOperate()
{
    //区域设备批量操作菜单
    QMenu *mZoneDevOperMenu=new QMenu(this);
    mZoneDevOperMenu->setStyleSheet("background-color: rgb(204, 228, 247);");
    for(int i = 0; i < m_vectMyStation.size(); i++)
    {
        QAction *m_addAction=new QAction(mZoneDevOperMenu);
        m_addAction->setText(m_vectMyStation.at(i)->m_strStationName);
        m_vectMyStation.at(i)->InitZoneData();
        mZoneDevOperMenu->addAction(m_addAction);
    }
    //区域批量设备操作
    connect(mZoneDevOperMenu, &QMenu::triggered, this, &MainWindow::ZoneBatchDevOperateDialogSolt);
    m_QYPLSBCZ->setMenu(mZoneDevOperMenu);
}

/***************************************************************/
void MainWindow::timerEvent(QTimerEvent *event)//定时器
{
    if(event->timerId() == timerIdMemorandum)
    {
        nTimeCount++;
        if(nTimeCount>100)
        {
            nTimeCount=0;
        }
        //设置图标为默认
        QIcon icon1;
        icon1.addFile("://res/9.png");
        QIcon icon2;
        icon2.addFile("://res/10.png");
        if(nTimeCount%2)
        {
            btn_Memorandum->setIcon(icon1);
        }
        else
        {
            btn_Memorandum->setIcon(icon2);
        }
    }
    if(event->timerId() == timerId_500)
    {
        if(GetServiceCommFlag())
        {
            killTimer(timerId_500);
            pZXBWindow->showMaximized();
            pZXBWindow->move(0, 0);
            pZXBWindow->show();
            pZXBWindow->activateWindow();
        }
    }
    update();
}
/***************************************************************/
void MainWindow::clicked_T3()
{
    emit SendTextAnalysisSignal(84, QCursor::pos());
    if(GetServiceCommFlag())
    {

    }
    else
    {
        QMessageBox::critical(this, tr("错误"), tr(COMMUNICATION_TIPS),tr("确定"),"",0);
    }
}
/***************************************************************/
void MainWindow::clicked_btn4_openMointer()
{
    if(GetServiceCommFlag())
    {
//        monitor->activateWindow();
//        monitor->setWindowState((monitor->windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
//        monitor->show();
    }
    else
    {
        QMessageBox::critical(this, tr("错误"), tr(COMMUNICATION_TIPS),tr("确定"),"",0);
    }
}
/***************************************************************/
void MainWindow::clicked_btn5_openmainWindowForStationTrack()
{
    emit SendTextAnalysisSignal(4, QCursor::pos());
    if(pZXBWindow && GetServiceCommFlag())
    {
        //pZXBWindow->activateWindow();
        //pZXBWindow->setWindowState((pZXBWindow->windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
        //pZXBWindow->showMaximized();
        //pZXBWindow->showMaximized();
        //pZXBWindow->move(0, 0);
        pZXBWindow->show();
        pZXBWindow->activateWindow();
    }
    else
    {
        QMessageBox::critical(this, tr("错误"), tr(COMMUNICATION_TIPS),tr("确定"),"",0);
    }
}
/************************备忘录***************************************/
void MainWindow::clicked_btn7_openBWL()
{
    emit SendTextAnalysisSignal(6, QCursor::pos());
    if(GetServiceCommFlag())
    {
        pMemorandumDialog->show();
    }
    else
    {
        QMessageBox::critical(this, tr("错误"), tr(COMMUNICATION_TIPS),tr("确定"),"",0);
    }
}
/***************************************************************/
void MainWindow::clicked_btn9_openJSJDJH()
{
    //emit SendTextAnalysisSignal(1, QCursor::pos());
    if(GetServiceCommFlag())
    {
//        Dialog_JSJDJH *dialog_jsjdjh = new Dialog_JSJDJH(this);
//        dialog_jsjdjh->exec();
    }
    else
    {
        QMessageBox::critical(this, tr("错误"), tr(COMMUNICATION_TIPS),tr("确定"),"",0);
    }
}
/***************************************************************/
void MainWindow::clicked_btn10_openDDMLCL()
{
    if(GetServiceCommFlag())
    {
//        Dialog_DDMLCL *dialog_ddmlcl = new Dialog_DDMLCL(this);
//        dialog_ddmlcl->exec();
    }
    else
    {
        QMessageBox::critical(this, tr("错误"), tr(COMMUNICATION_TIPS),tr("确定"),"",0);
    }
}
/***************************************************************/
void MainWindow::clicked_btn12_Close()
{
    //this->close();
    exit(0);
}
/***************************************************************/
void MainWindow::clicked_btn12_KeyDialogbox()
{
    emit SendTextAnalysisSignal(8, QCursor::pos());
    KeyDialogbox("是否关闭占线板？");
}
/***************************************************************/
void MainWindow::clicked_LoginButton()
{
    emit SendTextAnalysisSignal(162, QCursor::pos());
    if(GetServiceCommFlag())
    {
        LoginID=comboBox_ID->currentText();
        LoginName=comboBox_Name->currentText();
        if((LoginName=="")&&(LoginID==""))
        {
            btn_Enter->setText("**值班员A - 未登录");
        }
        else
        {
            btn_Enter->setText("**值班员CTC3.0终端 - <"+LoginName+">");
        }
    }
    else
    {
        QMessageBox::critical(this, tr("错误"), tr(COMMUNICATION_TIPS),tr("确定"),"",0);
    }
}
/***************************************************************/
void MainWindow::clicked_LogoutButton()
{
    emit SendTextAnalysisSignal(163, QCursor::pos());
    LoginID="";
    LoginName="";
    if((LoginName=="")&&(LoginID==""))
    {
        btn_Enter->setText("**值班员A - 未登录");
    }
    else
    {
        btn_Enter->setText("**值班员CTC3.0终端 - <"+LoginName+">");
    }
}
//槽-供电臂设置
void MainWindow::PowerSupplyArmDialogSolt()
{
    emit PowerSupplyArmDialogShowSignal();
    emit SendTextAnalysisSignal(1, QCursor::pos());
}
//槽-封锁设备操作
void  MainWindow::BlockingDevDialogSolt()
{
    emit  BlockingDevDialogShowSignal();
    emit SendTextAnalysisSignal(2, QCursor::pos());
}
//槽-区域批量设备操作
void MainWindow::ZoneBatchDevOperateDialogSolt(QAction *action)
{
    //区域批量设备操作
    emit SendTextAnalysisSignal(3, QCursor::pos());
    ZoneBatchDevOperateDialog* pZoneBatchDevOperateDialog = new ZoneBatchDevOperateDialog;
    connect(pZoneBatchDevOperateDialog, &ZoneBatchDevOperateDialog::SendTextAnalysisSignal,[=](int index, QPoint pt, QString dev){
        this->SendTextAnalysisSignal(index, pt, dev);
    });
    for(int i = 0; i < m_vectMyStation.size(); i++)
    {
        if(m_vectMyStation.at(i)->m_strStationName == action->text())
        {
            m_vectMyStation.at(i)->WndType = 2;
            pZoneBatchDevOperateDialog->Init(m_vectMyStation.at(i));
            connect(pZoneBatchDevOperateDialog, &ZoneBatchDevOperateDialog::SendZoneBatchDevOperateToSeverSignal,
                    [=](int nStationID,unsigned char FUNType,unsigned int FUNSet_DeviceNum){
                emit SendZoneBatchDevOperateToSeverSignal(nStationID, FUNType, FUNSet_DeviceNum);
            });
                    //this, SLOT(SendZoneBatchDevOperateToSeverSolt(int,unsigned char,unsigned int)));
            pZoneBatchDevOperateDialog->show();
            return;
        }
    }
}
void MainWindow::SendBtnIncoChangeSolt(bool b)
{
    if(b)
    {
        //启动定时器
        timerIdMemorandum =startTimer(500);
    }
    else
    {
        //关闭定时器
        if(nTimeCount>=0)
        {
           killTimer(timerIdMemorandum);
           nTimeCount=-1;
        }
        //设置图标为默认
        QIcon icon7;
        icon7.addFile("://res/9.png");
        btn_Memorandum->setIcon(icon7);
    }
}
void MainWindow::ResetMemorandumDialog()
{
    pMemorandumDialog->ResetStation();
    SendBtnIncoChangeSolt(false);
}
