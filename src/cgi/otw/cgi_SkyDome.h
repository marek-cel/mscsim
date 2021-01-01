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

    static const float _sunRadius;          ///< [m] average angular diameter 0.54 deg
    static const float _moonRadius;         ///< [m] average angular diameter 0.53 deg

    static const float _offsetStars;
    static const float _offsetSunHalo;
    static const float _offsetSunFace;
    static const float _offsetMoon;

    /** @brief Constructor. */
    SkyDome( const Module *parent = NULLPTR );

    /** @brief Destructor. */
    virtual ~SkyDome();

    /** @brief Updates sky dome. */
    void update();

private:

    Ephemeris _ephemeris;
    Ephemeris::DateTime _dateTime;

    osg::ref_ptr<osg::Switch> _switch;

    osg::ref_ptr<osg::PositionAttitudeTransform> _position;
    osg::ref_ptr<osg::PositionAttitudeTransform> _attitude;

    osg::ref_ptr<osg::PositionAttitudeTransform> _patSky;
    osg::ref_ptr<osg::PositionAttitudeTransform> _patSun;
    osg::ref_ptr<osg::PositionAttitudeTransform> _patMoon;

    osg::ref_ptr<osg::Switch> _switchStars;

    osg::ref_ptr<osg::Geode> _geodeSky;
    osg::ref_ptr<osg::Geode> _geodeSunFace;
    osg::ref_ptr<osg::Geode> _geodeSunHalo;
    osg::ref_ptr<osg::Geode> _geodeMoon;

    osg::ref_ptr<osg::Material> _materialSun;

    osg::ref_ptr<osg::LightSource> _lightSourceSun;
    osg::ref_ptr<osg::LightSource> _lightSourceMoon;
    osg::ref_ptr<osg::LightSource> _lightSourceStars;

    osg::ref_ptr<osgSim::LightPointNode> _stars;

    Textures::List _texturesSky;    ///<
    Textures::List _texturesFog;    ///<
    Textures::List _texturesSun;    ///<
    Textures::List _texturesMoon;   ///<
    Textures::List _texturesStars;  ///<

    unsigned short _textureSky;     ///<
    unsigned short _textureFog;     ///<
    unsigned short _textureSun;     ///<
    unsigned short _textureMoon;    ///<

    float _skyScale;        ///< [-] sky dome scaling factor

    float _sunAlpha;        ///< [rad] Sun right ascension
    float _sunDelta;        ///< [rad] Sun declination
    float _sunElev;         ///< [rad] Sun elevation
    float _sunAzim;         ///< [rad] Sun azimuth
    float _moonAlpha;       ///< [rad] Moon right ascension
    float _moonDelta;       ///< [rad] Moon declination
    float _moonElev;        ///< [rad] Moon elevation
    float _moonAzim;        ///< [rad] Moon azimuth

    float *_starIntensity;  ///<
    int _starsCount;        ///<

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
