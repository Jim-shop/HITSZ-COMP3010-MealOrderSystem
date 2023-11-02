#ifndef DIALOG_MANAGE_CANTEEN_H
#define DIALOG_MANAGE_CANTEEN_H

#include <QByteArray>
#include <QDialog>
#include <QString>

namespace Ui {
class DialogManageCanteen;
}

class DialogManageCanteen : public QDialog
{
    Q_OBJECT

public:
    explicit DialogManageCanteen(QWidget *parent = nullptr);
    ~DialogManageCanteen();

    void setCanteenName(QString canteenName);
    void setCanteenDesc(QString canteenDesc);
    void setCanteenPic(QByteArray canteenPic);
    QString getCanteenName();
    QString getCanteenDesc();
    QByteArray getCanteenPic();

private:
    Ui::DialogManageCanteen *ui;

private slots:
    void handleEditPic();
};

#endif // DIALOG_MANAGE_CANTEEN_H
