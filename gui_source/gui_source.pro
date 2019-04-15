QT       += core gui widgets

CONFIG += static_plugins

TARGET = gui_source
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

!contains(XCONFIG, formresult) {
    XCONFIG += formresult
    include(../StaticScan/dialogstaticscan.pri)
}
!contains(XCONFIG, plugin_interface) {
    XCONFIG += plugin_interface
    include(../xvdg_plugins/plugin_interface.pri)
}

static_plugins {
    DEFINES += "STATIC_PLUGINS=1"
    !contains(XCONFIG, plugin_zip) {
        XCONFIG += plugin_zip
        include(../xvdg_plugins/plugin_zip/plugin_zip.pri)
    }
    !contains(XCONFIG, plugin_pe) {
        XCONFIG += plugin_pe
        include(../xvdg_plugins/plugin_pe/plugin_pe.pri)
    }
    !contains(XCONFIG, plugin_upx) {
        XCONFIG += plugin_upx
        include(../xvdg_plugins/plugin_upx/plugin_upx.pri)
    }
}

win32 {
    RC_ICONS = ../icons/main.ico
}
macx {
    ICON = ../icons/main.icns
}

RESOURCES += \
    rsrc.qrc
