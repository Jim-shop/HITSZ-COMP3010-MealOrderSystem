#include "workbench_widget.h"
#include <QMessageBox>
#include "buzzing_widget.h"
#include "dialog_change_phone.h"
#include "dialog_manage_biz.h"
#include "dialog_manage_canteen.h"
#include "dialog_manage_courier.h"
#include "dialog_manage_user.h"
#include "dialog_register_courier.h"
#include "dialog_register_user.h"
#include "dialog_reset_password.h"
#include "ui_workbench_widget.h"

WorkbenchWidget::WorkbenchWidget(QSqlDatabase &db, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WorkbenchWidget)
    , db(db)
    , accountDao(db)
    , courierDao(db)
    , userDao(db)
    , canteenManagerDao(db)
    , bizManagerDao(db)
    , canteenDao(db)
    , bizDao(db)
{
    ui->setupUi(this);
    connect(ui->pushButtonLogout, &QPushButton::clicked, this, &WorkbenchWidget::handleLogout);

    connect(ui->pushButtonAccountManagePassword,
            &QPushButton::clicked,
            this,
            &WorkbenchWidget::handleResetPassword);
    connect(ui->pushButtonAccountManagePhone,
            &QPushButton::clicked,
            this,
            &WorkbenchWidget::handleChangePhone);

    connect(ui->pushButtonAdminManageCanteen,
            &QPushButton::clicked,
            this,
            &WorkbenchWidget::handleAdminManageCanteen);
    connect(ui->pushButtonAdminManageAccount,
            &QPushButton::clicked,
            this,
            &WorkbenchWidget::handleAdminManageAccount);
    connect(ui->pushButtonAdminManageBiz,
            &QPushButton::clicked,
            this,
            &WorkbenchWidget::handleAdminManageBiz);
    connect(ui->pushButtonAdminManageCanteenManager,
            &QPushButton::clicked,
            this,
            &WorkbenchWidget::handleAdminManageCanteenManager);
    connect(ui->pushButtonAdminManageBizManager,
            &QPushButton::clicked,
            this,
            &WorkbenchWidget::handleAdminManageBizManager);

    connect(ui->pushButtonUserRegister,
            &QPushButton::clicked,
            this,
            &WorkbenchWidget::handleUserRegister);
    connect(ui->pushButtonUserManageDestination,
            &QPushButton::clicked,
            this,
            &WorkbenchWidget::handleUserManageDestination);
    connect(ui->pushButtonUserCreateOrder,
            &QPushButton::clicked,
            this,
            &WorkbenchWidget::handleUserCreateOrder);
    connect(ui->pushButtonUserManageOrder,
            &QPushButton::clicked,
            this,
            &WorkbenchWidget::handleUserManageOrder);
    connect(ui->pushButtonUserManageUser,
            &QPushButton::clicked,
            this,
            &WorkbenchWidget::handleUserManageUser);

    connect(ui->pushButtonCourierRegister,
            &QPushButton::clicked,
            this,
            &WorkbenchWidget::handleCourierRegister);
    connect(ui->pushButtonCourierManageOrder,
            &QPushButton::clicked,
            this,
            &WorkbenchWidget::handleCourierManageOrder);
    connect(ui->pushButtonCourierManageCourier,
            &QPushButton::clicked,
            this,
            &WorkbenchWidget::handleCourierManageCourier);

    connect(ui->pushButtonBizManagerManageMeal,
            &QPushButton::clicked,
            this,
            &WorkbenchWidget::handleBizManagerManageMeal);
    connect(ui->pushButtonBizManagerManageBiz,
            &QPushButton::clicked,
            this,
            &WorkbenchWidget::handleBizManagerManageBiz);
    connect(ui->pushButtonBizManagerManageBizManager,
            &QPushButton::clicked,
            this,
            &WorkbenchWidget::handleBizManagerManageBizManager);
    connect(ui->pushButtonBizManagerManageOrder,
            &QPushButton::clicked,
            this,
            &WorkbenchWidget::handleBizManagerManageOrder);

    connect(ui->pushButtonCanteenManagerManageWindow,
            &QPushButton::clicked,
            this,
            &WorkbenchWidget::handleCanteenManagerManageWindow);
    connect(ui->pushButtonCanteenManagerManageCanteen,
            &QPushButton::clicked,
            this,
            &WorkbenchWidget::handleCanteenManagerManageCanteen);
    connect(ui->pushButtonCanteenManagerManageCanteenManager,
            &QPushButton::clicked,
            this,
            &WorkbenchWidget::handleCanteenManagerManageCanteenManager);
    connect(ui->pushButtonCanteenManagerManageBiz,
            &QPushButton::clicked,
            this,
            &WorkbenchWidget::handleCanteenManagerManageBiz);
}

