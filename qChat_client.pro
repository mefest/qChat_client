#-------------------------------------------------
#
# Project created by QtCreator 2013-09-03T17:00:17
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qChat_client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    win_connect.cpp \
    client.cpp \
    blowfish.cpp \
    settings.cpp \
    messinput.cpp \
    saveload.cpp

HEADERS  += mainwindow.h \
    win_connect.h \
    client.h \
    blowfish.h \
    settings.h \
    messinput.h \
    saveload.h

FORMS    += mainwindow.ui \
    win_connect.ui \
    settings.ui

RESOURCES += \
    resource.qrc
