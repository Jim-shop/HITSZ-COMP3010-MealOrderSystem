#ifndef WORKBENCH_WIDGET_H
#define WORKBENCH_WIDGET_H

#include <QList>
#include <QSqlDatabase>
#include <QWidget>
#include "account_manage_widget.h"
#include "biz_manage_widget.h"
#include "biz_manager_manage_biz_manager_widget.h"
#include "biz_manager_manage_meal_widget.h"
#include "biz_manager_manage_order_widget.h"
#include "biz_manager_manage_widget.h"
#include "canteen_manage_widget.h"
#include "canteen_manager_manage_biz_widget.h"
#include "canteen_manager_manage_canteen_manager_widget.h"
#include "canteen_manager_manage_widget.h"
#include "canteen_manager_manage_window_widget.h"
#include "courier_manage_order_widget.h"
#include "dao/account_dao.h"
#include "dao/biz_dao.h"
#include "dao/biz_manager_dao.h"
#include "dao/canteen_dao.h"
#include "dao/canteen_manager_dao.h"
#include "dao/courier_dao.h"
#include "dao/user_dao.h"
#include "destination_manage_widget.h"
#include "user_create_order_widget.h"
#include "user_manage_order_widget.h"

namespace Ui {
class WorkbenchWidget;
}

class WorkbenchWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WorkbenchWidget(QSqlDatabase &db, QWidget *parent = nullptr);
    ~WorkbenchWidget();

private:
    Ui::WorkbenchWidget *ui;
    QSqlDatabase db;

    AccountDao accountDao;
    CourierDao courierDao;
    UserDao userDao;
    CanteenManagerDao canteenManagerDao;
    BizManagerDao bizManagerDao;
    CanteenDao canteenDao;
    BizDao bizDao;

    int accountId, courierId, userId;
    QList<CanteenManagerDao::CanteenManagerInfo> canteenManagerInfo;
    QList<BizManagerDao::BizManagerInfo> bizManagerInfo;

    CanteenManageWidget *canteenManageWidget = nullptr;
    AccountManageWidget *accountManageWidget = nullptr;
    BizManageWidget *bizManageWidget = nullptr;
    DestinationManageWidget *destinationManageWidget = nullptr;
    BizManagerManageWidget *bizManagerManageWidget = nullptr;
    BizManagerManageMealWidget *bizManagerManageMealWidget = nullptr;
    BizManagerManageBizManagerWidget *bizManagerManageBizManagerWidget = nullptr;
    BizManagerManageOrderWidget *bizManagerManageOrderWidget = nullptr;
    CanteenManagerManageWidget *canteenManagerManageWidget = nullptr;
    CanteenManagerManageWindowWidget *canteenManagerManageWindowWidget = nullptr;
    CanteenManagerManageBizWidget *canteenManagerManageBizWidget = nullptr;
    CanteenManagerManageCanteenManagerWidget *canteenManagerManageCanteenManagerWidget = nullptr;
    UserCreateOrderWidget *userCreateOrderWidget = nullptr;
    UserManageOrderWidget *userManageOrderWidget = nullptr;
    CourierManageOrderWidget *courierManageOrderWidget = nullptr;

signals:
    void logout();

public slots:
    void load(int accountId);

private slots:
    void reload();

    void handleLogout();
    void handleResetPassword();
    void handleChangePhone();

    void handleAdminManageCanteen();
    void handleAdminManageAccount();
    void handleAdminManageBiz();
    void handleAdminManageCanteenManager();
    void handleAdminManageBizManager();

    void handleUserRegister();
    void handleUserManageDestination();
    void handleUserCreateOrder();
    void handleUserManageOrder();
    void handleUserManageUser();

    void handleCourierRegister();
    void handleCourierManageOrder();
    void handleCourierManageCourier();

    void handleBizManagerManageMeal();
    void handleBizManagerManageBiz();
    void handleBizManagerManageBizManager();
    void handleBizManagerManageOrder();

    void handleCanteenManagerManageWindow();
    void handleCanteenManagerManageCanteen();
    void handleCanteenManagerManageCanteenManager();
    void handleCanteenManagerManageBiz();
};

#endif // WORKBENCH_WIDGET_H
