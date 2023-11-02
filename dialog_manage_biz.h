#ifndef DIALOG_MANAGE_BIZ_H
#define DIALOG_MANAGE_BIZ_H

#include <QByteArray>
#include <QDialog>
#include <QString>

namespace Ui {
class DialogManageBiz;
}

class DialogManageBiz : public QDialog
{
    Q_OBJECT

public:
    explicit DialogManageBiz(QWidget *parent = nullptr);
    ~DialogManageBiz();

    void setBizName(QString bizName);
    void setBizDesc(QString bizDesc);
    void setBizPic(QByteArray bizPic);
    QString getBizName();
    QString getBizDesc();
    QByteArray getBizPic();

private:
    Ui::DialogManageBiz *ui;

private slots:
    void handleEditPic();
};

#endif // DIALOG_MANAGE_BIZ_H
