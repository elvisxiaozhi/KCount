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
    overview.cpp \
    totalpressed.cpp \
    hook.cpp \
    signalemitter.cpp \
    database.cpp \
    mostpressed.cpp \
    mouseclick.cpp \
    label.cpp \
    dashboard.cpp \
    users.cpp \
    settings.cpp \
    barchart.cpp \
    initialisation.cpp \
    messagebox.cpp \
    stackedbarchart.cpp \
    piechart.cpp \
    mostused.cpp \
    appusagestackedbarchart.cpp \
    customtitlebar.cpp \
    custombutton.cpp \
    callout.cpp \
    notification.cpp \
    applimits.cpp \
    about.cpp

HEADERS += \
        mainwindow.h \
    sidebar.h \
    overview.h \
    totalpressed.h \
    hook.h \
    signalemitter.h \
    singleton.h \
    database.h \
    mostpressed.h \
    mouseclick.h \
    label.h \
    dashboard.h \
    users.h \
    settings.h \
    barchart.h \
    initialisation.h \
    messagebox.h \
    stackedbarchart.h \
    piechart.h \
    mostused.h \
    appusagestackedbarchart.h \
    customtitlebar.h \
    custombutton.h \
    callout.h \
    notification.h \
    applimits.h \
    about.h

win32 {
    RC_FILE = config/AdminPriv.rc
}

RESOURCES += \
    icons/icons.qrc
