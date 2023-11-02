#ifndef ACCOUNTDAO_H
#define ACCOUNTDAO_H

#include <QSqlDatabase>
#include <QString>
#include "result.h"

class AccountDao final
{
public:
    explicit AccountDao(QSqlDatabase &db);

    typedef Result<int> AccountIdResult;
    typedef Result<void> VoidResult;

    static bool verifyPhoneFormat(QString phone);
    static bool verifyPasswordFormat(QString password);
    AccountIdResult regist(QString phone, QString password);
    AccountIdResult login(QString phone, QString password);
    VoidResult resetPassword(int accountId, QString oldPassword, QString newPassword);
    VoidResult changePhone(int accountId, QString phone);
    bool isRoot(int accountId);
    struct SaltAndPassword
    {
        QString salted, salt;
    };
    static SaltAndPassword generateSaltedPassword(QString password);

private:
    QSqlDatabase &db;
    static QString getSaltedPassword(QString password, QString salt);
};

#endif // ACCOUNTDAO_H
