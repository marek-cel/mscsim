set( MODULE_NAME fdm_f35a )

set( CPP_FILES
    f35a_Aerodynamics.cpp
    f35a_Aircraft.cpp
    f35a_Controls.cpp
    f35a_Engine.cpp
    f35a_FDM.cpp
    f35a_FLCS.cpp
    f35a_LandingGear.cpp
    f35a_Mass.cpp
    f35a_Propulsion.cpp
    f35a_StabilizerHor.cpp
    f35a_StabilizerVer.cpp
    f35a_TailOff.cpp
)

add_library( ${MODULE_NAME} STATIC ${CPP_FILES} )
