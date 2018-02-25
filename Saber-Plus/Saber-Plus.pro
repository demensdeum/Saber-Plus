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
    codeeditor.cpp \
    sppresenter.cpp \
    spproject.cpp \
    spfile.cpp \
    spserializable.cpp \
    spstate.cpp \
    spforwardstatemachine.cpp \
    spdiagnosticsservice.cpp \
    spprojectbuilderservice.cpp \
    spdiagnosticissue.cpp \
    spdiagnosticissuesfixer.cpp \
    sptextsearchinfilesservice.cpp \
    sptextsearchinfilesmatch.cpp \
    spstacknode.cpp \
    spvariablenode.cpp \
    spprojectsymbolsservice.cpp \
    spprojectsymbol.cpp

HEADERS  += mainwindow.h \
    spdebugger.h \
    spbreakpoint.h \
    codeeditor.h \
    sppresenter.h \
    spproject.h \
    spfile.h \
    spserializable.h \
    spstate.h \
    spforwardstatemachine.h \
    spdiagnosticsservice.h \
    spprojectbuilderservice.h \
    spdiagnosticissue.h \
    spdiagnosticissuesfixer.h \
    sptextsearchinfilesservice.h \
    splist.h \
    sptextsearchinfilesmatch.h \
    spstacknode.h \
    spvariablenode.h \
    spprojectsymbolsservice.h \
    spprojectsymbol.h

FORMS    += mainwindow.ui
