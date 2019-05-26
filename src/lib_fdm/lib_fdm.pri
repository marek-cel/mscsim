HEADERS += \
    $$PWD/fdmAuto/fdm_FlightDirector.h

SOURCES += \
    $$PWD/fdmAuto/fdm_FlightDirector.cpp

################################################################################

HEADERS += \
    $$PWD/fdmMain/fdm_Aerodynamics.h \
    $$PWD/fdmMain/fdm_Aircraft.h \
    $$PWD/fdmMain/fdm_Atmosphere.h \
    $$PWD/fdmMain/fdm_Base.h \
    $$PWD/fdmMain/fdm_Controls.h \
    $$PWD/fdmMain/fdm_DataManager.h \
    $$PWD/fdmMain/fdm_DataNode.h \
    $$PWD/fdmMain/fdm_DataRef.h \
    $$PWD/fdmMain/fdm_Defines.h \
    $$PWD/fdmMain/fdm_Engine.h \
    $$PWD/fdmMain/fdm_Environment.h \
    $$PWD/fdmMain/fdm_Exception.h \
    $$PWD/fdmMain/fdm_Fuselage.h \
    $$PWD/fdmMain/fdm_HingeMoment.h \
    $$PWD/fdmMain/fdm_Intersections.h \
    $$PWD/fdmMain/fdm_LandingGear.h \
    $$PWD/fdmMain/fdm_MainRotor.h \
    $$PWD/fdmMain/fdm_Mass.h \
    $$PWD/fdmMain/fdm_PistonEngine.h \
    $$PWD/fdmMain/fdm_Propeller.h \
    $$PWD/fdmMain/fdm_Propulsion.h \
    $$PWD/fdmMain/fdm_Schrenk.h \
    $$PWD/fdmMain/fdm_Stabilizer.h \
    $$PWD/fdmMain/fdm_TailOff.h \
    $$PWD/fdmMain/fdm_TailRotor.h \
    $$PWD/fdmMain/fdm_Turbojet.h \
    $$PWD/fdmMain/fdm_Types.h \
    $$PWD/fdmMain/fdm_WindShear.h

SOURCES += \
    $$PWD/fdmMain/fdm_Aerodynamics.cpp \
    $$PWD/fdmMain/fdm_Aircraft.cpp \
    $$PWD/fdmMain/fdm_Atmosphere.cpp \
    $$PWD/fdmMain/fdm_Base.cpp \
    $$PWD/fdmMain/fdm_Controls.cpp \
    $$PWD/fdmMain/fdm_DataManager.cpp \
    $$PWD/fdmMain/fdm_DataNode.cpp \
    $$PWD/fdmMain/fdm_Environment.cpp \
    $$PWD/fdmMain/fdm_Fuselage.cpp \
    $$PWD/fdmMain/fdm_HingeMoment.cpp \
    $$PWD/fdmMain/fdm_Intersections.cpp \
    $$PWD/fdmMain/fdm_LandingGear.cpp \
    $$PWD/fdmMain/fdm_MainRotor.cpp \
    $$PWD/fdmMain/fdm_Mass.cpp \
    $$PWD/fdmMain/fdm_PistonEngine.cpp \
    $$PWD/fdmMain/fdm_Propeller.cpp \
    $$PWD/fdmMain/fdm_Propulsion.cpp \
    $$PWD/fdmMain/fdm_Schrenk.cpp \
    $$PWD/fdmMain/fdm_Stabilizer.cpp \
    $$PWD/fdmMain/fdm_TailOff.cpp \
    $$PWD/fdmMain/fdm_TailRotor.cpp \
    $$PWD/fdmMain/fdm_Turbojet.cpp \
    $$PWD/fdmMain/fdm_WindShear.cpp

################################################################################

