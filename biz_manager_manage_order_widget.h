#ifndef BIZ_MANAGER_MANAGE_ORDER_WIDGET_H
#define BIZ_MANAGER_MANAGE_ORDER_WIDGET_H

#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlRelationalTableModel>
#include <QWidget>
#include "buzzing_widget.h"
#include "dao/order_dao.h"

namespace Ui {
class BizManagerManageOrderWidget;
}

class BizManagerManageOrderWidget : public BuzzingWidget
{
    Q_OBJECT
    Q_PROPERTY(int bizId READ bizId WRITE setBizId NOTIFY bizIdChanged FINAL)

public:
    explicit BizManagerManageOrderWidget(QSqlDatabase &db, QWidget *parent = nullptr);
    ~BizManagerManageOrderWidget();

    void setBizId(int bizId);
    int bizId() const;

private:
    Ui::BizManagerManageOrderWidget *ui;
    QSqlDatabase &db;
    OrderDao orderDao;
    QSqlRelationalTableModel *orderMeal;
    QSqlQueryModel *order;
    int _bizId;
    void refreshOrder();

private slots:
    void handleOffer();
    void handleSelectOrder();

signals:
    void bizIdChanged();
};

#endif // BIZ_MANAGER_MANAGE_ORDER_WIDGET_H
