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

#include <cgi/otw/cgi_SkyDome.h>

#ifdef _MSC_VER
#   include <algorithm>
#endif

#include <cmath>
#include <fstream>
#include <limits>

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Light>
#include <osg/PolygonOffset>

#include <Data.h>

#include <cgi/otw/cgi_FogScene.h>

#include <cgi/cgi_Colors.h>
#include <cgi/cgi_Defines.h>
#include <cgi/cgi_Geometry.h>
#include <cgi/cgi_WGS84.h>

#include <sim/Path.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

const float SkyDome::_sunRadius  = CGI_SKYDOME_RADIUS * tan( osg::DegreesToRadians( CGI_SKYDOME_DIAMETER_SUN  / 2.0 ) );
const float SkyDome::_moonRadius = CGI_SKYDOME_RADIUS * tan( osg::DegreesToRadians( CGI_SKYDOME_DIAMETER_MOON / 2.0 ) );

const float SkyDome::_offsetStars   = -1000.0f;
const float SkyDome::_offsetSunHalo = -2000.0f;
const float SkyDome::_offsetSunFace = -3000.0f;
const float SkyDome::_offsetMoon    = -4000.0f;

////////////////////////////////////////////////////////////////////////////////

SkyDome::SkyDome( const Module *parent ) :
    Module( parent ),

    _textureSky  ( 0 ),
    _textureFog  ( 0 ),
    _textureSun  ( 0 ),
    _textureMoon ( 0 ),

    _skyScale ( 1.0f ),

    _sunAlpha  ( 0.0f ),
    _sunDelta  ( 0.0f ),
    _sunElev   ( 0.0f ),
    _sunAzim   ( 0.0f ),
    _moonAlpha ( 0.0f ),
    _moonDelta ( 0.0f ),
    _moonElev  ( 0.0f ),
    _moonAzim  ( 0.0f ),

    _starIntensity ( 0 ),
    _starsCount ( 0 )
{
    _root->setName( "SkyDome" );

    _switch = new osg::Switch();
    _root->addChild( _switch.get() );

    // position
    _position = new osg::PositionAttitudeTransform();
    _switch->addChild( _position.get() );

    // attitude (due to local horizon)
    _attitude = new osg::PositionAttitudeTransform();
    _position->addChild( _attitude.get() );

    // Sky PAT
    _patSky = new osg::PositionAttitudeTransform();
    _position->addChild( _patSky.get() );

    // Sun PAT
    _patSun = new osg::PositionAttitudeTransform();
    _attitude->addChild( _patSun.get() );

    // Moon PAT
    _patMoon = new osg::PositionAttitudeTransform();
    _patSky->addChild( _patMoon.get() );

    createTextures();

    createSky();
    createSun();
    createMoon();
    createStars();

    createLightMoon();
    createLightSun();

    _root->getOrCreateStateSet()->setMode( GL_BLEND, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE );
}

////////////////////////////////////////////////////////////////////////////////

SkyDome::~SkyDome()
{
    if ( _starIntensity ) delete [] _starIntensity;
    _starIntensity = 0;
}

////////////////////////////////////////////////////////////////////////////////

