#include "user_create_order_widget.h"
#include "ui_user_create_order_widget.h"

#include <QMessageBox>
#include <QSqlError>
#include "image_delegate.h"

UserCreateOrderWidget::UserCreateOrderWidget(QSqlDatabase &db, QWidget *parent)
    : BuzzingWidget(parent)
    , ui(new Ui::UserCreateOrderWidget)
    , db(db)
    , orderDao(db)
    , price(0)
{
    ui->setupUi(this);

    canteen = new QSqlRelationalTableModel(this, db);
    canteen->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    canteen->setTable("canteen");
    canteen->setFilter(QString("canteen_enable = 1"));
    canteen->setHeaderData(canteen->fieldIndex("canteen_id"), Qt::Horizontal, tr("ID"));
    canteen->setHeaderData(canteen->fieldIndex("canteen_name"), Qt::Horizontal, tr("名称"));
    canteen->setHeaderData(canteen->fieldIndex("canteen_desc"), Qt::Horizontal, tr("描述"));
    canteen->setHeaderData(canteen->fieldIndex("canteen_pic"), Qt::Horizontal, tr("图片"));
    canteen->setHeaderData(canteen->fieldIndex("canteen_enable"), Qt::Horizontal, tr("启用"));
    canteen->select();
    ui->tableViewCanteen->setModel(canteen);
    ui->tableViewCanteen->setItemDelegateForColumn(canteen->fieldIndex("canteen_pic"),
                                                   new ImageDelegate());
    ui->tableViewCanteen->hideColumn(canteen->fieldIndex("canteen_id"));
    ui->tableViewCanteen->hideColumn(canteen->fieldIndex("canteen_enable"));

    biz = new QSqlQueryModel(this);
    ui->tableViewBiz->setModel(biz);
    ui->tableViewBiz->setItemDelegateForColumn(2, new ImageDelegate());
    ui->tableViewBiz->hideColumn(3);

    meal = new QSqlRelationalTableModel(this, db);
    meal->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    meal->setTable("meal");
    meal->setHeaderData(meal->fieldIndex("meal_id"), Qt::Horizontal, tr("ID"));
    meal->setHeaderData(meal->fieldIndex("biz_id"), Qt::Horizontal, tr("商家ID"));
    meal->setHeaderData(meal->fieldIndex("meal_name"), Qt::Horizontal, tr("名称"));
    meal->setHeaderData(meal->fieldIndex("meal_desc"), Qt::Horizontal, tr("描述"));
    meal->setHeaderData(meal->fieldIndex("meal_pic"), Qt::Horizontal, tr("图片"));
    meal->setHeaderData(meal->fieldIndex("meal_price"), Qt::Horizontal, tr("价格"));
    ui->tableViewMeal->setModel(meal);
    ui->tableViewMeal->setItemDelegateForColumn(meal->fieldIndex("meal_pic"), new ImageDelegate());
    ui->tableViewMeal->hideColumn(meal->fieldIndex("biz_id"));

    destination = new QSqlRelationalTableModel(this, db);
    destination->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    destination->setTable("destination");
    destination->setFilter(QString("user_id = %1").arg(_userId));
    destination->setHeaderData(destination->fieldIndex("destination_id"), Qt::Horizontal, tr("ID"));
    destination->setHeaderData(destination->fieldIndex("user_id"), Qt::Horizontal, tr("用户ID"));
    destination->setHeaderData(destination->fieldIndex("destination_phone"),
                               Qt::Horizontal,
                               tr("手机号"));
    destination->setHeaderData(destination->fieldIndex("destination_address"),
                               Qt::Horizontal,
                               tr("地址"));
    destination->select();
    ui->tableViewDestination->setModel(destination);
    ui->tableViewDestination->hideColumn(destination->fieldIndex("destination_id"));
    ui->tableViewDestination->hideColumn(destination->fieldIndex("user_id"));

    cart = new QStandardItemModel(0, 3, this);
    cart->setHeaderData(0, Qt::Horizontal, tr("菜品ID"));
    cart->setHeaderData(1, Qt::Horizontal, tr("菜品数量"));
    cart->setHeaderData(2, Qt::Horizontal, tr("菜品单价"));
    ui->tableViewCart->setModel(cart);

    connect(ui->tableViewCanteen->selectionModel(),
            &QItemSelectionModel::currentRowChanged,
            this,
            &UserCreateOrderWidget::onSelectCanteen);
    connect(ui->tableViewBiz->selectionModel(),
            &QItemSelectionModel::currentRowChanged,
            this,
            &UserCreateOrderWidget::onSelectBiz);
    connect(ui->pushButtonAdd, &QPushButton::clicked, this, &UserCreateOrderWidget::onAdd);
    connect(ui->pushButtonDelete, &QPushButton::clicked, this, &UserCreateOrderWidget::onDelete);
    connect(ui->pushButtonSubmit, &QPushButton::clicked, this, &UserCreateOrderWidget::onSubmit);
}

