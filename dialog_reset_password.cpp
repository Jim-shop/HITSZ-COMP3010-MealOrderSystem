#include "dialog_reset_password.h"
#include "ui_dialog_reset_password.h"

#include <QDialogButtonBox>
#include <QMessageBox>

DialogResetPassword::DialogResetPassword(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogResetPassword)
{
    ui->setupUi(this);
}

DialogResetPassword::~DialogResetPassword()
{
    delete ui;
}

QString DialogResetPassword::getOldPassword()
{
    return ui->lineEditOldPassword->text();
}

QString DialogResetPassword::getNewPassword()
{
    return ui->lineEditNewPassword->text();
}
