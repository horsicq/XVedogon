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
#include "xvdg_utils.h"

#ifdef QT_GUI_LIB
QList<QObject *> Xvdg_utils::getViewerPluginList(QObject *pParent)
{
    QList<QObject *> listResult;

#ifdef STATIC_PLUGINS
    listResult.append(new Viewer_Binary(pParent));
    listResult.append(new Viewer_PE(pParent));
    listResult.append(new Viewer_ELF(pParent));
    listResult.append(new Viewer_MSDOS(pParent));
    listResult.append(new Viewer_MACH(pParent));
#else
    QString sPluginPath=QCoreApplication::applicationDirPath()+QDir::separator()+"viewers";

    QDirIterator it(sPluginPath,QStringList() << "*.*",QDir::Files,QDirIterator::NoIteratorFlags);
    while(it.hasNext())
    {
        QPluginLoader *pPluginLoader=new QPluginLoader(pParent);
        pPluginLoader->setFileName(it.next());
        bool bLoaded=false;
        if(pPluginLoader->load())
        {
            QObject *pPlugin=pPluginLoader->instance();
            if(pPlugin)
            {
                listResult.append(pPlugin);
                bLoaded=true;
            }
        }
        if(!bLoaded)
        {
            delete pPluginLoader;
        }
    }
#endif

    return listResult;
}
#endif
#ifdef QT_GUI_LIB
XvdgViewerPluginInterface *Xvdg_utils::getViewerPlugin(QList<QObject *> *pListPlugins,SpecAbstract::SCAN_STRUCT ss)
{
    TODO

    XvdgViewerPluginInterface *pResult=nullptr;

    int nPluginsCount=pListPlugins->count();

    for(int i=0;i<nPluginsCount;i++)
    {
        XvdgViewerPluginInterface *pPluginInterface=qobject_cast<XvdgViewerPluginInterface *>(pListPlugins->at(i));

        if(pPluginInterface)
        {
            if(pPluginInterface->isValid(&ss))
            {
                pResult=pPluginInterface;
                break;
            }
        }
    }

    return pResult;
}
#endif
#ifdef QT_GUI_LIB
QList<XvdgViewerPluginInterface::INFO> Xvdg_utils::getViewerPluginInfos(QList<QObject *> *pListPlugins)
{
    QList<XvdgViewerPluginInterface::INFO> listResult;

    int nCount=pListPlugins->count();

    for(int i=0;i<nCount;i++)
    {
        XvdgViewerPluginInterface *pPluginInterface=qobject_cast<XvdgViewerPluginInterface *>(pListPlugins->at(i));
        if(pPluginInterface)
        {
            XvdgViewerPluginInterface::INFO info=pPluginInterface->getInfo();

            listResult.append(info);
        }
    }

    return listResult;
}
#endif
#ifdef QT_GUI_LIB
QString Xvdg_utils::infoViewerToString(XvdgViewerPluginInterface::INFO info)
{
    QString sResult;

    sResult=QString("%1 - (%2) [%3]").arg(info.sName).arg(info.sVersion).arg(info.sDescription);

    return sResult;
}
#endif
#ifdef QT_GUI_LIB
XvdgViewerPluginInterface *Xvdg_utils::getViewerPluginByName(QList<QObject *> *pListPlugins, QString sName)
{
    XvdgViewerPluginInterface *pResult=nullptr;

    int nCount=pListPlugins->count();

    for(int i=0;i<nCount;i++)
    {
        XvdgViewerPluginInterface *pPluginInterface=qobject_cast<XvdgViewerPluginInterface *>(pListPlugins->at(i));
        if(pPluginInterface)
        {
            if(pPluginInterface->getInfo().sName==sName)
            {
                pResult=pPluginInterface;
                break;
            }
        }
    }

    return pResult;
}
#endif
QList<QObject *> Xvdg_utils::getUnpackerPluginList(QObject *pParent)
{
    QList<QObject *> listResult;

#ifdef STATIC_PLUGINS
    listResult.append(new Unpacker_UPX(pParent));
#else
    QString sPluginPath=QCoreApplication::applicationDirPath()+QDir::separator()+"unpackers";

    QDirIterator it(sPluginPath,QStringList() << "*.*",QDir::Files,QDirIterator::NoIteratorFlags);
    while(it.hasNext())
    {
        QPluginLoader *pPluginLoader=new QPluginLoader(pParent);
        pPluginLoader->setFileName(it.next());
        bool bLoaded=false;
        if(pPluginLoader->load())
        {
            QObject *pPlugin=pPluginLoader->instance();
            if(pPlugin)
            {
                listResult.append(pPlugin);
                bLoaded=true;
            }
        }
        if(!bLoaded)
        {
            delete pPluginLoader;
        }
    }
#endif

    return listResult;
}

XvdgUnpackerPluginInterface *Xvdg_utils::getUnpackerPlugin(QList<QObject *> *pListPlugins, SpecAbstract::SCAN_STRUCT ss)
{
    XvdgUnpackerPluginInterface *pResult=nullptr;

    int nPluginsCount=pListPlugins->count();

    for(int i=0;i<nPluginsCount;i++)
    {
        XvdgUnpackerPluginInterface *pPluginInterface=qobject_cast<XvdgUnpackerPluginInterface *>(pListPlugins->at(i));
        if(pPluginInterface)
        {
            if(pPluginInterface->isValid(&ss))
            {
                pResult=pPluginInterface;
                break;
            }
        }
    }

    return pResult;
}

QList<XvdgUnpackerPluginInterface::INFO> Xvdg_utils::getUnpackerPluginInfos(QList<QObject *> *pListPlugins)
{
    QList<XvdgUnpackerPluginInterface::INFO> listResult;

    int nCount=pListPlugins->count();

    for(int i=0;i<nCount;i++)
    {
        XvdgUnpackerPluginInterface *pPluginInterface=qobject_cast<XvdgUnpackerPluginInterface *>(pListPlugins->at(i));
        if(pPluginInterface)
        {
            XvdgUnpackerPluginInterface::INFO info=pPluginInterface->getInfo();

            listResult.append(info);
        }
    }

    return listResult;
}

QString Xvdg_utils::infoUnpackerToString(XvdgUnpackerPluginInterface::INFO info)
{
    QString sResult;

    sResult=QString("%1 - (%2) [%3]").arg(info.sName).arg(info.sVersion).arg(info.sDescription);

    return sResult;
}

XvdgUnpackerPluginInterface *Xvdg_utils::getUnpackerPluginByName(QList<QObject *> *pListPlugins, QString sName)
{
    XvdgUnpackerPluginInterface *pResult=nullptr;

    int nCount=pListPlugins->count();

    for(int i=0;i<nCount;i++)
    {
        XvdgUnpackerPluginInterface *pPluginInterface=qobject_cast<XvdgUnpackerPluginInterface *>(pListPlugins->at(i));
        if(pPluginInterface)
        {
            if(pPluginInterface->getInfo().sName==sName)
            {
                pResult=pPluginInterface;
                break;
            }
        }
    }

    return pResult;
}
