# 登录窗口

QT       += core gui
QT       += widgets

INCLUDEPATH += $$PWD

HEADERS  += \
        $$PWD/login.h \
        $$PWD/passwdedit.h \
        $$PWD/register.h 

SOURCES += \
        $$PWD/login.cpp \
        $$PWD/passwdedit.cpp \
        $$PWD/register.cpp 

FORMS += \
        $$PWD/login.ui \
        $$PWD/passwdedit.ui \
        $$PWD/register.ui 

RESOURCES += \
        $$PWD/loginqrc.qrc

include(./sqlite/sqlite.pri)
