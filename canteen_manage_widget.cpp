#include "canteen_manage_widget.h"
#include <QSqlError>
#include "image_delegate.h"
#include "ui_canteen_manage_widget.h"

CanteenManageWidget::CanteenManageWidget(QSqlDatabase &db, QWidget *parent)
    : BuzzingWidget(parent)
    , ui(new Ui::CanteenManageWidget)
    , db(db)
{
    ui->setupUi(this);

    canteen = new QSqlRelationalTableModel(this, db);
    canteen->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    canteen->setTable("canteen");
    canteen->setHeaderData(canteen->fieldIndex("canteen_id"), Qt::Horizontal, tr("ID"));
    canteen->setHeaderData(canteen->fieldIndex("canteen_name"), Qt::Horizontal, tr("名称"));
    canteen->setHeaderData(canteen->fieldIndex("canteen_desc"), Qt::Horizontal, tr("描述"));
    canteen->setHeaderData(canteen->fieldIndex("canteen_pic"), Qt::Horizontal, tr("图片"));
    canteen->setHeaderData(canteen->fieldIndex("canteen_enable"), Qt::Horizontal, tr("启用"));
    canteen->select();

    ui->tableView->setModel(canteen);
    ui->tableView->setItemDelegateForColumn(canteen->fieldIndex("canteen_pic"), new ImageDelegate());

    connect(ui->pushButtonAdd, &QPushButton::clicked, this, &CanteenManageWidget::handleAdd);
    connect(ui->pushButtonDelete, &QPushButton::clicked, this, &CanteenManageWidget::handleDelete);
    connect(ui->pushButtonSubmit, &QPushButton::clicked, this, &CanteenManageWidget::handleSubmit);
}

CanteenManageWidget::~CanteenManageWidget()
{
    delete ui;
    delete canteen;
}

void CanteenManageWidget::handleAdd()
{
    canteen->insertRow(canteen->rowCount());
}

void CanteenManageWidget::handleDelete()
{
    canteen->removeRow(ui->tableView->currentIndex().row());
}

void CanteenManageWidget::handleSubmit()
{
    if (!canteen->submitAll()) {
        QMessageBox::information(this, tr("请检查输入"), canteen->lastError().text());
    }
    canteen->select();
}

#include "canteen_manage_widget.moc"
