HEADERS += \
    lib_fdm/fdmAuto/fdm_FlightDirector.h

SOURCES += \
    lib_fdm/fdmAuto/fdm_FlightDirector.cpp

################################################################################

HEADERS += \
    lib_fdm/fdmMain/fdm_Aerodynamics.h \
    lib_fdm/fdmMain/fdm_Aircraft.h \
    lib_fdm/fdmMain/fdm_Atmosphere.h \
    lib_fdm/fdmMain/fdm_Base.h \
    lib_fdm/fdmMain/fdm_Controls.h \
    lib_fdm/fdmMain/fdm_DataManager.h \
    lib_fdm/fdmMain/fdm_DataNode.h \
    lib_fdm/fdmMain/fdm_DataRef.h \
    lib_fdm/fdmMain/fdm_Defines.h \
    lib_fdm/fdmMain/fdm_Environment.h \
    lib_fdm/fdmMain/fdm_Exception.h \
    lib_fdm/fdmMain/fdm_Fuselage.h \
    lib_fdm/fdmMain/fdm_HingeMoment.h \
    lib_fdm/fdmMain/fdm_Intersections.h \
    lib_fdm/fdmMain/fdm_LandingGear.h \
    lib_fdm/fdmMain/fdm_MainRotor.h \
    lib_fdm/fdmMain/fdm_Mass.h \
    lib_fdm/fdmMain/fdm_PistonEngine.h \
    lib_fdm/fdmMain/fdm_Propeller.h \
    lib_fdm/fdmMain/fdm_Propulsion.h \
    lib_fdm/fdmMain/fdm_Schrenk.h \
    lib_fdm/fdmMain/fdm_Stabilizer.h \
    lib_fdm/fdmMain/fdm_TailOff.h \
    lib_fdm/fdmMain/fdm_TailRotor.h \
    lib_fdm/fdmMain/fdm_Types.h \
    lib_fdm/fdmMain/fdm_WindShear.h

SOURCES += \
    lib_fdm/fdmMain/fdm_Aerodynamics.cpp \
    lib_fdm/fdmMain/fdm_Aircraft.cpp \
    lib_fdm/fdmMain/fdm_Atmosphere.cpp \
    lib_fdm/fdmMain/fdm_Base.cpp \
    lib_fdm/fdmMain/fdm_Controls.cpp \
    lib_fdm/fdmMain/fdm_DataManager.cpp \
    lib_fdm/fdmMain/fdm_DataNode.cpp \
    lib_fdm/fdmMain/fdm_Environment.cpp \
    lib_fdm/fdmMain/fdm_Fuselage.cpp \
    lib_fdm/fdmMain/fdm_HingeMoment.cpp \
    lib_fdm/fdmMain/fdm_Intersections.cpp \
    lib_fdm/fdmMain/fdm_LandingGear.cpp \
    lib_fdm/fdmMain/fdm_MainRotor.cpp \
    lib_fdm/fdmMain/fdm_Mass.cpp \
    lib_fdm/fdmMain/fdm_PistonEngine.cpp \
    lib_fdm/fdmMain/fdm_Propeller.cpp \
    lib_fdm/fdmMain/fdm_Propulsion.cpp \
    lib_fdm/fdmMain/fdm_Schrenk.cpp \
    lib_fdm/fdmMain/fdm_Stabilizer.cpp \
    lib_fdm/fdmMain/fdm_TailOff.cpp \
    lib_fdm/fdmMain/fdm_TailRotor.cpp \
    lib_fdm/fdmMain/fdm_WindShear.cpp

################################################################################

HEADERS += \
    lib_fdm/fdmSys/fdm_Inertia.h \
    lib_fdm/fdmSys/fdm_Inertia2.h \
    lib_fdm/fdmSys/fdm_PID.h

SOURCES += \
    lib_fdm/fdmSys/fdm_Inertia.cpp \
    lib_fdm/fdmSys/fdm_Inertia2.cpp \
    lib_fdm/fdmSys/fdm_PID.cpp

################################################################################

HEADERS += \
    lib_fdm/fdmUtils/fdm_Angles.h \
    lib_fdm/fdmUtils/fdm_EulerRect.h \
    lib_fdm/fdmUtils/fdm_GaussJordan.h \
    lib_fdm/fdmUtils/fdm_Integrator.h \
    lib_fdm/fdmUtils/fdm_Matrix.h \
    lib_fdm/fdmUtils/fdm_Matrix3x3.h \
    lib_fdm/fdmUtils/fdm_Matrix4x4.h \
    lib_fdm/fdmUtils/fdm_Matrix6x6.h \
    lib_fdm/fdmUtils/fdm_Misc.h \
    lib_fdm/fdmUtils/fdm_Quaternion.h \
    lib_fdm/fdmUtils/fdm_Random.h \
    lib_fdm/fdmUtils/fdm_RungeKutta4.h \
    lib_fdm/fdmUtils/fdm_String.h \
    lib_fdm/fdmUtils/fdm_Table.h \
    lib_fdm/fdmUtils/fdm_Table2D.h \
    lib_fdm/fdmUtils/fdm_Units.h \
    lib_fdm/fdmUtils/fdm_Vector.h \
    lib_fdm/fdmUtils/fdm_Vector3.h \
    lib_fdm/fdmUtils/fdm_Vector4.h \
    lib_fdm/fdmUtils/fdm_Vector6.h \
    lib_fdm/fdmUtils/fdm_WGS84.h

SOURCES += \
    lib_fdm/fdmUtils/fdm_Angles.cpp \
    lib_fdm/fdmUtils/fdm_Matrix3x3.cpp \
    lib_fdm/fdmUtils/fdm_Matrix4x4.cpp \
    lib_fdm/fdmUtils/fdm_Matrix6x6.cpp \
    lib_fdm/fdmUtils/fdm_Quaternion.cpp \
    lib_fdm/fdmUtils/fdm_Random.cpp \
    lib_fdm/fdmUtils/fdm_String.cpp \
    lib_fdm/fdmUtils/fdm_Table.cpp \
    lib_fdm/fdmUtils/fdm_Table2D.cpp \
    lib_fdm/fdmUtils/fdm_Units.cpp \
    lib_fdm/fdmUtils/fdm_Vector3.cpp \
    lib_fdm/fdmUtils/fdm_Vector4.cpp \
    lib_fdm/fdmUtils/fdm_Vector6.cpp \
    lib_fdm/fdmUtils/fdm_WGS84.cpp

################################################################################

HEADERS += \
    lib_fdm/fdmXml/fdm_XmlDoc.h \
    lib_fdm/fdmXml/fdm_XmlNode.h \
    lib_fdm/fdmXml/fdm_XmlUtils.h

SOURCES += \
    lib_fdm/fdmXml/fdm_XmlDoc.cpp \
    lib_fdm/fdmXml/fdm_XmlNode.cpp \
    lib_fdm/fdmXml/fdm_XmlUtils.cpp
