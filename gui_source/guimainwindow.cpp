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
#include "guimainwindow.h"
#include "ui_guimainwindow.h"

GuiMainWindow::GuiMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GuiMainWindow)
{
    ui->setupUi(this);

    setWindowTitle(QString("%1 v%2").arg(X_APPLICATIONNAME).arg(X_APPLICATIONVERSION));

    setAcceptDrops(true);

    DialogOptions::loadOptions(&xvdgOptions);
    adjust();

    loadPlugins();

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
        mapButtonInfos.clear();

        SpecAbstract::SCAN_RESULT scanResult;

        SpecAbstract::SCAN_OPTIONS options= {0};
        options.bRecursive=ui->checkBoxRecursive->isChecked();
        options.bDeepScan=ui->checkBoxDeepScan->isChecked();

        DialogStaticScan ds(this);
        ds.setData(sFileName,&options,&scanResult);
        ds.exec();

        if(xvdgOptions.bSaveLastDirectory)
        {
            QFileInfo fi(sFileName);
            xvdgOptions.sLastDirectory=fi.absolutePath();
        }

        StaticScanItemModel *model=new StaticScanItemModel(&(scanResult.listRecords),this,3);
        ui->treeViewResult->setModel(model);

//        ui->treeViewResult->setColumnWidth(1,40);
//        ui->treeViewResult->setColumnWidth(2,40);

        ui->treeViewResult->header()->setSectionResizeMode(0,QHeaderView::Stretch);  // TODO Check Qt 4!

        QList<SRECORD> listSrecords;
        listSrecords.clear();

        handleItem(&listSrecords,model->rootItem(),model,QModelIndex());

        int nModelRowCount=listSrecords.count();

        for(int i=0;i<nModelRowCount;i++)
        {
            // TODO move to utils
            SpecAbstract::SCAN_STRUCT ss=listSrecords.at(i).scanStruct;
            XvdgPluginInterface *pPluginInterface=Xvdg_utils::getPlugin(&listPlugins,ss);

            if(pPluginInterface)
            {
                XvdgPluginInterface::INFO info=pPluginInterface->getInfo();

                if(info.bIsUnpacker)
                {
                    BUTTON_INFO bi={};
                    QString sGUID=QUuid::createUuid().toString();
                    bi.pPlugin=pPluginInterface;
                    bi.biType=BUTTON_INFO_TYPE_RTUNPACK;
                    bi.nOffset=ss.nOffset;
                    bi.nSize=ss.nSize;

                    mapButtonInfos.insert(sGUID,bi);

                    QPushButton *pPushButton=new QPushButton(tr("Unpack"));
                    pPushButton->setProperty("uid",sGUID);

                    connect(pPushButton,SIGNAL(clicked(bool)),this,SLOT(pushButtonSlot()));

                    ui->treeViewResult->setIndexWidget(listSrecords.at(i).modelIndex1,pPushButton);
                }
                if(info.bIsViewer)
                {
                    BUTTON_INFO bi={};
                    QString sGUID=QUuid::createUuid().toString();
                    bi.pPlugin=pPluginInterface;
                    bi.biType=BUTTON_INFO_TYPE_VIEWER;
                    bi.nOffset=ss.nOffset;
                    bi.nSize=ss.nSize;

                    mapButtonInfos.insert(sGUID,bi);

                    QPushButton *pPushButton=new QPushButton(tr("Info"));
                    pPushButton->setProperty("uid",sGUID);

                    connect(pPushButton,SIGNAL(clicked(bool)),this,SLOT(pushButtonSlot()));

                    ui->treeViewResult->setIndexWidget(listSrecords.at(i).modelIndex2,pPushButton);
                }
            }
        }

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

    ui->checkBoxDeepScan->setChecked(xvdgOptions.bDeepScan);
    ui->checkBoxRecursive->setChecked(xvdgOptions.bRecursive);

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

void GuiMainWindow::loadPlugins()
{
    listPlugins=Xvdg_utils::getPluginList(this);

    ui->pushButtonModules->setEnabled(listPlugins.count());
    ui->pushButtonModules->setText(tr("%1: %2").arg(tr("Modules")).arg(listPlugins.count()));
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
    DialogStaticScan::saveResult(this,(StaticScanItemModel *)pModel,sSaveFileNameDirectory);
}

void GuiMainWindow::on_pushButtonModules_clicked()
{
    DialogModules dialogModules(this,&listPlugins);
    dialogModules.exec();
}

void GuiMainWindow::handleItem(QList<SRECORD> *pListButtons, StaticScanItem *pItem, StaticScanItemModel *pModel, QModelIndex modelIndexParent)
{
    int nChildCount=pItem->childCount();

    for(int i=0;i<nChildCount;i++)
    {
        SRECORD buttonInfo={0};

        StaticScanItem *pChild=pItem->child(i);

        SpecAbstract::SCAN_STRUCT ss=pChild->scanStruct();
        buttonInfo.scanStruct=ss;

        QModelIndex modelIndex=pModel->index(i,0,modelIndexParent);
        buttonInfo.modelIndex1=pModel->index(i,1,modelIndexParent);
        buttonInfo.modelIndex2=pModel->index(i,2,modelIndexParent);

        pListButtons->append(buttonInfo);

        handleItem(pListButtons,pChild,pModel,modelIndex);
    }
}

void GuiMainWindow::pushButtonSlot()
{
    QPushButton *pPushButton=qobject_cast<QPushButton*>(sender());
    QString sUID=pPushButton->property("uid").toString();
    BUTTON_INFO bi=mapButtonInfos.value(sUID);

    if(bi.pPlugin)
    {
        if(bi.biType==BUTTON_INFO_TYPE_VIEWER)
        {
            QFile file;
            file.setFileName(ui->lineEditFileName->text());

            if(XBinary::tryToOpen(&file)) // TODO readOnly
            {
                SubDevice sd(&file,bi.nOffset,bi.nSize);

                sd.open(file.openMode());

                XvdgPluginInterface::DATA data={};
                data.pParent=this;
                data.pDevice=&sd;

                QWidget *pWidget=bi.pPlugin->getViewerWidget(&data);

                if(pWidget)
                {
                    DialogViewer dv(this);

                    dv.addWidget(pWidget);

                    dv.exec();
                }

                sd.close();
                file.close();
            }
        }
        else if(bi.biType==BUTTON_INFO_TYPE_RTUNPACK)
        {
            bi.pPlugin->rtUnpack(ui->lineEditFileName->text());
        }
    }
}
