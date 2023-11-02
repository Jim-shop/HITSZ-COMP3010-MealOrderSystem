#include "dialog_manage_user.h"
#include "ui_dialog_manage_user.h"

#include <QBuffer>
#include <QFileDialog>
#include <QMessageBox>

DialogManageUser::DialogManageUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogManageUser)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &DialogManageUser::handleEditPic);
}

DialogManageUser::~DialogManageUser()
{
    delete ui;
}

void DialogManageUser::setUserName(QString userName)
{
    ui->lineEditName->setText(userName);
}

QString DialogManageUser::getUserName()
{
    return ui->lineEditName->text();
}

void DialogManageUser::setUserPic(QByteArray userPic)
{
    QPixmap pixmap;
    pixmap.loadFromData(userPic);
    ui->label->setPixmap(pixmap);
}

QByteArray DialogManageUser::getUserPic()
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

void DialogManageUser::handleEditPic()
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
