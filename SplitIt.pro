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
    volumelabel.cpp \
    textinput.cpp \
    seeklabel.cpp

HEADERS  += splitit.h \
    clickablelabel.h \
    volumelabel.h \
    textinput.h \
    seeklabel.h

FORMS    += splitit.ui

RESOURCES += \
    splititresource.qrc


win32: LIBS += -L$$PWD/'../../../../Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/lowlevel/lib/' -lfmod_vc

INCLUDEPATH += $$PWD/'../../../../Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/lowlevel/inc'
DEPENDPATH += $$PWD/'../../../../Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/lowlevel/inc'
