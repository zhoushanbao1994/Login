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
    exam.cpp

HEADERS  += \
    app.h \
    exam.h

FORMS    += \
    exam.ui


include(./login/login.pri)
include(./systemtrayicon/systemtrayicon.pri)
