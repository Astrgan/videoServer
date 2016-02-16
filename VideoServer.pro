QT += core
QT += network
QT -= gui

CONFIG += c++11

TARGET = VideoServer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    mythread.cpp \
    myserver.cpp

HEADERS += \
    mythread.h \
    myserver.h
