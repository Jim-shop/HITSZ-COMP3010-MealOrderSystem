#ifndef CANTEENMANAGERDAO_H
#define CANTEENMANAGERDAO_H

#include <QList>
#include <QSqlDatabase>
#include "result.h"

class CanteenManagerDao final
{
public:
    explicit CanteenManagerDao(QSqlDatabase &db);

    struct CanteenManagerInfo
    {
        int canteenId;
        bool root;
    };
    typedef Result<QList<CanteenManagerInfo>> CanteenManagerInfoListResult;

    CanteenManagerInfoListResult getCanteens(int accountId);
    bool isManager(int accountId, int canteenId);

private:
    QSqlDatabase &db;
};

#endif // CANTEENMANAGERDAO_H
