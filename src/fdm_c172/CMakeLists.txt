set( MODULE_NAME fdm_c172 )

set( CPP_FILES
    c172_Aerodynamics.cpp
    c172_Aircraft.cpp
    c172_Controls.cpp
    c172_Engine.cpp
    c172_FDM.cpp
    c172_GFC700_AP.cpp
    c172_GFC700_FD.cpp
    c172_KAP140_AP.cpp
    c172_KAP140_FD.cpp
    c172_KFC325_AP.cpp
    c172_KFC325_FD.cpp
    c172_LandingGear.cpp
    c172_Mass.cpp
    c172_Propeller.cpp
    c172_Propulsion.cpp
    c172_StabilizerHor.cpp
    c172_StabilizerVer.cpp
    c172_TailOff.cpp
)

add_library( ${MODULE_NAME} STATIC ${CPP_FILES} )
