#-------------------------------------------------
#
# Project created by QtCreator 2018-05-11T19:25:13
#
#-------------------------------------------------

QT += core gui
QT += sql
QT += charts
LIBS += -L\lib\ -lpsapi

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Nana
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    sidebar.cpp \
    label.cpp \
    initialisation.cpp \
    messagebox.cpp \
    customtitlebar.cpp \
    custombutton.cpp \
    notification.cpp \
    core/hook.cpp \
    core/database.cpp \
    overview/mostpressed.cpp \
    overview/mostused.cpp \
    overview/mouseclick.cpp \
    overview/overview.cpp \
    overview/totalpressed.cpp \
    dashboard/appusagestackedbarchart.cpp \
    dashboard/barchart.cpp \
    dashboard/callout.cpp \
    dashboard/dashboard.cpp \
    dashboard/piechart.cpp \
    dashboard/stackedbarchart.cpp \
    settings/about.cpp \
    settings/applimits.cpp \
    settings/reset.cpp \
    settings/settings.cpp \
    users/users.cpp \
    core/signalemitter.cpp \
    core/runguard.cpp

HEADERS += \
        mainwindow.h \
    sidebar.h \
    label.h \
    initialisation.h \
    messagebox.h \
    customtitlebar.h \
    custombutton.h \
    notification.h \
    core/hook.h \
    core/database.h \
    overview/mostpressed.h \
    overview/mostused.h \
    overview/mouseclick.h \
    overview/overview.h \
    overview/totalpressed.h \
    dashboard/appusagestackedbarchart.h \
    dashboard/barchart.h \
    dashboard/callout.h \
    dashboard/dashboard.h \
    dashboard/piechart.h \
    dashboard/stackedbarchart.h \
    settings/about.h \
    settings/applimits.h \
    settings/reset.h \
    settings/settings.h \
    users/users.h \
    core/signalemitter.h \
    core/singleton.h \
    core/runguard.h

win32 {
    RC_FILE = config/AdminPriv.rc
}

RESOURCES += \
    icons/icons.qrc
