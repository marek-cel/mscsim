set( MODULE_NAME fdm_f16 )

set( CPP_FILES
    f16_Aerodynamics.cpp
    f16_Aircraft.cpp
    f16_Controls.cpp
    f16_Engine.cpp
    f16_FDM.cpp
    f16_FLCS.cpp
    f16_LandingGear.cpp
    f16_Mass.cpp
    f16_Propulsion.cpp
)

add_library( ${MODULE_NAME} STATIC ${CPP_FILES} )
