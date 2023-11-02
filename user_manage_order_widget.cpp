#include "user_manage_order_widget.h"
#include "ui_user_manage_order_widget.h"

UserManageOrderWidget::UserManageOrderWidget(QSqlDatabase &db, QWidget *parent)
    : BuzzingWidget(parent)
    , ui(new Ui::UserManageOrderWidget)
    , db(db)
{
    ui->setupUi(this);

    order = new QSqlRelationalTableModel(this);
    order->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    order->setTable("order_");
    order->setHeaderData(order->fieldIndex("order_id"), Qt::Horizontal, tr("ID"));
    order->setHeaderData(order->fieldIndex("user_id"), Qt::Horizontal, tr("用户ID"));
    order->setHeaderData(order->fieldIndex("destination_id"), Qt::Horizontal, tr("目的地ID"));
    order->setHeaderData(order->fieldIndex("delivery_id"), Qt::Horizontal, tr("配送信息ID"));
    order->setHeaderData(order->fieldIndex("review_id"), Qt::Horizontal, tr("评价ID"));
    order->setHeaderData(order->fieldIndex("order_state"), Qt::Horizontal, tr("订单状态"));
    order->setHeaderData(order->fieldIndex("order_price"), Qt::Horizontal, tr("订单价格"));
    order->setHeaderData(order->fieldIndex("order_time"), Qt::Horizontal, tr("订单时间"));
    ui->tableView->setModel(order);
    ui->tableView->hideColumn(order->fieldIndex("user_id"));
    ui->tableView->hideColumn(order->fieldIndex("delivery_id"));
    ui->tableView->hideColumn(order->fieldIndex("review_id"));
    ui->tableView->hideColumn(order->fieldIndex("order_price"));
}

UserManageOrderWidget::~UserManageOrderWidget()
{
    delete ui;
    delete order;
}

void UserManageOrderWidget::setUserId(int userId)
{
    _userId = userId;
    order->setFilter(QString("user_id = %1").arg(userId));
    order->select();
    emit userIdChanged();
}

int UserManageOrderWidget::userId() const
{
    return _userId;
}
