add_library( g1000 STATIC
    g1000_Log.cpp

    gdu/g1000_ADI.cpp
    gdu/g1000_ALT.cpp
    gdu/g1000_ASI.cpp
    gdu/g1000_Colors.cpp
    gdu/g1000_Fonts.cpp
    gdu/g1000_GDU.cpp
    gdu/g1000_HSI.cpp
    gdu/g1000_Models.cpp
    gdu/g1000_Module.cpp
    gdu/g1000_PFD.cpp
    gdu/g1000_VSI.cpp
    
    utils/g1000_String.cpp
    utils/g1000_Units.cpp
    
    xml/g1000_XmlDoc.cpp
    xml/g1000_XmlNode.cpp
    xml/g1000_XmlUtils.cpp
)
