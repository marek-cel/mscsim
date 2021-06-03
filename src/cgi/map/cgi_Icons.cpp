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

#include <cgi/map/cgi_Icons.h>

#include <osg/LineWidth>
#include <osg/Material>

#include <Data.h>

#include <cgi/cgi_Geometry.h>
#include <cgi/cgi_Mercator.h>
#include <cgi/cgi_Models.h>
#include <cgi/cgi_Textures.h>

#include <cgi/map/cgi_Map.h>

#include <fdm/utils/fdm_WGS84.h>

#include <nav/nav_DataBase.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

Icons::Icons( const Module *parent ) :
    Module( parent )
{
    _symbolAerodrome    = Models::get( "map/icons/aerodrome.stl" );
    _symbolNavaidVOR    = Models::get( "map/icons/vor.stl" );
    _symbolNavaidVORTAC = Models::get( "map/icons/vortac.stl" );
    _symbolNavaidVORDME = Models::get( "map/icons/vor_dme.stl" );
    _symbolNavaidILSLOC = createSymbolILS();

    osg::ref_ptr<osg::Material> material = new osg::Material();

    material->setColorMode( osg::Material::AMBIENT_AND_DIFFUSE );
    material->setAmbient( osg::Material::FRONT, osg::Vec4( Map::_colorAeroData, 1.0f ) );
    material->setDiffuse( osg::Material::FRONT, osg::Vec4( Map::_colorAeroData, 1.0f ) );

    _symbolAerodrome    ->getOrCreateStateSet()->setAttribute( material.get() );
    _symbolNavaidVOR    ->getOrCreateStateSet()->setAttribute( material.get() );
    _symbolNavaidVORTAC ->getOrCreateStateSet()->setAttribute( material.get() );
    _symbolNavaidVORDME ->getOrCreateStateSet()->setAttribute( material.get() );

    createOwnship();

    initAerodromes();
    initLocalizers();
    initNavaids();
    initRunways();

    setScale( 1.0 );
}

////////////////////////////////////////////////////////////////////////////////

Icons::~Icons() {}

////////////////////////////////////////////////////////////////////////////////

void Icons::update()
{
    updateOwnship();
}

////////////////////////////////////////////////////////////////////////////////

void Icons::setScale( double scale )
{
    double s = 1.5 * 1.0e6 * scale;
    osg::Vec3d sv( s, s, 1.0 );

    _ownship.pat->setScale( sv );

    for ( PATs::iterator it = _aerodromes.begin(); it != _aerodromes.end(); ++it )
    {
        (*it)->setScale( sv );
    }

    for ( PATs::iterator it = _navaids.begin(); it != _navaids.end(); ++it )
    {
        (*it)->setScale( sv );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Icons::createIcon( osg::Group *parent, float z, const char *textureFile )
{
    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    parent->addChild( geode.get() );

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
    geode->addDrawable( geometry.get() );

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // vertices

    v->push_back( osg::Vec3( -0.5f, -0.5f, z ) );
    v->push_back( osg::Vec3(  0.5f, -0.5f, z ) );
    v->push_back( osg::Vec3(  0.5f,  0.5f, z ) );
    v->push_back( osg::Vec3( -0.5f,  0.5f, z ) );

    Geometry::createQuad( geometry.get(), v.get(), true, true );

    osg::ref_ptr<osg::StateSet> stateSet = geode->getOrCreateStateSet();
    stateSet->setMode( GL_LIGHTING, osg::StateAttribute::OFF );

    // texture
    osg::ref_ptr<osg::Texture2D> texture = Textures::get( textureFile );

    if ( texture.valid() )
    {
        stateSet->setMode( GL_BLEND, osg::StateAttribute::ON );
        stateSet->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
        stateSet->setTextureAttributeAndModes( 0, texture, osg::StateAttribute::ON );
    }

    // material
    osg::ref_ptr<osg::Material> material = new osg::Material();

    material->setColorMode( osg::Material::AMBIENT_AND_DIFFUSE );
    material->setAmbient( osg::Material::FRONT, osg::Vec4( 1.0f, 1.0f, 1.0f, 1.0f ) );
    material->setDiffuse( osg::Material::FRONT, osg::Vec4( 1.0f, 1.0f, 1.0f, 1.0f ) );

    stateSet->setAttribute( material.get() );
}

////////////////////////////////////////////////////////////////////////////////

void Icons::createOwnship()
{
    _ownship.pat = new osg::PositionAttitudeTransform();
    _root->addChild( _ownship.pat.get() );

    _ownship.speedLeader = new osg::Group();
    _ownship.pat->addChild( _ownship.speedLeader.get() );

    createIcon( _ownship.pat.get(), Map::_zOwnship, "map/icons/air_friend.png" );
}

////////////////////////////////////////////////////////////////////////////////

osg::Node* Icons::createRunway( double length, double width )
{
    osg::ref_ptr<osg::Geode> geode = new osg::Geode();

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();
    osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();

    c->push_back( osg::Vec4( Map::_colorAirports, 1.0f ) );
    n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );

    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();

    geode->addDrawable( geom.get() );

    v->push_back( osg::Vec3( -width / 2.0, -length / 2.0, 0.0 ) );
    v->push_back( osg::Vec3(  width / 2.0, -length / 2.0, 0.0 ) );
    v->push_back( osg::Vec3(  width / 2.0,  length / 2.0, 0.0 ) );
    v->push_back( osg::Vec3( -width / 2.0,  length / 2.0, 0.0 ) );

    geom->setVertexArray( v.get() );
    geom->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::QUADS, 0, v->size() ) );
    geom->setNormalArray( n.get() );
    geom->setNormalBinding( osg::Geometry::BIND_OVERALL );
    geom->setColorArray( c.get() );
    geom->setColorBinding( osg::Geometry::BIND_OVERALL );

    return geode.release();
}

