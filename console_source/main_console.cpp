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
#include <QCoreApplication>
#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include "staticscanitemmodel.h"
#include "../global.h"
#include "../xvdg_utils.h"
#include "consoleoutput.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName(X_ORGANIZATIONNAME);
    QCoreApplication::setOrganizationDomain(X_ORGANIZATIONDOMAIN);
    QCoreApplication::setApplicationName(X_APPLICATIONNAME);
    QCoreApplication::setApplicationVersion(X_APPLICATIONVERSION);

    QCoreApplication app(argc, argv);

    QList<QObject *> listModules=Xvdg_utils::getPluginList(&app);

    ConsoleOutput co;

    QCommandLineParser parser;
    QString sDescription;
    sDescription.append(QString("%1 v%2\n").arg(X_APPLICATIONNAME).arg(X_APPLICATIONVERSION));
    sDescription.append(QString("%1").arg("Copyright(C) 2019 hors<horsicq@gmail.com> Web: http://ntinfo.biz\n"));
    sDescription.append(QString("%1 module(s)").arg(listModules.count()));
    parser.setApplicationDescription(sDescription);

    parser.addPositionalArgument("file","The file to open.");

    QCommandLineOption clModules(QStringList()<<"m"<<"modules","Show all modules.");
    QCommandLineOption clScan(QStringList()<<"S"<<"scan","Scan.");
    QCommandLineOption clScanOverlay(QStringList()<<"o"<<"scanoverlay","Scan overlay.");
    QCommandLineOption clDeepScan(QStringList()<<"d"<<"deepscan","Deep scan.");
    QCommandLineOption clResultAsXml(QStringList()<<"x"<<"xml","Scan result as XML.");   

    parser.addOption(clModules);
    parser.addOption(clScan);
    parser.addOption(clScanOverlay);
    parser.addOption(clDeepScan);
    parser.addOption(clResultAsXml);
    parser.addHelpOption();
    parser.addVersionOption();

    parser.process(app);

    bool bIsUsed=false;

    if(parser.isSet(clModules))
    {
        bIsUsed=true;

        QString sInfo=(QString("%1 module(s): ").arg(listModules.count()));
        printf("%s\n",sInfo.toLatin1().data());

        QList<XvdgPluginInterface::INFO> listInfos=Xvdg_utils::getPluginInfos(&listModules);

        int nCount=listInfos.count();

        for(int i=0;i<nCount;i++)
        {
            printf("%s\n",Xvdg_utils::infoToString(listInfos.at(i)).toLatin1().data());
        }
    }

    QList<QString> listArgs=parser.positionalArguments();

    if(listArgs.count())
    {
        bIsUsed=true;

        QList<QString> listFileNames;

        for(int i=0;i<listArgs.count();i++)
        {
            QString sFileName=listArgs.at(i);

            if(QFileInfo::exists(sFileName))
            {
                XBinary::findFiles(sFileName,&listFileNames);
            }
            else
            {
                co.infoMessage(QString("Cannot find: %1").arg(sFileName),0);
            }
        }

        bool bShowFileName=listFileNames.count()>1;

        int nCount=listFileNames.count();
        for(int i=0;i<nCount;i++)
        {
            QString sFileName=listFileNames.at(i);

            if(bShowFileName)
            {
                printf("%s:\n",sFileName.toLatin1().data());
            }

            if(     parser.isSet(clScan)||
                    parser.isSet(clScanOverlay)||
                    parser.isSet(clDeepScan)||
                    parser.isSet(clResultAsXml))
            {
                // Scan
                SpecAbstract::SCAN_OPTIONS scanOptions={};

                scanOptions.bDeepScan=parser.isSet(clDeepScan);
                scanOptions.bScanOverlay=parser.isSet(clScanOverlay);
                scanOptions.bResultAsXML=parser.isSet(clResultAsXml);

                SpecAbstract::SCAN_RESULT scanResult=StaticScan::processFile(sFileName,&scanOptions);
                StaticScanItemModel model(&scanResult.listRecords);

                printf("%s\n",model.toString(&scanOptions).toLatin1().data());

                int nRecordsCount=scanResult.listRecords.count();

                for(int j=0;j<nRecordsCount;j++)
                {
                    XvdgPluginInterface *pPlugin=Xvdg_utils::getPlugin(&listModules,scanResult.listRecords.at(j));
                    if(pPlugin)
                    {
                        XvdgPluginInterface::INFO info=pPlugin->getInfo();
                        if(info.bIsUnpacker)
                        {
                            printf("Available unpack module: %s\n",pPlugin->getInfo().sName.toLatin1().data());
                        }
                    }
                }
            }
        }
    }

    if(!bIsUsed)
    {
        parser.showHelp();
        Q_UNREACHABLE();
    }

    return 0;
}
