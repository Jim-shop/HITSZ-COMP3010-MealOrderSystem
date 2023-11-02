#include "image_delegate.h"

void ImageDelegate::paint(QPainter *painter,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
{
    painter->drawImage(option.rect, QImage::fromData(index.data().toByteArray()));
}

QWidget *ImageDelegate::createEditor(QWidget *parent,
                                     const QStyleOptionViewItem &option,
                                     const QModelIndex &index) const
{
    QFileDialog *dialog = new QFileDialog(parent, Qt::Widget);
    dialog->setFileMode(QFileDialog::ExistingFile);
    dialog->setNameFilter(tr("图像(*.png *.jpg *.bmp)"));
    dialog->setViewMode(QFileDialog::Detail);
    dialog->setAcceptMode(QFileDialog::AcceptOpen);
    dialog->open();
    return dialog;
}

void ImageDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {}

void ImageDelegate::updateEditorGeometry(QWidget *editor,
                                         const QStyleOptionViewItem &option,
                                         const QModelIndex &index) const
{}

void ImageDelegate::setModelData(QWidget *editor,
                                 QAbstractItemModel *model,
                                 const QModelIndex &index) const
{
    QFileDialog *dialog = qobject_cast<QFileDialog *>(editor);
    QList<QString> files = dialog->selectedFiles();
    if (dialog->result() == QFileDialog::Accepted && files.size() == 1) {
        QFile file(files[0]);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(editor, tr("文件打不开"), tr("文件是否被占用？"));
            return;
        }
        QByteArray content = file.readAll();
        model->setData(index, content, Qt::EditRole);
    }
}
