#include "courier_dao.h"
#include <QSqlQuery>
#include "account_dao.h"

CourierDao::CourierDao(QSqlDatabase &db)
    : db(db)
{}

CourierDao::CourierIdResult CourierDao::getCourierId(int accountId)
{
    QSqlQuery query(db);
    query.prepare("SELECT courier_id FROM courier WHERE account_id = ? and courier_enable = 1");
    query.addBindValue(accountId);
    query.exec();
    if (query.numRowsAffected() != 1) {
        return {false, QObject::tr("派送员不存在或已被禁用"), 0};
    }
    query.first();
    return {true, "", query.value("courier_id").toInt()};
}

bool CourierDao::verifyPhoneFormat(QString phone)
{
    return AccountDao::verifyPhoneFormat(phone);
}

CourierDao::CourierIdResult CourierDao::registerCourier(int accountId,
                                                        QString courierName,
                                                        QString courierPhone)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO courier(account_id, courier_name, courier_phone, courier_enable) "
                  "VALUES (?, ?, ?, 1)");
    query.addBindValue(accountId);
    query.addBindValue(courierName);
    query.addBindValue(courierPhone);
    query.exec();
    if (query.numRowsAffected() != 1) {
        return {false, QObject::tr("已经是派送员"), 0};
    }
    query.first();
    return {true, "", query.lastInsertId().toInt()};
}

Result<CourierDao::CourierInfo> CourierDao::getInfo(int courierId)
{
    QSqlQuery query(db);
    query.prepare(
        "SELECT courier_name, courier_pic, courier_phone FROM courier WHERE courier_id = ?");
    query.addBindValue(courierId);
    query.exec();
    if (query.numRowsAffected() != 1) {
        return {false, QObject::tr("找不到派送员"), {}};
    }
    query.first();
    return {true,
            "",
            {query.value("courier_name").toString(),
             query.value("courier_phone").toString(),
             query.value("courier_pic").toByteArray()}};
}

Result<void> CourierDao::setInfo(int courierId, CourierDao::CourierInfo courierInfo)
{
    QSqlQuery query(db);
    query.prepare("UPDATE courier SET courier_name = ?, courier_phone = ?, courier_pic = ? WHERE "
                  "courier_id = ?");
    query.addBindValue(courierInfo.name);
    query.addBindValue(courierInfo.phone);
    query.addBindValue(courierInfo.pic);
    query.addBindValue(courierId);
    query.exec();
    return {true, ""};
}
