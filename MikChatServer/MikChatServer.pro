TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
#CONFIG -= qt

HEADERS += \
    mikdatabasemanager.h \
    mikdatabase.h \
    usersmanager.h \
    commondatastruct.h \
    globals.h \
    mikchatserver.h \
    clientreceiver.h \
    clientsender.h \
    clienthandler.h

SOURCES += main.cpp \
    mikdatabasemanager.cpp \
    mikdatabase.cpp \
    usersmanager.cpp \
    mikchatserver.cpp \
    clientreceiver.cpp \
    clientsender.cpp \
    clienthandler.cpp

INCLUDEPATH += /usr/include/mysql/mysql
INCLUDEPATH += /home/mihooke/build/release-install/include

LIBS += -pthread
LIBS += -L/usr/lib/x86_64-linux-gnu -lmysqlclient
LIBS += -L/usr/lib/x86_64-linux-gnu -ljsoncpp
LIBS += -L/home/mihooke/build/release-install/lib -lmuduo_net -lmuduo_base
