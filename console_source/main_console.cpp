// Copyright (c) 2019-2023 hors<horsicq@gmail.com>
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
#include "../global.h"
#include "consoleoutput.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName(X_ORGANIZATIONNAME);
    QCoreApplication::setOrganizationDomain(X_ORGANIZATIONDOMAIN);
    QCoreApplication::setApplicationName(X_APPLICATIONNAME);
    QCoreApplication::setApplicationVersion(X_APPLICATIONVERSION);

    QCoreApplication app(argc, argv);

    ConsoleOutput co;

    QCommandLineParser parser;
    QString sDescription;
    sDescription.append(QString("%1 v%2\n").arg(X_APPLICATIONNAME,X_APPLICATIONVERSION));
    sDescription.append(QString("%1").arg("Copyright(C) 2019-2023 hors<horsicq@gmail.com> Web: http://ntinfo.biz\n"));
//    sDescription.append(QString("%1 module(s)").arg(listModules.count()));
    parser.setApplicationDescription(sDescription);

    parser.addPositionalArgument("file","The file to open.");

    QCommandLineOption clModules(QStringList()<<"m"<<"modules","Show all modules.");
    QCommandLineOption clScan(QStringList()<<"S"<<"scan","Scan.");
    QCommandLineOption clRecursive(QStringList()<<"r"<<"recursive","Recursive.");
    QCommandLineOption clDeepScan(QStringList()<<"d"<<"deepscan","Deep scan.");
    QCommandLineOption clResultAsXml(QStringList()<<"x"<<"xml","Scan result as XML.");
    QCommandLineOption clUnpack(QStringList()<<"U"<<"unpack","Unpack <method>.","method");
    QCommandLineOption clDefault(QStringList()<<"D"<<"default","Default unpack options <method>.","method");
    QCommandLineOption clResult(QStringList()<<"R"<<"result","Result file name.");
    QCommandLineOption clMessages(QStringList()<<"M"<<"messages","Show messages.");
    QCommandLineOption clOption(QStringList()<<"O"<<"option","Unpack <option>.","option");

    // TODO options for specific unpacker: -O OPTION:VALUE

    parser.addOption(clModules);
    parser.addOption(clScan);
    parser.addOption(clRecursive);
    parser.addOption(clDeepScan);
    parser.addOption(clResultAsXml);
    parser.addOption(clUnpack);
    parser.addOption(clDefault);
    parser.addOption(clResult);
    parser.addOption(clMessages);
    parser.addOption(clOption);

    parser.addHelpOption();
    parser.addVersionOption();

    parser.process(app);

    bool bIsUsed=false;

    if(!bIsUsed)
    {
        parser.showHelp();
        Q_UNREACHABLE();
    }

    return 0;
}
