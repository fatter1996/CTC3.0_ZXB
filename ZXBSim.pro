QT       += core gui
QT       += network
QT       += texttospeech
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

CONFIG += resources_big
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui
#全局变量模块
include(GlobalHeaders/GlobalHeaders.pri)
#车站基类模块
include(BaseDataPro/BaseDataPro.pri)

#include("StationTrack/StationTrack.pri")
#自定义车站模块
include(MyStation/MyStation.pri)
#通信模块
include(SocketPro/SocketPro.pri)
#主业务逻辑模块
include(MyDoc/MyDoc.pri)
#轻量级日志库
include(QsLog/QsLog.pri)
#自定义日志使用
include(Log/Log.pri)
#占线版窗口
include(ZXBWindow/ZXBWindow.pri)
#进路序列窗口
include(RouteOrder/RouteOrder.pri)
#表格类
include(TableView/TableView.pri)
#站存车与防溜
include(StationStorageWidget/StationStorageWidget.pri)
#股道数据类
include(GDData/GDData.pri)
#股道视图
include(TrackView/TrackView.pri)
#对话框
include("Dialog/Dialog.pri")
#调车
include("Shunting/Shunting.pri")
#
include(TextAnalysis/TextAnalysis.pri)
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Res.qrc

RC_ICONS = T3.ico
