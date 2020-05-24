set( MODULE_NAME fdm_test )

set( CPP_FILES
    test_Aerodynamics.cpp
    test_AFCS.cpp
    test_Aircraft.cpp
    test_Controls.cpp
    test_Fuselage.cpp
    test_LandingGear.cpp
    test_MainRotorAD.cpp
    test_MainRotorBE.cpp
    test_Mass.cpp
    test_Propulsion.cpp
    test_StabilizerHor.cpp
    test_StabilizerVer.cpp
    test_TailRotor.cpp
)

add_library( ${MODULE_NAME} STATIC ${CPP_FILES} )