////////////////////////////////////////////////////////////////////////////////

osg::Node* Icons::createSymbolILS()
{
    osg::ref_ptr<osg::Geode> geode = new osg::Geode();

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();
    osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();

    c->push_back( osg::Vec4( Map::_colorAeroData, 1.0f ) );
    n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );

    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();

    geode->addDrawable( geom.get() );

    v->push_back( osg::Vec3(    0.0f,      0.0f, 0.0f ) );
    v->push_back( osg::Vec3(    0.0f, -10500.0f, 0.0f ) );
    v->push_back( osg::Vec3(  600.0f, -11000.0f, 0.0f ) );
    v->push_back( osg::Vec3(    0.0f,      0.0f, 0.0f ) );
    v->push_back( osg::Vec3( -600.0f, -11000.0f, 0.0f ) );
    v->push_back( osg::Vec3(    0.0f, -10500.0f, 0.0f ) );

    geom->setVertexArray( v.get() );
    geom->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINE_STRIP, 0, v->size() ) );
    geom->setNormalArray( n.get() );
    geom->setNormalBinding( osg::Geometry::BIND_OVERALL );
    geom->setColorArray( c.get() );
    geom->setColorBinding( osg::Geometry::BIND_OVERALL );

    return geode.release();
}

////////////////////////////////////////////////////////////////////////////////

osg::Node* Icons::getNavaidSymbol( int type )
{
    switch ( type )
    {
        case nav::DataBase::NAV::VOR:     return _symbolNavaidVOR.get();
        case nav::DataBase::NAV::VORTAC:  return _symbolNavaidVORTAC.get();
//        case nav::DataBase::NAV::TACAN:   return _symbolNavaidTACAN.get();
        case nav::DataBase::NAV::VOR_DME: return _symbolNavaidVORDME.get();
//        case nav::DataBase::NAV::NDB:     return _symbolNavaidNDB.get();
//        case nav::DataBase::NAV::NDB_DME: return _symbolNavaidNDB_DME.get();
//        case nav::DataBase::NAV::LOCATOR: return _symbolNavaidLOCATOR.get();
//        case nav::DataBase::NAV::DME:     return _symbolNavaidDME.get();
    }

    return _symbolNavaidVOR.get();;
}

////////////////////////////////////////////////////////////////////////////////

