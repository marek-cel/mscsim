set( MODULE_NAME fdm_pw5 )

set( CPP_FILES
    pw5_Aerodynamics.cpp
    pw5_Aircraft.cpp
    pw5_Controls.cpp
    pw5_FDM.cpp
    pw5_LandingGear.cpp
    pw5_Mass.cpp
    pw5_Propulsion.cpp
    pw5_StabilizerHor.cpp
    pw5_StabilizerVer.cpp
    pw5_TailOff.cpp
)

add_library( ${MODULE_NAME} STATIC ${CPP_FILES} )
