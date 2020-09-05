set( MODULE_NAME fdm_p51 )

set( CPP_FILES
    p51_Aerodynamics.cpp
    p51_Aircraft.cpp
    p51_Controls.cpp
    p51_Engine.cpp
    p51_FDM.cpp
    p51_Governor.cpp
    p51_LandingGear.cpp
    p51_Mass.cpp
    p51_Propeller.cpp
    p51_Propulsion.cpp
    p51_StabilizerHor.cpp
    p51_StabilizerVer.cpp
    p51_TailOff.cpp
)

add_library( ${MODULE_NAME} STATIC ${CPP_FILES} )
