#include "dialog_register_user.h"
#include "ui_dialog_register_user.h"

DialogRegisterUser::DialogRegisterUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogRegisterUser)
{
    ui->setupUi(this);
}

DialogRegisterUser::~DialogRegisterUser()
{
    delete ui;
}

QString DialogRegisterUser::getUserName()
{
    return ui->lineEditUserName->text();
}
