#include "dialog_manage_biz.h"
#include <QBuffer>
#include <QFileDialog>
#include <QMessageBox>
#include "ui_dialog_manage_biz.h"

DialogManageBiz::DialogManageBiz(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogManageBiz)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &DialogManageBiz::handleEditPic);
}

DialogManageBiz::~DialogManageBiz()
{
    delete ui;
}

void DialogManageBiz::setBizName(QString bizName)
{
    ui->lineEditBizName->setText(bizName);
}

QString DialogManageBiz::getBizName()
{
    return ui->lineEditBizName->text();
}

void DialogManageBiz::setBizDesc(QString bizDesc)
{
    ui->lineEditBizDesc->setText(bizDesc);
}

QString DialogManageBiz::getBizDesc()
{
    return ui->lineEditBizDesc->text();
}

void DialogManageBiz::setBizPic(QByteArray bizPic)
{
    QPixmap pixmap;
    pixmap.loadFromData(bizPic);
    ui->label->setPixmap(pixmap);
}

QByteArray DialogManageBiz::getBizPic()
{
    QBuffer buffer;
    QByteArray bytes;
    if (buffer.open(QIODevice::WriteOnly)) {
        ui->label->pixmap().save(&buffer, "png");
        bytes.append(buffer.data());
        buffer.close();
    }
    return bytes;
}

void DialogManageBiz::handleEditPic()
{
    QFileDialog *dialog = new QFileDialog(this, Qt::Widget);
    dialog->setFileMode(QFileDialog::ExistingFile);
    dialog->setNameFilter(tr("图像(*.png *.jpg *.bmp)"));
    dialog->setViewMode(QFileDialog::Detail);
    dialog->setAcceptMode(QFileDialog::AcceptOpen);
    dialog->exec();
    QList<QString> files = dialog->selectedFiles();
    if (dialog->result() == QFileDialog::Accepted && files.size() == 1) {
        QFile file(files[0]);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, tr("文件打不开"), tr("文件是否被占用？"));
            return;
        }
        QPixmap pixmap;
        pixmap.loadFromData(file.readAll());
        ui->label->setPixmap(pixmap);
    }
    delete dialog;
}
