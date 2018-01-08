#-------------------------------------------------
#
# Project created by QtCreator 2014-09-24T23:28:33
#
#-------------------------------------------------

QT       += core gui concurrent printsupport
QT       += positioning

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Swarm-Detect
TEMPLATE = app


SOURCES += main.cpp\
    settings.cpp \
    qcustomplot/qcustomplot.cpp \
    graph.cpp \
    cluster.cpp \
    clusterviewer.cpp \
    EQCluster.cpp

HEADERS  += \
    settings.h \
    qcustomplot/qcustomplot.h \
    graph.h \
    cluster.h \
    clusterviewer.h \
    EQCluster.h

FORMS    += \
    settings.ui \
    graph.ui \
    clusterviewer.ui \
    EQCluster.ui
