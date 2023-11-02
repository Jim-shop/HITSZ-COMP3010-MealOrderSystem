#ifndef COURIERDAO_H
#define COURIERDAO_H

#include <QSqlDatabase>
#include "result.h"

class CourierDao final
{
public:
    explicit CourierDao(QSqlDatabase &db);

    typedef Result<int> CourierIdResult;

    CourierIdResult getCourierId(int accountId);
    static bool verifyPhoneFormat(QString phone);
    CourierIdResult registerCourier(int accountId, QString courierName, QString courierPhone);
    struct CourierInfo
    {
        QString name, phone;
        QByteArray pic;
    };
    Result<CourierInfo> getInfo(int courierId);
    Result<void> setInfo(int courierId, CourierInfo courierInfo);

private:
    QSqlDatabase &db;
};

#endif // COURIERDAO_H
