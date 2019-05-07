QT       += core gui widgets

CONFIG += static_plugins

TARGET = xvdg
TEMPLATE = app

CONFIG += c++11

SOURCES += \
    dialogviewer.cpp \
        guimainwindow.cpp \
        main_gui.cpp \
        dialogoptions.cpp \
        dialogabout.cpp \
    dialogmodules.cpp

HEADERS += \
    dialogviewer.h \
        guimainwindow.h \
        dialogoptions.h \
        dialogabout.h \
    dialogmodules.h

FORMS += \
    dialogviewer.ui \
        guimainwindow.ui \
        dialogoptions.ui \
        dialogabout.ui \
    dialogmodules.ui

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
    !contains(XCONFIG, plugin_binary) {
        XCONFIG += plugin_binary
        include(../../_mylibs/xvdg_plugins/plugin_binary/plugin_binary.pri)
    }
    !contains(XCONFIG, plugin_zip) {
        XCONFIG += plugin_zip
        include(../xvdg_plugins/plugin_zip/plugin_zip.pri)
    }
    !contains(XCONFIG, plugin_elf) {
        XCONFIG += plugin_elf
        include(../xvdg_plugins/plugin_elf/plugin_elf.pri)
    }
    !contains(XCONFIG, plugin_msdos) {
        XCONFIG += plugin_msdos
        include(../xvdg_plugins/plugin_msdos/plugin_msdos.pri)
    }
    !contains(XCONFIG, plugin_mach) {
        XCONFIG += plugin_mach
        include(../xvdg_plugins/plugin_mach/plugin_mach.pri)
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
