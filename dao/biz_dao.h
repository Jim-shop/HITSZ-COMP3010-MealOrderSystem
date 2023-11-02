#ifndef BIZDAO_H
#define BIZDAO_H

#include <QSqlDatabase>
#include "result.h"

class BizDao final
{
public:
    explicit BizDao(QSqlDatabase &db);

    Result<QString> getName(int bizId);
    struct BizInfo
    {
        QString name, desc;
        QByteArray pic;
    };
    Result<BizInfo> getInfo(int bizId);
    Result<void> setInfo(int bizId, BizInfo bizInfo);

private:
    QSqlDatabase &db;
};

#endif // BIZDAO_H
