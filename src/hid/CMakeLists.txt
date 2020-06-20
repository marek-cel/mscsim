set( MODULE_NAME hid )

set( CPP_FILES
    hid_AxisTune.cpp
    hid_Joysticks.cpp
    hid_Manager.cpp
)

add_library( ${MODULE_NAME} STATIC ${CPP_FILES} )
