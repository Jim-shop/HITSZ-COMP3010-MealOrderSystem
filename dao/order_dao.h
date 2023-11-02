#ifndef ORDERDAO_H
#define ORDERDAO_H

#include <QList>
#include <QSqlDatabase>
#include "result.h"

class OrderDao final
{
public:
    explicit OrderDao(QSqlDatabase &db);

    struct OrderMealInfo
    {
        int mealId;
        int amount;
    };
    Result<void> sendOrder(int userId,
                           int destinationId,
                           double orderPrice,
                           QList<OrderMealInfo> info);
    Result<void> offer(int orderId);
    Result<void> take(int orderId);
    Result<void> finish(int orderId);

private:
    QSqlDatabase &db;
};

#endif // ORDERDAO_H
