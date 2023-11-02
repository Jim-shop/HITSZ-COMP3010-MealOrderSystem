#include <QApplication>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include "login_dialog.h"
#include "workbench_widget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("localhost");
    db.setPort(3306);
    db.setDatabaseName("DBLAB3");
    db.setUserName("root");
    db.setPassword("root");
    if (!db.open()) {
        QMessageBox::information(nullptr, QObject::tr("连接不上数据库"), db.lastError().text());
        return 114514;
    }

    LoginDialog login(db);
    WorkbenchWidget workbench(db);

    QObject::connect(&login, &LoginDialog::login, &workbench, &WorkbenchWidget::show);
    QObject::connect(&login, &LoginDialog::login, &workbench, &WorkbenchWidget::load);
    QObject::connect(&login, &LoginDialog::login, &login, &LoginDialog::hide);

    QObject::connect(&workbench, &WorkbenchWidget::logout, &login, &LoginDialog::reload);
    QObject::connect(&workbench, &WorkbenchWidget::logout, &login, &LoginDialog::show);
    QObject::connect(&workbench, &WorkbenchWidget::logout, &workbench, &WorkbenchWidget::hide);

    login.show();
    return app.exec();
}
