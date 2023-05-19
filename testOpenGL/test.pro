#-------------------------------------------------
#
# Project created by QtCreator 2020-12-23T11:16:11
#
#-------------------------------------------------

QT       += core gui
QT       += charts
QT       += network
QT       += sql
static{
    QTPLUGIN+=qtvirtualkeyboardplugin
    QT+=svg
}
#INCLUDEPATH += .//SP++3.0//include

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
    widget_set.cpp \
    mywidget.cpp \
    thread_tcp.cpp \
    widget_report.cpp \
    widget_back.cpp

HEADERS += \
        mywidget.h \
    widget_set.h \
    thread_tcp.h \
    widget_report.h \
    widget_back.h

FORMS += \
        mywidget.ui \
    widget_set.ui \
    widget_report.ui \
    widget_back.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
