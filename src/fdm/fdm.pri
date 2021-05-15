!fdm_test: {
HEADERS += $$PWD/fdm_Manager.h
SOURCES += $$PWD/fdm_Manager.cpp
}

HEADERS += \
    $$PWD/fdm_Aerodynamics.h \
    $$PWD/fdm_Aircraft.h \
    $$PWD/fdm_Base.h \
    $$PWD/fdm_Controls.h \
    $$PWD/fdm_DataInp.h \
    $$PWD/fdm_DataOut.h \
    $$PWD/fdm_Defines.h \
    $$PWD/fdm_Environment.h \
    $$PWD/fdm_Exception.h \
    $$PWD/fdm_FDM.h \
    $$PWD/fdm_Input.h \
    $$PWD/fdm_Intersections.h \
    $$PWD/fdm_LandingGear.h \
    $$PWD/fdm_Log.h \
    $$PWD/fdm_Mass.h \
    $$PWD/fdm_Module.h \
    $$PWD/fdm_Path.h \
    $$PWD/fdm_Propulsion.h \
    $$PWD/fdm_Recorder.h \
    $$PWD/fdm_Test.h \
    $$PWD/fdm_Types.h

SOURCES += \
    $$PWD/fdm_Aerodynamics.cpp \
    $$PWD/fdm_Aircraft.cpp \
    $$PWD/fdm_Base.cpp \
    $$PWD/fdm_Controls.cpp \
    $$PWD/fdm_Environment.cpp \
    $$PWD/fdm_FDM.cpp \
    $$PWD/fdm_Input.cpp \
    $$PWD/fdm_Intersections.cpp \
    $$PWD/fdm_LandingGear.cpp \
    $$PWD/fdm_Log.cpp \
    $$PWD/fdm_Mass.cpp \
    $$PWD/fdm_Propulsion.cpp \
    $$PWD/fdm_Recorder.cpp \
    $$PWD/fdm_Test.cpp

################################################################################

HEADERS += \
    $$PWD/auto/fdm_Autopilot.h \
    $$PWD/auto/fdm_FlightDirector.h

SOURCES += \
    $$PWD/auto/fdm_Autopilot.cpp \
    $$PWD/auto/fdm_FlightDirector.cpp

################################################################################

HEADERS += \
    $$PWD/ctrl/fdm_BPF.h \
    $$PWD/ctrl/fdm_BSF.h \
    $$PWD/ctrl/fdm_Filter2.h \
    $$PWD/ctrl/fdm_HPF.h \
    $$PWD/ctrl/fdm_Lag.h \
    $$PWD/ctrl/fdm_Lag2.h \
    $$PWD/ctrl/fdm_Lead.h \
    $$PWD/ctrl/fdm_LeadLag.h \
    $$PWD/ctrl/fdm_LPF.h \
    $$PWD/ctrl/fdm_PID.h

SOURCES += \
    $$PWD/ctrl/fdm_BPF.cpp \
    $$PWD/ctrl/fdm_BSF.cpp \
    $$PWD/ctrl/fdm_Filter2.cpp \
    $$PWD/ctrl/fdm_HPF.cpp \
    $$PWD/ctrl/fdm_Lag.cpp \
    $$PWD/ctrl/fdm_Lag2.cpp \
    $$PWD/ctrl/fdm_Lead.cpp \
    $$PWD/ctrl/fdm_LeadLag.cpp \
    $$PWD/ctrl/fdm_LPF.cpp \
    $$PWD/ctrl/fdm_PID.cpp

################################################################################

HEADERS += \
    $$PWD/models/fdm_Atmosphere.h \
    $$PWD/models/fdm_Fuselage.h \
    $$PWD/models/fdm_Governor.h \
    $$PWD/models/fdm_HingeMoment.h \
    $$PWD/models/fdm_MainRotor.h \
    $$PWD/models/fdm_MainRotorBE.h \
    $$PWD/models/fdm_PistonEngine.h \
    $$PWD/models/fdm_Propeller.h \
    $$PWD/models/fdm_RotorBlade.h \
    $$PWD/models/fdm_Schrenk.h \
    $$PWD/models/fdm_SimpleRotor.h \
    $$PWD/models/fdm_StabilizerHor.h \
    $$PWD/models/fdm_StabilizerVer.h \
    $$PWD/models/fdm_TailOff.h \
    $$PWD/models/fdm_TailRotor.h \
    $$PWD/models/fdm_Turbofan.h \
    $$PWD/models/fdm_Wheel.h \
    $$PWD/models/fdm_WinchLauncher.h \
    $$PWD/models/fdm_WindShear.h \
    $$PWD/models/fdm_WingRunner.h

