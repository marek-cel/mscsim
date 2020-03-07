cmake_minimum_required( VERSION 2.8 )

project( mscsim )

################################################################################

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../bin )

################################################################################

# set( CMAKE_VERBOSE_MAKEFILE ON )

set( CMAKE_INCLUDE_CURRENT_DIR ON )

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

set( CMAKE_CXX_FLAGS "-Wall -std=c++0x -O3" )

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

find_package( Qt4 REQUIRED
    QtCore
    QtGui
    QtOpenGL
    QtSvg
    QtXml
)
include( ${QT_USE_FILE} )
add_definitions( ${QT_DEFINITIONS} )

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

find_package( LibXml2 REQUIRED )

find_package( OpenAL REQUIRED )
include_directories( ${OPENAL_INCLUDE_DIR} )

find_library( ALUT_LIBRARY alut )

################################################################################

add_subdirectory( cgi )
add_subdirectory( fdm )
add_subdirectory( fdm_c130 )
add_subdirectory( fdm_c172 )
add_subdirectory( fdm_f16 )
add_subdirectory( fdm_p51 )
add_subdirectory( fdm_test )
add_subdirectory( fdm_uh60 )
add_subdirectory( gui )
add_subdirectory( hid )
add_subdirectory( nav )
add_subdirectory( sfx )

################################################################################

qt4_wrap_cpp( MOC_SRCS
    Manager.h
    Simulation.h
)

qt4_add_resources( RES_SOURCES
    mscsim.qrc
)

################################################################################

add_executable( ${PROJECT_NAME}
    Autopilot.cpp
    main.cpp
    Manager.cpp
    ManagerFDM.cpp
    Simulation.cpp

    ${MOC_SRCS}
    ${RES_SOURCES}
)

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
    gui
    hid
    nav
    sfx

    ${LIBS}

    -Wl,--end-group
)
