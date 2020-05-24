set( MODULE_NAME nav )

set( CPP_FILES
    nav_DataBase.cpp
    nav_Frequency.cpp
    nav_Manager.cpp
)

if( USE_QT4 )
    qt4_wrap_cpp( MOC_SRCS ${MOC_FILES} )
    add_library( ${MODULE_NAME} STATIC ${CPP_FILES} )
else()
    add_library( ${MODULE_NAME} STATIC ${CPP_FILES} )
    qt5_use_modules( ${MODULE_NAME} Xml )
endif()
