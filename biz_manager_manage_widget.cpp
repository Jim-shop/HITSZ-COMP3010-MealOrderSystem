#include "biz_manager_manage_widget.h"
#include "ui_biz_manager_manage_widget.h"

#include <QMessageBox>
#include <QSqlRecord>

BizManagerManageWidget::BizManagerManageWidget(QSqlDatabase &db, QWidget *parent)
    : BuzzingWidget(parent)
    , ui(new Ui::BizManagerManageWidget)
    , db(db)
{
    ui->setupUi(this);

    account = new QSqlRelationalTableModel(this, db);
    account->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    account->setTable("account");
    account->setHeaderData(account->fieldIndex("account_id"), Qt::Horizontal, tr("ID"));
    account->setHeaderData(account->fieldIndex("account_phone"), Qt::Horizontal, tr("手机号"));
    account->setHeaderData(account->fieldIndex("account_password"), Qt::Horizontal, tr("加盐密码"));
    account->setHeaderData(account->fieldIndex("account_salt"), Qt::Horizontal, tr("盐"));
    account->setHeaderData(account->fieldIndex("account_enable"), Qt::Horizontal, tr("启用"));
    account->select();

    ui->tableViewAccount->setModel(account);
    ui->tableViewAccount->hideColumn(account->fieldIndex("account_password"));
    ui->tableViewAccount->hideColumn(account->fieldIndex("account_salt"));

    biz = new QSqlRelationalTableModel(this, db);
    biz->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    biz->setTable("biz");
    biz->setHeaderData(biz->fieldIndex("biz_id"), Qt::Horizontal, tr("ID"));
    biz->setHeaderData(biz->fieldIndex("biz_name"), Qt::Horizontal, tr("名称"));
    biz->setHeaderData(biz->fieldIndex("biz_desc"), Qt::Horizontal, tr("描述"));
    biz->setHeaderData(biz->fieldIndex("biz_pic"), Qt::Horizontal, tr("图片"));
    biz->setHeaderData(biz->fieldIndex("biz_enable"), Qt::Horizontal, tr("启用"));
    biz->select();

    ui->tableViewBiz->setModel(biz);
    ui->tableViewBiz->hideColumn(biz->fieldIndex("biz_desc"));
    ui->tableViewBiz->hideColumn(biz->fieldIndex("biz_pic"));

    bizManager = new QSqlRelationalTableModel(this, db);
    bizManager->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    bizManager->setTable("biz_manager");
    bizManager->setHeaderData(bizManager->fieldIndex("account_id"), Qt::Horizontal, tr("账户ID"));
    bizManager->setHeaderData(bizManager->fieldIndex("biz_id"), Qt::Horizontal, tr("餐厅ID"));
    bizManager->setHeaderData(bizManager->fieldIndex("biz_manager_root"),
                              Qt::Horizontal,
                              tr("特权用户"));
    bizManager->select();

    ui->tableViewBizManager->setModel(bizManager);

    connect(ui->pushButtonAdd, &QPushButton::clicked, this, &BizManagerManageWidget::handleAdd);
    connect(ui->pushButtonDelete,
            &QPushButton::clicked,
            this,
            &BizManagerManageWidget::handleDelete);
    connect(ui->pushButtonSubmit,
            &QPushButton::clicked,
            this,
            &BizManagerManageWidget::handleSubmit);
}

BizManagerManageWidget::~BizManagerManageWidget()
{
    delete ui;
    delete account;
    delete biz;
    delete bizManager;
}

void BizManagerManageWidget::handleAdd()
{
    bool ok;
    int accountId = account->data(account->index(ui->tableViewAccount->currentIndex().row(), 0))
                        .toInt(&ok);
    if (!ok) {
        return;
    }
    int bizId = biz->data(biz->index(ui->tableViewBiz->currentIndex().row(), 0)).toInt(&ok);
    if (!ok) {
        return;
    }
    for (int i = bizManager->rowCount(); i-- > 0;) {
        if (bizManager->data(bizManager->index(i, bizManager->fieldIndex("account_id"))) == accountId
            && bizManager->data(bizManager->index(i, bizManager->fieldIndex("biz_id"))) == bizId) {
            return;
        }
    }
    QSqlRecord record = bizManager->record();
    record.setValue("account_id", accountId);
    record.setValue("biz_id", bizId);
    record.setValue("biz_manager_root", 1);
    bizManager->insertRecord(-1, record);
}

void BizManagerManageWidget::handleDelete()
{
    bizManager->removeRow(ui->tableViewBizManager->currentIndex().row());
}

void BizManagerManageWidget::handleSubmit()
{
    if (!bizManager->submitAll()) {
        QMessageBox::information(this, tr("执行出错"), tr("是否已经是管理员"));
        bizManager->revertAll();
    }
}
