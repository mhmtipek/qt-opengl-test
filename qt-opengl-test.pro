#-------------------------------------------------
#
# Project created by QtCreator 2013-05-29T20:23:51
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt-opengl-test
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    myglwidget.cpp \
    vertexdata.cpp

HEADERS  += mainwindow.h \
    myglwidget.h \
    vertexdata.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    vertex_shader.vert \
    fragment_shader.frag

RESOURCES += \
    resources.qrc
