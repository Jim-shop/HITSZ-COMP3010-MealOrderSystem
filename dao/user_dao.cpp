#include "user_dao.h"
#include <QSqlQuery>

UserDao::UserDao(QSqlDatabase &db)
    : db(db)
{}

UserDao::UserIdResult UserDao::getUserId(int accountId)
{
    QSqlQuery query(db);
    query.prepare("SELECT user_id FROM user WHERE account_id = ? and user_enable = 1");
    query.addBindValue(accountId);
    query.exec();
    if (query.numRowsAffected() != 1) {
        return {false, QObject::tr("用户不存在或已被禁用"), 0};
    }
    query.first();
    return {true, "", query.value("user_id").toInt()};
}

UserDao::UserIdResult UserDao::registerUser(int accountId, QString userName)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO user(account_id, user_name, user_enable) VALUES (?, ?, 1)");
    query.addBindValue(accountId);
    query.addBindValue(userName);
    query.exec();
    if (query.numRowsAffected() != 1) {
        return {false, QObject::tr("已经是用户"), 0};
    }
    query.first();
    return {true, "", query.lastInsertId().toInt()};
}

Result<UserDao::UserInfo> UserDao::getInfo(int userId)
{
    QSqlQuery query(db);
    query.prepare("SELECT user_name, user_pic FROM user WHERE user_id = ?");
    query.addBindValue(userId);
    query.exec();
    if (query.numRowsAffected() != 1) {
        return {false, QObject::tr("找不到用户"), {}};
    }
    query.first();
    return {true, "", {query.value("user_name").toString(), query.value("user_pic").toByteArray()}};
}

Result<void> UserDao::setInfo(int userId, UserDao::UserInfo userInfo)
{
    QSqlQuery query(db);
    query.prepare("UPDATE user SET user_name = ?, user_pic = ? WHERE user_id = ?");
    query.addBindValue(userInfo.name);
    query.addBindValue(userInfo.pic);
    query.addBindValue(userId);
    query.exec();
    return {true, ""};
}
