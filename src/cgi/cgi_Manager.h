/****************************************************************************//*
 * Copyright (C) 2021 Marek M. Cel
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 ******************************************************************************/
#ifndef CGI_MANAGER_H
#define CGI_MANAGER_H

////////////////////////////////////////////////////////////////////////////////

#include <sim/Singleton.h>

#include <cgi/hud/cgi_HUD.h>
#include <cgi/map/cgi_Map.h>
#include <cgi/otw/cgi_OTW.h>

#include <cgi/cgi_Camera.h>

////////////////////////////////////////////////////////////////////////////////

namespace cgi
{

/**
 * @brief CGI manager class.
 */
class Manager : public Singleton< Manager >
{
    friend class Singleton< Manager >;

private:

    /**
     * You should use static function instance() due to get refernce
     * to Manager class instance.
     */
    Manager();

    /** Using this constructor is forbidden. */
    Manager( const Manager & ) {}

public:

    /** @brief Destructor. */
    virtual ~Manager();

    inline void resetTraces() { _map->resetTraces(); }

    /** @brief Updates HUD. */
    void updateHUD();

    /** @brief Updates map. */
    void updateMap();

    /** @brief Updates OTW. */
    void updateOTW();

    /** */
    inline osgGA::CameraManipulator* getCameraManipulator()
    {
        return _camera->getManipulator();
    }

    /** */
    inline osg::Vec3d getCameraPosition() const
    {
        return _camera->getPosition();
    }

    /** */
    inline osg::Quat getCameraAttitude() const
    {
        return _camera->getAttitude();
    }

    /** @brief Returns HUD root node. */
    inline osg::Node* getNodeHUD()
    {
        return _hud->getNode();
    }

    /** @brief Returns map root node. */
    inline osg::Node* getNodeMap()
    {
        return _map->getNode();
    }

    /** @brief Returns OTW root node. */
    inline osg::Node* getNodeOTW()
    {
        return _otw->getNode();
    }

    /** */
    void setCameraManipulatorChase();

    /** */
    void setCameraManipulatorOrbit();

    /** */
    void setCameraManipulatorPilot();

    /** */
    void setCameraManipulatorWorld();

    /** */
    void setDistanceDef( double distance_def );

    /** */
    void setDistanceMin( double distance_min );

    /** @brief Sets map scale factor. */
    void setMapScale( double scale );

    inline void setVisibilityCrops     ( bool visible ) { _map->setVisibilityCrops     ( visible ); }
    inline void setVisibilityGrassland ( bool visible ) { _map->setVisibilityGrassland ( visible ); }
    inline void setVisibilityWoodland  ( bool visible ) { _map->setVisibilityWoodland  ( visible ); }
    inline void setVisibilityBuiltup   ( bool visible ) { _map->setVisibilityBuiltup   ( visible ); }
    inline void setVisibilityRailroads ( bool visible ) { _map->setVisibilityRailroads ( visible ); }
    inline void setVisibilityRoads     ( bool visible ) { _map->setVisibilityRoads     ( visible ); }
    inline void setVisibilityAirports  ( bool visible ) { _map->setVisibilityAirports  ( visible ); }
    inline void setVisibilityInWaters  ( bool visible ) { _map->setVisibilityInWaters  ( visible ); }
    inline void setVisibilitySatellite ( bool visible ) { _map->setVisibilitySatellite ( visible ); }
    inline void setVisibilityBorders   ( bool visible ) { _map->setVisibilityBorders   ( visible ); }
    inline void setVisibilityTraces    ( bool visible ) { _map->setVisibilityTraces    ( visible ); }

private:

    HUD *_hud;                  ///< HUD object
    Map *_map;                  ///< map object
    OTW *_otw;                  ///< OTW object

    Camera *_camera;            ///< camera object

    /** */
    void updateGround();
};

} // end of cgi namespace

////////////////////////////////////////////////////////////////////////////////

#endif // CGI_MANAGER_H