void SkyDome::update()
{
    /////////////////
    Module::update();
    /////////////////

    _dateTime.year   = Data::get()->dateTime.year;
    _dateTime.month  = Data::get()->dateTime.month;
    _dateTime.day    = Data::get()->dateTime.day;
    _dateTime.hour   = Data::get()->dateTime.hour;
    _dateTime.minute = Data::get()->dateTime.minute;
    _dateTime.second = Data::get()->dateTime.second;

    _ephemeris.update( _dateTime,
                       Data::get()->cgi.camera.latitude,
                       Data::get()->cgi.camera.longitude );

    _sunAlpha  = _ephemeris.getSunAlpha();
    _sunDelta  = _ephemeris.getSunDelta();
    _sunElev   = _ephemeris.getSunElev();
    _sunAzim   = _ephemeris.getSunAzim();
    _moonAlpha = _ephemeris.getMoonAlpha();
    _moonDelta = _ephemeris.getMoonDelta();
    _moonElev  = _ephemeris.getMoonElev();
    _moonAzim  = _ephemeris.getMoonAzim();

    ////////////////////////////////////////

#   ifdef SIM_SKYDOME_SCALING
    // scale
    if ( Data::get()->cgi.camera.altitude_agl > CGI_SKYDOME_SCALING_TRANSIENT_ALT_MIN )
    {
        // Scaling (distance to the horizon - simplified formula)
        _skyScale = 3600.0 * sqrt( Data::get()->cgi.camera.altitude_asl ) / CGI_SKYDOME_RADIUS;

        float coef = ( Data::get()->cgi.camera.altitude_agl - CGI_SKYDOME_SCALING_TRANSIENT_ALT_MIN )
                   / ( CGI_SKYDOME_SCALING_TRANSIENT_ALT_MAX - CGI_SKYDOME_SCALING_TRANSIENT_ALT_MIN );
        if ( coef > 1.0f ) coef = 1.0f;

        _skyScale *= coef;

        if ( _skyScale < 1.0 ) _skyScale = 1.0;
        if ( _skyScale > 9.0 ) _skyScale = 9.0;
    }
    else
    {
        _skyScale = 1.0;
    }
#   else
    _skyScale = 1.0;
#   endif

    // position
    osg::Vec3d pos_wgs = WGS84( Data::get()->cgi.camera.latitude,
                                Data::get()->cgi.camera.longitude,
                                0.0 ).getPosition();

    double r_s = _skyScale * CGI_SKYDOME_RADIUS;
    double r_e = pos_wgs.length();

    double delta_h = -r_s * tan( asin( r_s / r_e ) );

    WGS84 wgs( Data::get()->cgi.camera.latitude,
               Data::get()->cgi.camera.longitude,
               delta_h );

    _attitude->setAttitude( wgs.getAttitude() );
    _position->setPosition( wgs.getPosition() );
#   ifdef SIM_SKYDOME_SCALING
    osg::Vec3 vecSkyScale( _skyScale, _skyScale, _skyScale );
    _position ->setScale( vecSkyScale );
    _patSun   ->setScale( vecSkyScale );
    _patMoon  ->setScale( vecSkyScale );
#   endif

    osg::Quat Q_gst( -_ephemeris.getGST(), osg::Z_AXIS );

    osg::Quat Q_sun( M_PI_2, osg::X_AXIS,
                   _sunElev, osg::Y_AXIS,
                   _sunAzim, osg::Z_AXIS );

    osg::Quat Q_moon( 0.0, osg::X_AXIS,
              -_moonDelta, osg::Y_AXIS,
               _moonAlpha, osg::Z_AXIS );

    _patSky->setAttitude( Q_gst );
    _patSun->setAttitude( Q_sun );
    _patMoon->setAttitude( Q_moon );

    float sun_elevation_deg  = osg::RadiansToDegrees( _sunElev  );
    float moon_elevation_deg = osg::RadiansToDegrees( _moonElev );

    // Sky
    unsigned short sky_num = 0;

    if ( sun_elevation_deg >= -10.0f )
    {
        if ( sun_elevation_deg < 10.0f )
        {
            sky_num = floor( 8 * ( sun_elevation_deg + 10.0f ) / 20.0f + 0.5f );
            sky_num = std::min( std::max( (int)sky_num, 0 ), 8 );
        }
        else
        {
            sky_num = 8;
        }
    }

    if ( Data::get()->cgi.environment.visibility < CGI_FOG_LIMIT )
    {
        unsigned short fog_num = FogScene::getFogNumber( sun_elevation_deg );

        if ( _textureFog != fog_num && fog_num < _texturesFog.size() )
        {
            _textureSky = 0;
            _textureFog = fog_num;
            _geodeSky->getOrCreateStateSet()->setTextureAttributeAndModes( 0, _texturesFog[ _textureFog ], osg::StateAttribute::ON );
        }
    }
    else
    {
        if ( _textureSky != sky_num && sky_num < _texturesSky.size() )
        {
            _textureFog = 0;
            _textureSky = sky_num;
            _geodeSky->getOrCreateStateSet()->setTextureAttributeAndModes( 0, _texturesSky[ _textureSky ], osg::StateAttribute::ON );
        }
    }

    // Sun
    unsigned short numSun = 0;

    if ( sun_elevation_deg >= 0.0f && sun_elevation_deg < 10.0f )
    {
        numSun = floor( 10.0f * ( sun_elevation_deg ) / 10.0f + 0.5f );
    }
    else
    {
        numSun = ( sun_elevation_deg < 0.0f ) ? 0 : 10;
    }

    numSun = std::min( std::max( (int)numSun, 0 ), 10 );

    osg::Vec4 sunColor( Colors::_sun[ numSun ], 1.0f );

    _materialSun->setColorMode( osg::Material::AMBIENT_AND_DIFFUSE );
    _materialSun->setAmbient( osg::Material::FRONT, sunColor );
    _materialSun->setDiffuse( osg::Material::FRONT, sunColor );

    _geodeSunFace->getOrCreateStateSet()->setAttribute( _materialSun.get() );

    if ( _textureSun != numSun && numSun < _texturesSun.size() )
    {
        _textureSun = numSun;

        _geodeSunHalo->getOrCreateStateSet()->setTextureAttributeAndModes( 0, _texturesSun[ _textureSun ], osg::StateAttribute::ON );
    }

    // Moon
    unsigned short numMoon = 0;

    // known new moon : 2000-01-06 18:15 UTC
    // synodic month  : 29.530587981 days
    // New Moon    : 0.0
    // 1st Quarter : 0.25
    // Full Moon   : 0.5
    // 3rd Quarter : 0.75
    // New Moon    : 1.0
    double moonTmp = ( _ephemeris.getJD() - 2451550.0 ) / 29.530587981;
    double moonAge = moonTmp - floor( moonTmp );

    numMoon = floor( 7.0 * moonAge + 0.5 );
    numMoon = std::min( std::max( (int)numMoon, 0 ), 7 );

    if ( _textureMoon != numMoon && numMoon < _texturesMoon.size() )
    {
        _textureMoon = numMoon;

        _geodeMoon->getOrCreateStateSet()->setTextureAttributeAndModes( 0, _texturesMoon[ _textureMoon ], osg::StateAttribute::ON );
    }

    // Stars
    float starsIntensity = 0.0f;

    if ( -5.0f < sun_elevation_deg && sun_elevation_deg < 5.0f )
    {
        starsIntensity = 1.0f - ( sun_elevation_deg + 5.0f ) / 10.0f;
    }
    else
    {
        if ( sun_elevation_deg < -5.0f )
        {
            starsIntensity = 1.0f;
        }
        else
        {
            starsIntensity = 0.0f;
        }
    }

    if ( starsIntensity > 0.0f )
    {
        _switchStars->setAllChildrenOn();

        if ( starsIntensity > 1.0f ) starsIntensity = 1.0f;

        osgSim::LightPointNode::LightPointList lightPointList = _stars->getLightPointList();

        for ( unsigned int i = 0; i < lightPointList.size(); i++ )
        {
            lightPointList[ i ]._intensity = starsIntensity * _starIntensity[ i ];
        }

        _stars->setLightPointList( lightPointList );
    }
    else
    {
        _switchStars->setAllChildrenOff();
    }

    // Sun Light
    osg::ref_ptr<osg::Light> lightSun = _lightSourceSun->getLight();

    if ( lightSun.valid() )
    {
        float intensityDiffuse = 0.0f;
        float intensityAmbient = 0.0f;

        if ( -0.5f < sun_elevation_deg && sun_elevation_deg < 0.5f )
        {
            intensityDiffuse = sun_elevation_deg + 0.5f;
        }
        else
        {
            intensityDiffuse = ( sun_elevation_deg < -0.5f ) ? 0.0f : 1.0f;
        }

        if ( -5.0f < sun_elevation_deg && sun_elevation_deg < 5.0f )
        {
            intensityAmbient = ( sun_elevation_deg + 5.0f ) / 10.0f;
        }
        else
        {
            intensityAmbient = ( sun_elevation_deg < -5.0f ) ? 0.0f : 0.9f;
        }

        osg::Vec4 sunLightColorDiffuse( Colors::_sun[ numSun ] * intensityDiffuse, 0.0f );
        osg::Vec4 sunLightColorAmbient( Colors::_sun[   10   ] * intensityAmbient, 0.0f );

        lightSun->setAmbient( sunLightColorAmbient );
        lightSun->setDiffuse( sunLightColorDiffuse );
    }

    // Moon Light
    osg::ref_ptr<osg::Light> lightMoon = _lightSourceMoon->getLight();

    if ( lightMoon.valid() )
    {
        float intensity = 0.0f;

        if ( 0.0f < moon_elevation_deg )
        {
            if ( moon_elevation_deg < 5.0f )
            {
                intensity = moon_elevation_deg / 5.0f;
            }
            else
            {
                intensity = 1.0f;
            }

            float ageCoef = 1.0 - fabs( 2.0f * ( moonAge - 0.5f ) );
            intensity *= ageCoef;
        }

        osg::Vec4 moonLightColor( Colors::_moon * intensity, 0.0f );

        lightMoon->setAmbient( moonLightColor );
        lightMoon->setDiffuse( moonLightColor );
    }

    Data::get()->cgi.skyDome.skyScale = _skyScale;
    Data::get()->cgi.skyDome.sunAlpha = _sunAlpha;
    Data::get()->cgi.skyDome.sunDelta = _sunDelta;
    Data::get()->cgi.skyDome.sunElev  = _sunElev;
    Data::get()->cgi.skyDome.sunAzim  = _sunAzim;
}