WorkbenchWidget::~WorkbenchWidget()
{
    delete ui;
    delete canteenManageWidget;
    delete accountManageWidget;
    delete bizManageWidget;
    delete canteenManagerManageWidget;
    delete bizManagerManageWidget;
    delete destinationManageWidget;
}

void WorkbenchWidget::load(int accountId)
{
    this->accountId = accountId;
    reload();
}

void WorkbenchWidget::reload()
{
    bool isRoot = accountDao.isRoot(accountId);
    if (isRoot) {
        ui->stackedWidgetAdmin->setCurrentWidget(ui->pageAdminEnabled);
    } else {
        ui->stackedWidgetAdmin->setCurrentWidget(ui->pageAdminDisabled);
    }

    auto &&[isUser, userError, userId] = userDao.getUserId(accountId);
    this->userId = userId;
    if (isUser) {
        ui->stackedWidgetUser->setCurrentWidget(ui->pageUserEnabled);
    } else {
        ui->stackedWidgetUser->setCurrentWidget(ui->pageUserDisabled);
    }

    auto &&[isCourier, courierError, courierId] = courierDao.getCourierId(accountId);
    this->courierId = courierId;
    if (isCourier) {
        ui->stackedWidgetCourier->setCurrentWidget(ui->pageCourierEnabled);
    } else {
        ui->stackedWidgetCourier->setCurrentWidget(ui->pageCourierDisabled);
    }

    auto &&[isCanteenManager, canteenManagerError, canteenManagerInfo]
        = canteenManagerDao.getCanteens(accountId);
    this->canteenManagerInfo = canteenManagerInfo;
    ui->comboBoxCanteenManager->clear();
    if (isCanteenManager) {
        ui->stackedWidgetCanteenManager->setCurrentWidget(ui->pageCanteenManagerEnabled);
        for (auto &&[canteenId, _] : canteenManagerInfo) {
            auto &&[ok, error, canteenName] = canteenDao.getName(canteenId);
            if (!ok) {
                QMessageBox::information(this, tr("内部错误"), error);
                return;
            }
            ui->comboBoxCanteenManager->addItem(canteenName, canteenId);
        }
    } else {
        ui->stackedWidgetCanteenManager->setCurrentWidget(ui->pageCanteenManagerDisabled);
    }

    auto &&[isBizManager, bizManagerError, bizManagerInfo] = bizManagerDao.getBizs(accountId);
    this->bizManagerInfo = bizManagerInfo;
    ui->comboBoxBizManager->clear();
    if (isBizManager) {
        ui->stackedWidgetBizManager->setCurrentWidget(ui->pageBizManagerEnabled);
        for (auto &&[bizId, _] : bizManagerInfo) {
            auto &&[ok, error, bizName] = bizDao.getName(bizId);
            if (!ok) {
                QMessageBox::information(this, tr("内部错误"), error);
                return;
            }
            ui->comboBoxBizManager->addItem(bizName, bizId);
        }
    } else {
        ui->stackedWidgetBizManager->setCurrentWidget(ui->pageBizManagerDisabled);
    }
}

// Account
void WorkbenchWidget::handleLogout()
{
    emit logout();
}

