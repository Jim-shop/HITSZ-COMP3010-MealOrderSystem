#include "dialog_register_courier.h"
#include "ui_dialog_register_courier.h"

DialogRegisterCourier::DialogRegisterCourier(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogRegisterCourier)
{
    ui->setupUi(this);
}

DialogRegisterCourier::~DialogRegisterCourier()
{
    delete ui;
}

QString DialogRegisterCourier::getCourierName()
{
    return ui->lineEditCourierName->text();
}

QString DialogRegisterCourier::getCourierPhone()
{
    return ui->lineEditCourierPhone->text();
}
