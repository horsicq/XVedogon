#ifndef XVDG_UTILS_H
#define XVDG_UTILS_H

#include <QObject>
#ifdef STATIC_PLUGINS
#include "plugin_binary.h"
#include "plugin_zip.h"
#include "plugin_pe.h"
#include "plugin_elf.h"
#include "plugin_msdos.h"
#include "plugin_mach.h"
#include "plugin_upx.h"
#else
#include <QPluginLoader>
#include <QDirIterator>
#endif

class Xvdg_utils
{
public:
    static QList<QObject *> getPluginList(QObject *pParent);
};

#endif // XVDG_UTILS_H
