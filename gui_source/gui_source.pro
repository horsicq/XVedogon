QT       += core gui widgets

CONFIG(debug, debug|release) {
    CONFIG += static_plugins
}

CONFIG+=gui_build

TARGET = xvdg
TEMPLATE = app

CONFIG += c++11

SOURCES += \
        dialogviewer.cpp \
        guimainwindow.cpp \
        main_gui.cpp \
        dialogoptions.cpp \
        dialogabout.cpp \
        dialogviewers.cpp \
        dialogunpackers.cpp \
    dialogunpacker.cpp

HEADERS += \
        dialogviewer.h \
        guimainwindow.h \
        dialogoptions.h \
        dialogabout.h \
        dialogviewers.h \
        dialogunpackers.h \
    dialogunpacker.h

FORMS += \
        dialogviewer.ui \
        guimainwindow.ui \
        dialogoptions.ui \
        dialogabout.ui \
        dialogviewers.ui \
        dialogunpackers.ui \
    dialogunpacker.ui

include(../build.pri)
include(../xvdg_utils.pri)

!contains(XCONFIG, formresult) {
    XCONFIG += formresult
    include(../StaticScan/dialogstaticscan.pri)
}

win32 {
    RC_ICONS = ../icons/main.ico
}
macx {
    ICON = ../icons/main.icns
}

RESOURCES += \
    rsrc.qrc
