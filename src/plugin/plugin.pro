TEMPLATE = lib
TARGET  = nemodevicelockplugin
TARGET = $$qtLibraryTarget($$TARGET)
MODULENAME = org/nemomobile/devicelock
TARGETPATH = $$[QT_INSTALL_QML]/$$MODULENAME

QT -= gui
QT += dbus qml

CONFIG += \
        plugin \
        hide_symbols \
        c++11 \
        link_pkgconfig

INCLUDEPATH += \
        $$PWD/../ \
        $$PWD/../nemo-devicelock

LIBS += -L$$OUT_PWD/../nemo-devicelock -lnemodevicelock

SOURCES = \
        plugin.cpp

import.files = \
        qmldir
import.path = $$TARGETPATH

target.path = $$TARGETPATH

INSTALLS += \
        import \
        target
