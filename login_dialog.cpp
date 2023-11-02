#include "login_dialog.h"
#include <QCryptographicHash>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QSqlQuery>
#include "ui_login_dialog.h"

LoginDialog::LoginDialog(QSqlDatabase &db, QDialog *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
    , db(db)
    , accountDao(db)
{
    ui->setupUi(this);

    connect(ui->pushButtonLogin, &QPushButton::clicked, this, &LoginDialog::handleLogin);
    connect(ui->pushButtonRegister, &QPushButton::clicked, this, &LoginDialog::handleRegister);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::reload()
{
    ui->lineEditPhone->setText("");
    ui->lineEditPassword->setText("");
}

void LoginDialog::handleLogin()
{
    // Check input
    QString phone = ui->lineEditPhone->text();
    QString password = ui->lineEditPassword->text();
    if (!accountDao.verifyPhoneFormat(phone)) {
        QMessageBox::information(this, tr("请检查输入"), tr("账户太短"));
        return;
    }
    if (!accountDao.verifyPasswordFormat(password)) {
        QMessageBox::information(this, tr("请检查输入"), tr("密码太短"));
        return;
    }
    // Send to Dao
    auto &&[ok, error, accountId] = accountDao.login(phone, password);
    // Responce
    if (!ok) {
        QMessageBox::information(this, tr("请检查输入"), error);
        return;
    }
    emit login(accountId);
}

void LoginDialog::handleRegister()
{
    // Check input
    QString phone = ui->lineEditPhone->text();
    QString password = ui->lineEditPassword->text();
    if (!accountDao.verifyPhoneFormat(phone)) {
        QMessageBox::information(this, tr("请检查输入"), tr("账户太短"));
        return;
    }
    if (!accountDao.verifyPasswordFormat(password)) {
        QMessageBox::information(this, tr("请检查输入"), tr("密码太短"));
        return;
    }
    // Send to Dao
    auto &&[ok, error, accountId] = accountDao.regist(phone, password);
    // Responce
    if (!ok) {
        QMessageBox::information(this, tr("用户已存在"), error);
        return;
    }
    emit login(accountId);
}
