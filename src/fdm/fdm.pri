!fdm_test: {
HEADERS += $$PWD/fdm_Manager.h
SOURCES += $$PWD/fdm_Manager.cpp
}

HEADERS += \
    $$PWD/fdm_Base.h \
    $$PWD/fdm_DataInp.h \
    $$PWD/fdm_DataOut.h \
    $$PWD/fdm_Defines.h \
    $$PWD/fdm_Exception.h \
    $$PWD/fdm_FDM.h \
    $$PWD/fdm_Log.h \
    $$PWD/fdm_Path.h \
    $$PWD/fdm_Recorder.h \
    $$PWD/fdm_Test.h \
    $$PWD/fdm_Types.h

SOURCES += \
    $$PWD/fdm_Base.cpp \
    $$PWD/fdm_FDM.cpp \
    $$PWD/fdm_Log.cpp \
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
    $$PWD/main/fdm_Aerodynamics.h \
    $$PWD/main/fdm_Aircraft.h \
    $$PWD/main/fdm_Controls.h \
    $$PWD/main/fdm_Environment.h \
    $$PWD/main/fdm_Intersections.h \
    $$PWD/main/fdm_LandingGear.h \
    $$PWD/main/fdm_Mass.h \
    $$PWD/main/fdm_Module.h \
    $$PWD/main/fdm_Propulsion.h

SOURCES += \
    $$PWD/main/fdm_Aerodynamics.cpp \
    $$PWD/main/fdm_Aircraft.cpp \
    $$PWD/main/fdm_Controls.cpp \
    $$PWD/main/fdm_Environment.cpp \
    $$PWD/main/fdm_Intersections.cpp \
    $$PWD/main/fdm_LandingGear.cpp \
    $$PWD/main/fdm_Mass.cpp \
    $$PWD/main/fdm_Propulsion.cpp

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
    $$PWD/sys/fdm_BPF.h \
    $$PWD/sys/fdm_BSF.h \
    $$PWD/sys/fdm_Filter2.h \
    $$PWD/sys/fdm_HPF.h \
    $$PWD/sys/fdm_Lag.h \
    $$PWD/sys/fdm_Lag2.h \
    $$PWD/sys/fdm_Lead.h \
    $$PWD/sys/fdm_LeadLag.h \
    $$PWD/sys/fdm_LPF.h \
    $$PWD/sys/fdm_PID.h

SOURCES += \
    $$PWD/sys/fdm_BPF.cpp \
    $$PWD/sys/fdm_BSF.cpp \
    $$PWD/sys/fdm_Filter2.cpp \
    $$PWD/sys/fdm_HPF.cpp \
    $$PWD/sys/fdm_Lag.cpp \
    $$PWD/sys/fdm_Lag2.cpp \
    $$PWD/sys/fdm_Lead.cpp \
    $$PWD/sys/fdm_LeadLag.cpp \
    $$PWD/sys/fdm_LPF.cpp \
    $$PWD/sys/fdm_PID.cpp

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
    $$PWD/utils/fdm_Period.h \
    $$PWD/utils/fdm_Quaternion.h \
    $$PWD/utils/fdm_Random.h \
    $$PWD/utils/fdm_RungeKutta4.h \
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
    $$PWD/utils/fdm_Period.cpp \
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
