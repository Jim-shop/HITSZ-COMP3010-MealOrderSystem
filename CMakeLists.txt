cmake_minimum_required(VERSION 3.5)

project(dblab VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

set(PROJECT_SOURCES
    main.cpp
    login_dialog.cpp login_dialog.h login_dialog.ui
    workbench_widget.h workbench_widget.cpp workbench_widget.ui
    dialog_reset_password.h dialog_reset_password.cpp dialog_reset_password.ui
    dialog_change_phone.h dialog_change_phone.cpp dialog_change_phone.ui
    biz_manage_widget.h biz_manage_widget.cpp biz_manage_widget.ui
    account_manage_widget.h account_manage_widget.cpp account_manage_widget.ui
    canteen_manage_widget.h canteen_manage_widget.cpp canteen_manage_widget.ui
    canteen_manager_manage_widget.h canteen_manager_manage_widget.cpp canteen_manager_manage_widget.ui
    biz_manager_manage_widget.h biz_manager_manage_widget.cpp biz_manager_manage_widget.ui
    destination_manage_widget.h destination_manage_widget.cpp destination_manage_widget.ui
    dialog_register_user.h dialog_register_user.cpp dialog_register_user.ui
    dialog_register_courier.h dialog_register_courier.cpp dialog_register_courier.ui
    biz_manager_manage_meal_widget.h biz_manager_manage_meal_widget.cpp biz_manager_manage_meal_widget.ui
    buzzing_widget.h buzzing_widget.cpp
    image_delegate.h image_delegate.cpp
    dialog_manage_biz.h dialog_manage_biz.cpp dialog_manage_biz.ui
    canteen_manager_manage_window_widget.h canteen_manager_manage_window_widget.cpp canteen_manager_manage_window_widget.ui
    dialog_manage_canteen.h dialog_manage_canteen.cpp dialog_manage_canteen.ui
    canteen_manager_manage_canteen_manager_widget.h canteen_manager_manage_canteen_manager_widget.cpp canteen_manager_manage_canteen_manager_widget.ui
    biz_manager_manage_biz_manager_widget.h biz_manager_manage_biz_manager_widget.cpp biz_manager_manage_biz_manager_widget.ui
    user_create_order_widget.h user_create_order_widget.cpp user_create_order_widget.ui
    canteen_manager_manage_biz_widget.h canteen_manager_manage_biz_widget.cpp canteen_manager_manage_biz_widget.ui
    dialog_manage_user.h dialog_manage_user.cpp dialog_manage_user.ui
    dialog_manage_courier.h dialog_manage_courier.cpp dialog_manage_courier.ui
    biz_manager_manage_order_widget.h biz_manager_manage_order_widget.cpp biz_manager_manage_order_widget.ui
    courier_manage_order_widget.h courier_manage_order_widget.cpp courier_manage_order_widget.ui
    dao/order_dao.h dao/order_dao.cpp
    dao/canteen_dao.h dao/canteen_dao.cpp
    dao/biz_manager_dao.h dao/biz_manager_dao.cpp
    dao/courier_dao.h dao/courier_dao.cpp
    dao/result.h
    dao/account_dao.h dao/account_dao.cpp
    dao/user_dao.h dao/user_dao.cpp
    dao/canteen_manager_dao.h dao/canteen_manager_dao.cpp
    dao/biz_dao.h dao/biz_dao.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(dblab
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        res.qrc
        logo.rc
        user_manage_order_widget.h user_manage_order_widget.cpp user_manage_order_widget.ui
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET dblab APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(dblab SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(dblab
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(dblab PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.dblab)
endif()
set_target_properties(dblab PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    AUTOMOC TRUE
)

include(GNUInstallDirs)
install(TARGETS dblab
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(dblab)
endif()
