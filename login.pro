#-------------------------------------------------
#
# Project created by QtCreator 2015-05-11T16:52:48
#
#-------------------------------------------------

QT       += core gui
QT       += widgets

TARGET = login
TEMPLATE = app

RC_FILE = myapp.rc


SOURCES += main.cpp \
    app.cpp \
    login.cpp \
    exam.cpp \
    passwdedit.cpp \
    register.cpp

HEADERS  += login.h \
    app.h \
    exam.h \
    passwdedit.h \
    register.h

FORMS    += login.ui \
    exam.ui \
    passwdedit.ui \
    register.ui

RESOURCES += \
    myqrc.qrc


include(./sqlite/sqlite.pri)
include(./systemtrayicon/systemtrayicon.pri)
