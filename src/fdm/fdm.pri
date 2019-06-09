HEADERS += \
    $$PWD/fdm_DataInp.h \
    $$PWD/fdm_DataOut.h \
    $$PWD/fdm_Defines.h \
    $$PWD/fdm_Manager.h \
    $$PWD/fdm_Types.h

SOURCES += \
    $$PWD/fdm_Manager.cpp

################################################################################

HEADERS += \
    $$PWD/auto/fdm_FlightDirector.h

SOURCES += \
    $$PWD/auto/fdm_FlightDirector.cpp

################################################################################

HEADERS += \
    $$PWD/main/fdm_Aerodynamics.h \
    $$PWD/main/fdm_Aircraft.h \
    $$PWD/main/fdm_Atmosphere.h \
    $$PWD/main/fdm_Base.h \
    $$PWD/main/fdm_Controls.h \
    $$PWD/main/fdm_Engine.h \
    $$PWD/main/fdm_Environment.h \
    $$PWD/main/fdm_Exception.h \
    $$PWD/main/fdm_Fuselage.h \
    $$PWD/main/fdm_HingeMoment.h \
    $$PWD/main/fdm_Intersections.h \
    $$PWD/main/fdm_LandingGear.h \
    $$PWD/main/fdm_MainRotor.h \
    $$PWD/main/fdm_Mass.h \
    $$PWD/main/fdm_PistonEngine.h \
    $$PWD/main/fdm_Propeller.h \
    $$PWD/main/fdm_Propulsion.h \
    $$PWD/main/fdm_Schrenk.h \
    $$PWD/main/fdm_Stabilizer.h \
    $$PWD/main/fdm_TailOff.h \
    $$PWD/main/fdm_TailRotor.h \
    $$PWD/main/fdm_Turbojet.h \
    $$PWD/main/fdm_WindShear.h

SOURCES += \
    $$PWD/main/fdm_Aerodynamics.cpp \
    $$PWD/main/fdm_Aircraft.cpp \
    $$PWD/main/fdm_Atmosphere.cpp \
    $$PWD/main/fdm_Base.cpp \
    $$PWD/main/fdm_Controls.cpp \
    $$PWD/main/fdm_Environment.cpp \
    $$PWD/main/fdm_Fuselage.cpp \
    $$PWD/main/fdm_HingeMoment.cpp \
    $$PWD/main/fdm_Intersections.cpp \
    $$PWD/main/fdm_LandingGear.cpp \
    $$PWD/main/fdm_MainRotor.cpp \
    $$PWD/main/fdm_Mass.cpp \
    $$PWD/main/fdm_PistonEngine.cpp \
    $$PWD/main/fdm_Propeller.cpp \
    $$PWD/main/fdm_Propulsion.cpp \
    $$PWD/main/fdm_Schrenk.cpp \
    $$PWD/main/fdm_Stabilizer.cpp \
    $$PWD/main/fdm_TailOff.cpp \
    $$PWD/main/fdm_TailRotor.cpp \
    $$PWD/main/fdm_Turbojet.cpp \
    $$PWD/main/fdm_WindShear.cpp

################################################################################

HEADERS += \
    $$PWD/sys/fdm_BPF.h \
    $$PWD/sys/fdm_BSF.h \
    $$PWD/sys/fdm_Filter2.h \
    $$PWD/sys/fdm_HPF.h \
    $$PWD/sys/fdm_Lag.h \
    $$PWD/sys/fdm_Lag2.h \
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
    $$PWD/sys/fdm_LeadLag.cpp \
    $$PWD/sys/fdm_LPF.cpp \
    $$PWD/sys/fdm_PID.cpp

################################################################################

HEADERS += \
    $$PWD/utils/fdm_Angles.h \
    $$PWD/utils/fdm_EulerRect.h \
    $$PWD/utils/fdm_GaussJordan.h \
    $$PWD/utils/fdm_Integrator.h \
    $$PWD/utils/fdm_Matrix.h \
    $$PWD/utils/fdm_Matrix3x3.h \
    $$PWD/utils/fdm_Matrix4x4.h \
    $$PWD/utils/fdm_Matrix6x6.h \
    $$PWD/utils/fdm_Misc.h \
    $$PWD/utils/fdm_Quaternion.h \
    $$PWD/utils/fdm_Random.h \
    $$PWD/utils/fdm_RungeKutta4.h \
    $$PWD/utils/fdm_String.h \
    $$PWD/utils/fdm_Table.h \
    $$PWD/utils/fdm_Table2D.h \
    $$PWD/utils/fdm_Units.h \
    $$PWD/utils/fdm_Vector.h \
    $$PWD/utils/fdm_Vector3.h \
    $$PWD/utils/fdm_Vector4.h \
    $$PWD/utils/fdm_Vector6.h \
    $$PWD/utils/fdm_WGS84.h

SOURCES += \
    $$PWD/utils/fdm_Angles.cpp \
    $$PWD/utils/fdm_Matrix3x3.cpp \
    $$PWD/utils/fdm_Matrix4x4.cpp \
    $$PWD/utils/fdm_Matrix6x6.cpp \
    $$PWD/utils/fdm_Quaternion.cpp \
    $$PWD/utils/fdm_Random.cpp \
    $$PWD/utils/fdm_String.cpp \
    $$PWD/utils/fdm_Table.cpp \
    $$PWD/utils/fdm_Table2D.cpp \
    $$PWD/utils/fdm_Units.cpp \
    $$PWD/utils/fdm_Vector3.cpp \
    $$PWD/utils/fdm_Vector4.cpp \
    $$PWD/utils/fdm_Vector6.cpp \
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
