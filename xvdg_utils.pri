INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/xvdg_utils.h

SOURCES += \
    $$PWD/xvdg_utils.cpp

gui_build {
    !contains(XCONFIG, viewer_plugin_interface) {
        XCONFIG += viewer_plugin_interface
        include(xvdg_plugins/viewers/viewer_plugin_interface.pri)
    }
}

!contains(XCONFIG, unpacker_plugin_interface) {
    XCONFIG += unpacker_plugin_interface
    include(xvdg_plugins/unpackers/unpacker_plugin_interface.pri)
}

!contains(XCONFIG, staticscan) {
    XCONFIG += staticscan
    include(StaticScan/staticscan.pri))
}

static_plugins {
    DEFINES += "STATIC_PLUGINS=1"

    gui_build {
        !contains(XCONFIG, viewer_plugin_binary) {
            XCONFIG += viewer_plugin_binary
            include(xvdg_plugins/viewers/viewer_binary/viewer_binary.pri)
        }
        !contains(XCONFIG, viewer_plugin_elf) {
            XCONFIG += viewer_plugin_elf
            include(xvdg_plugins/viewers/viewer_elf/viewer_elf.pri)
        }
        !contains(XCONFIG, viewer_plugin_msdos) {
            XCONFIG += viewer_plugin_msdos
            include(xvdg_plugins/viewers/viewer_msdos/viewer_msdos.pri)
        }
        !contains(XCONFIG, viewer_plugin_mach) {
            XCONFIG += viewer_plugin_mach
            include(xvdg_plugins/viewers/viewer_mach/viewer_mach.pri)
        }
        !contains(XCONFIG, viewer_plugin_pe) {
            XCONFIG += viewer_plugin_pe
            include(xvdg_plugins/viewers/viewer_pe/viewer_pe.pri)
        }
        !contains(XCONFIG, viewer_plugin_le) {
            XCONFIG += viewer_plugin_le
            include(xvdg_plugins/viewers/viewer_le/viewer_le.pri)
        }
        !contains(XCONFIG, viewer_plugin_ne) {
            XCONFIG += viewer_plugin_ne
            include(xvdg_plugins/viewers/viewer_ne/viewer_ne.pri)
        }
    }

    !contains(XCONFIG, unpacker_upx) {
        XCONFIG += unpacker_upx
        include(xvdg_plugins/unpackers/unpacker_upx/unpacker_upx.pri)
    }

    !contains(XCONFIG, unpacker_pex) {
        XCONFIG += unpacker_pex
        include(xvdg_plugins/unpackers/unpacker_pex/unpacker_pex.pri)
    }

    !contains(XCONFIG, unpacker_aspack) {
        XCONFIG += unpacker_aspack
        include(xvdg_plugins/unpackers/unpacker_aspack/unpacker_aspack.pri)
    }

    !contains(XCONFIG, unpacker_vmprotect) {
        XCONFIG += unpacker_vmprotect
        include(xvdg_plugins/unpackers/unpacker_vmprotect/unpacker_vmprotect.pri)
    }

    !contains(XCONFIG, unpacker_mpress) {
        XCONFIG += unpacker_mpress
        include(xvdg_plugins/unpackers/unpacker_mpress/unpacker_mpress.pri)
    }

    !contains(XCONFIG, unpacker_pecompact) {
        XCONFIG += unpacker_pecompact
        include(xvdg_plugins/unpackers/unpacker_pecompact/unpacker_pecompact.pri)
    }

    !contains(XCONFIG, unpacker_ahpacker) {
        XCONFIG += unpacker_ahpacker
        include(xvdg_plugins/unpackers/unpacker_ahpacker/unpacker_ahpacker.pri)
    }

    !contains(XCONFIG, unpacker_kkrunchy) {
        XCONFIG += unpacker_kkrunchy
        include(xvdg_plugins/unpackers/unpacker_kkrunchy/unpacker_kkrunchy.pri)
    }

    !contains(XCONFIG, unpacker_beroexepacker) {
        XCONFIG += unpacker_beroexepacker
        include(xvdg_plugins/unpackers/unpacker_beroexepacker/unpacker_beroexepacker.pri)
    }

    !contains(XCONFIG, unpacker_petite) {
        XCONFIG += unpacker_petite
        include(xvdg_plugins/unpackers/unpacker_petite/unpacker_petite.pri)
    }

    !contains(XCONFIG, unpacker_fsg) {
        XCONFIG += unpacker_fsg
        include(xvdg_plugins/unpackers/unpacker_fsg/unpacker_fsg.pri)
    }

    !contains(XCONFIG, unpacker_nspack) {
        XCONFIG += unpacker_nspack
        include(xvdg_plugins/unpackers/unpacker_nspack/unpacker_nspack.pri)
    }
}
