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
#ifndef CGI_SKYDOME_H
#define CGI_SKYDOME_H

////////////////////////////////////////////////////////////////////////////////

#include <osg/LightSource>
#include <osg/Material>
#include <osg/PositionAttitudeTransform>
#include <osg/Switch>
#include <osg/Texture2D>

#include <osgSim/LightPointNode>

#include <cgi/cgi_Ephemeris.h>
#include <cgi/cgi_Module.h>
#include <cgi/cgi_Textures.h>

////////////////////////////////////////////////////////////////////////////////

namespace cgi
{

/**
 * @brief Sky dome class.
 */
class SkyDome : public Module
{
public:

    static const float m_sunRadius;         ///< [m] average angular diameter 0.54 deg
    static const float m_moonRadius;        ///< [m] average angular diameter 0.53 deg

    static const float m_offsetStars;
    static const float m_offsetSunHalo;
    static const float m_offsetSunFace;
    static const float m_offsetMoon;

    /** Constructor. */
    SkyDome( Module *parent = 0 );

    /** Destructor. */
    virtual ~SkyDome();

    /** Updates sky dome. */
    void update();

private:

    Ephemeris m_ephemeris;
    Ephemeris::DateTime m_dateTime;

    osg::ref_ptr<osg::Switch> m_switch;

    osg::ref_ptr<osg::PositionAttitudeTransform> m_position;
    osg::ref_ptr<osg::PositionAttitudeTransform> m_attitude;

    osg::ref_ptr<osg::PositionAttitudeTransform> m_patSky;
    osg::ref_ptr<osg::PositionAttitudeTransform> m_patSun;
    osg::ref_ptr<osg::PositionAttitudeTransform> m_patMoon;

    osg::ref_ptr<osg::Switch> m_switchStars;

    osg::ref_ptr<osg::Geode> m_geodeSky;
    osg::ref_ptr<osg::Geode> m_geodeSunFace;
    osg::ref_ptr<osg::Geode> m_geodeSunHalo;
    osg::ref_ptr<osg::Geode> m_geodeMoon;

    osg::ref_ptr<osg::Material> m_materialSun;

    osg::ref_ptr<osg::LightSource> m_lightSourceSun;
    osg::ref_ptr<osg::LightSource> m_lightSourceMoon;
    osg::ref_ptr<osg::LightSource> m_lightSourceStars;

    osg::ref_ptr<osgSim::LightPointNode> m_stars;

    Textures::List m_texturesSky;   ///<
    Textures::List m_texturesFog;   ///<
    Textures::List m_texturesSun;   ///<
    Textures::List m_texturesMoon;  ///<
    Textures::List m_texturesStars; ///<

    unsigned short m_textureSky;    ///<
    unsigned short m_textureFog;    ///<
    unsigned short m_textureSun;    ///<
    unsigned short m_textureMoon;   ///<

    float m_skyScale;       ///< [-] sky dome scaling factor

    float m_sunAlpha;       ///< [rad] Sun right ascension
    float m_sunDelta;       ///< [rad] Sun declination
    float m_sunElev;        ///< [rad] Sun elevation
    float m_sunAzim;        ///< [rad] Sun azimuth
    float m_moonAlpha;      ///< [rad] Moon right ascension
    float m_moonDelta;      ///< [rad] Moon declination
    float m_moonElev;       ///< [rad] Moon elevation
    float m_moonAzim;       ///< [rad] Moon azimuth

    float *m_starIntensity; ///<
    int    m_starsCount;    ///<

    void createTextures();

    void createSky();
    void createSun();
    void createSunFace();
    void createSunHalo();
    void createMoon();
    void createStars();
    void createStar( float alpha, float delta, float mag, bool trueIntensity = false );

    void createLightMoon();
    void createLightSun();

    void createDome( osg::Geode *dome, osg::Texture2D *texture,
                     float radius, bool blend = false );
};

} // end of cgi namespace

////////////////////////////////////////////////////////////////////////////////

#endif // CGI_SKYDOME_H
