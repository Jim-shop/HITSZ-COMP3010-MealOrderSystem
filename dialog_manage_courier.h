#ifndef DIALOG_MANAGE_COURIER_H
#define DIALOG_MANAGE_COURIER_H

#include <QDialog>

namespace Ui {
class DialogManageCourier;
}

class DialogManageCourier : public QDialog
{
    Q_OBJECT

public:
    explicit DialogManageCourier(QWidget *parent = nullptr);
    ~DialogManageCourier();

    void setCourierName(QString courierName);
    void setCourierPhone(QString courierPhone);
    void setCourierPic(QByteArray courierPic);
    QString getCourierName();
    QString getCourierPhone();
    QByteArray getCourierPic();

private:
    Ui::DialogManageCourier *ui;

private slots:
    void handleEditPic();
};

#endif // DIALOG_MANAGE_COURIER_H
