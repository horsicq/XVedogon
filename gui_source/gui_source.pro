QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gui_source
TEMPLATE = app

CONFIG += c++11

SOURCES += \
        guimainwindow.cpp \
        main_gui.cpp \
        dialogoptions.cpp

HEADERS += \
        guimainwindow.h \
        dialogoptions.h

FORMS += \
        guimainwindow.ui \
        dialogoptions.ui

include(../build.pri)

include(../../_mylibs/StaticScan/formresult.pri)

include(../xvdg_plugins/interface.pri)
include(../xvdg_plugins/upx.pri)

win32 {
    RC_ICONS = ../icons/main.ico
}
macx {
    ICON = ../icons/main.icns
}
