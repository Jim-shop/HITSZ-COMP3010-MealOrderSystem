#include "canteen_manager_dao.h"

#include <QSqlQuery>

CanteenManagerDao::CanteenManagerDao(QSqlDatabase &db)
    : db(db)
{}

CanteenManagerDao::CanteenManagerInfoListResult CanteenManagerDao::getCanteens(int accountId)
{
    QSqlQuery query(db);
    query.prepare(
        "SELECT canteen_id, canteen_manager_root FROM canteen_manager WHERE account_id = ?");
    query.addBindValue(accountId);
    query.exec();
    query.first();
    QList<CanteenManagerInfo> result;
    while (query.isValid()) {
        result.append(
            {query.value("canteen_id").toInt(), query.value("canteen_manager_root").toBool()});
        query.next();
    }
    if (result.size() == 0) {
        return {false, QObject::tr("没找到任何食堂管理员身份"), result};
    }
    return {true, "", result};
}

bool CanteenManagerDao::isManager(int accountId, int canteenId)
{
    QSqlQuery query(db);
    query.prepare("SELECT * FROM canteen_manager WHERE account_id = ? AND canteen_id = ?");
    query.addBindValue(accountId);
    query.addBindValue(canteenId);
    query.exec();
    return (query.numRowsAffected() == 1);
}