UserCreateOrderWidget::~UserCreateOrderWidget()
{
    delete ui;
    delete canteen;
    delete biz;
    delete meal;
    delete destination;
    delete cart;
}

void UserCreateOrderWidget::setUserId(int userId)
{
    _userId = userId;
    destination->setFilter(QString("user_id = %1").arg(_userId));
    destination->select();
    emit userIdChanged();
}

int UserCreateOrderWidget::userId() const
{
    return _userId;
}

void UserCreateOrderWidget::onSelectCanteen()
{
    int row = ui->tableViewCanteen->currentIndex().row();
    int canteenId = canteen->data(canteen->index(row, 0)).toInt();
    biz->setQuery(
        QString(
            "SELECT biz_name, biz_desc, biz_pic, biz_id FROM biz NATURAL JOIN window_ NATURAL JOIN "
            "canteen WHERE biz_enable = 1 and canteen_id = %1")
            .arg(canteenId));
    biz->setHeaderData(0, Qt::Horizontal, tr("名称"));
    biz->setHeaderData(1, Qt::Horizontal, tr("描述"));
    biz->setHeaderData(2, Qt::Horizontal, tr("图片"));
}
void UserCreateOrderWidget::onSelectBiz()
{
    int row = ui->tableViewBiz->currentIndex().row();
    int bizId = biz->data(biz->index(row, 3)).toInt();
    meal->setFilter(QString("biz_id = %1").arg(bizId));
    meal->select();
}
void UserCreateOrderWidget::onAdd()
{
    int mealId = meal->data(meal->index(ui->tableViewMeal->currentIndex().row(),
                                        meal->fieldIndex("meal_id")))
                     .toInt();
    double mealPrice = meal->data(meal->index(ui->tableViewMeal->currentIndex().row(),
                                              meal->fieldIndex("meal_price")))
                           .toDouble();
    price += mealPrice;
    ui->labelPrice->setText(tr("总价：%1").arg(price));
    for (int i = cart->rowCount(); i-- > 0;) {
        if (cart->data(cart->index(i, 0)) == mealId) {
            cart->setData(cart->index(i, 1), cart->data(cart->index(i, 1)).toInt() + 1);
            return;
        }
    }
    int row = cart->rowCount();
    cart->insertRow(row);
    cart->setData(cart->index(row, 0), mealId);
    cart->setData(cart->index(row, 1), 1);
    cart->setData(cart->index(row, 2), mealPrice);
}
void UserCreateOrderWidget::onDelete()
{
    int row = ui->tableViewCart->currentIndex().row();
    int amount = cart->data(cart->index(row, 1)).toInt();
    double mealPrice = cart->data(cart->index(row, 2)).toDouble();
    cart->removeRow(row);
    price -= mealPrice * amount;
    ui->labelPrice->setText(tr("总价：%1").arg(price));
}
void UserCreateOrderWidget::onSubmit()
{
    int row = ui->tableViewDestination->currentIndex().row();
    int destinationId = destination->data(destination->index(row, 0)).toInt();
    QList<OrderDao::OrderMealInfo> info;
    for (int i = cart->rowCount(); i-- > 0;) {
        info.push_back({
            cart->data(cart->index(i, 0)).toInt(),
            cart->data(cart->index(i, 1)).toInt(),
        });
    }
    auto &&[ok, error] = orderDao.sendOrder(_userId, destinationId, price, info);
    if (!ok) {
        QMessageBox::information(this, tr("失败"), error);
    } else {
        this->close();
    }
}
