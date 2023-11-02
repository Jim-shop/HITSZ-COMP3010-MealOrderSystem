#include "order_dao.h"
#include <QDateTime>
#include <QSqlQuery>

OrderDao::OrderDao(QSqlDatabase &db)
    : db(db)
{}

Result<void> OrderDao::sendOrder(int userId,
                                 int destinationId,
                                 double orderPrice,
                                 QList<OrderDao::OrderMealInfo> info)
{
    QSqlQuery query1(db);
    query1.prepare(
        "INSERT INTO order_(user_id, destination_id, order_state, order_price, order_time) VALUES "
        "(?, ?, 'init', ?, ?)");
    query1.addBindValue(userId);
    query1.addBindValue(destinationId);
    query1.addBindValue(orderPrice);
    query1.addBindValue(QDateTime::currentDateTime());
    query1.exec();
    query1.first();
    int orderId = query1.lastInsertId().toInt();
    query1.finish();

    QSqlQuery query2(db);
    query2.prepare("INSERT INTO order_meal(meal_id, order_id, amount) VALUES (?, ?, ?)");
    for (auto &&[mealId, amount] : info) {
        query2.addBindValue(mealId);
        query2.addBindValue(orderId);
        query2.addBindValue(amount);
        query2.exec();
    }
    return {true, ""};
}

Result<void> OrderDao::offer(int orderId)
{
    QSqlQuery query(db);
    query.prepare("UPDATE order_ SET order_state = 'offered' WHERE order_id = ?");
    query.addBindValue(orderId);
    query.exec();
    return {true, ""};
}

Result<void> OrderDao::take(int orderId)
{
    QSqlQuery query(db);
    query.prepare("UPDATE order_ SET order_state = 'sending' WHERE order_id = ?");
    query.addBindValue(orderId);
    query.exec();
    return {true, ""};
}

Result<void> OrderDao::finish(int orderId)
{
    QSqlQuery query(db);
    query.prepare("UPDATE order_ SET order_state = 'finish' WHERE order_id = ?");
    query.addBindValue(orderId);
    query.exec();
    return {true, ""};
}
