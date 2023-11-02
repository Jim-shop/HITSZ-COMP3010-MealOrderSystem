#ifndef CANTEEN_MANAGER_MANAGE_CANTEEN_MANAGER_WIDGET_H
#define CANTEEN_MANAGER_MANAGE_CANTEEN_MANAGER_WIDGET_H

#include <QSqlDatabase>
#include <QSqlRelationalTableModel>
#include <QWidget>
#include "buzzing_widget.h"

namespace Ui {
class CanteenManagerManageCanteenManagerWidget;
}

class CanteenManagerManageCanteenManagerWidget : public BuzzingWidget
{
    Q_OBJECT
    Q_PROPERTY(int canteenId READ canteenId WRITE setCanteenId NOTIFY canteenIdChanged FINAL)

public:
    explicit CanteenManagerManageCanteenManagerWidget(QSqlDatabase &db, QWidget *parent = nullptr);
    ~CanteenManagerManageCanteenManagerWidget();

    void setCanteenId(int canteenID);
    int canteenId() const;

private:
    Ui::CanteenManagerManageCanteenManagerWidget *ui;
    QSqlDatabase &db;
    QSqlRelationalTableModel *account, *canteenManager;
    int _canteenId;

private slots:
    void handleAdd();
    void handleDelete();
    void handleSubmit();

signals:
    void canteenIdChanged();
};

#endif // CANTEEN_MANAGER_MANAGE_CANTEEN_MANAGER_WIDGET_H
