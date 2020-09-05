set( MODULE_NAME fdm_xh )

set( CPP_FILES
    xh_Aerodynamics.cpp
    xh_AFCS.cpp
    xh_Aircraft.cpp
    xh_Controls.cpp
    xh_FDM.cpp
    xh_Fuselage.cpp
    xh_LandingGear.cpp
    xh_MainRotorAD.cpp
    xh_MainRotorBE.cpp
    xh_Mass.cpp
    xh_Propulsion.cpp
    xh_StabilizerHor.cpp
    xh_StabilizerVer.cpp
    xh_TailRotor.cpp
)

add_library( ${MODULE_NAME} STATIC ${CPP_FILES} )
