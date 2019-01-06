/****************************************************************************//*
 * Copyright (C) 2019 Marek M. Cel
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

#include <cgi/cgi_HUD.h>
#include <cgi/cgi_OTW.h>

#include <cgi/cgi_Camera.h>
#include <cgi/cgi_Singleton.h>

////////////////////////////////////////////////////////////////////////////////

namespace cgi
{

/**
 * @brief CGI manager class.
 */
class Manager : public Singleton< Manager >
{
    friend class Singleton< Manager >;

public:

    /** Destructor. */
    virtual ~Manager();

    /** */
    void update();

    /** */
    inline osgGA::CameraManipulator* getCameraManipulator()
    {
        return m_camera->getManipulator();
    }

    /** */
    inline osg::Vec3d getCameraPosition() const
    {
        return m_camera->getPosition();
    }

    /** */
    inline osg::Quat getCameraAttitude() const
    {
        return m_camera->getAttitude();
    }

    /** */
    inline osg::Node* getNodeHUD()
    {
        return m_hud->getNode();
    }

    /** */
    inline osg::Node* getNodeOTW()
    {
        return m_otw->getNode();
    }

    /** */
    void setCameraManipulatorOrbit();

    /** */
    void setCameraManipulatorPilot();

    /** */
    void setCameraManipulatorWorld();

private:

    HUD *m_hud;         ///<
    OTW *m_otw;         ///<

    Camera *m_camera;   ///<

    /**
     * You should use static function instance() due to get refernce
     * to Manager class instance.
     */
    Manager();

    /** Using this constructor is forbidden. */
    Manager( const Manager & ) {}

    /** */
    void updateGround();
};

} // end of cgi namespace

////////////////////////////////////////////////////////////////////////////////

#endif // CGI_MANAGER_H
