cmake_minimum_required( VERSION 3.5 )

project( mscsim )

################################################################################

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../bin )

################################################################################

set( CMAKE_INCLUDE_CURRENT_DIR ON )

set(CMAKE_CXX_STANDARD 11 )
set(CMAKE_CXX_STANDARD_REQUIRED ON )

################################################################################

# add_definitions( -DSIM_CONSOLE_OUTPUT )
# add_definitions( -DSIM_OSG_DEBUG_INFO )
add_definitions( -DSIM_INTERSECTIONS )
# add_definitions( -DSIM_LOCAL_DATA_DIR )
add_definitions( -DSIM_SKYDOME_SCALING )
add_definitions( -DSIM_USE_THREADS )
add_definitions( -DSIM_VERTICALSYNC )

if( UNIX )
    add_definitions( -D_LINUX_ )
elseif( WIN32 )
    add_definitions( -DWIN32 )
    add_definitions( -D_WINDOWS )
    add_definitions( -D_CRT_SECURE_NO_DEPRECATE )
    add_definitions( -D_SCL_SECURE_NO_WARNINGS )
    add_definitions( -D_USE_MATH_DEFINES )
endif()

################################################################################

set( CMAKE_CXX_FLAGS "-Wall -O3" )

################################################################################

include_directories( . )

if( WIN32 )
    include_directories( ${OSG_ROOT}/include/ )
    include_directories( ${OSG_ROOT}/include/libxml2 )
endif()

if( UNIX )
    include_directories( /usr/include/libxml2 )
endif()

################################################################################

if( NOT USE_QT4 )
    if( WIN32 )
        set( USE_QT4 false )
    endif()
    
    if( UNIX )
        set( USE_QT4 true )
    endif()
endif()

if( USE_QT4 )
    find_package( Qt4 REQUIRED
        QtCore
        QtGui
        QtOpenGL
        QtSvg
        QtXml
        qtmain
    )
    
    include( ${QT_USE_FILE} )
    add_definitions( ${QT_DEFINITIONS} )
else()
    set( CMAKE_AUTOMOC ON )
    set( CMAKE_AUTORCC ON )
    set( CMAKE_AUTOUIC ON )

    find_package( Qt5 COMPONENTS Core Widgets OpenGL Svg Xml REQUIRED )
endif()

################################################################################

find_package( OpenSceneGraph REQUIRED
    OpenThreads
    osg
    osgDB
    osgGA
    osgParticle
    osgQt
    osgSim
    osgText
    osgUtil
    osgViewer
    osgWidget
)
include_directories( ${OPENSCENEGRAPH_INCLUDE_DIRS} )

################################################################################

find_package( LibXml2 REQUIRED )

################################################################################

find_package( OpenAL REQUIRED )
include_directories( ${OPENAL_INCLUDE_DIR} )

################################################################################

find_library( ALUT_LIBRARY alut )

################################################################################

find_library( X11_LIBRARY X11 )
find_library( XSS_LIBRARY Xss )

################################################################################

add_subdirectory( cgi )
add_subdirectory( fdm )
add_subdirectory( fdm_c130 )
add_subdirectory( fdm_c172 )
add_subdirectory( fdm_f16 )
add_subdirectory( fdm_p51 )
add_subdirectory( fdm_uh60 )
add_subdirectory( g1000 )
add_subdirectory( gui )
add_subdirectory( hid )
add_subdirectory( nav )
add_subdirectory( sfx )

################################################################################

set( CPP_FILES
    Autopilot.cpp
    main.cpp
    Manager.cpp
    ManagerFDM.cpp
    Simulation.cpp
)

set( MOC_SRC_FILES
    Manager.h
    Simulation.h
)

set( RCC_SRC_FILES
    mscsim.qrc
)

if( USE_QT4 )
    qt4_wrap_cpp( MOC_SRCS ${MOC_SRC_FILES} )
    qt4_add_resources( RES_SOURCES ${RCC_SRC_FILES} )
    set( SOURCES ${CPP_FILES} ${MOC_SRCS} ${RES_SOURCES} )
else()
    set( SOURCES ${CPP_FILES} ${MOC_SRC_FILES} ${RCC_SRC_FILES} )
endif()

if( WIN32 )
    ENABLE_LANGUAGE(RC)
    set( SOURCES mscsim.rc ${SOURCES} )
endif()

################################################################################

add_executable( ${PROJECT_NAME} WIN32 ${SOURCES} )

################################################################################

if( WIN32 )
    set( LIBS
        ${QT_LIBRARIES}
        ${OPENSCENEGRAPH_LIBRARIES}
        ${LIBXML2_LIBRARIES}
        ${OPENAL_LIBRARY}
        ${ALUT_LIBRARY}
        winmm
    )
endif()

if( UNIX )
    set( LIBS
        ${QT_LIBRARIES}
        ${OPENSCENEGRAPH_LIBRARIES}
        ${LIBXML2_LIBRARIES}
        ${OPENAL_LIBRARY}
        ${ALUT_LIBRARY}
        ${X11_LIBRARY}
        ${XSS_LIBRARY}
    )
endif()

################################################################################

target_link_libraries( ${PROJECT_NAME}

    -Wl,--start-group

    cgi
    fdm
    fdm_c130
    fdm_c172
    fdm_f16
    fdm_p51
    fdm_uh60
    g1000
    gui
    hid
    nav
    sfx

    ${LIBS}

    -Wl,--end-group
)
