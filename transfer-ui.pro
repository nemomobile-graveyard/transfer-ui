TEMPLATE = subdirs
SUBDIRS += transfer-ui \
           libtuiclient \
           tests
                       
# API Documentation with Doxygen
dox.path        = /usr/share/doc/transfer-ui
!contains( DEFINES, NO_DOCS ) {
    dox.commands = doxygen doxygen.cfg
}
dox.depends     = libtuiclient/TransferUI
dox.files       = doc/html/*


# Doxygen Examples
tui_example.path       = /usr/share/doc/transfer-ui/examples
tui_example.files      = libtuiclient/examples/tuiexample.cpp   

INSTALLS       += dox tui_example

QMAKE_EXTRA_TARGETS  += dox 
