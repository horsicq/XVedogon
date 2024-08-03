// Copyright (c) 2020-2024 hors<horsicq@gmail.com>
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
#include "dialogoptions.h"
#include "ui_dialogoptions.h"

DialogOptions::DialogOptions(QWidget *parent, XVDG::OPTIONS *pOptions) : QDialog(parent), ui(new Ui::DialogOptions)
{
    ui->setupUi(this);

    this->pOptions = pOptions;

    ui->checkBoxDeepScan->setChecked(pOptions->bDeepScan);
    ui->checkBoxScanAfterOpen->setChecked(pOptions->bScanAfterOpen);
    ui->checkBoxRecursive->setChecked(pOptions->bRecursive);
    ui->checkBoxSaveLastDirectory->setChecked(pOptions->bSaveLastDirectory);

    ui->checkBoxStayOnTop->setChecked(pOptions->bStayOnTop);
    ui->checkBoxContext->setChecked(pOptions->bContext);
}

DialogOptions::~DialogOptions()
{
    delete ui;
}

void DialogOptions::loadOptions(XVDG::OPTIONS *pOptions)
{
    QSettings settings(QApplication::applicationDirPath() + QDir::separator() + "xvdg.ini", QSettings::IniFormat);

    pOptions->bScanAfterOpen = settings.value("ScanAfterOpen", true).toBool();
    pOptions->bRecursive = settings.value("Recursive", true).toBool();
    pOptions->bDeepScan = settings.value("DeepScan", true).toBool();
    pOptions->bSaveLastDirectory = settings.value("SaveLastDirectory", true).toBool();
    pOptions->sLastDirectory = settings.value("LastDirectory", "").toString();

    pOptions->bStayOnTop = settings.value("StayOnTop", false).toBool();
#ifdef WIN32
    pOptions->bContext = checkContext("*");
#endif

    if (!QDir(pOptions->sLastDirectory).exists()) {
        pOptions->sLastDirectory = "";
    }
}

void DialogOptions::saveOptions(XVDG::OPTIONS *pOptions)
{
    QSettings settings(QApplication::applicationDirPath() + QDir::separator() + "xvdg.ini", QSettings::IniFormat);

    settings.setValue("ScanAfterOpen", pOptions->bScanAfterOpen);
    settings.setValue("Recursive", pOptions->bRecursive);
    settings.setValue("DeepScan", pOptions->bDeepScan);
    settings.setValue("SaveLastDirectory", pOptions->bSaveLastDirectory);
    settings.setValue("LastDirectory", pOptions->sLastDirectory);

    settings.setValue("StayOnTop", pOptions->bStayOnTop);

#ifdef WIN32
    if (!setContextState("*", pOptions->bContext)) {
        pOptions->bContext = !pOptions->bContext;
    }
#endif
}

bool DialogOptions::checkContext(QString sType)
{
    QSettings settings(QString("HKEY_CLASSES_ROOT\\%1\\shell").arg(sType), QSettings::NativeFormat);
    QString sRecord = settings.value("XVedogon/command/Default").toString();

    return (sRecord != "");
}

void DialogOptions::clearContext(QString sType)
{
    QSettings settings(QString("HKEY_CLASSES_ROOT\\%1\\shell\\XVedogon").arg(sType), QSettings::NativeFormat);
    settings.clear();
}

void DialogOptions::registerContext(QString sType)
{
    QSettings settings(QString("HKEY_CLASSES_ROOT\\%1\\shell\\XVedogon\\command").arg(sType), QSettings::NativeFormat);
    settings.setValue(".", "\"" + QCoreApplication::applicationFilePath().replace("/", "\\") + "\" \"%1\"");

    QSettings settingsIcon(QString("HKEY_CLASSES_ROOT\\%1\\shell\\XVedogon").arg(sType), QSettings::NativeFormat);
    settingsIcon.setValue("Icon", "\"" + QCoreApplication::applicationFilePath().replace("/", "\\") + "\"");
}

bool DialogOptions::setContextState(QString sType, bool bState)
{
    if (checkContext(sType) != bState) {
        if (bState) {
            registerContext(sType);
        } else {
            clearContext(sType);
        }
    }

    return (checkContext(sType) == bState);
}

void DialogOptions::on_pushButtonOK_clicked()
{
    pOptions->bDeepScan = ui->checkBoxDeepScan->isChecked();
    pOptions->bScanAfterOpen = ui->checkBoxScanAfterOpen->isChecked();
    pOptions->bRecursive = ui->checkBoxRecursive->isChecked();
    pOptions->bSaveLastDirectory = ui->checkBoxSaveLastDirectory->isChecked();
    pOptions->bStayOnTop = ui->checkBoxStayOnTop->isChecked();

    pOptions->bContext = ui->checkBoxContext->isChecked();

    saveOptions(pOptions);
    this->close();
}

void DialogOptions::on_pushButtonCancel_clicked()
{
    this->close();
}
