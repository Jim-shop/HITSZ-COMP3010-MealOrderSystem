#include "biz_dao.h"

#include <QSqlError>
#include <QSqlQuery>

BizDao::BizDao(QSqlDatabase &db)
    : db(db)
{}

Result<QString> BizDao::getName(int bizId)
{
    QSqlQuery query(db);
    query.prepare("SELECT biz_name FROM biz WHERE biz_id = ?");
    query.addBindValue(bizId);
    query.exec();
    if (query.numRowsAffected() != 1) {
        return {false, QObject::tr("找不到商家名称"), ""};
    }
    query.first();
    return {true, "", query.value("biz_name").toString()};
}

Result<BizDao::BizInfo> BizDao::getInfo(int bizId)
{
    QSqlQuery query(db);
    query.prepare("SELECT biz_name, biz_desc, biz_pic FROM biz WHERE biz_id = ?");
    query.addBindValue(bizId);
    query.exec();
    if (query.numRowsAffected() != 1) {
        return {false, QObject::tr("找不到商家名称"), {}};
    }
    query.first();
    return {true,
            "",
            {query.value("biz_name").toString(),
             query.value("biz_desc").toString(),
             query.value("biz_pic").toByteArray()}};
}

Result<void> BizDao::setInfo(int bizId, BizDao::BizInfo bizInfo)
{
    QSqlQuery query(db);
    query.prepare("UPDATE biz SET biz_name = ?, biz_desc = ?, biz_pic = ? WHERE biz_id = ?");
    query.addBindValue(bizInfo.name);
    query.addBindValue(bizInfo.desc);
    query.addBindValue(bizInfo.pic);
    query.addBindValue(bizId);
    query.exec();
    return {true, ""};
}
