#ifndef CANTEENDAO_H
#define CANTEENDAO_H

#include <QList>
#include <QSqlDatabase>
#include "result.h"

class CanteenDao final
{
public:
    explicit CanteenDao(QSqlDatabase &db);

    Result<QString> getName(int canteenId);
    struct CanteenInfo
    {
        QString name, desc;
        QByteArray pic;
    };
    Result<CanteenInfo> getInfo(int canteenId);
    Result<void> setInfo(int canteenId, CanteenInfo canteenInfo);
    Result<QList<CanteenInfo>> getAllCanteenInfo();

private:
    QSqlDatabase &db;
};

#endif // CANTEENDAO_H
