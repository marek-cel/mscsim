set( MODULE_NAME fdm_xf )

set( CPP_FILES
    xf_Aerodynamics.cpp
    xf_Aircraft.cpp
    xf_Controls.cpp
    xf_Engine.cpp
    xf_FDM.cpp
    xf_FLCS.cpp
    xf_LandingGear.cpp
    xf_Mass.cpp
    xf_Propulsion.cpp
    xf_StabilizerHor.cpp
    xf_StabilizerVer.cpp
    xf_TailOff.cpp
)

add_library( ${MODULE_NAME} STATIC ${CPP_FILES} )
