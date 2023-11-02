#ifndef CANTEEN_MANAGER_MANAGE_BIZ_WIDGET_H
#define CANTEEN_MANAGER_MANAGE_BIZ_WIDGET_H

#include <QSqlDatabase>
#include <QSqlRelationalTableModel>
#include <QWidget>
#include "buzzing_widget.h"

namespace Ui {
class CanteenManagerManageBizWidget;
}

class CanteenManagerManageBizWidget : public BuzzingWidget
{
    Q_OBJECT
    Q_PROPERTY(int canteenId READ canteenId WRITE setCanteenId NOTIFY canteenIdChanged FINAL)

public:
    explicit CanteenManagerManageBizWidget(QSqlDatabase &db, QWidget *parent = nullptr);
    ~CanteenManagerManageBizWidget();

    void setCanteenId(int canteenID);
    int canteenId() const;

private:
    Ui::CanteenManagerManageBizWidget *ui;
    QSqlDatabase &db;
    QSqlRelationalTableModel *window, *biz;
    int _canteenId;

private slots:
    void handleAssign();
    void handleDeassign();

signals:
    void canteenIdChanged();
};

#endif // CANTEEN_MANAGER_MANAGE_BIZ_WIDGET_H
