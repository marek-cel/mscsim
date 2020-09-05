set( MODULE_NAME fdm_uh60 )

set( CPP_FILES
    uh60_Aerodynamics.cpp
    uh60_AFCS.cpp
    uh60_Aircraft.cpp
    uh60_Controls.cpp
    uh60_FDM.cpp
    uh60_Fuselage.cpp
    uh60_LandingGear.cpp
    uh60_MainRotor.cpp
    uh60_Mass.cpp
    uh60_Propulsion.cpp
    uh60_StabilizerHor.cpp
    uh60_StabilizerVer.cpp
    uh60_TailRotor.cpp
)

add_library( ${MODULE_NAME} STATIC ${CPP_FILES} )
