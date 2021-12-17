QT += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

################################################################################

DESTDIR = ../bin
TARGET = mscsim-test-rotor

################################################################################

CONFIG += fdm_test

################################################################################

DEFINES += \
    SIM_ROTOR_TEST

greaterThan(QT_MAJOR_VERSION, 4):win32: DEFINES += USE_QT5

win32: DEFINES += \
    WIN32 \
    _WINDOWS \
    _CRT_SECURE_NO_DEPRECATE \
    _SCL_SECURE_NO_WARNINGS \
    _USE_MATH_DEFINES

win32: CONFIG(release, debug|release): DEFINES += \
    NDEBUG

win32: CONFIG(debug, debug|release): DEFINES += \
    _DEBUG

unix: DEFINES += _LINUX_

################################################################################

INCLUDEPATH += . ..

win32: INCLUDEPATH += \
    $(OSG_ROOT)/include/ \
    $(OSG_ROOT)/include/libxml2

unix: INCLUDEPATH += \
    /usr/include/libxml2

################################################################################

win32: LIBS += \
    -L$(OSG_ROOT)/lib \
    -llibxml2 \
    -lopengl32 \
    -lwinmm

win32: CONFIG(release, debug|release): LIBS += \
    -lOpenThreads \
    -losg \
    -losgDB \
    -losgGA \
    -losgParticle \
    -losgSim \
    -losgText \
    -losgUtil \
    -losgViewer \
    -losgWidget

win32: CONFIG(debug, debug|release): LIBS += \
    -lOpenThreadsd \
    -losgd \
    -losgDBd \
    -losgGAd \
    -losgParticled \
    -losgSimd \
    -losgTextd \
    -losgUtild \
    -losgViewerd \
    -losgWidgetd

unix: LIBS += \
    -L/lib \
    -L/usr/lib \
    -lxml2 \
    -lOpenThreads \
    -losg \
    -losgDB \
    -losgGA \
    -losgParticle \
    -losgSim \
    -losgText \
    -losgUtil \
    -losgViewer \
    -losgWidget

################################################################################

HEADERS += \
    $$PWD/Data.h \
    $$PWD/defs.h

SOURCES += \
    $$PWD/main.cpp

include(cgi/cgi.pri)
include(fdm/fdm.pri)
include(gui/gui.pri)
include(hid/hid.pri)
include(fdm_test/fdm_test.pri)
