INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/xvdg_utils.h

SOURCES += \
    $$PWD/xvdg_utils.cpp

!contains(XCONFIG, plugin_interface) {
    XCONFIG += plugin_interface
    include(xvdg_plugins/plugin_interface.pri)
}

!contains(XCONFIG, staticscan) {
    XCONFIG += staticscan
    include(../_mylibs/StaticScan/staticscan.pri)
}

static_plugins {
    DEFINES += "STATIC_PLUGINS=1"
    !contains(XCONFIG, plugin_binary) {
        XCONFIG += plugin_binary
        include(xvdg_plugins/plugin_binary/plugin_binary.pri)
    }
    !contains(XCONFIG, plugin_zip) {
        XCONFIG += plugin_zip
        include(xvdg_plugins/plugin_zip/plugin_zip.pri)
    }
    !contains(XCONFIG, plugin_elf) {
        XCONFIG += plugin_elf
        include(xvdg_plugins/plugin_elf/plugin_elf.pri)
    }
    !contains(XCONFIG, plugin_msdos) {
        XCONFIG += plugin_msdos
        include(xvdg_plugins/plugin_msdos/plugin_msdos.pri)
    }
    !contains(XCONFIG, plugin_mach) {
        XCONFIG += plugin_mach
        include(xvdg_plugins/plugin_mach/plugin_mach.pri)
    }
    !contains(XCONFIG, plugin_pe) {
        XCONFIG += plugin_pe
        include(xvdg_plugins/plugin_pe/plugin_pe.pri)
    }
    !contains(XCONFIG, plugin_upx) {
        XCONFIG += plugin_upx
        include(xvdg_plugins/plugin_upx/plugin_upx.pri)
    }
}
