#include "canteen_manager_manage_window_widget.h"
#include <QSqlError>
#include "image_delegate.h"
#include "ui_canteen_manager_manage_window_widget.h"

CanteenManagerManageWindowWidget::CanteenManagerManageWindowWidget(QSqlDatabase &db, QWidget *parent)
    : BuzzingWidget(parent)
    , ui(new Ui::CanteenManagerManageWindowWidget)
    , db(db)
{
    ui->setupUi(this);

    window = new QSqlRelationalTableModel(this, db);
    window->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    window->setTable("window_");
    window->setHeaderData(window->fieldIndex("window_id"), Qt::Horizontal, tr("ID"));
    window->setHeaderData(window->fieldIndex("canteen_id"), Qt::Horizontal, tr("食堂ID"));
    window->setHeaderData(window->fieldIndex("biz_id"), Qt::Horizontal, tr("商户ID"));
    window->setHeaderData(window->fieldIndex("window_name"), Qt::Horizontal, tr("名称"));
    window->setHeaderData(window->fieldIndex("window_desc"), Qt::Horizontal, tr("描述"));
    window->setHeaderData(window->fieldIndex("window_pic"), Qt::Horizontal, tr("图片"));
    window->setHeaderData(window->fieldIndex("window_location"), Qt::Horizontal, tr("位置"));
    window->setHeaderData(window->fieldIndex("window_enable"), Qt::Horizontal, tr("启用"));
    window->select();

    ui->tableView->setModel(window);
    ui->tableView->setItemDelegateForColumn(window->fieldIndex("window_pic"), new ImageDelegate());
    ui->tableView->hideColumn(window->fieldIndex("window_id"));
    ui->tableView->hideColumn(window->fieldIndex("canteen_id"));
    ui->tableView->hideColumn(window->fieldIndex("biz_id"));

    connect(ui->pushButtonAdd,
            &QPushButton::clicked,
            this,
            &CanteenManagerManageWindowWidget::handleAdd);
    connect(ui->pushButtonDelete,
            &QPushButton::clicked,
            this,
            &CanteenManagerManageWindowWidget::handleDelete);
    connect(ui->pushButtonSubmit,
            &QPushButton::clicked,
            this,
            &CanteenManagerManageWindowWidget::handleSubmit);
}

CanteenManagerManageWindowWidget::~CanteenManagerManageWindowWidget()
{
    delete ui;
    delete window;
}

void CanteenManagerManageWindowWidget::setCanteenId(int canteenId)
{
    _canteenId = canteenId;
    window->setFilter(QString("canteen_id = %1").arg(canteenId));
    window->select();
    emit canteenIdChanged();
}

int CanteenManagerManageWindowWidget::canteenId() const
{
    return _canteenId;
}

void CanteenManagerManageWindowWidget::handleAdd()
{
    int row = window->rowCount();
    window->insertRow(row);
    window->setData(window->index(row, window->fieldIndex("canteen_id")), _canteenId);
}

void CanteenManagerManageWindowWidget::handleDelete()
{
    window->removeRow(ui->tableView->currentIndex().row());
}

void CanteenManagerManageWindowWidget::handleSubmit()
{
    if (!window->submitAll()) {
        QMessageBox::information(this, tr("请检查输入"), window->lastError().text());
    }
    window->select();
}
