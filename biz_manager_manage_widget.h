#ifndef BIZ_MANAGER_MANAGE_WIDGET_H
#define BIZ_MANAGER_MANAGE_WIDGET_H

#include <QSqlDatabase>
#include <QSqlRelationalTableModel>
#include <QWidget>
#include "buzzing_widget.h"

namespace Ui {
class BizManagerManageWidget;
}

class BizManagerManageWidget : public BuzzingWidget
{
    Q_OBJECT

public:
    explicit BizManagerManageWidget(QSqlDatabase &db, QWidget *parent = nullptr);
    ~BizManagerManageWidget();

private:
    Ui::BizManagerManageWidget *ui;
    QSqlDatabase &db;
    QSqlRelationalTableModel *account, *biz, *bizManager;

private slots:
    void handleAdd();
    void handleDelete();
    void handleSubmit();
};

#endif // BIZ_MANAGER_MANAGE_WIDGET_H
