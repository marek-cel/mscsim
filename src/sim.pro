QT += core gui opengl svg xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

################################################################################

DESTDIR = ../bin
TARGET = mscsim

################################################################################

CONFIG += c++11

################################################################################

win32: RC_FILE = sim.rc

################################################################################

win32: CONFIG(release, debug|release): QMAKE_CXXFLAGS += -O2
unix:  CONFIG(release, debug|release): QMAKE_CXXFLAGS += -O3

#win32: QMAKE_CXXFLAGS += /Zc:wchar_t

win32: QMAKE_LFLAGS += /INCREMENTAL:NO

################################################################################

DEFINES += QT_DEPRECATED_WARNINGS

DEFINES += \
    SIM_CONSOLE_OUTPUT \
#    SIM_OSG_DEBUG_INFO \
    SIM_INTERSECTIONS \
    SIM_LOCAL_DATA_DIR \
    SIM_ROTOR_BLUR \
    SIM_SKYDOME_SCALING \
    SIM_USE_THREADS \
    SIM_VERTICALSYNC

greaterThan(QT_MAJOR_VERSION, 4):win32: DEFINES += USE_QT5

win32: DEFINES += \
    WIN32 \
    _WINDOWS \
    _CRT_SECURE_NO_DEPRECATE \
    _SCL_SECURE_NO_WARNINGS \
    _USE_MATH_DEFINES

win32: CONFIG(release, debug|release): DEFINES += NDEBUG
win32: CONFIG(debug, debug|release):   DEFINES += _DEBUG

#CONFIG(release, debug|release): DEFINES += QT_NO_DEBUG_OUTPUT
#DEFINES += QT_NO_DEBUG_OUTPUT

unix: DEFINES += _LINUX_

################################################################################

INCLUDEPATH += ./

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
    -losgQt \
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
    -losgQtd \
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
    -losgQt \
    -losgSim \
    -losgText \
    -losgUtil \
    -losgViewer \
    -losgWidget

################################################################################

HEADERS += \
    $$PWD/Autopilot.h \
    $$PWD/Common.h \
    $$PWD/Data.h \
    $$PWD/Defines.h \
    $$PWD/fdm_Manager.h \
    $$PWD/Manager.h \
    $$PWD/Simulation.h

SOURCES += \
    $$PWD/main.cpp \
    $$PWD/Autopilot.cpp \
    $$PWD/fdm_Manager.cpp \
    $$PWD/Manager.cpp \
    $$PWD/Simulation.cpp

RESOURCES += \
    $$PWD/sim.qrc

################################################################################

include($$PWD/cgi/cgi.pri)
include($$PWD/fdm/fdm.pri)
include($$PWD/fdm_c130/fdm_c130.pri)
include($$PWD/fdm_c172/fdm_c172.pri)
include($$PWD/fdm_f16/fdm_f16.pri)
include($$PWD/fdm_p51/fdm_p51.pri)
include($$PWD/fdm_uh60/fdm_uh60.pri)
include($$PWD/fdm_test/fdm_test.pri)
include($$PWD/gui/gui.pri)
include($$PWD/hid/hid.pri)
include($$PWD/nav/nav.pri)
