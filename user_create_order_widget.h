#ifndef USER_CREATE_ORDER_WIDGET_H
#define USER_CREATE_ORDER_WIDGET_H

#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlRelationalTableModel>
#include <QStandardItemModel>
#include <QWidget>
#include "buzzing_widget.h"
#include "dao/order_dao.h"

namespace Ui {
class UserCreateOrderWidget;
}

class UserCreateOrderWidget : public BuzzingWidget
{
    Q_OBJECT
    Q_PROPERTY(int userId READ userId WRITE setUserId NOTIFY userIdChanged FINAL)

public:
    explicit UserCreateOrderWidget(QSqlDatabase &db, QWidget *parent = nullptr);
    ~UserCreateOrderWidget();

    void setUserId(int userID);
    int userId() const;

private:
    Ui::UserCreateOrderWidget *ui;
    QSqlDatabase &db;
    OrderDao orderDao;
    QSqlRelationalTableModel *canteen, *meal, *destination;
    QSqlQueryModel *biz;
    QStandardItemModel *cart;
    int _userId;
    double price;

private slots:
    void onSelectCanteen();
    void onSelectBiz();
    void onAdd();
    void onDelete();
    void onSubmit();

signals:
    void userIdChanged();
};

#endif // USER_CREATE_ORDER_WIDGET_H