////////////////////////////////////////////////////////////////////////////////

void SkyDome::createTextures()
{
    _texturesSky.push_back( Textures::get( "cgi/textures/sky_clear_0.png" ) );
    _texturesSky.push_back( Textures::get( "cgi/textures/sky_clear_1.png" ) );
    _texturesSky.push_back( Textures::get( "cgi/textures/sky_clear_2.png" ) );
    _texturesSky.push_back( Textures::get( "cgi/textures/sky_clear_3.png" ) );
    _texturesSky.push_back( Textures::get( "cgi/textures/sky_clear_4.png" ) );
    _texturesSky.push_back( Textures::get( "cgi/textures/sky_clear_5.png" ) );
    _texturesSky.push_back( Textures::get( "cgi/textures/sky_clear_6.png" ) );
    _texturesSky.push_back( Textures::get( "cgi/textures/sky_clear_7.png" ) );
    _texturesSky.push_back( Textures::get( "cgi/textures/sky_clear_8.png" ) );

    _texturesFog.push_back( Textures::get( "cgi/textures/sky_foggy_0.png" ) );
    _texturesFog.push_back( Textures::get( "cgi/textures/sky_foggy_1.png" ) );
    _texturesFog.push_back( Textures::get( "cgi/textures/sky_foggy_2.png" ) );
    _texturesFog.push_back( Textures::get( "cgi/textures/sky_foggy_3.png" ) );
    _texturesFog.push_back( Textures::get( "cgi/textures/sky_foggy_4.png" ) );
    _texturesFog.push_back( Textures::get( "cgi/textures/sky_foggy_5.png" ) );
    _texturesFog.push_back( Textures::get( "cgi/textures/sky_foggy_6.png" ) );
    _texturesFog.push_back( Textures::get( "cgi/textures/sky_foggy_7.png" ) );
    _texturesFog.push_back( Textures::get( "cgi/textures/sky_foggy_8.png" ) );

    _texturesSun.push_back( Textures::get( "cgi/textures/sun_000.png" ) );
    _texturesSun.push_back( Textures::get( "cgi/textures/sun_010.png" ) );
    _texturesSun.push_back( Textures::get( "cgi/textures/sun_020.png" ) );
    _texturesSun.push_back( Textures::get( "cgi/textures/sun_030.png" ) );
    _texturesSun.push_back( Textures::get( "cgi/textures/sun_040.png" ) );
    _texturesSun.push_back( Textures::get( "cgi/textures/sun_050.png" ) );
    _texturesSun.push_back( Textures::get( "cgi/textures/sun_060.png" ) );
    _texturesSun.push_back( Textures::get( "cgi/textures/sun_070.png" ) );
    _texturesSun.push_back( Textures::get( "cgi/textures/sun_080.png" ) );
    _texturesSun.push_back( Textures::get( "cgi/textures/sun_090.png" ) );
    _texturesSun.push_back( Textures::get( "cgi/textures/sun_100.png" ) );

    _texturesMoon.push_back( Textures::get( "cgi/textures/moon_0.png" ) );
    _texturesMoon.push_back( Textures::get( "cgi/textures/moon_1.png" ) );
    _texturesMoon.push_back( Textures::get( "cgi/textures/moon_2.png" ) );
    _texturesMoon.push_back( Textures::get( "cgi/textures/moon_3.png" ) );
    _texturesMoon.push_back( Textures::get( "cgi/textures/moon_4.png" ) );
    _texturesMoon.push_back( Textures::get( "cgi/textures/moon_5.png" ) );
    _texturesMoon.push_back( Textures::get( "cgi/textures/moon_6.png" ) );
    _texturesMoon.push_back( Textures::get( "cgi/textures/moon_7.png" ) );

    _texturesStars.push_back( Textures::get( "cgi/textures/star.png" ) );
}

