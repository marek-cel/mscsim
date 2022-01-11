QT += core gui opengl svg xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

unix: greaterThan(QT_MAJOR_VERSION, 4): QT += x11extras

TEMPLATE = app

################################################################################

DESTDIR = $$PWD/../bin
TARGET = mscsim

################################################################################

CONFIG += c++17

################################################################################

win32: RC_FILE = mscsim.rc

################################################################################

win32: CONFIG(release, debug|release): QMAKE_CXXFLAGS += -O2
unix:  CONFIG(release, debug|release): QMAKE_CXXFLAGS += -O2

#win32: QMAKE_CXXFLAGS += /Zc:wchar_t

win32: QMAKE_LFLAGS += /INCREMENTAL:NO

################################################################################

DEFINES += QT_DEPRECATED_WARNINGS

DEFINES += \
#    SIM_OSG_DEBUG_INFO \
    SIM_INTERSECTIONS \
    SIM_SKYDOME_SCALING \
    SIM_USE_THREADS \
    SIM_VERTICALSYNC

DEFINES += FDM_TEST
DEFINES += SIM_TEST
#DEFINES += SIM_TEST_WORLD

greaterThan(QT_MAJOR_VERSION, 4):win32: DEFINES += USE_QT5

win32: DEFINES += \
    NOMINMAX \
    WIN32 \
    _WINDOWS \
    _CRT_SECURE_NO_DEPRECATE \
    _SCL_SECURE_NO_WARNINGS \
    _USE_MATH_DEFINES

win32: CONFIG(release, debug|release): DEFINES += NDEBUG
win32: CONFIG(debug, debug|release):   DEFINES += _DEBUG

unix: CONFIG(debug, debug|release):   DEFINES += _DEBUG

#CONFIG(release, debug|release): DEFINES += QT_NO_DEBUG_OUTPUT
#DEFINES += QT_NO_DEBUG_OUTPUT

unix: DEFINES += _LINUX_

################################################################################

INCLUDEPATH += ./

win32: INCLUDEPATH += \
    $(ALUT_DIR)/include \
    $(OPENAL_DIR)/include \
    $(OSG_ROOT)/include/ \
    $(OSG_ROOT)/include/libxml2

unix: INCLUDEPATH += \
    /usr/include/libxml2

################################################################################

win32: LIBS += \
    -L$(ALUT_DIR)/lib \
    -L$(OPENAL_DIR)/libs/Win64 \
    -L$(OSG_ROOT)/lib \
    -lalut \
    -lgdal_i \
    -llibxml2 \
    -lopenal32 \
    -lopengl32 \
    -lwinmm \
    -lws2_32

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
    -lalut \
    -lgdal \
    -lopenal \
    -lX11 \
    -lXss \
    -lxml2

unix: LIBS += \
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
    $$PWD/defs.h \
    $$PWD/Autopilot.h \
    $$PWD/Data.h \
    $$PWD/Manager.h \
    $$PWD/Simulation.h

SOURCES += \
    $$PWD/main.cpp \
    $$PWD/Autopilot.cpp \
    $$PWD/Manager.cpp \
    $$PWD/Simulation.cpp

RESOURCES += \
    $$PWD/mscsim.qrc

################################################################################

include($$PWD/cgi/cgi.pri)
include($$PWD/fdm/fdm.pri)
include($$PWD/fdm_aw101/fdm_aw101.pri)
include($$PWD/fdm_c130/fdm_c130.pri)
include($$PWD/fdm_c172/fdm_c172.pri)
include($$PWD/fdm_f16/fdm_f16.pri)
include($$PWD/fdm_f35a/fdm_f35a.pri)
include($$PWD/fdm_p51/fdm_p51.pri)
include($$PWD/fdm_pw5/fdm_pw5.pri)
include($$PWD/fdm_r44/fdm_r44.pri)
include($$PWD/fdm_uh60/fdm_uh60.pri)
include($$PWD/g1000/g1000.pri)
include($$PWD/gui/gui.pri)
include($$PWD/hid/hid.pri)
include($$PWD/nav/nav.pri)
include($$PWD/sfx/sfx.pri)
include($$PWD/sim/sim.pri)

include($$PWD/fdm_xh/fdm_xh.pri)
include($$PWD/fdm_xf/fdm_xf.pri)
