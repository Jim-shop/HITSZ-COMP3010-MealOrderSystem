#ifndef BIZ_MANAGER_MANAGE_MEAL_WIDGET_H
#define BIZ_MANAGER_MANAGE_MEAL_WIDGET_H

#include <QSqlDatabase>
#include <QSqlRelationalTableModel>
#include <QWidget>
#include "buzzing_widget.h"

namespace Ui {
class BizManagerManageMealWidget;
}

class BizManagerManageMealWidget : public BuzzingWidget
{
    Q_OBJECT
    Q_PROPERTY(int bizId READ bizId WRITE setBizId NOTIFY bizIdChanged FINAL)

public:
    explicit BizManagerManageMealWidget(QSqlDatabase &db, QWidget *parent = nullptr);
    ~BizManagerManageMealWidget();

    void setBizId(int bizId);
    int bizId() const;

private:
    Ui::BizManagerManageMealWidget *ui;
    QSqlDatabase &db;
    QSqlRelationalTableModel *meal;
    int _bizId;

private slots:
    void handleAdd();
    void handleDelete();
    void handleSubmit();

signals:
    void bizIdChanged();
};

#endif // BIZ_MANAGER_MANAGE_MEAL_WIDGET_H
