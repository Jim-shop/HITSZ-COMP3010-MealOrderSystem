#include "biz_manager_manage_biz_manager_widget.h"
#include "ui_biz_manager_manage_biz_manager_widget.h"

#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

BizManagerManageBizManagerWidget::BizManagerManageBizManagerWidget(QSqlDatabase &db, QWidget *parent)
    : BuzzingWidget(parent)
    , ui(new Ui::BizManagerManageBizManagerWidget)
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

    bizManager = new QSqlRelationalTableModel(this, db);
    bizManager->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    bizManager->setTable("biz_manager");
    bizManager->setHeaderData(bizManager->fieldIndex("account_id"), Qt::Horizontal, tr("账户ID"));
    bizManager->setHeaderData(bizManager->fieldIndex("biz_id"), Qt::Horizontal, tr("餐厅ID"));
    bizManager->setHeaderData(bizManager->fieldIndex("biz_manager_root"),
                              Qt::Horizontal,
                              tr("特权用户"));
    ui->tableViewBizManager->setModel(bizManager);
    ui->tableViewBizManager->hideColumn(bizManager->fieldIndex("biz_id"));

    connect(ui->pushButtonAdd,
            &QPushButton::clicked,
            this,
            &BizManagerManageBizManagerWidget::handleAdd);
    connect(ui->pushButtonDelete,
            &QPushButton::clicked,
            this,
            &BizManagerManageBizManagerWidget::handleDelete);
    connect(ui->pushButtonSubmit,
            &QPushButton::clicked,
            this,
            &BizManagerManageBizManagerWidget::handleSubmit);
}

BizManagerManageBizManagerWidget::~BizManagerManageBizManagerWidget()
{
    delete ui;
    delete account;
    delete bizManager;
}

void BizManagerManageBizManagerWidget::setBizId(int bizId)
{
    _bizId = bizId;
    bizManager->setFilter(QString("biz_id = %1").arg(bizId));
    bizManager->select();
    emit bizIdChanged();
}

int BizManagerManageBizManagerWidget::bizId() const
{
    return _bizId;
}

void BizManagerManageBizManagerWidget::handleAdd()
{
    bool ok;
    int accountId = account->data(account->index(ui->tableViewAccount->currentIndex().row(), 0))
                        .toInt(&ok);
    if (!ok) {
        return;
    }
    for (int i = bizManager->rowCount(); i-- > 0;) {
        if (bizManager->data(bizManager->index(i, bizManager->fieldIndex("account_id")))
            == accountId) {
            return;
        }
    }
    QSqlRecord record = bizManager->record();
    record.setValue("account_id", accountId);
    record.setValue("biz_id", _bizId);
    record.setValue("biz_manager_root", 0);
    bizManager->insertRecord(-1, record);
}

void BizManagerManageBizManagerWidget::handleDelete()
{
    bizManager->removeRow(ui->tableViewBizManager->currentIndex().row());
}

void BizManagerManageBizManagerWidget::handleSubmit()
{
    if (!bizManager->submitAll()) {
        QMessageBox::information(this, tr("执行出错"), tr("是否已经是管理员"));
    }
    bizManager->select();
}
