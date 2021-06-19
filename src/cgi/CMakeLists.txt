set( MODULE_NAME cgi )

set( CPP_FILES
    hud/cgi_HUD.cpp
    map/cgi_Icons.cpp
    map/cgi_Layers.cpp
    map/cgi_Map.cpp
    map/cgi_Traces.cpp
    otw/cgi_Airport.cpp
    otw/cgi_Clouds.cpp
    otw/cgi_CloudsBlock.cpp
    otw/cgi_CloudsLayer.cpp
    otw/cgi_Entities.cpp
    otw/cgi_FogScene.cpp
    otw/cgi_Landmark.cpp
    otw/cgi_OTW.cpp
    otw/cgi_Ownship.cpp
    otw/cgi_Reflection.cpp
    otw/cgi_Rotor.cpp
    otw/cgi_Scenery.cpp
    otw/cgi_SkyDome.cpp
    otw/cgi_Terrain.cpp
    otw/cgi_Vector.cpp
    cgi_Camera.cpp
    cgi_Colors.cpp
    cgi_Ephemeris.cpp
    cgi_FindNode.cpp
    cgi_Fonts.cpp
    cgi_Geometry.cpp
    cgi_GeoTIFF.cpp
    cgi_Intersections.cpp
    cgi_Manager.cpp
    cgi_ManipulatorMap.cpp
    cgi_ManipulatorOrbit.cpp
    cgi_ManipulatorShift.cpp
    cgi_ManipulatorWorld.cpp
    cgi_Mercator.cpp
    cgi_Models.cpp
    cgi_Module.cpp
    cgi_Textures.cpp
    cgi_Utils.cpp
    cgi_WGS84.cpp
)

add_library( ${MODULE_NAME} STATIC ${CPP_FILES} )
