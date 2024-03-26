include("NewWorksheets/NewWorksheets.pri")
include("ShuntingManage/ShuntingManage.pri")
include("ParameterSetting/ParameterSetting.pri")
include("RoutePreview/RoutePreview.pri")
include("RouteHandling/RouteHandling.pri")
include("CancelRoute/CancelRoute.pri")
include("WorksheetsSend/WorksheetsSend.pri")

FORMS += \
    $$PWD/shuntroutetablewidget.ui \
    $$PWD/shuntroutewidget.ui

HEADERS += \
    $$PWD/shuntroutetablewidget.h \
    $$PWD/shuntroutewidget.h

SOURCES += \
    $$PWD/shuntroutetablewidget.cpp \
    $$PWD/shuntroutewidget.cpp
