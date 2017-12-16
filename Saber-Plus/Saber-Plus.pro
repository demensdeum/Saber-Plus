#-------------------------------------------------
#
# Project created by QtCreator 2017-11-13T23:10:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Saber-Plus
TEMPLATE = app

CONFIG += c++14

SOURCES += main.cpp\
        mainwindow.cpp \
    spdebugger.cpp \
    spbreakpoint.cpp \
    spnode.cpp \
    codeeditor.cpp

HEADERS  += mainwindow.h \
    spdebugger.h \
    spbreakpoint.h \
    spnode.h \
    codeeditor.h

FORMS    += mainwindow.ui