////////////////////////////////////////////////////////////////////////////////

void SkyDome::createSky()
{
    _geodeSky = new osg::Geode();
    _attitude->addChild( _geodeSky.get() );

    createDome( _geodeSky, _texturesSky.at( _textureSky ), CGI_SKYDOME_RADIUS );

    _geodeSky->getOrCreateStateSet()->setRenderBinDetails( CGI_DEPTH_SORTED_BIN_SKY, "DepthSortedBin" );
}

////////////////////////////////////////////////////////////////////////////////

void SkyDome::createSun()
{
    createSunFace();
    createSunHalo();
}

////////////////////////////////////////////////////////////////////////////////

void SkyDome::createSunFace()
{
    osg::ref_ptr<osg::PositionAttitudeTransform> offsetSun = new osg::PositionAttitudeTransform();
    _patSun->addChild( offsetSun.get() );

    offsetSun->setPosition( osg::Vec3( CGI_SKYDOME_RADIUS, 0, 0 ) );

    _geodeSunFace = new osg::Geode();
    offsetSun->addChild( _geodeSunFace.get() );

    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();
    _geodeSunFace->addDrawable( geom.get() );

    Geometry::createFace( geom.get(), _sunRadius, false, 32 );

    osg::ref_ptr<osg::StateSet> stateSet = _geodeSunFace->getOrCreateStateSet();
    stateSet->setMode( GL_LIGHTING , osg::StateAttribute::OFF );

    // material
    _materialSun = new osg::Material();

    osg::Vec4 sunColor( Colors::_sun[ 0 ], 1.0f );

    _materialSun->setColorMode( osg::Material::AMBIENT_AND_DIFFUSE );
    _materialSun->setAmbient( osg::Material::FRONT, sunColor );
    _materialSun->setDiffuse( osg::Material::FRONT, sunColor );

    stateSet->setAttribute( _materialSun.get() );
    stateSet->setAttributeAndModes( new osg::PolygonOffset( 1.0f, _offsetSunFace ), osg::StateAttribute::ON );
    stateSet->setRenderBinDetails( CGI_DEPTH_SORTED_BIN_SUN, "DepthSortedBin" );
}

