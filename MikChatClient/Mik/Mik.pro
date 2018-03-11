#-------------------------------------------------
#
# Project created by QtCreator 2017-11-26T18:58:12
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Mik
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    logindialog.cpp \
    networkagentdialog.cpp \
    registeraccountdialog.cpp \
    headimgdialog.cpp \
    settingsdialog.cpp \
    personalinfosdialog.cpp \
    everyfriendwidget.cpp \
    chatdialog.cpp \
    widgets/chattablesubwidget.cpp \
    trivial/tablewidgetautowarp.cpp \
    utils/util.cpp

HEADERS  += mainwindow.h \
    logindialog.h \
    networkagentdialog.h \
    registeraccountdialog.h \
    headimgdialog.h \
    settingsdialog.h \
    common.h \
    personalinfosdialog.h \
    everyfriendwidget.h \
    chatdialog.h \
    widgets/chattablesubwidget.h \
    trivial/tablewidgetautowarp.h \
    utils/util.h \
    utils/defines.h

FORMS    += mainwindow.ui \
    logindialog.ui \
    networkagentdialog.ui \
    registeraccountdialog.ui \
    headimgdialog.ui \
    settingsdialog.ui \
    personalinfosdialog.ui \
    everyfriendwidget.ui \
    chatdialog.ui \
    widgets/chattablesubwidget.ui

RESOURCES += \
    background.qrc