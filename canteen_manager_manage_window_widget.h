#ifndef CANTEEN_MANAGER_MANAGE_WINDOW_WIDGET_H
#define CANTEEN_MANAGER_MANAGE_WINDOW_WIDGET_H

#include <QSqlDatabase>
#include <QSqlRelationalTableModel>
#include <QWidget>
#include "buzzing_widget.h"

namespace Ui {
class CanteenManagerManageWindowWidget;
}

class CanteenManagerManageWindowWidget : public BuzzingWidget
{
    Q_OBJECT
    Q_PROPERTY(int canteenId READ canteenId WRITE setCanteenId NOTIFY canteenIdChanged FINAL)

public:
    explicit CanteenManagerManageWindowWidget(QSqlDatabase &db, QWidget *parent = nullptr);
    ~CanteenManagerManageWindowWidget();

    void setCanteenId(int canteenID);
    int canteenId() const;

private:
    Ui::CanteenManagerManageWindowWidget *ui;
    QSqlDatabase &db;
    QSqlRelationalTableModel *window;
    int _canteenId;

private slots:
    void handleAdd();
    void handleDelete();
    void handleSubmit();

signals:
    void canteenIdChanged();
};

#endif // CANTEEN_MANAGER_MANAGE_WINDOW_WIDGET_H
