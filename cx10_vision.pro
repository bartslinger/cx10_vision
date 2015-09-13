#-------------------------------------------------
#
# Project created by QtCreator 2015-08-17T15:29:50
#
#-------------------------------------------------

QT       += core gui gamecontroller serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = cx10_vision
TEMPLATE = app

INCLUDEPATH += /usr/local/include/opencv/
LIBS += -L/usr/local/lib
LIBS += -lopencv_core
LIBS += -lopencv_imgproc
LIBS += -lopencv_highgui
LIBS += -lopencv_ml
LIBS += -lopencv_video
LIBS += -lopencv_features2d
LIBS += -lopencv_calib3d
LIBS += -lopencv_objdetect
LIBS += -lopencv_contrib
LIBS += -lopencv_legacy
LIBS += -lopencv_flann
LIBS += -lopencv_videoio
#LIBS += -lopencv_nonfree


SOURCES += main.cpp\
        mainwindow.cpp \
    serialcontroller.cpp \
    recorder.cpp \
    visionmodule.cpp \
    qcustomplot.cpp \
    altitudeplot.cpp \
    datalogger.cpp \
    pdcontroller.cpp \
    pidcontroller.cpp \
    binarycontroller.cpp

HEADERS  += mainwindow.h \
    serialcontroller.h \
    recorder.h \
    visionmodule.h \
    qcustomplot.h \
    altitudeplot.h \
    datalogger.h \
    pdcontroller.h \
    pidcontroller.h \
    binarycontroller.h

FORMS    += mainwindow.ui
