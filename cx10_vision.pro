#-------------------------------------------------
#
# Project created by QtCreator 2015-08-17T15:29:50
#
#-------------------------------------------------

QT       += core gui gamecontroller serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Joy2CX10
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    serialcontroller.cpp \
    recorder.cpp

HEADERS  += mainwindow.h \
    serialcontroller.h \
    recorder.h

FORMS    += mainwindow.ui
