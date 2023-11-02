#ifndef ACCOUNT_MANAGE_WIDGET_H
#define ACCOUNT_MANAGE_WIDGET_H

#include <QDataWidgetMapper>
#include <QSqlDatabase>
#include <QSqlRelationalTableModel>
#include <QWidget>
#include "buzzing_widget.h"

namespace Ui {
class AccountManageWidget;
}

class AccountManageWidget : public BuzzingWidget
{
    Q_OBJECT

public:
    explicit AccountManageWidget(QSqlDatabase &db, QWidget *parent = nullptr);
    ~AccountManageWidget();

private:
    Ui::AccountManageWidget *ui;
    QSqlDatabase &db;
    QSqlRelationalTableModel *account;
    QDataWidgetMapper *mapper;

private slots:
    void handleAdd();
    void handleDelete();
    void handleSetPassword();
    void handleSubmit();
};

#endif // ACCOUNT_MANAGE_WIDGET_H
