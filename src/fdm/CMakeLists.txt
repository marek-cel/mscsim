set( MODULE_NAME fdm )

set( CPP_FILES
    fdm_Base.cpp
    fdm_Log.cpp
    fdm_Manager.cpp
    fdm_Test.cpp

    auto/fdm_Autopilot.cpp
    auto/fdm_FlightDirector.cpp
    
    ctrl/fdm_BPF.cpp
    ctrl/fdm_BSF.cpp
    ctrl/fdm_Filter2.cpp
    ctrl/fdm_HPF.cpp
    ctrl/fdm_Lag.cpp
    ctrl/fdm_Lag2.cpp
    ctrl/fdm_Lead.cpp
    ctrl/fdm_LeadLag.cpp
    ctrl/fdm_LPF.cpp
    ctrl/fdm_PID.cpp
    
    main/fdm_Aerodynamics.cpp
    main/fdm_Aircraft.cpp
    main/fdm_Controls.cpp
    main/fdm_Environment.cpp
    main/fdm_FDM.cpp
    main/fdm_Input.cpp
    main/fdm_Intersections.cpp
    main/fdm_LandingGear.cpp
    main/fdm_Mass.cpp
    main/fdm_Propulsion.cpp
    main/fdm_Recorder.cpp
    
    models/fdm_Atmosphere.cpp
    models/fdm_Fuselage.cpp
    models/fdm_Governor.cpp
    models/fdm_HingeMoment.cpp
    models/fdm_MainRotor.cpp
    models/fdm_MainRotorBE.cpp
    models/fdm_PistonEngine.cpp
    models/fdm_Propeller.cpp
    models/fdm_RotorBlade.cpp
    models/fdm_Schrenk.cpp
    models/fdm_SimpleRotor.cpp
    models/fdm_StabilizerHor.cpp
    models/fdm_StabilizerVer.cpp
    models/fdm_TailOff.cpp
    models/fdm_TailRotor.cpp
    models/fdm_Turbofan.cpp
    models/fdm_Wheel.cpp
    models/fdm_WinchLauncher.cpp
    models/fdm_WindShear.cpp
    models/fdm_WingRunner.cpp
    
    utils/fdm_Angles.cpp
    utils/fdm_DataNode.cpp
    utils/fdm_Geom.cpp
    utils/fdm_Matrix3x3.cpp
    utils/fdm_Matrix4x4.cpp
    utils/fdm_Matrix6x6.cpp
    utils/fdm_Oscillation.cpp
    utils/fdm_Quaternion.cpp
    utils/fdm_Random.cpp
    utils/fdm_String.cpp
    utils/fdm_Table1.cpp
    utils/fdm_Table2.cpp
    utils/fdm_Units.cpp
    utils/fdm_Vector3.cpp
    utils/fdm_Vector4.cpp
    utils/fdm_Vector6.cpp
    utils/fdm_VectorN.cpp
    utils/fdm_WGS84.cpp
    
    xml/fdm_XmlDoc.cpp
    xml/fdm_XmlNode.cpp
    xml/fdm_XmlUtils.cpp
)

add_library( ${MODULE_NAME} STATIC ${CPP_FILES} )
