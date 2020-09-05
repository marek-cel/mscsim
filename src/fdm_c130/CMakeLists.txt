set( MODULE_NAME fdm_c130 )

set( CPP_FILES
    c130_Aerodynamics.cpp
    c130_Aircraft.cpp
    c130_Controls.cpp
    c130_Engine.cpp
    c130_FDM.cpp
    c130_Governor.cpp
    c130_LandingGear.cpp
    c130_Mass.cpp
    c130_Propeller.cpp
    c130_Propulsion.cpp
    c130_StabilizerHor.cpp
    c130_StabilizerVer.cpp
    c130_TailOff.cpp
)

add_library( ${MODULE_NAME} STATIC ${CPP_FILES} )
