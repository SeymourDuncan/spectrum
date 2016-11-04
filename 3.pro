TEMPLATE = app

QT += widgets qml quick sql
CONFIG += c++11

SOURCES += main.cpp \
    myapplication.cpp\
    connector.cpp \
    constanthelper.cpp \
    modeldata.cpp \
    dbthread.cpp \
    dataloader.cpp


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
    dbthread.h \
    dataloader.h



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../CoreDataPlugin/release/ -lCoreDataPlugin
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../CoreDataPlugin/debug/ -lCoreDataPlugin
else:unix: LIBS += -L$$PWD/../CoreDataPlugin/ -lCoreDataPlugin

INCLUDEPATH += $$PWD/../CoreDataPlugin/debug
DEPENDPATH += $$PWD/../CoreDataPlugin/debug
