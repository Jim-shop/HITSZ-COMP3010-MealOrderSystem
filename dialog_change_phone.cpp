#include "dialog_change_phone.h"
#include "ui_dialog_change_phone.h"

DialogChangePhone::DialogChangePhone(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogChangePhone)
{
    ui->setupUi(this);
}

DialogChangePhone::~DialogChangePhone()
{
    delete ui;
}

QString DialogChangePhone::getPhone()
{
    return ui->lineEditPhone->text();
}
