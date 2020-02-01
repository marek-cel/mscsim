QT += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

CONFIG += c++11

################################################################################

DESTDIR = ../bin
TARGET = mscsim-test-efis

################################################################################

win32: CONFIG(release, debug|release): QMAKE_CXXFLAGS += -O2
unix:  CONFIG(release, debug|release): QMAKE_CXXFLAGS += -O3

#win32: QMAKE_CXXFLAGS += /Zc:wchar_t

win32: QMAKE_LFLAGS += /INCREMENTAL:NO

################################################################################

greaterThan(QT_MAJOR_VERSION, 4):win32: DEFINES += USE_QT5

win32: DEFINES += \
    WIN32 \
    _WINDOWS \
    _CRT_SECURE_NO_DEPRECATE \
    _SCL_SECURE_NO_WARNINGS \
    _USE_MATH_DEFINES

win32: CONFIG(release, debug|release): DEFINES += NDEBUG
win32: CONFIG(debug, debug|release):   DEFINES += _DEBUG

unix: DEFINES += _LINUX_

################################################################################

INCLUDEPATH += ./

################################################################################

win32: LIBS +=

################################################################################

HEADERS += \
    Common.h \
    Defines.h

################################################################################

HEADERS += \
    gui/Colors.h \
    gui/DockWidgetEFIS.h \
    gui/Fonts.h \
    gui/GraphicsEADI.h \
    gui/GraphicsEHSI.h \
    gui/LayoutSquare.h \
    gui/WidgetSquare.h

SOURCES += \
    gui/Colors.cpp \
    gui/DockWidgetEFIS.cpp \
    gui/Fonts.cpp \
    gui/GraphicsEADI.cpp \
    gui/GraphicsEHSI.cpp \
    gui/LayoutSquare.cpp \
    gui/WidgetSquare.cpp

FORMS += \
    gui/DockWidgetEFIS.ui

RESOURCES += \
    sim.qrc

################################################################################

include(test_efis/test_efis.pri)
