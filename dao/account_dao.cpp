#include "account_dao.h"
#include <QCryptographicHash>
#include <QRandomGenerator>
#include <QSqlError>
#include <QSqlQuery>

AccountDao::AccountDao(QSqlDatabase &db)
    : db(db)
{}

QString AccountDao::getSaltedPassword(QString password, QString salt)
{
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData((password + salt).toLocal8Bit());
    return hash.result().toHex();
}

AccountDao::SaltAndPassword AccountDao::generateSaltedPassword(QString password)
{
    QString salt = QString::number(QRandomGenerator::global()->bounded(1000));
    return {getSaltedPassword(password, salt), salt};
}

AccountDao::AccountIdResult AccountDao::regist(QString phone, QString password)
{
    auto &&[salted, salt] = generateSaltedPassword(password);
    QSqlQuery query(db);
    query.prepare("INSERT INTO account (account_phone, account_password, account_salt, "
                  "account_enable) VALUES (?, ?, ?, 1)");
    query.addBindValue(phone);
    query.addBindValue(salted);
    query.addBindValue(salt);
    query.exec();
    if (query.numRowsAffected() != 1) {
        return {false, QObject::tr("手机号已被使用"), 0};
    } else {
        return {true, "", query.lastInsertId().toInt()};
    }
}

AccountDao::AccountIdResult AccountDao::login(QString phone, QString password)
{
    QSqlQuery query(db);
    query.prepare("SELECT account_id, account_password, account_salt FROM account WHERE "
                  "account_phone = ? and account_enable = 1");
    query.addBindValue(phone);
    query.exec();
    if (query.numRowsAffected() != 1) {
        return {false, QObject::tr("用户不存在或用户被禁用"), 0};
    }

    query.first();
    QString salt = query.value("account_salt").toString();
    QString salted = getSaltedPassword(password, salt);
    QString rightSalted = query.value("account_password").toString();

    if (salted != rightSalted) {
        return {false, QObject::tr("密码错误"), 0};
    }

    return {true, "", query.value("account_id").toInt()};
}

AccountDao::VoidResult AccountDao::resetPassword(int accountId,
                                                 QString oldPassword,
                                                 QString newPassword)
{
    QSqlQuery query1(db);
    query1.prepare("SELECT account_salt FROM account WHERE account_id = ? and account_enable = 1");
    query1.addBindValue(accountId);
    query1.exec();
    if (query1.numRowsAffected() != 1) {
        return {false, QObject::tr("找不到用户或用户被禁用")};
    }

    query1.first();
    QString oldSalt = query1.value("account_salt").toString();
    auto &&[newSalted, newSalt] = generateSaltedPassword(newPassword);
    QString oldSalted = getSaltedPassword(oldPassword, oldSalt);
    query1.finish();

    QSqlQuery query2(db);
    query2.prepare("UPDATE account SET account_password = ?, account_salt = ? WHERE account_id = ? "
                   "and account_enable = 1 and account_password = ?");
    query2.addBindValue(newSalted);
    query2.addBindValue(newSalt);
    query2.addBindValue(accountId);
    query2.addBindValue(oldSalted);
    query2.exec();
    if (query2.numRowsAffected() != 1) {
        return {false, QObject::tr("密码错误")};
    }

    return {true, ""};
}

AccountDao::VoidResult AccountDao::changePhone(int accountId, QString phone)
{
    QSqlQuery query(db);
    query.prepare(
        "UPDATE account SET account_phone = ? WHERE account_id = ? and account_enable = 1");
    query.addBindValue(phone);
    query.addBindValue(accountId);
    query.exec();
    if (query.numRowsAffected() != 1) {
        return {false, QObject::tr("手机号已注册或当前用户异常")};
    }

    return {true, ""};
}

bool AccountDao::verifyPhoneFormat(QString phone)
{
    return phone.length() > 0;
}
bool AccountDao::verifyPasswordFormat(QString password)
{
    return password.length() > 0;
}

bool AccountDao::isRoot(int accountId)
{
    return accountId == 1;
}
