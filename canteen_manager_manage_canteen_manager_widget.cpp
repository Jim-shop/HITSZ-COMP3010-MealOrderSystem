#include "canteen_manager_manage_canteen_manager_widget.h"
#include "ui_canteen_manager_manage_canteen_manager_widget.h"

#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

CanteenManagerManageCanteenManagerWidget::CanteenManagerManageCanteenManagerWidget(QSqlDatabase &db,
                                                                                   QWidget *parent)
    : BuzzingWidget(parent)
    , ui(new Ui::CanteenManagerManageCanteenManagerWidget)
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

    canteenManager = new QSqlRelationalTableModel(this, db);
    canteenManager->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    canteenManager->setTable("canteen_manager");
    canteenManager->setHeaderData(canteenManager->fieldIndex("account_id"),
                                  Qt::Horizontal,
                                  tr("账户ID"));
    canteenManager->setHeaderData(canteenManager->fieldIndex("canteen_id"),
                                  Qt::Horizontal,
                                  tr("餐厅ID"));
    canteenManager->setHeaderData(canteenManager->fieldIndex("canteen_manager_root"),
                                  Qt::Horizontal,
                                  tr("特权用户"));

    ui->tableViewCanteenManager->setModel(canteenManager);
    ui->tableViewCanteenManager->hideColumn(canteenManager->fieldIndex("canteen_id"));

    connect(ui->pushButtonAdd,
            &QPushButton::clicked,
            this,
            &CanteenManagerManageCanteenManagerWidget::handleAdd);
    connect(ui->pushButtonDelete,
            &QPushButton::clicked,
            this,
            &CanteenManagerManageCanteenManagerWidget::handleDelete);
    connect(ui->pushButtonSubmit,
            &QPushButton::clicked,
            this,
            &CanteenManagerManageCanteenManagerWidget::handleSubmit);
}

CanteenManagerManageCanteenManagerWidget::~CanteenManagerManageCanteenManagerWidget()
{
    delete ui;
    delete account;
    delete canteenManager;
}

void CanteenManagerManageCanteenManagerWidget::setCanteenId(int canteenId)
{
    _canteenId = canteenId;
    canteenManager->setFilter(QString("canteen_id = %1").arg(canteenId));
    canteenManager->select();
    emit canteenIdChanged();
}

int CanteenManagerManageCanteenManagerWidget::canteenId() const
{
    return _canteenId;
}

void CanteenManagerManageCanteenManagerWidget::handleAdd()
{
    bool ok;
    int accountId = account->data(account->index(ui->tableViewAccount->currentIndex().row(), 0))
                        .toInt(&ok);
    if (!ok) {
        return;
    }
    for (int i = canteenManager->rowCount(); i-- > 0;) {
        if (canteenManager->data(canteenManager->index(i, canteenManager->fieldIndex("account_id")))
            == accountId) {
            return;
        }
    }
    QSqlRecord record = canteenManager->record();
    record.setValue("account_id", accountId);
    record.setValue("canteen_id", _canteenId);
    record.setValue("canteen_manager_root", 0);
    canteenManager->insertRecord(-1, record);
}

void CanteenManagerManageCanteenManagerWidget::handleDelete()
{
    canteenManager->removeRow(ui->tableViewCanteenManager->currentIndex().row());
}

void CanteenManagerManageCanteenManagerWidget::handleSubmit()
{
    if (!canteenManager->submitAll()) {
        QMessageBox::information(this, tr("执行出错"), tr("是否已经是管理员"));
    }
    canteenManager->select();
}
