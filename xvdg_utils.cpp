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