void Icons::initAerodromes()
{
    nav::DataBase::ListAPT list = nav::DataBase::instance()->getListAPT();

    for ( nav::DataBase::ListAPT::iterator it = list.begin(); it != list.end(); ++it )
    {
        osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform();
        _root->addChild( pat.get() );
        pat->addChild( _symbolAerodrome.get() );
        pat->setPosition( osg::Vec3d(  Mercator::x( (*it).lon ),
                                       Mercator::y( (*it).lat ),
                                       Map::_zAerodromes ) );
        _aerodromes.push_back( pat.get() );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Icons::initLocalizers()
{
    nav::DataBase::ListILS listILS = nav::DataBase::instance()->getListILS();

    for ( nav::DataBase::ListILS::iterator it = listILS.begin(); it != listILS.end(); ++it )
    {
        if ( (*it).type == nav::DataBase::ILS::LOC )
        {
            osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform();
            _root->addChild( pat.get() );
            pat->addChild( _symbolNavaidILSLOC.get() );
            pat->setPosition( osg::Vec3d(  Mercator::x( (*it).lon ),
                                           Mercator::y( (*it).lat ),
                                           Map::_zNavaids ) );
            pat->setAttitude( osg::Quat( -(*it).true_bearing, osg::Z_AXIS ) );
            _localizers.push_back( pat.get() );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Icons::initNavaids()
{
    nav::DataBase::ListNAV listNAV = nav::DataBase::instance()->getListNAV();

    for ( nav::DataBase::ListNAV::iterator it = listNAV.begin(); it != listNAV.end(); ++it )
    {
        osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform();
        _root->addChild( pat.get() );
        pat->addChild( getNavaidSymbol( (*it).type ) );
        pat->setPosition( osg::Vec3d(  Mercator::x( (*it).lon ),
                                       Mercator::y( (*it).lat ),
                                       Map::_zNavaids ) );
        _navaids.push_back( pat.get() );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Icons::initRunways()
{
    nav::DataBase::ListRWY listRWY = nav::DataBase::instance()->getListRWY();

    for ( nav::DataBase::ListRWY::iterator it = listRWY.begin(); it != listRWY.end(); ++it )
    {
        osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform();
        _root->addChild( pat.get() );
        pat->addChild( createRunway( (*it).length, (*it).width ) );

        double he_x = Mercator::x( (*it).he_lon );
        double he_y = Mercator::y( (*it).he_lat );
        double le_x = Mercator::x( (*it).le_lon );
        double le_y = Mercator::y( (*it).le_lat );

        fdm::Geo he_pos_geo;
        fdm::Geo le_pos_geo;

        he_pos_geo.lat = (*it).he_lat;
        he_pos_geo.lon = (*it).he_lon;
        he_pos_geo.alt = (*it).he_elev;

        le_pos_geo.lat = (*it).le_lat;
        le_pos_geo.lon = (*it).le_lon;
        le_pos_geo.alt = (*it).le_elev;

        fdm::Vector3 he_pos_wgs = fdm::WGS84::geo2wgs( he_pos_geo );
        fdm::Vector3 le_pos_wgs = fdm::WGS84::geo2wgs( le_pos_geo );

        fdm::WGS84 av_pos_wgs( 0.5 * ( he_pos_wgs + le_pos_wgs ) );

        fdm::Vector3 he_pos_ned = av_pos_wgs.getWGS2NED() * ( he_pos_wgs - av_pos_wgs.getPos_WGS() );
        fdm::Vector3 le_pos_ned = av_pos_wgs.getWGS2NED() * ( le_pos_wgs - av_pos_wgs.getPos_WGS() );

        fdm::Vector3 v_delta_ned = he_pos_ned - le_pos_ned;

        double heading = atan2( v_delta_ned.v(), v_delta_ned.u() );

        pat->setPosition( osg::Vec3d( 0.5 * ( he_x + le_x ),
                                      0.5 * ( he_y + le_y ),
                                      Map::_zAirports ) );
        pat->setAttitude( osg::Quat( -heading, osg::Z_AXIS ) );
        _runways.push_back( pat.get() );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Icons::updateOwnship()
{
    _ownship.pat->setPosition( osg::Vec3( Mercator::x( Data::get()->ownship.longitude ),
                                          Mercator::y( Data::get()->ownship.latitude ),
                                          0.0f ) );

    if ( _ownship.speedLeader->getNumChildren() > 0 )
    {
        _ownship.speedLeader->removeChildren( 0, _ownship.speedLeader->getNumChildren() );
    }

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    _ownship.speedLeader->addChild( geode.get() );

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();  // colors
    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();

    n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );
    c->push_back( osg::Vec4( 0.0f, 0.0f, 0.0f, 1.0f ) );

    const float coef = 1.0f / 50.0f; // 180 km/h
    osg::Vec3 vel_ned( coef * Data::get()->ownship.vel_east,
                       coef * Data::get()->ownship.vel_north,
                       Map::_zSpeedLeader );

    if ( vel_ned.length2() > 1.0f )
    {
        vel_ned.normalize();
    }

    v->push_back( osg::Vec3( 0.0f, 0.0f, Map::_zSpeedLeader ) );
    v->push_back( vel_ned );

    geometry->setVertexArray( v.get() );
    geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, v->size() ) );
    geometry->setNormalArray( n.get() );
    geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );
    geometry->setColorArray( c.get() );
    geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

    geode->addDrawable( geometry.get() );

    ////////////////////

    osg::ref_ptr<osg::StateSet> stateSet = geode->getOrCreateStateSet();

    osg::ref_ptr<osg::LineWidth> lineWidth = new osg::LineWidth();
    lineWidth->setWidth( 1.25f );

    stateSet->setAttributeAndModes( lineWidth, osg::StateAttribute::ON );
    stateSet->setMode( GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE );
}