////////////////////////////////////////////////////////////////////////////////

void SkyDome::createSunHalo()
{
    _geodeSunHalo = new osg::Geode();
    _patSun->addChild( _geodeSunHalo.get() );

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array;
    osg::ref_ptr<osg::Vec2Array> t = new osg::Vec2Array;
    osg::ref_ptr<osg::Vec3Array> n  = new osg::Vec3Array;

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
    _geodeSunHalo->addDrawable( geometry.get() );

    double y = CGI_SKYDOME_RADIUS * tan( osg::DegreesToRadians( 5.0 ) );

    double azimuth = -40.0;

    while ( azimuth <= 40.0 )
    {
        double azimuth_rad = osg::DegreesToRadians( azimuth );

        double x = CGI_SKYDOME_RADIUS * cos( azimuth_rad );
        double z = CGI_SKYDOME_RADIUS * sin( azimuth_rad );

        v->push_back( osg::Vec3( x, -y, z ) );
        v->push_back( osg::Vec3( x,  y, z ) );

        osg::Vec3d norm( -x, 0, -z );
        norm.normalize();

        n->push_back( norm );
        n->push_back( norm );

        float temp = ( azimuth + 40.0 ) / 80.0;

        t->push_back( osg::Vec2( temp, 0 ) );
        t->push_back( osg::Vec2( temp, 1 ) );

        azimuth += 10.0;
    }

    geometry->setNormalArray( n.get() );
    geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );

    geometry->setVertexArray( v.get() );

    geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::TRIANGLE_STRIP, 0, v->size() ) );

    osg::ref_ptr<osg::StateSet> stateSet = _geodeSunHalo->getOrCreateStateSet();
    stateSet->setMode( GL_LIGHTING , osg::StateAttribute::OFF );

    // texture
    geometry->setTexCoordArray( 0, t.get() );

    stateSet->setMode( GL_BLEND, osg::StateAttribute::ON );
    stateSet->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
    stateSet->setAttributeAndModes( new osg::PolygonOffset( 1.0f, _offsetSunHalo ), osg::StateAttribute::ON );
    stateSet->setRenderBinDetails( CGI_DEPTH_SORTED_BIN_SUN, "DepthSortedBin" );
    stateSet->setTextureAttributeAndModes( 0, _texturesSun.at( _textureSun ), osg::StateAttribute::ON );

    osg::ref_ptr<osg::Material> material = new osg::Material();

    material->setColorMode( osg::Material::AMBIENT_AND_DIFFUSE );
    material->setAmbient( osg::Material::FRONT, osg::Vec4( 1.0f, 1.0f, 1.0f, 1.0f ) );
    material->setDiffuse( osg::Material::FRONT, osg::Vec4( 1.0f, 1.0f, 1.0f, 1.0f ) );

    stateSet->setAttribute( material.get() );
}

