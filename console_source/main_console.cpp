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

    QList<QObject *> listModules=Xvdg_utils::getUnpackerPluginList(&app);

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
    QCommandLineOption clRecursive(QStringList()<<"r"<<"recursive","Recursive.");
    QCommandLineOption clDeepScan(QStringList()<<"d"<<"deepscan","Deep scan.");
    QCommandLineOption clResultAsXml(QStringList()<<"x"<<"xml","Scan result as XML.");
    QCommandLineOption clUnpack(QStringList()<<"U"<<"unpack","Unpack <method>.","method");
    QCommandLineOption clResult(QStringList()<<"R"<<"result","Result file name.");
    QCommandLineOption clMessages(QStringList()<<"M"<<"messages","Show messages.");

    // TODO options for specific unpacker: -O OPTION:VALUE

    parser.addOption(clModules);
    parser.addOption(clScan);
    parser.addOption(clRecursive);
    parser.addOption(clDeepScan);
    parser.addOption(clResultAsXml);
    parser.addOption(clUnpack);
    parser.addOption(clResult);
    parser.addOption(clMessages);

    parser.addHelpOption();
    parser.addVersionOption();

    parser.process(app);

    bool bIsUsed=false;
    QObject *pUnpacker=nullptr;
    QString sResultFileName;

    if(parser.isSet(clUnpack))
    {
        bIsUsed=true;

        QString sUnpackMethod=parser.value(clUnpack);
        pUnpacker=Xvdg_utils::getUnpackerPluginByName(&listModules,sUnpackMethod);
        if(pUnpacker)
        {
            if(parser.isSet(clMessages))
            {
                QObject::connect(pUnpacker,SIGNAL(messageString(quint32,QString)),&co,SLOT(messageString(quint32,QString)));
            }
        }
        else
        {
            co.messageString(0,QString("Cannot find unpack module: %1").arg(sUnpackMethod));
        }
    }

    if(parser.isSet(clResult))
    {
        sResultFileName=parser.value(clResult);
    }

    if(parser.isSet(clModules))
    {
        bIsUsed=true;

        QString sInfo=(QString("%1 module(s): ").arg(listModules.count()));
        printf("%s\n",sInfo.toLatin1().data());

        QList<XvdgUnpackerPluginInterface::INFO> listInfos=Xvdg_utils::getUnpackerPluginInfos(&listModules);

        int nCount=listInfos.count();

        for(int i=0;i<nCount;i++)
        {
            printf("%s\n",Xvdg_utils::infoUnpackerToString(listInfos.at(i)).toLatin1().data());
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
                co.messageString(0,QString("Cannot find: %1").arg(sFileName));
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

            if(     parser.isSet(clScan)        ||
                    parser.isSet(clRecursive)   ||
                    parser.isSet(clDeepScan)    ||
                    parser.isSet(clResultAsXml) ||
                    (!parser.isSet(clUnpack)))
            {
                // Scan
                SpecAbstract::SCAN_OPTIONS scanOptions={};

                scanOptions.bDeepScan=parser.isSet(clDeepScan);
                scanOptions.bRecursive=parser.isSet(clRecursive);
                scanOptions.bResultAsXML=parser.isSet(clResultAsXml);

                SpecAbstract::SCAN_RESULT scanResult=StaticScan::processFile(sFileName,&scanOptions);
                StaticScanItemModel model(&scanResult.listRecords);

                printf("%s\n",model.toString(&scanOptions).toLatin1().data());

                int nRecordsCount=scanResult.listRecords.count();

                for(int j=0;j<nRecordsCount;j++)
                {
                    QObject *pPlugin=Xvdg_utils::getUnpackerPlugin(&listModules,scanResult.listRecords.at(j));
                    if(pPlugin)
                    {
//                        XvdgUnpackerPluginInterface::INFO info=pPlugin->getInfo();
                        // TODO not overlay!
                        printf("Available unpack method: %s\n",Xvdg_utils::getUnpackerPluginInfo(pPlugin).sName.toLatin1().data());
                    }
                }
            }

            if(pUnpacker)
            {
                if(sResultFileName=="")
                {
                    sResultFileName=XBinary::getUnpackedName(sFileName);
                }

                QList<XvdgUnpackerPluginInterface::OPTIONS_RECORD> listOptions=Xvdg_utils::getDefaultOptions(pUnpacker);

                Xvdg_utils::rtUnpack(pUnpacker,sFileName,sResultFileName,&listOptions);
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
