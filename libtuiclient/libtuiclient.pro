CONFIG += qt debug dbusinterfaces
#QT -= gui
QT += dbus
TEMPLATE = lib

VER_MAJ=0
VER_MIN=1
VER_PAT=0

#enable deprecations
DEFINES += NO_DEPRECATION

QMAKE_CXXFLAGS += -DTUI_EXPORTS
QMAKE_CXXFLAGS += -Werror
QMAKE_CXXFLAGS += -g
CONFIG += debug_and_release
QMAKE_CXXFLAGS += -O2

DBUS_INTERFACES = ../transfer-ui/dbus-xml/TransferUI.xml

TARGET = tuiclient

DEPENDPATH += ./src .
INCLUDEPATH += . ../transfer-ui/TransferUI

VPATH += $$DEPENDPATH $$INCLUDEPATH

OBJECTS_DIR = ./obj
MOC_DIR = ./moc
DESTDIR = ./out

HEADERS += TransferUI/client.h \
           TransferUI/transfer.h \
           clientprivate.h \
           persistenttransfer.h \
           transferprivate.h \
           transienttransfer.h \
           TransferUI/export.h \
		   TransferUI/showdetailsinterface.h \
           TransferUI/ShowDetailsInterface

SOURCES += client.cpp \
           persistenttransfer.cpp \
           transfer.cpp \
           transienttransfer.cpp    \
           tuistructures.cpp

# Install binary application
target.path = /usr/lib

#remove deprecated headers
pubheaders.path = /usr/include/TransferUI
#pubheaders.files =  TransferUI/*
pubheaders.files =  TransferUI/Client \
                    TransferUI/Transfer \
                    TransferUI/client.h \
                    TransferUI/transfer.h   \
                    TransferUI/export.h \
                    TransferUI/showdetailsinterface.h \
                    TransferUI/ShowDetailsInterface


#remove deprecated headers
pubheaders_old.path = /usr/include/
pubheaders_old.files =  TransferUI/TUIClient.h \
                        TransferUI/TUITransfer.h \



# Install prf file for others to use
prf.path = $$[QT_INSTALL_DATA]/mkspecs/features
prf.files = libtuiclient.prf

# Also provide pkgconfig files
pkgconfig.path = /usr/lib/pkgconfig/
pkgconfig.files = libtuiclient.pc

INSTALLS += target        \
            pubheaders    \
            pubheaders_old    \
            prf           \
            pkgconfig
