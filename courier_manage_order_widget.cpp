#include "courier_manage_order_widget.h"
#include "ui_courier_manage_order_widget.h"

CourierManageOrderWidget::CourierManageOrderWidget(QSqlDatabase &db, QWidget *parent)
    : BuzzingWidget(parent)
    , ui(new Ui::CourierManageOrderWidget)
    , db(db)
    , orderDao(db)
{
    ui->setupUi(this);

    orderPairing = new QSqlRelationalTableModel(this);
    orderPairing->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    orderPairing->setTable("order_");
    orderPairing->setFilter("order_state = 'offered'");
    orderPairing->setHeaderData(orderPairing->fieldIndex("order_id"), Qt::Horizontal, tr("ID"));
    orderPairing->setHeaderData(orderPairing->fieldIndex("user_id"), Qt::Horizontal, tr("用户ID"));
    orderPairing->setHeaderData(orderPairing->fieldIndex("destination_id"),
                                Qt::Horizontal,
                                tr("目的地ID"));
    orderPairing->setHeaderData(orderPairing->fieldIndex("delivery_id"),
                                Qt::Horizontal,
                                tr("配送信息ID"));
    orderPairing->setHeaderData(orderPairing->fieldIndex("review_id"), Qt::Horizontal, tr("评价ID"));
    orderPairing->setHeaderData(orderPairing->fieldIndex("order_state"),
                                Qt::Horizontal,
                                tr("订单状态"));
    orderPairing->setHeaderData(orderPairing->fieldIndex("order_price"),
                                Qt::Horizontal,
                                tr("订单价格"));
    orderPairing->setHeaderData(orderPairing->fieldIndex("order_time"),
                                Qt::Horizontal,
                                tr("订单时间"));
    orderPairing->setRelation(orderPairing->fieldIndex("destination_id"),
                              QSqlRelation("destination", "destination_id", "destination_address"));
    orderPairing->select();
    ui->tableViewOrderPairing->setModel(orderPairing);
    ui->tableViewOrderPairing->hideColumn(orderPairing->fieldIndex("order_id"));
    ui->tableViewOrderPairing->hideColumn(orderPairing->fieldIndex("user_id"));
    ui->tableViewOrderPairing->hideColumn(orderPairing->fieldIndex("delivery_id"));
    ui->tableViewOrderPairing->hideColumn(orderPairing->fieldIndex("review_id"));
    ui->tableViewOrderPairing->hideColumn(orderPairing->fieldIndex("order_price"));

    orderSending = new QSqlRelationalTableModel(this);
    orderSending = new QSqlRelationalTableModel(this);
    orderSending->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    orderSending->setTable("order_");
    orderSending->setFilter("order_state = 'sending'");
    orderSending->setHeaderData(orderSending->fieldIndex("order_id"), Qt::Horizontal, tr("ID"));
    orderSending->setHeaderData(orderSending->fieldIndex("user_id"), Qt::Horizontal, tr("用户ID"));
    orderSending->setHeaderData(orderSending->fieldIndex("destination_id"),
                                Qt::Horizontal,
                                tr("目的地ID"));
    orderSending->setHeaderData(orderSending->fieldIndex("delivery_id"),
                                Qt::Horizontal,
                                tr("配送信息ID"));
    orderSending->setHeaderData(orderSending->fieldIndex("review_id"), Qt::Horizontal, tr("评价ID"));
    orderSending->setHeaderData(orderSending->fieldIndex("order_state"),
                                Qt::Horizontal,
                                tr("订单状态"));
    orderSending->setHeaderData(orderSending->fieldIndex("order_price"),
                                Qt::Horizontal,
                                tr("订单价格"));
    orderSending->setHeaderData(orderSending->fieldIndex("order_time"),
                                Qt::Horizontal,
                                tr("订单时间"));
    orderSending->setRelation(orderSending->fieldIndex("destination_id"),
                              QSqlRelation("destination", "destination_id", "destination_address"));
    orderSending->select();
    ui->tableViewOrderSending->setModel(orderSending);
    ui->tableViewOrderSending->hideColumn(orderSending->fieldIndex("order_id"));
    ui->tableViewOrderSending->hideColumn(orderSending->fieldIndex("user_id"));
    ui->tableViewOrderSending->hideColumn(orderSending->fieldIndex("delivery_id"));
    ui->tableViewOrderSending->hideColumn(orderSending->fieldIndex("review_id"));
    ui->tableViewOrderSending->hideColumn(orderSending->fieldIndex("order_price"));

    connect(ui->pushButtonTake, &QPushButton::clicked, this, &CourierManageOrderWidget::handleTake);
    connect(ui->pushButtonFinish,
            &QPushButton::clicked,
            this,
            &CourierManageOrderWidget::handleFinish);
}

CourierManageOrderWidget::~CourierManageOrderWidget()
{
    delete ui;
    delete orderPairing;
    delete orderSending;
}

void CourierManageOrderWidget::setCourierId(int courierId)
{
    _courierId = courierId;
    emit courierIdChanged();
}

int CourierManageOrderWidget::courierId() const
{
    return _courierId;
}

void CourierManageOrderWidget::handleTake()
{
    bool ok;
    int orderId = orderPairing
                      ->data(orderPairing->index(ui->tableViewOrderPairing->currentIndex().row(), 0))
                      .toInt(&ok);
    if (!ok) {
        return;
    }
    orderDao.take(orderId);
    orderPairing->select();
    orderSending->select();
}

void CourierManageOrderWidget::handleFinish()
{
    bool ok;
    int orderId = orderPairing
                      ->data(orderPairing->index(ui->tableViewOrderPairing->currentIndex().row(), 0))
                      .toInt(&ok);
    if (!ok) {
        return;
    }
    orderDao.finish(orderId);
    orderSending->select();
}
