#-------------------------------------------------
#
# Project created by QtCreator 2020-02-24T21:04:12
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mains_grapher
TEMPLATE = app

include (qwtconfig.pri)

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
    ccalibrationdata.cpp \
    ccalibrationwidget.cpp \
    cmeasurements.cpp \
        main.cpp \
        mainwindow.cpp \
    plot.cpp \
    cdatabufffer.cpp \
    bufferedcurve.cpp \
    cadccontroller.cpp \
    ccs5451_controller.cpp \
    cqserialinterface.cpp \
    powers_widget.cpp

HEADERS += \
    ccalibrationdata.h \
    ccalibrationwidget.h \
    cmeasurements.h \
        mainwindow.h \
    plot.h \
    cdatabufffer.h \
    bufferedcurve.h \
    cadccontroller.h \
    cadcctrl_interface.h \
    ccs5451_controller.h \
    cqserialinterface.h \
    powers_widget.h
FORMS += \
        ccalibrationwidget.ui \
        mainwindow.ui \
        powers_widget.ui

INCLUDEPATH += "/usr/include/qwt6"
DEPENDPATH  += "/usr/include/qwt6"
LIBS        += -lqwt6-qt5


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
