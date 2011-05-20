include (transfer-ui.pri)

QMAKE_CXXFLAGS += -Werror
QMAKE_CXXFLAGS += -g 
QMAKE_CXXFLAGS += -O2
CONFIG += debug_and_release

TARGET = transfer-ui
TRANSLATIONS = transfer-ui.ts
DBUS_ADAPTORS    = dbus-xml/TransferUI.xml 

CONFIG(profiling) {
    message(Building with profiling)
    QMAKE_CXXFLAGS += -ftest-coverage -fprofile-arcs
    LIBS += -lgcov
}

#enable new transfer ui changes
DEFINES += SHOW_HISTORY

DEPENDPATH  += ./src
INCLUDEPATH += ./inc ./src ./TransferUI

VPATH       += $$DEPENDPATH $$INCLUDEPATH

# installations configuration
target.path = /usr/bin

INCLUDEPATH += ../src ./TransferUI
SOURCES +=main.cpp             

pubheaders.path  = /usr/include/TransferUI
pubheaders.files = dbus-xml/TransferUI.xml TransferUI/*


# Install prf file for others to use
prf.path            = $$[QT_INSTALL_DATA]/mkspecs/features
prf.files           = transfer-ui.prf

# service file
tui_service_entry.path = /usr/share/dbus-1/services/
tui_service_entry.files = com.meego.transferui.service

tui_settings_file.path = /usr/share/transfer-ui/settings/
tui_settings_file.files = tuisettings

INSTALLS           +=   target      \
                        prf         \
                        pubheaders  \
                        tui_service_entry   \
                        tui_settings_file

