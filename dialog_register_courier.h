#ifndef DIALOG_REGISTER_COURIER_H
#define DIALOG_REGISTER_COURIER_H

#include <QDialog>

namespace Ui {
class DialogRegisterCourier;
}

class DialogRegisterCourier : public QDialog
{
    Q_OBJECT

public:
    explicit DialogRegisterCourier(QWidget *parent = nullptr);
    ~DialogRegisterCourier();

    QString getCourierName();
    QString getCourierPhone();

private:
    Ui::DialogRegisterCourier *ui;
};

#endif // DIALOG_REGISTER_COURIER_H
