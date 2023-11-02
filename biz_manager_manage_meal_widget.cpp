#include "biz_manager_manage_meal_widget.h"
#include "image_delegate.h"
#include "ui_biz_manager_manage_meal_widget.h"

#include <QSqlError>

BizManagerManageMealWidget::BizManagerManageMealWidget(QSqlDatabase &db, QWidget *parent)
    : BuzzingWidget(parent)
    , ui(new Ui::BizManagerManageMealWidget)
    , db(db)
{
    ui->setupUi(this);

    meal = new QSqlRelationalTableModel(this, db);
    meal->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    meal->setTable("meal");
    meal->setHeaderData(meal->fieldIndex("meal_id"), Qt::Horizontal, tr("ID"));
    meal->setHeaderData(meal->fieldIndex("biz_id"), Qt::Horizontal, tr("商户ID"));
    meal->setHeaderData(meal->fieldIndex("meal_name"), Qt::Horizontal, tr("名称"));
    meal->setHeaderData(meal->fieldIndex("meal_desc"), Qt::Horizontal, tr("描述"));
    meal->setHeaderData(meal->fieldIndex("meal_pic"), Qt::Horizontal, tr("图片"));
    meal->setHeaderData(meal->fieldIndex("meal_price"), Qt::Horizontal, tr("价格"));
    meal->select();

    ui->tableView->setModel(meal);
    ui->tableView->setItemDelegateForColumn(meal->fieldIndex("meal_pic"), new ImageDelegate());
    ui->tableView->hideColumn(meal->fieldIndex("meal_id"));
    ui->tableView->hideColumn(meal->fieldIndex("biz_id"));

    connect(ui->pushButtonAdd, &QPushButton::clicked, this, &BizManagerManageMealWidget::handleAdd);
    connect(ui->pushButtonDelete,
            &QPushButton::clicked,
            this,
            &BizManagerManageMealWidget::handleDelete);
    connect(ui->pushButtonSubmit,
            &QPushButton::clicked,
            this,
            &BizManagerManageMealWidget::handleSubmit);
}

void BizManagerManageMealWidget::setBizId(int bizId)
{
    _bizId = bizId;
    meal->setFilter(QString("biz_id = %1").arg(_bizId));
    meal->select();
    emit bizIdChanged();
}

int BizManagerManageMealWidget::bizId() const
{
    return _bizId;
}

BizManagerManageMealWidget::~BizManagerManageMealWidget()
{
    delete ui;
    delete meal;
}

void BizManagerManageMealWidget::handleAdd()
{
    int row = meal->rowCount();
    meal->insertRow(row);
    meal->setData(meal->index(row, meal->fieldIndex("biz_id")), _bizId);
}

void BizManagerManageMealWidget::handleDelete()
{
    int row = ui->tableView->currentIndex().row();
    if (meal->data(meal->index(row, meal->fieldIndex("meal_id"))).isNull()) { // new added
        meal->removeRow(row);
    } else { // exists
        meal->setData(meal->index(row, meal->fieldIndex("biz_id")), QVariant());
    }
}

void BizManagerManageMealWidget::handleSubmit()
{
    if (!meal->submitAll()) {
        QMessageBox::information(this, tr("请检查输入"), meal->lastError().text());
    }
    meal->select();
}
