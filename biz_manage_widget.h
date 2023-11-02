#ifndef BIZ_MANAGE_WIDGET_H
#define BIZ_MANAGE_WIDGET_H

#include <QSqlDatabase>
#include <QSqlRelationalTableModel>
#include <QWidget>
#include "buzzing_widget.h"

namespace Ui {
class BizManageWidget;
}

class BizManageWidget : public BuzzingWidget
{
    Q_OBJECT

public:
    explicit BizManageWidget(QSqlDatabase &db, QWidget *parent = nullptr);
    ~BizManageWidget();

private:
    Ui::BizManageWidget *ui;
    QSqlDatabase &db;
    QSqlRelationalTableModel *biz;

private slots:
    void handleAdd();
    void handleDelete();
    void handleSubmit();
};

#endif // BIZ_MANAGE_WIDGET_H
