set( MODULE_NAME fdm_aw101 )

set( CPP_FILES
    aw101_Aerodynamics.cpp
    aw101_AFCS.cpp
    aw101_Aircraft.cpp
    aw101_Controls.cpp
    aw101_FDM.cpp
    aw101_Fuselage.cpp
    aw101_LandingGear.cpp
    aw101_MainRotor.cpp
    aw101_Mass.cpp
    aw101_Propulsion.cpp
    aw101_StabilizerHor.cpp
    aw101_StabilizerVer.cpp
    aw101_TailRotor.cpp
)

add_library( ${MODULE_NAME} STATIC ${CPP_FILES} )
