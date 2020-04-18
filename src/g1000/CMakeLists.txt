add_library( g1000 STATIC
    g1000_Log.cpp
    g1000_Main.cpp

    cgi/g1000_ADI.cpp
    cgi/g1000_AFCS.cpp
    cgi/g1000_ALT.cpp
    cgi/g1000_ASI.cpp
    cgi/g1000_Colors.cpp
    cgi/g1000_COM.cpp
    cgi/g1000_Fonts.cpp
    cgi/g1000_GDU.cpp
    cgi/g1000_HSI.cpp
    cgi/g1000_Models.cpp
    cgi/g1000_Module.cpp
    cgi/g1000_NAV.cpp
    cgi/g1000_PFD.cpp
    cgi/g1000_VSI.cpp
    cgi/g1000_WYPT.cpp

    sim/g1000_GDC.cpp
    sim/g1000_GEA.cpp
    sim/g1000_GIA.cpp
    sim/g1000_GMU.cpp
    sim/g1000_GRS.cpp
    sim/g1000_GTX.cpp
    
    utils/g1000_String.cpp
    utils/g1000_Units.cpp
    
    xml/g1000_XmlDoc.cpp
    xml/g1000_XmlNode.cpp
    xml/g1000_XmlUtils.cpp
)
