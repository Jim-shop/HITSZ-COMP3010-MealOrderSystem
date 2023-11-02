#include "biz_manager_dao.h"

#include <QSqlQuery>

BizManagerDao::BizManagerDao(QSqlDatabase &db)
    : db(db)
{}

BizManagerDao::BizManagerInfoListResult BizManagerDao::getBizs(int accountId)
{
    QSqlQuery query(db);
    query.prepare("SELECT biz_id, biz_manager_root FROM biz_manager WHERE account_id = ?");
    query.addBindValue(accountId);
    query.exec();
    query.first();
    QList<BizManagerInfo> result;
    while (query.isValid()) {
        result.append({query.value("biz_id").toInt(), query.value("biz_manager_root").toBool()});
        query.next();
    }
    if (result.size() == 0) {
        return {false, QObject::tr("没找到任何商家管理员身份"), result};
    }
    return {true, "", result};
}
