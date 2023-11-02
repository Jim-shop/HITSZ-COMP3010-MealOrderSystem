#include "account_manage_widget.h"
#include <QMessageBox>
#include <QSqlError>
#include "dao/account_dao.h"
#include "ui_account_manage_widget.h"

AccountManageWidget::AccountManageWidget(QSqlDatabase &db, QWidget *parent)
    : BuzzingWidget(parent)
    , ui(new Ui::AccountManageWidget)
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

    ui->tableView->setModel(account);

    mapper = new QDataWidgetMapper;
    mapper->setModel(account);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    mapper->setOrientation(Qt::Horizontal);
    mapper->addMapping(ui->lineEditPhone, account->fieldIndex("account_phone"));
    mapper->addMapping(ui->checkBoxEnable, account->fieldIndex("account_enable"));

    connect(ui->tableView->selectionModel(),
            &QItemSelectionModel::currentRowChanged,
            mapper,
            &QDataWidgetMapper::setCurrentModelIndex);
    connect(ui->pushButtonAdd, &QPushButton::clicked, this, &AccountManageWidget::handleAdd);
    connect(ui->pushButtonDelete, &QPushButton::clicked, this, &AccountManageWidget::handleDelete);
    connect(ui->pushButtonSetPassword,
            &QPushButton::clicked,
            this,
            &AccountManageWidget::handleSetPassword);
    connect(ui->pushButtonSubmit, &QPushButton::clicked, this, &AccountManageWidget::handleSubmit);
}

AccountManageWidget::~AccountManageWidget()
{
    delete ui;
    delete account;
    delete mapper;
}

void AccountManageWidget::handleAdd()
{
    account->insertRow(account->rowCount());
}

void AccountManageWidget::handleDelete()
{
    account->removeRow(mapper->currentIndex());
}

void AccountManageWidget::handleSetPassword()
{
    QString password = ui->lineEditPassword->text();
    if (!AccountDao::verifyPasswordFormat(password)) {
        return;
    }
    auto &&[salted, salt] = AccountDao::generateSaltedPassword(password);
    int row = mapper->currentIndex();
    account->setData(account->index(row, account->fieldIndex("account_password")), salted);
    account->setData(account->index(row, account->fieldIndex("account_salt")), salt);
}

void AccountManageWidget::handleSubmit()
{
    if (!account->submitAll()) {
        QMessageBox::information(this, tr("请检查输入"), account->lastError().text());
    }
    account->select();
}
