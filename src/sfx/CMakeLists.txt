set( MODULE_NAME sfx )

set( CPP_FILES
    sfx_Manager.cpp
    sfx_Sample.cpp
    sfx_Thread.cpp
)

set( MOC_SRC_FILES
    sfx_Thread.h
)

if( USE_QT4 )
    qt4_wrap_cpp( MOC_SRCS ${MOC_SRC_FILES} )
    add_library( ${MODULE_NAME} STATIC ${CPP_FILES} ${MOC_SRCS} )
else()
    add_library( ${MODULE_NAME} STATIC ${CPP_FILES} ${MOC_SRC_FILES} )
    qt5_use_modules( ${MODULE_NAME} Core )
endif()
