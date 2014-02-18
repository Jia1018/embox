#-------------------------------------------------
#
# Project created by QtCreator 2013-11-13T14:24:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qpidhello
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

INCLUDEPATH += $${ROOT_DIR}/build/gcc/include
INCLUDEPATH += $${ROOT_DIR}/build/qpid/include
INCLUDEPATH += $${ROOT_DIR}/build/boost/include
INCLUDEPATH += $${ROOT_DIR}/build/stlport/include/stlport
HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

contains(QMAKE_LIBS_THREAD, -lpthread) {
    unix: LIBS += -lqpidtypes
    unix: LIBS += -lqpidmessaging
    unix: LIBS += -lqpidcommon
}
