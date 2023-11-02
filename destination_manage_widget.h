#ifndef DESTINATION_MANAGE_WIDGET_H
#define DESTINATION_MANAGE_WIDGET_H

#include <QSqlDatabase>
#include <QSqlRelationalTableModel>
#include <QWidget>
#include "buzzing_widget.h"

namespace Ui {
class DestinationManageWidget;
}

class DestinationManageWidget : public BuzzingWidget
{
    Q_OBJECT
    Q_PROPERTY(int userId READ userId WRITE setUserId NOTIFY userIdChanged FINAL)

public:
    explicit DestinationManageWidget(QSqlDatabase &db, QWidget *parent = nullptr);
    ~DestinationManageWidget();

    void setUserId(int userID);
    int userId() const;

private:
    Ui::DestinationManageWidget *ui;
    QSqlDatabase &db;
    QSqlRelationalTableModel *destination;
    int _userId;

private slots:
    void handleAdd();
    void handleDelete();
    void handleSubmit();

signals:
    void userIdChanged();
};

#endif // DESTINATION_MANAGE_WIDGET_H
