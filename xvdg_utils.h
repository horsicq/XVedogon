// Copyright (c) 2019 hors<horsicq@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#ifndef XVDG_UTILS_H
#define XVDG_UTILS_H

#include <QObject>
#include <QCoreApplication>
#ifdef QT_GUI_LIB
#include "viewer_plugin_interface.h"
#endif
#include "unpacker_plugin_interface.h"
#ifdef STATIC_PLUGINS
#ifdef QT_GUI_LIB
#include "viewer_binary.h"
#include "viewer_pe.h"
#include "viewer_elf.h"
#include "viewer_msdos.h"
#include "viewer_mach.h"
#endif
#include "unpacker_upx.h"
#include "unpacker_pex.h"
#include "unpacker_aspack.h"
#include "unpacker_mpress.h"
#include "unpacker_vmprotect.h"
#include "unpacker_pecompact.h"
#include "unpacker_ahpacker.h"
#include "unpacker_kkrunchy.h"
#include "unpacker_beroexepacker.h"
#include "unpacker_petite.h"
#include "unpacker_fsg.h"
#include "unpacker_nspack.h"
#else
#include <QPluginLoader>
#include <QDirIterator>
#endif
#include "staticscan.h"

class Xvdg_utils
{
public:
#ifdef QT_GUI_LIB
    static QList<QObject *> getViewerPluginList(QObject *pParent);
    static QObject *getViewerPlugin(QList<QObject *> *pListPlugins,SpecAbstract::SCAN_STRUCT ss);
    static QList<XvdgViewerPluginInterface::INFO> getViewerPluginInfos(QList<QObject *> *pListPlugins);
    static XvdgViewerPluginInterface::INFO getViewerPluginInfo(QObject *pPlugin);
    static QString infoViewerToString(XvdgViewerPluginInterface::INFO info);
    static XvdgViewerPluginInterface *getViewerPluginByName(QList<QObject *> *pListPlugins,QString sName);
    static QWidget *getViewerPluginWidget(QObject *pPlugin, XvdgViewerPluginInterface::DATA *pData);
#endif
    static QList<QObject *> getUnpackerPluginList(QObject *pParent);
    static QObject *getUnpackerPlugin(QList<QObject *> *pListPlugins,SpecAbstract::SCAN_STRUCT ss);
    static QList<XvdgUnpackerPluginInterface::INFO> getUnpackerPluginInfos(QList<QObject *> *pListPlugins);
    static XvdgUnpackerPluginInterface::INFO getUnpackerPluginInfo(QObject *pPlugin);
    static QString infoUnpackerToString(XvdgUnpackerPluginInterface::INFO info);
    static QObject *getUnpackerPluginByName(QList<QObject *> *pListPlugins,QString sName);
    static QList<XvdgUnpackerPluginInterface::OPTIONS_RECORD> getDefaultOptions(QObject *pPlugin);
    static bool rtUnpack(QObject *pPlugin, QString sFileName, QString sResultFileName,QList<XvdgUnpackerPluginInterface::OPTIONS_RECORD> *pListOptions);
    static void rtStop(QObject *pPlugin);
    static QString optionToString(XvdgUnpackerPluginInterface::OPTIONS_RECORD record);
    static bool stringToOption(QObject *pPlugin, QString sString,XvdgUnpackerPluginInterface::OPTIONS_RECORD *pRecord);
};

#endif // XVDG_UTILS_H
