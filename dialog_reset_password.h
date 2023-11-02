#ifndef DIALOG_RESET_PASSWORD_H
#define DIALOG_RESET_PASSWORD_H

#include <QDialog>

namespace Ui {
class DialogResetPassword;
}

class DialogResetPassword : public QDialog
{
    Q_OBJECT

public:
    explicit DialogResetPassword(QWidget *parent = nullptr);
    ~DialogResetPassword();

    QString getOldPassword();
    QString getNewPassword();

private:
    Ui::DialogResetPassword *ui;
};

#endif // DIALOG_RESET_PASSWORD_H
