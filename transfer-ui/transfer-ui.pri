CONFIG      += qt debug dbusadaptors notifications
TEMPLATE     = app
QT          += dbus
#DEFINES     += QT_NO_WARNING_OUTPUT QT_NO_DEBUG_OUTPUT

CONFIG += link_pkgconfig
PKGCONFIG += contentaction-0.1
CONFIG += qmsystem2

OBJECTS_DIR  = ./obj
MOC_DIR      = ./moc
DESTDIR      = ./out

# Input
HEADERS +=  transferenums.h         \
            tuiservice.h            \
            tuiserviceprivate.h     \
            tuidatamodel.h          \
            tuistructures.h         \
            tuiclientdatamodel.h   \
            logger.h                \
            tuidatamodelproxy.h     \
            tuiabstractmodel.h	\
			tuireadhistorythread.h



SOURCES +=  tuiservice.cpp       \
            tuidatamodel.cpp     \
            tuiclientdatamodel.cpp  \
            logger.cpp               \
            tuidatamodelproxy.cpp \
            tuiabstractmodel.cpp	\
			tuireadhistorythread.cpp \
            tuistructures.cpp

