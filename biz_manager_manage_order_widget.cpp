#include "biz_manager_manage_order_widget.h"
#include "ui_biz_manager_manage_order_widget.h"

BizManagerManageOrderWidget::BizManagerManageOrderWidget(QSqlDatabase &db, QWidget *parent)
    : BuzzingWidget(parent)
    , ui(new Ui::BizManagerManageOrderWidget)
    , db(db)
    , orderDao(db)
{
    ui->setupUi(this);

    order = new QSqlQueryModel(this);
    ui->tableViewOrder->setModel(order);

    orderMeal = new QSqlRelationalTableModel(this, db);
    orderMeal->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    orderMeal->setTable("order_meal");
    orderMeal->setHeaderData(orderMeal->fieldIndex("meal_id"), Qt::Horizontal, tr("菜品ID"));
    orderMeal->setHeaderData(orderMeal->fieldIndex("order_id"), Qt::Horizontal, tr("订单ID"));
    orderMeal->setHeaderData(orderMeal->fieldIndex("amount"), Qt::Horizontal, tr("数量"));
    orderMeal->setRelation(orderMeal->fieldIndex("meal_id"),
                           QSqlRelation("meal", "meal_id", "meal_name"));
    ui->tableViewMeal->setModel(orderMeal);
    ui->tableViewMeal->hideColumn(orderMeal->fieldIndex("order_id"));

    connect(ui->tableViewOrder->selectionModel(),
            &QItemSelectionModel::currentRowChanged,
            this,
            &BizManagerManageOrderWidget::handleSelectOrder);
    connect(ui->pushButton, &QPushButton::clicked, this, &BizManagerManageOrderWidget::handleOffer);
}

BizManagerManageOrderWidget::~BizManagerManageOrderWidget()
{
    delete ui;
    delete order;
    delete orderMeal;
}

void BizManagerManageOrderWidget::setBizId(int bizId)
{
    _bizId = bizId;
    refreshOrder();
    emit bizIdChanged();
}

void BizManagerManageOrderWidget::refreshOrder()
{
    order->setQuery(QString("SELECT DISTINCT(order_id) FROM order_ NATURAL JOIN order_meal NATURAL "
                            "JOIN meal WHERE biz_id = %1 AND order_state = 'init'")
                        .arg(_bizId));
    order->setHeaderData(0, Qt::Horizontal, tr("订单号"));
}

int BizManagerManageOrderWidget::bizId() const
{
    return _bizId;
}

void BizManagerManageOrderWidget::handleSelectOrder()
{
    int row = ui->tableViewOrder->currentIndex().row();
    int orderId = order->data(order->index(row, 0)).toInt();
    orderMeal->setFilter(QString("order_id = %1").arg(orderId));
    orderMeal->select();
}

void BizManagerManageOrderWidget::handleOffer()
{
    int row = ui->tableViewOrder->currentIndex().row();
    int orderId = order->data(order->index(row, 0)).toInt();
    orderDao.offer(orderId);
    refreshOrder();
}