////////////////////////////////////////////////////////////////////////////////

void SkyDome::createMoon()
{
    osg::ref_ptr<osg::PositionAttitudeTransform> offsetMoon = new osg::PositionAttitudeTransform();
    _patMoon->addChild( offsetMoon.get() );

    offsetMoon->setPosition( osg::Vec3( CGI_SKYDOME_RADIUS, 0, 0 ) );

    _geodeMoon = new osg::Geode();
    offsetMoon->addChild( _geodeMoon.get() );

    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();
    _geodeMoon->addDrawable( geom.get() );

    Geometry::createFace( geom.get(), _moonRadius, true, 32 );

    osg::ref_ptr<osg::StateSet> stateSet = _geodeMoon->getOrCreateStateSet();

    // texture
    osg::ref_ptr<osg::Texture2D> texture = _texturesMoon.at( _textureMoon );

    if ( texture.valid() )
    {
        stateSet->setMode( GL_BLEND, osg::StateAttribute::ON );
        stateSet->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
        stateSet->setTextureAttributeAndModes( 0, texture.get(), osg::StateAttribute::ON );
    }

    stateSet->setMode( GL_LIGHTING , osg::StateAttribute::OFF );

    // material
    osg::ref_ptr<osg::Material> material = new osg::Material();

    material->setColorMode( osg::Material::AMBIENT_AND_DIFFUSE );
    material->setAmbient( osg::Material::FRONT, osg::Vec4( 1.0, 1.0, 1.0, 1.0 ) );
    material->setDiffuse( osg::Material::FRONT, osg::Vec4( 1.0, 1.0, 1.0, 1.0 ) );

    stateSet->setAttribute( material.get() );
    stateSet->setAttributeAndModes( new osg::PolygonOffset( 1.0f, _offsetMoon ), osg::StateAttribute::ON );
    stateSet->setRenderBinDetails( CGI_DEPTH_SORTED_BIN_MOON, "DepthSortedBin" );
}

