add_library( g1000 STATIC
    g1000_Log.cpp
    g1000_Main.cpp

    gdu/g1000_ADI.cpp
    gdu/g1000_AFCS.cpp
    gdu/g1000_ALT.cpp
    gdu/g1000_ASI.cpp
    gdu/g1000_Colors.cpp
    gdu/g1000_COM.cpp
    gdu/g1000_Fonts.cpp
    gdu/g1000_GDU.cpp
    gdu/g1000_HSI.cpp
    gdu/g1000_Models.cpp
    gdu/g1000_Module.cpp
    gdu/g1000_NAV.cpp
    gdu/g1000_PFD.cpp
    gdu/g1000_VSI.cpp

    sys/g1000_GDC74A.cpp
    sys/g1000_GEA71.cpp
    sys/g1000_GIA63.cpp
    sys/g1000_GMU44.cpp
    sys/g1000_GRS77.cpp
    sys/g1000_GTX33.cpp
    
    utils/g1000_String.cpp
    utils/g1000_Units.cpp
    
    xml/g1000_XmlDoc.cpp
    xml/g1000_XmlNode.cpp
    xml/g1000_XmlUtils.cpp
)
