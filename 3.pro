TEMPLATE = app

QT += widgets qml quick sql
CONFIG += c++11

SOURCES += main.cpp \
    myapplication.cpp\
    connector.cpp \
    constanthelper.cpp \
    modeldata.cpp \
    dbthread.cpp


RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

DISTFILES +=

HEADERS += \
    myapplication.h \
    connector.h \
    constanthelper.h \
    modeldata.h \
    dbthread.h


