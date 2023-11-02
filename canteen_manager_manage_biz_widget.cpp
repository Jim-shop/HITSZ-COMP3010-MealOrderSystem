#include "canteen_manager_manage_biz_widget.h"
#include "ui_canteen_manager_manage_biz_widget.h"

#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include "image_delegate.h"

CanteenManagerManageBizWidget::CanteenManagerManageBizWidget(QSqlDatabase &db, QWidget *parent)
    : BuzzingWidget(parent)
    , ui(new Ui::CanteenManagerManageBizWidget)
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

    ui->tableViewWindow->setModel(window);
    ui->tableViewWindow->setItemDelegateForColumn(window->fieldIndex("window_pic"),
                                                  new ImageDelegate());
    ui->tableViewWindow->hideColumn(window->fieldIndex("window_id"));
    ui->tableViewWindow->hideColumn(window->fieldIndex("canteen_id"));

    biz = new QSqlRelationalTableModel(this, db);
    biz->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    biz->setTable("biz");
    biz->setHeaderData(biz->fieldIndex("biz_id"), Qt::Horizontal, tr("ID"));
    biz->setHeaderData(biz->fieldIndex("biz_name"), Qt::Horizontal, tr("名称"));
    biz->setHeaderData(biz->fieldIndex("biz_desc"), Qt::Horizontal, tr("描述"));
    biz->setHeaderData(biz->fieldIndex("biz_pic"), Qt::Horizontal, tr("图片"));
    biz->setHeaderData(biz->fieldIndex("biz_enable"), Qt::Horizontal, tr("启用"));
    biz->select();

    ui->tableViewBiz->setModel(biz);
    ui->tableViewBiz->hideColumn(biz->fieldIndex("biz_desc"));
    ui->tableViewBiz->hideColumn(biz->fieldIndex("biz_pic"));
    ui->tableViewBiz->hideColumn(biz->fieldIndex("biz_enable"));

    connect(ui->pushButtonAssign,
            &QPushButton::clicked,
            this,
            &CanteenManagerManageBizWidget::handleAssign);
    connect(ui->pushButtonDeassign,
            &QPushButton::clicked,
            this,
            &CanteenManagerManageBizWidget::handleDeassign);
}

CanteenManagerManageBizWidget::~CanteenManagerManageBizWidget()
{
    delete ui;
    delete window;
    delete biz;
}

int CanteenManagerManageBizWidget::canteenId() const
{
    return _canteenId;
}

void CanteenManagerManageBizWidget::setCanteenId(int canteenId)
{
    _canteenId = canteenId;
    window->setFilter(QString("canteen_id = %1").arg(canteenId));
    window->select();
    emit canteenIdChanged();
}

void CanteenManagerManageBizWidget::handleAssign()
{
    bool ok;
    int bizId = biz->data(biz->index(ui->tableViewBiz->currentIndex().row(), 0)).toInt(&ok);
    if (!ok) {
        return;
    }
    QModelIndex index = window->index(ui->tableViewWindow->currentIndex().row(),
                                      window->fieldIndex("biz_id"));
    window->setData(index, bizId);
    window->submitAll();
}

void CanteenManagerManageBizWidget::handleDeassign()
{
    QModelIndex index = window->index(ui->tableViewWindow->currentIndex().row(),
                                      window->fieldIndex("biz_id"));
    window->setData(index, QVariant());
    window->submitAll();
}
