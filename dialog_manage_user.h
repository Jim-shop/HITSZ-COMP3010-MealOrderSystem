#ifndef DIALOG_MANAGE_USER_H
#define DIALOG_MANAGE_USER_H

#include <QDialog>

namespace Ui {
class DialogManageUser;
}

class DialogManageUser : public QDialog
{
    Q_OBJECT

public:
    explicit DialogManageUser(QWidget *parent = nullptr);
    ~DialogManageUser();

    void setUserName(QString userName);
    void setUserPic(QByteArray userPic);
    QString getUserName();
    QByteArray getUserPic();

private:
    Ui::DialogManageUser *ui;

private slots:
    void handleEditPic();
};

#endif // DIALOG_MANAGE_USER_H
