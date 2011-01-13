\rm -rf \
    debian/files                \
    debian/tmp                  \
    debian/transfer-ui/         \
    debian/transfer-ui-dbg/     \
    debian/transfer-ui-dev/     \
    debian/transfer-ui-doc/     \
    debian/transfer-ui-tests/   \
    debian/transfer-ui-utils/   \
    debian/*.substvars          \
    debian/*.debhelper

find . -name Makefile -exec \rm -rf {} \;
find . -name moc -exec \rm -rf {} \;
find . -name obj -exec \rm -rf {} \;
find . -name out -exec \rm -rf {} \;
find . -name *~ -exec \rm -rf {} \;

\rm -rf \
    build-stamp     \
    doc/            \
    config*

\rm -rf \
    transfer-ui/transferengine_adaptor.*            \
    transfer-ui-utils/transferengine_interface.*    \
    tests/transferengine_adaptor.* 

\rm -rf \
    ../transfer-ui*.deb    \
    ../transfer-ui*.dsc    \
    ../transfer-ui*.tar.gz \
    ../transfer-ui*.changes 

qmake
make distclean
