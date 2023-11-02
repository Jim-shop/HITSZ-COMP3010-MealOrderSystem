#include "dialog_manage_canteen.h"
#include "ui_dialog_manage_canteen.h"

#include <QBuffer>
#include <QFileDialog>
#include <QMessageBox>

DialogManageCanteen::DialogManageCanteen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogManageCanteen)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &DialogManageCanteen::handleEditPic);
}

DialogManageCanteen::~DialogManageCanteen()
{
    delete ui;
}

void DialogManageCanteen::setCanteenName(QString canteenName)
{
    ui->lineEditCanteenName->setText(canteenName);
}

QString DialogManageCanteen::getCanteenName()
{
    return ui->lineEditCanteenName->text();
}

void DialogManageCanteen::setCanteenDesc(QString canteenDesc)
{
    ui->lineEditCanteenDesc->setText(canteenDesc);
}

QString DialogManageCanteen::getCanteenDesc()
{
    return ui->lineEditCanteenDesc->text();
}

void DialogManageCanteen::setCanteenPic(QByteArray canteenPic)
{
    QPixmap pixmap;
    pixmap.loadFromData(canteenPic);
    ui->label->setPixmap(pixmap);
}

QByteArray DialogManageCanteen::getCanteenPic()
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

void DialogManageCanteen::handleEditPic()
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
