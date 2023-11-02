#ifndef BIZ_MANAGER_MANAGE_BIZ_MANAGER_WIDGET_H
#define BIZ_MANAGER_MANAGE_BIZ_MANAGER_WIDGET_H

#include <QSqlDatabase>
#include <QSqlRelationalTableModel>
#include <QWidget>
#include "buzzing_widget.h"

namespace Ui {
class BizManagerManageBizManagerWidget;
}

class BizManagerManageBizManagerWidget : public BuzzingWidget
{
    Q_OBJECT
    Q_PROPERTY(int bizId READ bizId WRITE setBizId NOTIFY bizIdChanged FINAL)

public:
    explicit BizManagerManageBizManagerWidget(QSqlDatabase &db, QWidget *parent = nullptr);
    ~BizManagerManageBizManagerWidget();

    void setBizId(int bizId);
    int bizId() const;

private:
    Ui::BizManagerManageBizManagerWidget *ui;
    QSqlDatabase &db;
    QSqlRelationalTableModel *account, *bizManager;
    int _bizId;

private slots:
    void handleAdd();
    void handleDelete();
    void handleSubmit();

signals:
    void bizIdChanged();
};

#endif // BIZ_MANAGER_MANAGE_BIZ_MANAGER_WIDGET_H
