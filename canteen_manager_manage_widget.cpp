#include "canteen_manager_manage_widget.h"
#include "ui_canteen_manager_manage_widget.h"

#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

CanteenManagerManageWidget::CanteenManagerManageWidget(QSqlDatabase &db, QWidget *parent)
    : BuzzingWidget(parent)
    , ui(new Ui::CanteenManagerManageWidget)
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

    canteen = new QSqlRelationalTableModel(this, db);
    canteen->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    canteen->setTable("canteen");
    canteen->setHeaderData(canteen->fieldIndex("canteen_id"), Qt::Horizontal, tr("ID"));
    canteen->setHeaderData(canteen->fieldIndex("canteen_name"), Qt::Horizontal, tr("名称"));
    canteen->setHeaderData(canteen->fieldIndex("canteen_desc"), Qt::Horizontal, tr("描述"));
    canteen->setHeaderData(canteen->fieldIndex("canteen_pic"), Qt::Horizontal, tr("图片"));
    canteen->setHeaderData(canteen->fieldIndex("canteen_enable"), Qt::Horizontal, tr("启用"));
    canteen->select();

    ui->tableViewCanteen->setModel(canteen);
    ui->tableViewCanteen->hideColumn(canteen->fieldIndex("canteen_desc"));
    ui->tableViewCanteen->hideColumn(canteen->fieldIndex("canteen_pic"));

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
    canteenManager->select();

    ui->tableViewCanteenManager->setModel(canteenManager);

    connect(ui->pushButtonAdd, &QPushButton::clicked, this, &CanteenManagerManageWidget::handleAdd);
    connect(ui->pushButtonDelete,
            &QPushButton::clicked,
            this,
            &CanteenManagerManageWidget::handleDelete);
    connect(ui->pushButtonSubmit,
            &QPushButton::clicked,
            this,
            &CanteenManagerManageWidget::handleSubmit);
}

CanteenManagerManageWidget::~CanteenManagerManageWidget()
{
    delete ui;
    delete account;
    delete canteen;
    delete canteenManager;
}

void CanteenManagerManageWidget::handleAdd()
{
    bool ok;
    int accountId = account->data(account->index(ui->tableViewAccount->currentIndex().row(), 0))
                        .toInt(&ok);
    if (!ok) {
        return;
    }
    int canteenId = canteen->data(canteen->index(ui->tableViewCanteen->currentIndex().row(), 0))
                        .toInt(&ok);
    if (!ok) {
        return;
    }
    for (int i = canteenManager->rowCount(); i-- > 0;) {
        if (canteenManager->data(canteenManager->index(i, canteenManager->fieldIndex("account_id")))
                == accountId
            && canteenManager->data(
                   canteenManager->index(i, canteenManager->fieldIndex("canteen_id")))
                   == canteenId) {
            return;
        }
    }
    QSqlRecord record = canteenManager->record();
    record.setValue("account_id", accountId);
    record.setValue("canteen_id", canteenId);
    record.setValue("canteen_manager_root", 1);
    canteenManager->insertRecord(-1, record);
}

void CanteenManagerManageWidget::handleDelete()
{
    canteenManager->removeRow(ui->tableViewCanteenManager->currentIndex().row());
}

void CanteenManagerManageWidget::handleSubmit()
{
    if (!canteenManager->submitAll()) {
        QMessageBox::information(this, tr("执行出错"), tr("是否已经是管理员"));
        canteenManager->revertAll();
    }
}
