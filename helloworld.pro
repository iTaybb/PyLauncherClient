#-------------------------------------------------
#
# Project created by QtCreator 2019-01-26T01:07:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PyLauncherClient
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    servergateway.cpp \
    rest.cpp \
    utils.cpp \
    workers.cpp

HEADERS  += mainwindow.h \
    rest.h \
    servergateway.h \
    utils.h \
    vars.h \
    workers.h

FORMS    += mainwindow.ui

LIBS     += -lboost_system -lcrypto -lssl -lcpprest

CONFIG   += c++11

QMAKE_CXXFLAGS += -D_FORTIFY_SOURCE=2 -D_GLIBCXX_ASSERTIONS -fstack-protector-strong -pie

RESOURCES += \
    qml.qrc
