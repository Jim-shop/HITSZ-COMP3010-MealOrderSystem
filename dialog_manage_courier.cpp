#include "dialog_manage_courier.h"
#include "ui_dialog_manage_courier.h"

#include <QBuffer>
#include <QFileDialog>
#include <QMessageBox>

DialogManageCourier::DialogManageCourier(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogManageCourier)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &DialogManageCourier::handleEditPic);
}

DialogManageCourier::~DialogManageCourier()
{
    delete ui;
}

void DialogManageCourier::setCourierName(QString courierName)
{
    ui->lineEditName->setText(courierName);
}

QString DialogManageCourier::getCourierName()
{
    return ui->lineEditName->text();
}

void DialogManageCourier::setCourierPhone(QString courierPhone)
{
    ui->lineEditPhone->setText(courierPhone);
}

QString DialogManageCourier::getCourierPhone()
{
    return ui->lineEditPhone->text();
}

void DialogManageCourier::setCourierPic(QByteArray courierPic)
{
    QPixmap pixmap;
    pixmap.loadFromData(courierPic);
    ui->label->setPixmap(pixmap);
}

QByteArray DialogManageCourier::getCourierPic()
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

void DialogManageCourier::handleEditPic()
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
