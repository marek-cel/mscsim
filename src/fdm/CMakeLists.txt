add_library( fdm STATIC
    fdm_Manager.cpp
    
    auto/fdm_FlightDirector.cpp
    
    main/fdm_Aerodynamics.cpp
    main/fdm_Aircraft.cpp
    main/fdm_Atmosphere.cpp
    main/fdm_Base.cpp
    main/fdm_Controls.cpp
    main/fdm_Environment.cpp
    main/fdm_Fuselage.cpp
    main/fdm_HingeMoment.cpp
    main/fdm_Intersections.cpp
    main/fdm_LandingGear.cpp
    main/fdm_MainRotor.cpp
    main/fdm_Mass.cpp
    main/fdm_PistonEngine.cpp
    main/fdm_Propeller.cpp
    main/fdm_Propulsion.cpp
    main/fdm_Schrenk.cpp
    main/fdm_Stabilizer.cpp
    main/fdm_TailOff.cpp
    main/fdm_TailRotor.cpp
    main/fdm_Turbojet.cpp
    main/fdm_WindShear.cpp
    
    sys/fdm_BPF.cpp
    sys/fdm_BSF.cpp
    sys/fdm_Filter2.cpp
    sys/fdm_HPF.cpp
    sys/fdm_Lag.cpp
    sys/fdm_Lag2.cpp
    sys/fdm_LeadLag.cpp
    sys/fdm_LPF.cpp
    sys/fdm_PID.cpp
    
    utils/fdm_Angles.cpp
    utils/fdm_Matrix3x3.cpp
    utils/fdm_Matrix4x4.cpp
    utils/fdm_Matrix6x6.cpp
    utils/fdm_Quaternion.cpp
    utils/fdm_Random.cpp
    utils/fdm_String.cpp
    utils/fdm_Table.cpp
    utils/fdm_Table2D.cpp
    utils/fdm_Units.cpp
    utils/fdm_Vector3.cpp
    utils/fdm_Vector4.cpp
    utils/fdm_Vector6.cpp
    utils/fdm_WGS84.cpp
    
    xml/fdm_XmlDoc.cpp
    xml/fdm_XmlNode.cpp
    xml/fdm_XmlUtils.cpp
)
