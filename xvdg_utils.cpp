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

QList<QObject *> Xvdg_utils::getPluginList(QObject *pParent)
{
    QList<QObject *> listResult;

#ifdef STATIC_PLUGINS
    listResult.append(new Plugin_Binary(pParent));
    listResult.append(new Plugin_Zip(pParent));
    listResult.append(new Plugin_PE(pParent));
    listResult.append(new Plugin_ELF(pParent));
    listResult.append(new Plugin_MSDOS(pParent));
    listResult.append(new Plugin_MACH(pParent));
    listResult.append(new Plugin_UPX(pParent));
#else
    QString sPluginPath=QCoreApplication::applicationDirPath()+QDir::separator()+"modules";

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

XvdgPluginInterface *Xvdg_utils::getPlugin(QList<QObject *> *pListPlugins,SpecAbstract::SCAN_STRUCT ss)
{
    XvdgPluginInterface *pResult=nullptr;

    int nPluginsCount=pListPlugins->count();

    for(int i=0;i<nPluginsCount;i++)
    {
        XvdgPluginInterface *pPluginInterface=qobject_cast<XvdgPluginInterface *>(pListPlugins->at(i));
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

QList<XvdgPluginInterface::INFO> Xvdg_utils::getPluginInfos(QList<QObject *> *pListPlugins)
{
    QList<XvdgPluginInterface::INFO> listResult;

    int nCount=pListPlugins->count();

    for(int i=0;i<nCount;i++)
    {
        XvdgPluginInterface *pPluginInterface=qobject_cast<XvdgPluginInterface *>(pListPlugins->at(i));
        if(pPluginInterface)
        {
            XvdgPluginInterface::INFO info=pPluginInterface->getInfo();

            listResult.append(info);
        }
    }

    return listResult;
}

QString Xvdg_utils::infoToString(XvdgPluginInterface::INFO info)
{
    QString sResult;

    sResult=QString("%1 - (%2) [%3]").arg(info.sName).arg(info.sVersion).arg(info.sDescription);

    return sResult;
}

XvdgPluginInterface *Xvdg_utils::getPluginByName(QList<QObject *> *pListPlugins, QString sName)
{
    XvdgPluginInterface *pResult=nullptr;

    int nCount=pListPlugins->count();

    for(int i=0;i<nCount;i++)
    {
        XvdgPluginInterface *pPluginInterface=qobject_cast<XvdgPluginInterface *>(pListPlugins->at(i));
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