void WorkbenchWidget::handleResetPassword()
{
    DialogResetPassword *dialog = new DialogResetPassword(this);
    int state = dialog->exec();
    if (state != QDialog::Accepted) {
        return;
    }
    QString oldPassword = dialog->getOldPassword();
    QString newPassword = dialog->getNewPassword();
    if (!accountDao.verifyPasswordFormat(oldPassword)) {
        QMessageBox::information(this, tr("请检查输入"), tr("旧密码太短"));
        return;
    }
    if (!accountDao.verifyPasswordFormat(newPassword)) {
        QMessageBox::information(this, tr("请检查输入"), tr("新密码太短"));
        return;
    }
    auto &&[ok, error] = accountDao.resetPassword(accountId, oldPassword, newPassword);
    if (!ok) {
        QMessageBox::information(this, tr("请检查输入"), error);
        return;
    }
    delete dialog;
}

void WorkbenchWidget::handleChangePhone()
{
    DialogChangePhone *dialog = new DialogChangePhone(this);
    int state = dialog->exec();
    if (state != QDialog::Accepted) {
        return;
    }
    QString phone = dialog->getPhone();
    if (!accountDao.verifyPhoneFormat(phone)) {
        QMessageBox::information(this, tr("请检查输入"), tr("手机号太短"));
        return;
    }
    auto &&[ok, error] = accountDao.changePhone(accountId, phone);
    if (!ok) {
        QMessageBox::information(this, tr("请检查输入"), error);
        return;
    }
    delete dialog;
}

// Admin
void WorkbenchWidget::handleAdminManageCanteen()
{
    if (canteenManageWidget == nullptr) {
        canteenManageWidget = new CanteenManageWidget(db, this);
        connect(canteenManageWidget, &BuzzingWidget::quit, this, &WorkbenchWidget::reload);
    }
    canteenManageWidget->setWindowModality(Qt::ApplicationModal);
    canteenManageWidget->setWindowFlag(Qt::Dialog);
    canteenManageWidget->show();
}

void WorkbenchWidget::handleAdminManageAccount()
{
    if (accountManageWidget == nullptr) {
        accountManageWidget = new AccountManageWidget(db, this);
        connect(accountManageWidget, &BuzzingWidget::quit, this, &WorkbenchWidget::reload);
    }
    accountManageWidget->setWindowModality(Qt::ApplicationModal);
    accountManageWidget->setWindowFlag(Qt::Dialog);
    accountManageWidget->show();
}

void WorkbenchWidget::handleAdminManageBiz()
{
    if (bizManageWidget == nullptr) {
        bizManageWidget = new BizManageWidget(db, this);
        connect(bizManageWidget, &BuzzingWidget::quit, this, &WorkbenchWidget::reload);
    }
    bizManageWidget->setWindowModality(Qt::ApplicationModal);
    bizManageWidget->setWindowFlag(Qt::Dialog);
    bizManageWidget->show();
}

void WorkbenchWidget::handleAdminManageCanteenManager()
{
    if (canteenManagerManageWidget == nullptr) {
        canteenManagerManageWidget = new CanteenManagerManageWidget(db, this);
        connect(canteenManagerManageWidget, &BuzzingWidget::quit, this, &WorkbenchWidget::reload);
    }
    canteenManagerManageWidget->setWindowModality(Qt::ApplicationModal);
    canteenManagerManageWidget->setWindowFlag(Qt::Dialog);
    canteenManagerManageWidget->show();
}

void WorkbenchWidget::handleAdminManageBizManager()
{
    if (bizManagerManageWidget == nullptr) {
        bizManagerManageWidget = new BizManagerManageWidget(db, this);
        connect(bizManagerManageWidget, &BuzzingWidget::quit, this, &WorkbenchWidget::reload);
    }
    bizManagerManageWidget->setWindowModality(Qt::ApplicationModal);
    bizManagerManageWidget->setWindowFlag(Qt::Dialog);
    bizManagerManageWidget->show();
}

// User
void WorkbenchWidget::handleUserRegister()
{
    DialogRegisterUser *dialog = new DialogRegisterUser(this);
    int state = dialog->exec();
    if (state != QDialog::Accepted) {
        return;
    }
    QString userName = dialog->getUserName();
    auto &&[ok, error, userId] = userDao.registerUser(accountId, userName);
    if (!ok) {
        QMessageBox::information(this, tr("请检查输入"), error);
        return;
    }
    this->reload();
    delete dialog;
}

