QT += core network sql
QT -= gui

CONFIG += c++11

TARGET = NewTCPThread
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    myserver.cpp \
    systemonlist.cpp \
    receivthread.cpp \
    connectionpool.cpp \
    updateonlist.cpp \
    sendmessagethread.cpp

HEADERS += \
    myserver.h \
    systemonlist.h \
    receivthread.h \
    connectionpool.h \
    updateonlist.h \
    sendmessagethread.h
