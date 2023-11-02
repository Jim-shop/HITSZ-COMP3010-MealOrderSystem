#include "biz_manage_widget.h"
#include <QSqlError>
#include "image_delegate.h"
#include "ui_biz_manage_widget.h"

BizManageWidget::BizManageWidget(QSqlDatabase &db, QWidget *parent)
    : BuzzingWidget(parent)
    , ui(new Ui::BizManageWidget)
    , db(db)
{
    ui->setupUi(this);

    biz = new QSqlRelationalTableModel(this, db);
    biz->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    biz->setTable("biz");
    biz->setHeaderData(biz->fieldIndex("biz_id"), Qt::Horizontal, tr("ID"));
    biz->setHeaderData(biz->fieldIndex("biz_name"), Qt::Horizontal, tr("名称"));
    biz->setHeaderData(biz->fieldIndex("biz_desc"), Qt::Horizontal, tr("描述"));
    biz->setHeaderData(biz->fieldIndex("biz_pic"), Qt::Horizontal, tr("图片"));
    biz->setHeaderData(biz->fieldIndex("biz_enable"), Qt::Horizontal, tr("启用"));
    biz->select();

    ui->tableView->setModel(biz);
    ui->tableView->setItemDelegateForColumn(biz->fieldIndex("biz_pic"), new ImageDelegate());

    connect(ui->pushButtonAdd, &QPushButton::clicked, this, &BizManageWidget::handleAdd);
    connect(ui->pushButtonDelete, &QPushButton::clicked, this, &BizManageWidget::handleDelete);
    connect(ui->pushButtonSubmit, &QPushButton::clicked, this, &BizManageWidget::handleSubmit);
}

BizManageWidget::~BizManageWidget()
{
    delete ui;
    delete biz;
}

void BizManageWidget::handleAdd()
{
    biz->insertRow(biz->rowCount());
}

void BizManageWidget::handleDelete()
{
    biz->removeRow(ui->tableView->currentIndex().row());
}

void BizManageWidget::handleSubmit()
{
    if (!biz->submitAll()) {
        QMessageBox::information(this, tr("请检查输入"), biz->lastError().text());
    }
    biz->select();
}
