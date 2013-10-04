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
    client.cpp

HEADERS  += mainwindow.h \
    win_connect.h \
    client.h

FORMS    += mainwindow.ui \
    win_connect.ui
