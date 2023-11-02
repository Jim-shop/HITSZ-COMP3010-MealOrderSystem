#ifndef USER_MANAGE_ORDER_WIDGET_H
#define USER_MANAGE_ORDER_WIDGET_H

#include <QSqlDatabase>
#include <QSqlRelationalTableModel>
#include <QWidget>
#include "buzzing_widget.h"

namespace Ui {
class UserManageOrderWidget;
}

class UserManageOrderWidget : public BuzzingWidget
{
    Q_OBJECT
    Q_PROPERTY(int userId READ userId WRITE setUserId NOTIFY userIdChanged FINAL)

public:
    explicit UserManageOrderWidget(QSqlDatabase &db, QWidget *parent = nullptr);
    ~UserManageOrderWidget();

    void setUserId(int userID);
    int userId() const;

private:
    Ui::UserManageOrderWidget *ui;
    QSqlDatabase &db;
    QSqlRelationalTableModel *order;
    int _userId;

signals:
    void userIdChanged();
};

#endif // USER_MANAGE_ORDER_WIDGET_H
