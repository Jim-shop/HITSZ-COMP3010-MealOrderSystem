#ifndef CANTEEN_MANAGER_MANAGE_WIDGET_H
#define CANTEEN_MANAGER_MANAGE_WIDGET_H

#include <QSqlDatabase>
#include <QSqlRelationalTableModel>
#include <QWidget>
#include "buzzing_widget.h"

namespace Ui {
class CanteenManagerManageWidget;
}

class CanteenManagerManageWidget : public BuzzingWidget
{
    Q_OBJECT

public:
    explicit CanteenManagerManageWidget(QSqlDatabase &db, QWidget *parent = nullptr);
    ~CanteenManagerManageWidget();

private:
    Ui::CanteenManagerManageWidget *ui;
    QSqlDatabase &db;
    QSqlRelationalTableModel *account, *canteen, *canteenManager;

private slots:
    void handleAdd();
    void handleDelete();
    void handleSubmit();
};

#endif // CANTEEN_MANAGER_MANAGE_WIDGET_H
