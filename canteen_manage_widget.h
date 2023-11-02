#ifndef CANTEEN_MANAGE_WIDGET_H
#define CANTEEN_MANAGE_WIDGET_H

#include <QSqlDatabase>
#include <QSqlRelationalTableModel>
#include <QWidget>
#include "buzzing_widget.h"

namespace Ui {
class CanteenManageWidget;
}

class CanteenManageWidget : public BuzzingWidget
{
    Q_OBJECT

public:
    explicit CanteenManageWidget(QSqlDatabase &db, QWidget *parent = nullptr);
    ~CanteenManageWidget();

private:
    Ui::CanteenManageWidget *ui;
    QSqlDatabase &db;
    QSqlRelationalTableModel *canteen;

private slots:
    void handleAdd();
    void handleDelete();
    void handleSubmit();
};

#endif // CANTEEN_MANAGE_WIDGET_H
