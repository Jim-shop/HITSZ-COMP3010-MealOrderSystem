#ifndef BIZMANAGERDAO_H
#define BIZMANAGERDAO_H

#include <QList>
#include <QSqlDatabase>
#include "result.h"

class BizManagerDao final
{
public:
    explicit BizManagerDao(QSqlDatabase &db);

    struct BizManagerInfo
    {
        int bizId;
        bool root;
    };
    typedef Result<QList<BizManagerInfo>> BizManagerInfoListResult;
    BizManagerInfoListResult getBizs(int accountId);

private:
    QSqlDatabase &db;
};

#endif // BIZMANAGERDAO_H
