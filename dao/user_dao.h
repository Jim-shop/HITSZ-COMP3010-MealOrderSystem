#ifndef USERDAO_H
#define USERDAO_H

#include <QSqlDatabase>
#include "result.h"

class UserDao final
{
public:
    explicit UserDao(QSqlDatabase &db);

    typedef Result<int> UserIdResult;

    UserIdResult getUserId(int accountId);
    UserIdResult registerUser(int accountId, QString userName);
    struct UserInfo
    {
        QString name;
        QByteArray pic;
    };
    Result<UserInfo> getInfo(int userId);
    Result<void> setInfo(int userId, UserInfo userInfo);

private:
    QSqlDatabase &db;
};

#endif // USERDAO_H
