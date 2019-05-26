TEMPLATE = app

CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt


################################################################################

DESTDIR = ../bin
TARGET = aero_coefficients

################################################################################

DEFINES += \

win32: QMAKE_LFLAGS += \
    /INCREMENTAL:NO

win32: DEFINES += \
    WIN32 \
    _WINDOWS \
    _SCL_SECURE_NO_WARNINGS \
    _CRT_SECURE_NO_DEPRECATE \
    _USE_MATH_DEFINES \
    USE_QT5

unix: DEFINES += \
    _LINUX_

win32:CONFIG(release, debug|release): DEFINES += \
    NDEBUG

win32:CONFIG(debug, debug|release): DEFINES += \
    _DEBUG

################################################################################

INCLUDEPATH += ./

################################################################################

HEADERS += \
    CoefDrag.h \
    CoefLift.h \
    defs.h \
    global.h \
    utils.h \
    write.h

SOURCES += \
    CoefDrag.cpp \
    CoefLift.cpp \
    main.cpp \
    utils.cpp \
    write.cpp

RESOURCES += \