SOURCES += \
    $$PWD/models/fdm_Atmosphere.cpp \
    $$PWD/models/fdm_Fuselage.cpp \
    $$PWD/models/fdm_Governor.cpp \
    $$PWD/models/fdm_HingeMoment.cpp \
    $$PWD/models/fdm_MainRotor.cpp \
    $$PWD/models/fdm_MainRotorBE.cpp \
    $$PWD/models/fdm_PistonEngine.cpp \
    $$PWD/models/fdm_Propeller.cpp \
    $$PWD/models/fdm_RotorBlade.cpp \
    $$PWD/models/fdm_Schrenk.cpp \
    $$PWD/models/fdm_SimpleRotor.cpp \
    $$PWD/models/fdm_StabilizerHor.cpp \
    $$PWD/models/fdm_StabilizerVer.cpp \
    $$PWD/models/fdm_TailOff.cpp \
    $$PWD/models/fdm_TailRotor.cpp \
    $$PWD/models/fdm_Turbofan.cpp \
    $$PWD/models/fdm_Wheel.cpp \
    $$PWD/models/fdm_WinchLauncher.cpp \
    $$PWD/models/fdm_WindShear.cpp \
    $$PWD/models/fdm_WingRunner.cpp

################################################################################

HEADERS += \
    $$PWD/utils/fdm_Angles.h \
    $$PWD/utils/fdm_DataNode.h \
    $$PWD/utils/fdm_DataRef.h \
    $$PWD/utils/fdm_EulerRect.h \
    $$PWD/utils/fdm_GaussJordan.h \
    $$PWD/utils/fdm_Geom.h \
    $$PWD/utils/fdm_Integrator.h \
    $$PWD/utils/fdm_Map.h \
    $$PWD/utils/fdm_Matrix.h \
    $$PWD/utils/fdm_Matrix3x3.h \
    $$PWD/utils/fdm_Matrix4x4.h \
    $$PWD/utils/fdm_Matrix6x6.h \
    $$PWD/utils/fdm_Misc.h \
    $$PWD/utils/fdm_Oscillation.h \
    $$PWD/utils/fdm_Quaternion.h \
    $$PWD/utils/fdm_Random.h \
    $$PWD/utils/fdm_RungeKutta4.h \
    $$PWD/utils/fdm_Singleton.h \
    $$PWD/utils/fdm_String.h \
    $$PWD/utils/fdm_Table1.h \
    $$PWD/utils/fdm_Table2.h \
    $$PWD/utils/fdm_Time.h \
    $$PWD/utils/fdm_Units.h \
    $$PWD/utils/fdm_Vector.h \
    $$PWD/utils/fdm_Vector3.h \
    $$PWD/utils/fdm_Vector4.h \
    $$PWD/utils/fdm_Vector6.h \
    $$PWD/utils/fdm_VectorN.h \
    $$PWD/utils/fdm_WGS84.h

SOURCES += \
    $$PWD/utils/fdm_Angles.cpp \
    $$PWD/utils/fdm_DataNode.cpp \
    $$PWD/utils/fdm_Geom.cpp \
    $$PWD/utils/fdm_Matrix3x3.cpp \
    $$PWD/utils/fdm_Matrix4x4.cpp \
    $$PWD/utils/fdm_Matrix6x6.cpp \
    $$PWD/utils/fdm_Oscillation.cpp \
    $$PWD/utils/fdm_Quaternion.cpp \
    $$PWD/utils/fdm_Random.cpp \
    $$PWD/utils/fdm_String.cpp \
    $$PWD/utils/fdm_Table1.cpp \
    $$PWD/utils/fdm_Table2.cpp \
    $$PWD/utils/fdm_Units.cpp \
    $$PWD/utils/fdm_Vector3.cpp \
    $$PWD/utils/fdm_Vector4.cpp \
    $$PWD/utils/fdm_Vector6.cpp \
    $$PWD/utils/fdm_VectorN.cpp \
    $$PWD/utils/fdm_WGS84.cpp

################################################################################

HEADERS += \
    $$PWD/xml/fdm_XmlDoc.h \
    $$PWD/xml/fdm_XmlNode.h \
    $$PWD/xml/fdm_XmlUtils.h

SOURCES += \
    $$PWD/xml/fdm_XmlDoc.cpp \
    $$PWD/xml/fdm_XmlNode.cpp \
    $$PWD/xml/fdm_XmlUtils.cpp
