QT       += core gui widgets

CONFIG(debug, debug|release) {
    CONFIG += static_plugins
}

CONFIG+=gui_build

TARGET = xvdg
TEMPLATE = app

CONFIG += c++11

SOURCES += \
        guimainwindow.cpp \
        main_gui.cpp \
        dialogoptions.cpp \
        dialogabout.cpp

HEADERS += \
        guimainwindow.h \
        dialogoptions.h \
        dialogabout.h

FORMS += \
        guimainwindow.ui \
        dialogoptions.ui \
        dialogabout.ui

include(../build.pri)

!contains(XCONFIG, dialogstaticscanprocess) {
    XCONFIG += dialogstaticscanprocess
    include(../StaticScan/dialogstaticscanprocess.pri)
}

win32 {
    RC_ICONS = ../icons/main.ico
}
macx {
    ICON = ../icons/main.icns
}

RESOURCES += \
    rsrc.qrc