void WorkbenchWidget::handleUserManageDestination()
{
    if (destinationManageWidget == nullptr) {
        destinationManageWidget = new DestinationManageWidget(db, this);
        connect(destinationManageWidget, &BuzzingWidget::quit, this, &WorkbenchWidget::reload);
    }
    destinationManageWidget->setUserId(userId);
    destinationManageWidget->setWindowModality(Qt::ApplicationModal);
    destinationManageWidget->setWindowFlag(Qt::Dialog);
    destinationManageWidget->show();
}

void WorkbenchWidget::handleUserCreateOrder()
{
    if (userCreateOrderWidget == nullptr) {
        userCreateOrderWidget = new UserCreateOrderWidget(db, this);
        connect(userCreateOrderWidget, &BuzzingWidget::quit, this, &WorkbenchWidget::reload);
    }
    userCreateOrderWidget->setUserId(userId);
    userCreateOrderWidget->setWindowModality(Qt::ApplicationModal);
    userCreateOrderWidget->setWindowFlag(Qt::Dialog);
    userCreateOrderWidget->show();
}

void WorkbenchWidget::handleUserManageOrder()
{
    if (userManageOrderWidget == nullptr) {
        userManageOrderWidget = new UserManageOrderWidget(db, this);
        connect(userManageOrderWidget, &BuzzingWidget::quit, this, &WorkbenchWidget::reload);
    }
    userManageOrderWidget->setUserId(userId);
    userManageOrderWidget->setWindowModality(Qt::ApplicationModal);
    userManageOrderWidget->setWindowFlag(Qt::Dialog);
    userManageOrderWidget->show();
}

void WorkbenchWidget::handleUserManageUser()
{
    auto &&[infoOk, infoError, userInfo] = userDao.getInfo(userId);
    if (!infoOk) {
        QMessageBox::information(this, tr("内部错误"), infoError);
        return;
    }

    DialogManageUser *dialog = new DialogManageUser(this);
    dialog->setUserName(userInfo.name);
    dialog->setUserPic(userInfo.pic);

    int state = dialog->exec();
    if (state != QDialog::Accepted) {
        return;
    }

    auto &&[setOk, setError] = userDao.setInfo(userId,
                                               {
                                                   dialog->getUserName(),
                                                   dialog->getUserPic(),
                                               });
    if (!setOk) {
        QMessageBox::information(this, tr("请检查输入"), setError);
        return;
    }
    this->reload();
    delete dialog;
}

// Courier
void WorkbenchWidget::handleCourierRegister()
{
    DialogRegisterCourier *dialog = new DialogRegisterCourier(this);
    int state = dialog->exec();
    if (state != QDialog::Accepted) {
        return;
    }
    QString courierName = dialog->getCourierName();
    QString courierPhone = dialog->getCourierPhone();
    if (!courierDao.verifyPhoneFormat(courierPhone)) {
        QMessageBox::information(this, tr("请检查输入"), tr("手机号太短"));
        return;
    }
    auto &&[ok, error, courierId] = courierDao.registerCourier(accountId, courierName, courierPhone);
    if (!ok) {
        QMessageBox::information(this, tr("请检查输入"), error);
        return;
    }
    this->reload();
    delete dialog;
}

void WorkbenchWidget::handleCourierManageOrder()
{
    if (courierManageOrderWidget == nullptr) {
        courierManageOrderWidget = new CourierManageOrderWidget(db, this);
        connect(courierManageOrderWidget, &BuzzingWidget::quit, this, &WorkbenchWidget::reload);
    }
    courierManageOrderWidget->setCourierId(courierId);
    courierManageOrderWidget->setWindowModality(Qt::ApplicationModal);
    courierManageOrderWidget->setWindowFlag(Qt::Dialog);
    courierManageOrderWidget->show();
}

