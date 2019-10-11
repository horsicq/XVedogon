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
QObject *Xvdg_utils::getViewerPlugin(QList<QObject *> *pListPlugins,SpecAbstract::SCAN_STRUCT ss)
{

    QObject *pResult=nullptr;

    int nPluginsCount=pListPlugins->count();

    for(int i=0;i<nPluginsCount;i++)
    {
        XvdgViewerPluginInterface *pPluginInterface=qobject_cast<XvdgViewerPluginInterface *>(pListPlugins->at(i));

        if(pPluginInterface)
        {
            if(pPluginInterface->isValid(&ss))
            {
                pResult=pListPlugins->at(i);
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
XvdgViewerPluginInterface::INFO Xvdg_utils::getViewerPluginInfo(QObject *pPlugin)
{
    XvdgViewerPluginInterface::INFO result={};

    XvdgViewerPluginInterface *pPluginInterface=qobject_cast<XvdgViewerPluginInterface *>(pPlugin);

    if(pPluginInterface)
    {
        result=pPluginInterface->getInfo();
    }

    return result;
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
#ifdef QT_GUI_LIB
QWidget *Xvdg_utils::getViewerPluginWidget(QObject *pPlugin, XvdgViewerPluginInterface::DATA *pData)
{
    QWidget *pResult=0;

    XvdgViewerPluginInterface *pPluginInterface=qobject_cast<XvdgViewerPluginInterface *>(pPlugin);

    if(pPluginInterface)
    {
        pResult=pPluginInterface->getWidget(pData);
    }

    return pResult;
}
#endif
QList<QObject *> Xvdg_utils::getUnpackerPluginList(QObject *pParent)
{
    QList<QObject *> listResult;

#ifdef STATIC_PLUGINS
    listResult.append(new Unpacker_UPX(pParent));
    listResult.append(new Unpacker_PEX(pParent));
    listResult.append(new Unpacker_ASPack(pParent));
    listResult.append(new Unpacker_MPress(pParent));
    listResult.append(new Unpacker_VMProtect(pParent));
    listResult.append(new Unpacker_PECompact(pParent));
    listResult.append(new Unpacker_AHPacker(pParent));
    listResult.append(new Unpacker_kkrunchy(pParent));
    listResult.append(new Unpacker_BeRoEXEPacker(pParent));
    listResult.append(new Unpacker_Petite(pParent));
    listResult.append(new Unpacker_FSG(pParent));
    listResult.append(new Unpacker_NSPack(pParent));
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

QObject *Xvdg_utils::getUnpackerPlugin(QList<QObject *> *pListPlugins, SpecAbstract::SCAN_STRUCT ss)
{
    QObject *pResult=nullptr;

    int nPluginsCount=pListPlugins->count();

    for(int i=0;i<nPluginsCount;i++)
    {
        XvdgUnpackerPluginInterface *pPluginInterface=qobject_cast<XvdgUnpackerPluginInterface *>(pListPlugins->at(i));
        if(pPluginInterface)
        {
            if(pPluginInterface->isValid(&ss))
            {
                pResult=pListPlugins->at(i);
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

XvdgUnpackerPluginInterface::INFO Xvdg_utils::getUnpackerPluginInfo(QObject *pPlugin)
{
    XvdgUnpackerPluginInterface::INFO result={};

    XvdgUnpackerPluginInterface *pPluginInterface=qobject_cast<XvdgUnpackerPluginInterface *>(pPlugin);

    if(pPluginInterface)
    {
        result=pPluginInterface->getInfo();
    }

    return result;
}

QString Xvdg_utils::infoUnpackerToString(XvdgUnpackerPluginInterface::INFO info)
{
    QString sResult;

    sResult=QString("%1 - (%2) [%3]").arg(info.sName).arg(info.sVersion).arg(info.sDescription);

    return sResult;
}

QObject *Xvdg_utils::getUnpackerPluginByName(QList<QObject *> *pListPlugins, QString sName)
{
    QObject *pResult=nullptr;

    int nCount=pListPlugins->count();

    for(int i=0;i<nCount;i++)
    {
        XvdgUnpackerPluginInterface *pPluginInterface=qobject_cast<XvdgUnpackerPluginInterface *>(pListPlugins->at(i));
        if(pPluginInterface)
        {
            if(pPluginInterface->getInfo().sName==sName)
            {
                pResult=pListPlugins->at(i);
                break;
            }
        }
    }

    return pResult;
}

QList<XvdgUnpackerPluginInterface::OPTIONS_RECORD> Xvdg_utils::getDefaultOptions(QObject *pPlugin)
{
    QList<XvdgUnpackerPluginInterface::OPTIONS_RECORD> listResult;

    XvdgUnpackerPluginInterface *pPluginInterface=qobject_cast<XvdgUnpackerPluginInterface *>(pPlugin);

    if(pPluginInterface)
    {
        listResult=pPluginInterface->getDefaultOptions();
    }

    return listResult;
}

bool Xvdg_utils::rtUnpack(QObject *pPlugin, QString sFileName, QString sResultFileName, QList<XvdgUnpackerPluginInterface::OPTIONS_RECORD> *pListOptions)
{
    bool bResult=false;

    XvdgUnpackerPluginInterface *pPluginInterface=qobject_cast<XvdgUnpackerPluginInterface *>(pPlugin);

    if(pPluginInterface)
    {
        bResult=pPluginInterface->rtUnpack(sFileName,sResultFileName,pListOptions);
    }

    return bResult;
}

void Xvdg_utils::rtStop(QObject *pPlugin)
{
    XvdgUnpackerPluginInterface *pPluginInterface=qobject_cast<XvdgUnpackerPluginInterface *>(pPlugin);

    if(pPluginInterface)
    {
        pPluginInterface->rtStop();
    }
}

QString Xvdg_utils::optionToString(XvdgUnpackerPluginInterface::OPTIONS_RECORD record)
{
    QString sResult;
    QString sValue;

    if(record.varType==XvdgUnpackerPluginInterface::OPTIONS_VAR_TYPE_BOOL)
    {
        sValue=record.var.toBool()?QString("true"):QString("false");
    }

    sResult+=QString("%1:%2").arg(record.sName).arg(sValue);

    int nSize=20-sResult.size();

    for(int i=0;i<nSize;i++)
    {
        sResult+=" ";
    }

    sResult+=QString("// %1").arg(record.sDescription);

    return sResult;
}

bool Xvdg_utils::stringToOption(QObject *pPlugin,QString sString, XvdgUnpackerPluginInterface::OPTIONS_RECORD *pRecord)
{
    bool bResult=false;

    XvdgUnpackerPluginInterface *pPluginInterface=qobject_cast<XvdgUnpackerPluginInterface *>(pPlugin);

    if(pPluginInterface)
    {
        bResult=pPluginInterface->stringToOption(sString,pRecord);
    }

    return bResult;
}