////////////////////////////////////////////////////////////////////////////////

void SkyDome::createStars()
{
    _switchStars = new osg::Switch();
    _patSky->addChild( _switchStars.get() );

    _stars = new osgSim::LightPointNode();
    _switchStars->addChild( _stars.get() );

    float magLimit = 4.5f;

    std::fstream file( Path::get( "cgi/stars.csv" ).c_str(), std::ios_base::in );

    if ( file.is_open() )
    {
        char separator;

        float alpha = 0.0f;
        float delta = 0.0f;
        float mag   = 0.0f;

        while ( !file.eof() )
        {
            alpha = std::numeric_limits< float >::quiet_NaN();
            delta = std::numeric_limits< float >::quiet_NaN();
            mag   = std::numeric_limits< float >::quiet_NaN();

            file >> alpha;
            file >> separator;
            file >> delta;
            file >> separator;
            file >> mag;

            if ( !osg::isNaN( alpha ) && !osg::isNaN( delta ) && !osg::isNaN( mag ) )
            {
                if ( mag < magLimit )
                {
                    createStar( alpha, delta, mag );
                }
                else
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }

        //osg::notify(osg::ALWAYS) << _starsCount << " stars brighter than " << magLimit << "mag" << std::endl;

        file.close();
    }

    // texture
    osg::ref_ptr<osg::Texture2D> texture = _texturesStars.at( 0 );
    if ( texture.valid() )
    {
        osg::ref_ptr<osg::StateSet> stateSet = _stars->getOrCreateStateSet();
        _stars->setPointSprite();
        stateSet->setTextureAttributeAndModes( 0, texture.get(), osg::StateAttribute::ON );
    }

    _switchStars->getOrCreateStateSet()->setAttributeAndModes( new osg::PolygonOffset( 1.0f, _offsetStars ), osg::StateAttribute::ON );
    _switchStars->getOrCreateStateSet()->setRenderBinDetails( CGI_DEPTH_SORTED_BIN_STARS, "DepthSortedBin" );
}

////////////////////////////////////////////////////////////////////////////////

void SkyDome::createStar( float alpha, float delta, float mag, bool trueIntensity )
{
    float intensity = 0.0f;

    if ( !trueIntensity )
    {
        if ( mag < 1.0 ) mag = 1.0;
        if ( mag > 3.0 ) mag = 3.0;
    }

    // illuminance (bisection)
    double E_0 = 1.0e-7;
    double E_1 = 1.0;

    double E_C = E_0 + ( E_1 - E_0 ) / 2.0;

    for ( int i = 0; i < 10000; i++ )
    {
        double m = -2.5 * log10( E_C ) - 14.05;

        if ( m > mag )
            E_0 = E_C;
        else
            E_1 = E_C;

        E_C = E_0 + ( E_1 - E_0 ) / 2.0;

        if ( fabs( mag - m ) < 0.001 ) break;
    }

    // sphere area
    float r = CGI_SKYDOME_RADIUS + _offsetStars;
    float S = 4.0 * M_PI * r * r;

    // total luminous flux
    float phi = S * E_C;

    intensity = phi / 1000.0;

    // intensity tuning
    intensity *= 2.0f;

    ////////////////////////////////////////

    double alpha_rad = M_PI * alpha / 12.0;
    double delta_rad = osg::DegreesToRadians( delta );

    double z  = r * sin( delta_rad );
    double xy = r * cos( delta_rad );
    double x  = xy * cos( alpha_rad );
    double y  = xy * sin( alpha_rad );

    osg::Vec3d position( x, y, z );
    osg::Vec4 color( 1.0, 1.0, 1.0, 1.0 );

    osgSim::LightPoint star( position, color );

    star._intensity = intensity;
    star._radius    = 0.0005f * CGI_SKYDOME_RADIUS;

    if ( _starsCount > 0 )
    {
        std::vector< float > tempIntensity;

        for ( int i = 0; i < _starsCount; i++ )
        {
            tempIntensity.push_back( _starIntensity[ i ] );
        }

        delete [] _starIntensity;

        _starIntensity = new float [ _starsCount + 1 ];

        for ( int i = 0; i < _starsCount; i++ )
        {
            _starIntensity[ i ] = tempIntensity.at( i );
        }
    }
    else
    {
        _starIntensity = new float [ 1 ];
    }

    _starIntensity[ _starsCount ] = star._intensity;

    _starsCount++;

    _stars->addLightPoint( star );
}

////////////////////////////////////////////////////////////////////////////////

void SkyDome::createLightMoon()
{
    _lightSourceMoon = new osg::LightSource();
    _patMoon->addChild( _lightSourceMoon.get() );

    osg::ref_ptr<osg::Light> lightMoon = new osg::Light();

    lightMoon->setLightNum( CGI_LIGHT_MOON_NUM );

    lightMoon->setPosition( osg::Vec4d( CGI_SKYDOME_RADIUS, 0.0f, 0.0f, 0.0f ) );

    lightMoon->setAmbient( osg::Vec4( 1.0, 1.0, 1.0, 0.0 ) );
    lightMoon->setDiffuse( osg::Vec4( 1.0, 1.0, 1.0, 0.0 ) );
    lightMoon->setSpecular( osg::Vec4( 1.0, 1.0, 1.0, 0.0 ) );

    lightMoon->setConstantAttenuation( 1.0 );

    _lightSourceMoon->setLight( lightMoon.get() );

    _lightSourceMoon->setLocalStateSetModes( osg::StateAttribute::ON );
    _lightSourceMoon->setStateSetModes( *_root->getOrCreateStateSet(), osg::StateAttribute::ON );
}

////////////////////////////////////////////////////////////////////////////////

void SkyDome::createLightSun()
{
    _lightSourceSun = new osg::LightSource();
    _patSun->addChild( _lightSourceSun.get() );

    osg::ref_ptr<osg::Light> lightSun = new osg::Light();

    lightSun->setLightNum( CGI_LIGHT_SUN_NUM );

    lightSun->setPosition( osg::Vec4d( CGI_SKYDOME_RADIUS, 0.0f, 0.0f, 0.0f ) );

    lightSun->setAmbient(  osg::Vec4( Colors::_sun[ 10 ], 1.0 ) );
    lightSun->setDiffuse(  osg::Vec4( Colors::_sun[ 10 ], 1.0 ) );
    lightSun->setSpecular( osg::Vec4( Colors::_sun[ 10 ], 1.0 ) );

    lightSun->setConstantAttenuation( 1.0 );

    _lightSourceSun->setLight( lightSun.get() );

    _lightSourceSun->setLocalStateSetModes( osg::StateAttribute::ON );
    _lightSourceSun->setStateSetModes( *_root->getOrCreateStateSet(), osg::StateAttribute::ON );
}

////////////////////////////////////////////////////////////////////////////////

void SkyDome::createDome( osg::Geode *dome, osg::Texture2D *texture,
                          float radius, bool blend )
{
    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();
    dome->addDrawable( geom.get() );

    Geometry::createDome( geom, radius, true, Geometry::Cylindrical, 18, 36 );

    // state set
    osg::ref_ptr<osg::StateSet> stateSet = dome->getOrCreateStateSet();
    stateSet->setMode( GL_LIGHTING, osg::StateAttribute::OFF );

    // texture
    if ( texture != 0 )
    {
        if ( blend )
        {
            stateSet->setMode( GL_BLEND, osg::StateAttribute::ON );
            stateSet->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
        }

        stateSet->setTextureAttributeAndModes( 0, texture, osg::StateAttribute::ON );
    }

    // material
    osg::ref_ptr<osg::Material> material = new osg::Material();

    material->setColorMode( osg::Material::AMBIENT_AND_DIFFUSE );
    material->setAmbient( osg::Material::FRONT, osg::Vec4( 1.0f, 1.0f, 1.0f, 1.0f ) );
    material->setDiffuse( osg::Material::FRONT, osg::Vec4( 1.0f, 1.0f, 1.0f, 1.0f ) );

    stateSet->setAttribute( material.get() );
}