void WorkbenchWidget::handleCourierManageCourier()
{
    auto &&[infoOk, infoError, courierInfo] = courierDao.getInfo(courierId);
    if (!infoOk) {
        QMessageBox::information(this, tr("内部错误"), infoError);
        return;
    }
    DialogManageCourier *dialog = new DialogManageCourier(this);
    dialog->setCourierName(courierInfo.name);
    dialog->setCourierPhone(courierInfo.phone);
    dialog->setCourierPic(courierInfo.pic);

    int state = dialog->exec();
    if (state != QDialog::Accepted) {
        return;
    }

    QString courierPhone = dialog->getCourierPhone();
    if (!courierDao.verifyPhoneFormat(courierPhone)) {
        QMessageBox::information(this, tr("请检查输入"), tr("手机号太短"));
        return;
    }
    auto &&[setOk, setError] = courierDao.setInfo(courierId,
                                                  {dialog->getCourierName(),
                                                   courierPhone,
                                                   dialog->getCourierPic()});
    if (!setOk) {
        QMessageBox::information(this, tr("请检查输入"), setError);
        return;
    }
    this->reload();
    delete dialog;
}

// Biz Manager
void WorkbenchWidget::handleBizManagerManageMeal()
{
    if (bizManagerManageMealWidget == nullptr) {
        bizManagerManageMealWidget = new BizManagerManageMealWidget(db, this);
        connect(bizManagerManageMealWidget, &BuzzingWidget::quit, this, &WorkbenchWidget::reload);
    }
    int bizId = ui->comboBoxBizManager->currentData().toInt();
    bizManagerManageMealWidget->setBizId(bizId);
    bizManagerManageMealWidget->setWindowModality(Qt::ApplicationModal);
    bizManagerManageMealWidget->setWindowFlag(Qt::Dialog);
    bizManagerManageMealWidget->show();
}

void WorkbenchWidget::handleBizManagerManageBiz()
{
    int bizId = ui->comboBoxBizManager->currentData().toInt();
    auto &&[infoOk, infoError, bizInfo] = bizDao.getInfo(bizId);
    if (!infoOk) {
        QMessageBox::information(this, tr("内部错误"), infoError);
        return;
    }

    DialogManageBiz *dialog = new DialogManageBiz(this);
    dialog->setBizName(bizInfo.name);
    dialog->setBizDesc(bizInfo.desc);
    dialog->setBizPic(bizInfo.pic);

    int state = dialog->exec();
    if (state != QDialog::Accepted) {
        return;
    }

    auto &&[setOk, setError] = bizDao.setInfo(bizId,
                                              {
                                                  dialog->getBizName(),
                                                  dialog->getBizDesc(),
                                                  dialog->getBizPic(),
                                              });
    if (!setOk) {
        QMessageBox::information(this, tr("请检查输入"), setError);
        return;
    }
    this->reload();
    delete dialog;
}

void WorkbenchWidget::handleBizManagerManageBizManager()
{
    int bizId = ui->comboBoxBizManager->currentData().toInt();
    auto it = bizManagerInfo.begin();
    for (; it != bizManagerInfo.end(); it++) {
        if (it->bizId == bizId) {
            break;
        }
    }
    if (!it->root) {
        QMessageBox::information(this, tr("暂无权限"), tr("此功能仅特权管理使用"));
        return;
    }
    if (bizManagerManageBizManagerWidget == nullptr) {
        bizManagerManageBizManagerWidget = new BizManagerManageBizManagerWidget(db, this);
        connect(bizManagerManageBizManagerWidget,
                &BuzzingWidget::quit,
                this,
                &WorkbenchWidget::reload);
    }
    bizManagerManageBizManagerWidget->setBizId(bizId);
    bizManagerManageBizManagerWidget->setWindowModality(Qt::ApplicationModal);
    bizManagerManageBizManagerWidget->setWindowFlag(Qt::Dialog);
    bizManagerManageBizManagerWidget->show();
}

void WorkbenchWidget::handleBizManagerManageOrder()
{
    if (bizManagerManageOrderWidget == nullptr) {
        bizManagerManageOrderWidget = new BizManagerManageOrderWidget(db, this);
        connect(bizManagerManageOrderWidget, &BuzzingWidget::quit, this, &WorkbenchWidget::reload);
    }
    int bizId = ui->comboBoxBizManager->currentData().toInt();
    bizManagerManageOrderWidget->setBizId(bizId);
    bizManagerManageOrderWidget->setWindowModality(Qt::ApplicationModal);
    bizManagerManageOrderWidget->setWindowFlag(Qt::Dialog);
    bizManagerManageOrderWidget->show();
}

