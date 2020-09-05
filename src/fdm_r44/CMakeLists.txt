set( MODULE_NAME fdm_r44 )

set( CPP_FILES
    r44_Aerodynamics.cpp
    r44_Aircraft.cpp
    r44_Controls.cpp
    r44_FDM.cpp
    r44_Fuselage.cpp
    r44_LandingGear.cpp
    r44_MainRotor.cpp
    r44_Mass.cpp
    r44_Propulsion.cpp
    r44_StabilizerHor.cpp
    r44_StabilizerVer.cpp
    r44_TailRotor.cpp
)

add_library( ${MODULE_NAME} STATIC ${CPP_FILES} )
