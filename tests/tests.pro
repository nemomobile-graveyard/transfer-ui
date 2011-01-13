include (../transfer-ui/transfer-ui.pri)
CONFIG += qt debug link_pkgconfig
QT += testlib
TEMPLATE = app

TARGET = transfer-ui-tests

DEPENDPATH  += ./src  ../transfer-ui/src ../transfer-ui/TransferUI
               
INCLUDEPATH += ./src ../transfer-ui/ ../transfer-ui/src \
                ../transfer-ui/TransferUI

DEFINES += _UNIT_TESTING_

DBUS_ADAPTORS    = ../transfer-ui/dbus-xml/TransferUI.xml 

VPATH += $$DEPENDPATH $$INCLUDEPATH

OBJECTS_DIR = ./obj
MOC_DIR     = ./moc
DESTDIR     = ./out

HEADERS += transferuitests.h
            
SOURCES += transferuitests.cpp
                        
CONFIG(profiling) {
    message(Building with profiling)
    QMAKE_CXXFLAGS += -ftest-coverage -fprofile-arcs
    LIBS += -lgcov
}

DEFINES     += QT_NO_WARNING_OUTPUT QT_NO_DEBUG_OUTPUT

target.path = /usr/bin

INSTALLS += target

testfiles.path = /usr/share/transfer-ui-tests
testfiles.files = tests.xml
INSTALLS += testfiles
