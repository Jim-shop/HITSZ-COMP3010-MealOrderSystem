#include "destination_manage_widget.h"
#include "ui_destination_manage_widget.h"

#include <QMessageBox>
#include <QSqlError>

DestinationManageWidget::DestinationManageWidget(QSqlDatabase &db, QWidget *parent)
    : BuzzingWidget(parent)
    , ui(new Ui::DestinationManageWidget)
    , db(db)
{
    ui->setupUi(this);

    destination = new QSqlRelationalTableModel(this, db);
    destination->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    destination->setTable("destination");
    destination->setHeaderData(destination->fieldIndex("destination_id"), Qt::Horizontal, tr("ID"));
    destination->setHeaderData(destination->fieldIndex("user_id"), Qt::Horizontal, tr("用户ID"));
    destination->setHeaderData(destination->fieldIndex("destination_phone"),
                               Qt::Horizontal,
                               tr("手机号"));
    destination->setHeaderData(destination->fieldIndex("destination_address"),
                               Qt::Horizontal,
                               tr("地址"));
    destination->select();

    ui->tableView->setModel(destination);
    ui->tableView->hideColumn(destination->fieldIndex("destination_id"));
    ui->tableView->hideColumn(destination->fieldIndex("user_id"));

    connect(ui->pushButtonAdd, &QPushButton::clicked, this, &DestinationManageWidget::handleAdd);
    connect(ui->pushButtonDelete,
            &QPushButton::clicked,
            this,
            &DestinationManageWidget::handleDelete);
    connect(ui->pushButtonSubmit,
            &QPushButton::clicked,
            this,
            &DestinationManageWidget::handleSubmit);
}

void DestinationManageWidget::setUserId(int userId)
{
    _userId = userId;
    destination->setFilter(QString("user_id = %1").arg(_userId));
    destination->select();
    emit userIdChanged();
}

int DestinationManageWidget::userId() const
{
    return _userId;
}

DestinationManageWidget::~DestinationManageWidget()
{
    delete ui;
    delete destination;
}

void DestinationManageWidget::handleAdd()
{
    int row = destination->rowCount();
    destination->insertRow(row);
    destination->setData(destination->index(row, destination->fieldIndex("user_id")), _userId);
}

void DestinationManageWidget::handleDelete()
{
    int row = ui->tableView->currentIndex().row();
    if (destination->data(destination->index(row, destination->fieldIndex("destination_id")))
            .isNull()) { // new added
        destination->removeRow(row);
    } else { // exists
        destination->setData(destination->index(row, destination->fieldIndex("user_id")),
                             QVariant());
    }
}

void DestinationManageWidget::handleSubmit()
{
    if (!destination->submitAll()) {
        QMessageBox::information(this, tr("请检查输入"), destination->lastError().text());
    }
    destination->select();
}
