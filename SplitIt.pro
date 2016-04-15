#-------------------------------------------------
#
# Project created by QtCreator 2016-04-11T20:49:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SplitIt
TEMPLATE = app


SOURCES += main.cpp\
        splitit.cpp \
    clickablelabel.cpp \
    draglabel.cpp

HEADERS  += splitit.h \
    clickablelabel.h \
    draglabel.h

FORMS    += splitit.ui

RESOURCES += \
    splititresource.qrc
