#ifndef DIALOG_REGISTER_USER_H
#define DIALOG_REGISTER_USER_H

#include <QDialog>

namespace Ui {
class DialogRegisterUser;
}

class DialogRegisterUser : public QDialog
{
    Q_OBJECT

public:
    explicit DialogRegisterUser(QWidget *parent = nullptr);
    ~DialogRegisterUser();

    QString getUserName();

private:
    Ui::DialogRegisterUser *ui;
};

#endif // DIALOG_REGISTER_USER_H
