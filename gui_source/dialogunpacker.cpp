// Copyright (c) 2020 hors<horsicq@gmail.com>
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
#include "dialogunpacker.h"
#include "ui_dialogunpacker.h"

DialogUnpacker::DialogUnpacker(QObject *pPlugin, QString sFileName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogUnpacker)
{
    ui->setupUi(this);

    this->pPlugin=pPlugin;
    this->sFileName=sFileName;

    listOptions=Xvdg_utils::getDefaultOptions(pPlugin);

    setWindowTitle(Xvdg_utils::infoUnpackerToString(Xvdg_utils::getUnpackerPluginInfo(pPlugin)));

    ui->lineEditResultFileName->setText(XBinary::getUnpackedName(sFileName));

    int nItemCount=listOptions.count();

    ui->tableWidgetOptions->setColumnCount(2);
    ui->tableWidgetOptions->setRowCount(nItemCount);

    ui->tableWidgetOptions->setColumnWidth(0,220);

    for(int i=0;i<nItemCount;i++)
    {
        QTableWidgetItem *pItem=new QTableWidgetItem(listOptions.at(i).sDescription);

        ui->tableWidgetOptions->setItem(i,0,pItem);

        if(listOptions.at(i).varType==XvdgUnpackerPluginInterface::OPTIONS_VAR_TYPE_BOOL)
        {
            QCheckBox *pCheckBox=new QCheckBox(this);
            pCheckBox->setProperty("Number",i);
            pCheckBox->setChecked(listOptions.at(i).var.toBool());

            connect(pCheckBox,SIGNAL(toggled(bool)),this,SLOT(checkBoxToggled(bool)));

            ui->tableWidgetOptions->setCellWidget(i,1,pCheckBox);
        }
    }
}

DialogUnpacker::~DialogUnpacker()
{
    delete ui;
}

void DialogUnpacker::on_pushButtonClose_clicked()
{
    this->close();
}

void DialogUnpacker::on_pushButtonUnpack_clicked()
{
    DialogUnpackerProcess dup(this);

    QString sResultFileName=ui->lineEditResultFileName->text();

    dup.setData(pPlugin,sFileName,sResultFileName,&listOptions);

    dup.exec();
}

void DialogUnpacker::on_toolButtonResultFileName_clicked()
{
    QString sFileName=QFileDialog::getSaveFileName(this,tr("Result"),ui->lineEditResultFileName->text());

    if(!sFileName.isEmpty())
    {
        ui->lineEditResultFileName->setText(sFileName);
    }
}

void DialogUnpacker::checkBoxToggled(bool bState)
{
    QCheckBox *pCheckBox=qobject_cast<QCheckBox *>(sender());

    int nNumber=pCheckBox->property("Number").toInt();

    if(nNumber<listOptions.count())
    {
        listOptions[nNumber].var=bState;
    }
}
