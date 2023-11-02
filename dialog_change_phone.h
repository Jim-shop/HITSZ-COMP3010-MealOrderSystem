#ifndef DIALOG_CHANGE_PHONE_H
#define DIALOG_CHANGE_PHONE_H

#include <QDialog>

namespace Ui {
class DialogChangePhone;
}

class DialogChangePhone : public QDialog
{
    Q_OBJECT

public:
    explicit DialogChangePhone(QWidget *parent = nullptr);
    ~DialogChangePhone();

    QString getPhone();

private:
    Ui::DialogChangePhone *ui;
};

#endif // DIALOG_CHANGE_PHONE_H
