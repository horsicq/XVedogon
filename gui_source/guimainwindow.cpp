// Copyright (c) 2020-2023 hors<horsicq@gmail.com>
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
#include "guimainwindow.h"
#include "ui_guimainwindow.h"

GuiMainWindow::GuiMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GuiMainWindow)
{
    ui->setupUi(this);

    setWindowTitle(QString("%1 v%2").arg(X_APPLICATIONNAME).arg(X_APPLICATIONVERSION)); // TODO

    setAcceptDrops(true);

    DialogOptions::loadOptions(&xvdgOptions); // TODO
    adjust();

    ui->checkBoxDeepScan->setChecked(true);
    ui->checkBoxRecursive->setChecked(true);
    ui->checkBoxHeuristic->setChecked(false);

    if(QCoreApplication::arguments().count()>1)
    {
        QString sFileName=QCoreApplication::arguments().at(1);

        _scan(sFileName);
    }
}

GuiMainWindow::~GuiMainWindow()
{
    DialogOptions::saveOptions(&xvdgOptions);

    delete ui;
}

void GuiMainWindow::on_pushButtonExit_clicked()
{
    this->close();
}

void GuiMainWindow::on_pushButtonOpenFile_clicked()
{
    QString sDirectory;
    if(xvdgOptions.bSaveLastDirectory&&QDir().exists(xvdgOptions.sLastDirectory))
    {
        sDirectory=xvdgOptions.sLastDirectory;
    }

    QString sFileName=QFileDialog::getOpenFileName(this,tr("Open file..."),sDirectory,tr("All files (*)"));

    if(!sFileName.isEmpty())
    {
        ui->lineEditFileName->setText(sFileName);

        if(xvdgOptions.bScanAfterOpen)
        {
            _scan(sFileName);
        }
    }
}

void GuiMainWindow::_scan(QString sFileName)
{
    if((sFileName!="")&&(QFileInfo(sFileName).isFile()))
    {
        SpecAbstract::SCAN_RESULT scanResult;

        SpecAbstract::SCAN_OPTIONS options= {0};
        options.bRecursiveScan=ui->checkBoxRecursive->isChecked();
        options.bDeepScan=ui->checkBoxDeepScan->isChecked();

        DialogStaticScanProcess ds(this);
        ds.setData(sFileName,&options,&scanResult);
        ds.exec();

        if(xvdgOptions.bSaveLastDirectory)
        {
            QFileInfo fi(sFileName);
            xvdgOptions.sLastDirectory=fi.absolutePath();
        }

        QList<XBinary::SCANSTRUCT> _listRecords = SpecAbstract::convert(&(scanResult.listRecords));

        ScanItemModel *pModel=new ScanItemModel(&_listRecords);
        ui->treeViewResult->setModel(pModel);

//        ui->treeViewResult->setColumnWidth(1,40);
//        ui->treeViewResult->setColumnWidth(2,40);

        ui->treeViewResult->header()->setSectionResizeMode(0,QHeaderView::Stretch);  // TODO Check Qt 4!

        ui->treeViewResult->expandAll();
    }
}

void GuiMainWindow::on_pushButtonScan_clicked()
{
    QString sFileName=ui->lineEditFileName->text().trimmed();

    _scan(sFileName);
}

void GuiMainWindow::adjust()
{
    Qt::WindowFlags wf=windowFlags();
    if(xvdgOptions.bStayOnTop)
    {
        wf|=Qt::WindowStaysOnTopHint;
    }
    else
    {
        wf&=~(Qt::WindowStaysOnTopHint);
    }
    setWindowFlags(wf);

    show();
}

void GuiMainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void GuiMainWindow::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void GuiMainWindow::dropEvent(QDropEvent *event)
{
    const QMimeData* mimeData=event->mimeData();

    if(mimeData->hasUrls())
    {
        QList<QUrl> urlList=mimeData->urls();

        if(urlList.count())
        {
            QString sFileName=urlList.at(0).toLocalFile();

            sFileName=XBinary::convertFileName(sFileName);

            if(xvdgOptions.bScanAfterOpen)
            {
                _scan(sFileName);
            }
        }
    }
}

void GuiMainWindow::on_pushButtonOptions_clicked()
{
    DialogOptions dialogOptions(this,&xvdgOptions);
    dialogOptions.exec();

    adjust();
}

void GuiMainWindow::on_pushButtonAbout_clicked()
{
    DialogAbout dialogAbout(this);
    dialogAbout.exec();
}

void GuiMainWindow::on_pushButtonClear_clicked()
{
    ui->treeViewResult->setModel(nullptr);
}

void GuiMainWindow::on_pushButtonSave_clicked()
{
    QString sSaveFileNameDirectory=xvdgOptions.sLastDirectory+QDir::separator()+"result"; // mb TODO

    QAbstractItemModel *pModel=ui->treeViewResult->model();
    DialogStaticScanProcess::saveResult(this,(ScanItemModel *)pModel,sSaveFileNameDirectory);
}

void GuiMainWindow::on_pushButtonUnpack_clicked()
{
    // TODO
}

void GuiMainWindow::on_pushButtonAdvanced_clicked()
{
    // TODO
    // Windows with advanced options
}

