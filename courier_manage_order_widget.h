#ifndef COURIER_MANAGE_ORDER_WIDGET_H
#define COURIER_MANAGE_ORDER_WIDGET_H

#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlRelationalTableModel>
#include <QWidget>
#include "buzzing_widget.h"
#include "dao/order_dao.h"

namespace Ui {
class CourierManageOrderWidget;
}

class CourierManageOrderWidget : public BuzzingWidget
{
    Q_OBJECT
    Q_PROPERTY(int courierId READ courierId WRITE setCourierId NOTIFY courierIdChanged FINAL)

public:
    explicit CourierManageOrderWidget(QSqlDatabase &db, QWidget *parent = nullptr);
    ~CourierManageOrderWidget();

    void setCourierId(int courierId);
    int courierId() const;

private:
    Ui::CourierManageOrderWidget *ui;
    QSqlDatabase &db;
    OrderDao orderDao;
    QSqlRelationalTableModel *orderPairing, *orderSending;
    int _courierId;

private slots:
    void handleTake();
    void handleFinish();

signals:
    void courierIdChanged();
};

#endif // COURIER_MANAGE_ORDER_WIDGET_H