HEADERS += \
    $$PWD/fdmSys/fdm_BPF.h \
    $$PWD/fdmSys/fdm_BSF.h \
    $$PWD/fdmSys/fdm_Filter2.h \
    $$PWD/fdmSys/fdm_HPF.h \
    $$PWD/fdmSys/fdm_Lag.h \
    $$PWD/fdmSys/fdm_Lag2.h \
    $$PWD/fdmSys/fdm_LeadLag.h \
    $$PWD/fdmSys/fdm_LPF.h \
    $$PWD/fdmSys/fdm_PID.h

SOURCES += \
    $$PWD/fdmSys/fdm_BPF.cpp \
    $$PWD/fdmSys/fdm_BSF.cpp \
    $$PWD/fdmSys/fdm_Filter2.cpp \
    $$PWD/fdmSys/fdm_HPF.cpp \
    $$PWD/fdmSys/fdm_Lag.cpp \
    $$PWD/fdmSys/fdm_Lag2.cpp \
    $$PWD/fdmSys/fdm_LeadLag.cpp \
    $$PWD/fdmSys/fdm_LPF.cpp \
    $$PWD/fdmSys/fdm_PID.cpp

################################################################################

HEADERS += \
    $$PWD/fdmUtils/fdm_Angles.h \
    $$PWD/fdmUtils/fdm_EulerRect.h \
    $$PWD/fdmUtils/fdm_GaussJordan.h \
    $$PWD/fdmUtils/fdm_Integrator.h \
    $$PWD/fdmUtils/fdm_Matrix.h \
    $$PWD/fdmUtils/fdm_Matrix3x3.h \
    $$PWD/fdmUtils/fdm_Matrix4x4.h \
    $$PWD/fdmUtils/fdm_Matrix6x6.h \
    $$PWD/fdmUtils/fdm_Misc.h \
    $$PWD/fdmUtils/fdm_Quaternion.h \
    $$PWD/fdmUtils/fdm_Random.h \
    $$PWD/fdmUtils/fdm_RungeKutta4.h \
    $$PWD/fdmUtils/fdm_String.h \
    $$PWD/fdmUtils/fdm_Table.h \
    $$PWD/fdmUtils/fdm_Table2D.h \
    $$PWD/fdmUtils/fdm_Units.h \
    $$PWD/fdmUtils/fdm_Vector.h \
    $$PWD/fdmUtils/fdm_Vector3.h \
    $$PWD/fdmUtils/fdm_Vector4.h \
    $$PWD/fdmUtils/fdm_Vector6.h \
    $$PWD/fdmUtils/fdm_WGS84.h

SOURCES += \
    $$PWD/fdmUtils/fdm_Angles.cpp \
    $$PWD/fdmUtils/fdm_Matrix3x3.cpp \
    $$PWD/fdmUtils/fdm_Matrix4x4.cpp \
    $$PWD/fdmUtils/fdm_Matrix6x6.cpp \
    $$PWD/fdmUtils/fdm_Quaternion.cpp \
    $$PWD/fdmUtils/fdm_Random.cpp \
    $$PWD/fdmUtils/fdm_String.cpp \
    $$PWD/fdmUtils/fdm_Table.cpp \
    $$PWD/fdmUtils/fdm_Table2D.cpp \
    $$PWD/fdmUtils/fdm_Units.cpp \
    $$PWD/fdmUtils/fdm_Vector3.cpp \
    $$PWD/fdmUtils/fdm_Vector4.cpp \
    $$PWD/fdmUtils/fdm_Vector6.cpp \
    $$PWD/fdmUtils/fdm_WGS84.cpp

################################################################################

HEADERS += \
    $$PWD/fdmXml/fdm_XmlDoc.h \
    $$PWD/fdmXml/fdm_XmlNode.h \
    $$PWD/fdmXml/fdm_XmlUtils.h

SOURCES += \
    $$PWD/fdmXml/fdm_XmlDoc.cpp \
    $$PWD/fdmXml/fdm_XmlNode.cpp \
    $$PWD/fdmXml/fdm_XmlUtils.cpp
