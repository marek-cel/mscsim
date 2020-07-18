set( MODULE_NAME gui )

set( CPP_FILES
    Aircrafts.cpp
    Airports.cpp
    Colors.cpp
    ComboUnits.cpp
    ComboUnitsAngle.cpp
    ComboUnitsFlowRate.cpp
    ComboUnitsLength.cpp
    ComboUnitsMass.cpp
    ComboUnitsNumber.cpp
    ComboUnitsPressure.cpp
    ComboUnitsRotation.cpp
    ComboUnitsTemperature.cpp
    ComboUnitsVelocity.cpp
    DialogAction.cpp
    DialogConf.cpp
    DialogEnvr.cpp
    DialogInit.cpp
    DialogMass.cpp
    DialogTime.cpp
    DockWidgetAuto.cpp
    DockWidgetCtrl.cpp
    DockWidgetData.cpp
    DockWidgetEFIS.cpp
    DockWidgetMain.cpp
    DockWidgetMap.cpp
    DockWidgetProp.cpp
    Fonts.cpp
    GraphicsEADI.cpp
    GraphicsEHSI.cpp
    GraphicsPedals.cpp
    GraphicsStick.cpp
    GraphicsWinQt.cpp
    KeyHandler.cpp
    KeyMap.cpp
    LayoutSquare.cpp
    MainWindow.cpp
    PageControls.cpp
    PageGeneral.cpp
    ScreenSaver.cpp
    SpinBoxHighlight.cpp
    WidgetCGI.cpp
    WidgetGDU.cpp
    WidgetMap.cpp
    WidgetOSG.cpp
    WidgetOTW.cpp
    WidgetPFD.cpp
    WidgetSquare.cpp
)

set( MOC_SRC_FILES
    ComboUnits.h
    ComboUnitsAngle.h
    ComboUnitsFlowRate.h
    ComboUnitsLength.h
    ComboUnitsMass.h
    ComboUnitsNumber.h
    ComboUnitsPressure.h
    ComboUnitsRotation.h
    ComboUnitsTemperature.h
    ComboUnitsVelocity.h
    DialogAction.h
    DialogConf.h
    DialogEnvr.h
    DialogInit.h
    DialogMass.h
    DialogTime.h
    DockWidgetAuto.h
    DockWidgetCtrl.h
    DockWidgetData.h
    DockWidgetEFIS.h
    DockWidgetMain.h
    DockWidgetMap.h
    DockWidgetProp.h
    GraphicsEADI.h
    GraphicsEHSI.h
    GraphicsPedals.h
    GraphicsStick.h
    LayoutSquare.h
    MainWindow.h
    PageControls.h
    PageGeneral.h
    SpinBoxHighlight.h
    WidgetCGI.h
    WidgetGDU.h
    WidgetMap.h
    WidgetOSG.h
    WidgetOTW.h
    WidgetPFD.h
    WidgetSquare.h
)

set( UIC_SRC_FILES
    DialogAction.ui
    DialogConf.ui
    DialogEnvr.ui
    DialogInit.ui
    DialogMass.ui
    DialogTime.ui
    DockWidgetAuto.ui
    DockWidgetCtrl.ui
    DockWidgetData.ui
    DockWidgetEFIS.ui
    DockWidgetMain.ui
    DockWidgetMap.ui
    DockWidgetProp.ui
    MainWindow.ui
    PageControls.ui
    PageGeneral.ui
    WidgetGDU.ui
    WidgetPFD.ui
)

add_library( ${MODULE_NAME} STATIC ${CPP_FILES} ${MOC_SRC_FILES} ${UIC_SRC_FILES} )

if( WIN32 )
    qt5_use_modules( ${MODULE_NAME} Widgets OpenGL Svg Xml )
endif()

if( UNIX )
    qt5_use_modules( ${MODULE_NAME} Widgets OpenGL Svg X11Extras Xml )
endif()
