#include "canteen_dao.h"

#include <QSqlQuery>

CanteenDao::CanteenDao(QSqlDatabase &db)
    : db(db)
{}

Result<QString> CanteenDao::getName(int canteenId)
{
    QSqlQuery query(db);
    query.prepare("SELECT canteen_name FROM canteen WHERE canteen_id = ?");
    query.addBindValue(canteenId);
    query.exec();
    if (query.numRowsAffected() != 1) {
        return {false, QObject::tr("找不到食堂名称"), ""};
    }
    query.first();
    return {true, "", query.value("canteen_name").toString()};
}

Result<CanteenDao::CanteenInfo> CanteenDao::getInfo(int canteenId)
{
    QSqlQuery query(db);
    query.prepare(
        "SELECT canteen_name, canteen_desc, canteen_pic FROM canteen WHERE canteen_id = ?");
    query.addBindValue(canteenId);
    query.exec();
    if (query.numRowsAffected() != 1) {
        return {false, QObject::tr("找不到食堂名称"), {}};
    }
    query.first();
    return {true,
            "",
            {query.value("canteen_name").toString(),
             query.value("canteen_desc").toString(),
             query.value("canteen_pic").toByteArray()}};
}

Result<void> CanteenDao::setInfo(int canteenId, CanteenDao::CanteenInfo canteenInfo)
{
    QSqlQuery query(db);
    query.prepare("UPDATE canteen SET canteen_name = ?, canteen_desc = ?, canteen_pic = ? WHERE "
                  "canteen_id = ?");
    query.addBindValue(canteenInfo.name);
    query.addBindValue(canteenInfo.desc);
    query.addBindValue(canteenInfo.pic);
    query.addBindValue(canteenId);
    query.exec();
    return {true, ""};
}

Result<QList<CanteenDao::CanteenInfo>> CanteenDao::getAllCanteenInfo()
{
    QSqlQuery query(db);
    query.prepare(
        "SELECT canteen_name, canteen_desc, canteen_pic FROM canteen WHERE canteen_enable = 1");
    query.exec();
    query.first();
    QList<CanteenInfo> result;
    while (query.isValid()) {
        result.append({
            query.value("canteen_name").toString(),
            query.value("canteen_desc").toString(),
            query.value("canteen_pic").toByteArray(),
        });
    }
    return {true, "", result};
}