// Canteen Manager
void WorkbenchWidget::handleCanteenManagerManageWindow()
{
    if (canteenManagerManageWindowWidget == nullptr) {
        canteenManagerManageWindowWidget = new CanteenManagerManageWindowWidget(db, this);
        connect(canteenManagerManageWindowWidget,
                &BuzzingWidget::quit,
                this,
                &WorkbenchWidget::reload);
    }
    int canteenId = ui->comboBoxCanteenManager->currentData().toInt();
    canteenManagerManageWindowWidget->setCanteenId(canteenId);
    canteenManagerManageWindowWidget->setWindowModality(Qt::ApplicationModal);
    canteenManagerManageWindowWidget->setWindowFlag(Qt::Dialog);
    canteenManagerManageWindowWidget->show();
}

void WorkbenchWidget::handleCanteenManagerManageCanteen()
{
    int canteenId = ui->comboBoxCanteenManager->currentData().toInt();
    auto &&[infoOk, infoError, canteenInfo] = canteenDao.getInfo(canteenId);
    if (!infoOk) {
        QMessageBox::information(this, tr("内部错误"), infoError);
        return;
    }

    DialogManageCanteen *dialog = new DialogManageCanteen(this);
    dialog->setCanteenName(canteenInfo.name);
    dialog->setCanteenDesc(canteenInfo.desc);
    dialog->setCanteenPic(canteenInfo.pic);

    int state = dialog->exec();
    if (state != QDialog::Accepted) {
        return;
    }

    auto &&[setOk, setError] = canteenDao.setInfo(canteenId,
                                                  {
                                                      dialog->getCanteenName(),
                                                      dialog->getCanteenDesc(),
                                                      dialog->getCanteenPic(),
                                                  });
    if (!setOk) {
        QMessageBox::information(this, tr("请检查输入"), setError);
        return;
    }
    this->reload();
    delete dialog;
}

void WorkbenchWidget::handleCanteenManagerManageCanteenManager()
{
    int canteenId = ui->comboBoxCanteenManager->currentData().toInt();
    auto it = canteenManagerInfo.begin();
    for (; it != canteenManagerInfo.end(); it++) {
        if (it->canteenId == canteenId) {
            break;
        }
    }
    if (!it->root) {
        QMessageBox::information(this, tr("暂无权限"), tr("此功能仅特权管理使用"));
        return;
    }
    if (canteenManagerManageCanteenManagerWidget == nullptr) {
        canteenManagerManageCanteenManagerWidget
            = new CanteenManagerManageCanteenManagerWidget(db, this);
        connect(canteenManagerManageCanteenManagerWidget,
                &BuzzingWidget::quit,
                this,
                &WorkbenchWidget::reload);
    }
    canteenManagerManageCanteenManagerWidget->setCanteenId(canteenId);
    canteenManagerManageCanteenManagerWidget->setWindowModality(Qt::ApplicationModal);
    canteenManagerManageCanteenManagerWidget->setWindowFlag(Qt::Dialog);
    canteenManagerManageCanteenManagerWidget->show();
}

void WorkbenchWidget::handleCanteenManagerManageBiz()
{
    if (canteenManagerManageBizWidget == nullptr) {
        canteenManagerManageBizWidget = new CanteenManagerManageBizWidget(db, this);
        connect(canteenManagerManageBizWidget, &BuzzingWidget::quit, this, &WorkbenchWidget::reload);
    }
    int canteenId = ui->comboBoxCanteenManager->currentData().toInt();
    canteenManagerManageBizWidget->setCanteenId(canteenId);
    canteenManagerManageBizWidget->setWindowModality(Qt::ApplicationModal);
    canteenManagerManageBizWidget->setWindowFlag(Qt::Dialog);
    canteenManagerManageBizWidget->show();
}
