#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include "dao/account_dao.h"

QT_BEGIN_NAMESPACE
namespace Ui { class LoginDialog; }
QT_END_NAMESPACE

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QSqlDatabase &db, QDialog *parent = nullptr);
    ~LoginDialog();

private:
    Ui::LoginDialog *ui;
    QSqlDatabase &db;
    AccountDao accountDao;

private slots:
    void handleLogin();
    void handleRegister();

public slots:
    void reload();

signals:
    void login(int accountId);
};

#endif // LOGINDIALOG_H
