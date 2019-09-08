#include <iostream>

#include <osg/ShapeDrawable>
#include <osg/LightSource>
#include <osg/PositionAttitudeTransform>
#include <osg/Material>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include "SceneRoot.h"

////////////////////////////////////////////////////////////////////////////////

const osg::Vec3d SceneRoot::colorFuselage = osg::Vec3d( 0.27843, 0.34902, 0.24706 );
const osg::Vec3d SceneRoot::colorRotor    = osg::Vec3d( 0.3, 0.3, 0.3 );

////////////////////////////////////////////////////////////////////////////////

using namespace std;

osg::Vec3d SceneRoot::getNormal( const osg::Vec3d &r0,
                                 const osg::Vec3d &r1,
                                 const osg::Vec3d &r2 )
{
    osg::Vec3d r01 = r1 - r0;
    osg::Vec3d r02 = r2 - r0;

    return r01 ^ r02;
}

////////////////////////////////////////////////////////////////////////////////

SceneRoot::SceneRoot()
{
    //std::cout << "SceneRoot::create()" << std::endl;

    _root = new osg::Group();
    _root->setName( "SceneRoot" );

    osg::ref_ptr<osg::StateSet> rootStateSet = _root->getOrCreateStateSet();

    rootStateSet->setMode( GL_RESCALE_NORMAL , osg::StateAttribute::ON );
    rootStateSet->setMode( GL_LIGHT0         , osg::StateAttribute::ON );
    rootStateSet->setMode( GL_LIGHT1         , osg::StateAttribute::ON );
    rootStateSet->setMode( GL_LIGHTING       , osg::StateAttribute::ON );
//    rootStateSet->setMode( GL_BLEND          , osg::StateAttribute::ON );
//    rootStateSet->setMode( GL_ALPHA_TEST     , osg::StateAttribute::ON );
//    rootStateSet->setMode( GL_DEPTH_TEST     , osg::StateAttribute::ON );
//    rootStateSet->setRenderBinDetails( 1, "DepthSortedBin" );

//    osg::ref_ptr<osg::Node> glider = osgDB::readNodeFile( "cessna.osgt" );

//    if ( glider.valid() )
//    {
//        m_root->addChild( glider.get() );
//    }

    createHelicopter();
    createSceneLight();

    osg::ref_ptr<osg::PositionAttitudeTransform> patRotor = new osg::PositionAttitudeTransform();
    _root->addChild( patRotor );
    patRotor->setPosition( osg::Vec3d( 0.0, 0.0, 1.0 ) );
    drawRotor( patRotor.get(), 0.0, 2.5, osg::Vec4( 0.5, 0.5, 0.5, 0.5 ) );

    _rotor = new osg::PositionAttitudeTransform();
    patRotor->addChild( _rotor );
    drawRotor( _rotor.get(), 0.2, 2.5, osg::Vec4( colorRotor, 0.9 ) );
}

////////////////////////////////////////////////////////////////////////////////

SceneRoot::~SceneRoot() {}

////////////////////////////////////////////////////////////////////////////////

void SceneRoot::update( const double coning, const double roll, const double pitch )
{
    if ( _rotor->getNumChildren() > 0 )
    {
        _rotor->removeChildren( 0, _rotor->getNumChildren() );
    }

    drawRotor( _rotor.get(), coning, 2.5, osg::Vec4( colorRotor, 0.9 ) );

    _rotor->setAttitude( osg::Quat( -pitch, osg::Y_AXIS, roll, osg::X_AXIS, 0.0, osg::Z_AXIS ) );
}

////////////////////////////////////////////////////////////////////////////////

void SceneRoot::createHelicopter()
{
    osg::Vec3d nose(  2.0, 0.0, 0.0  );
    osg::Vec3d cs_1(  1.9, 0.0, 0.0  );  double ry_1 = 0.3;  double rz_1 = 0.15;
    osg::Vec3d cs_2(  1.8, 0.0, 0.0  );  double ry_2 = 0.45; double rz_2 = 0.25;
    osg::Vec3d cs_3(  1.7, 0.0, 0.05 );  double ry_3 = 0.5;  double rz_3 = 0.3;
    osg::Vec3d cs_4(  0.8, 0.0, 0.3  );  double ry_4 = 0.7;  double rz_4 = 0.6;
    osg::Vec3d cs_5(  0.0, 0.0, 0.3  );  double ry_5 = 0.72; double rz_5 = 0.61;
    osg::Vec3d cs_6( -0.6, 0.0, 0.3  );  double ry_6 = 0.72; double rz_6 = 0.5;
    osg::Vec3d cs_7( -1.6, 0.0, 0.4  );  double ry_7 = 0.25; double rz_7 = 0.25;
    osg::Vec3d cs_8( -3.1, 0.0, 0.5  );  double ry_8 = 0.1;  double rz_8 = 0.1;
    osg::Vec3d tail( -3.2, 0.0, 0.5  );

    drawStartSection( nose, cs_1, ry_1, rz_1 );
    drawCrossSection( cs_1, cs_2, ry_1, rz_1, ry_2, rz_2 );
    drawCrossSection( cs_2, cs_3, ry_2, rz_2, ry_3, rz_3 );
    drawCrossSection( cs_3, cs_4, ry_3, rz_3, ry_4, rz_4 );
    drawCrossSection( cs_4, cs_5, ry_4, rz_4, ry_5, rz_5 );
    drawCrossSection( cs_5, cs_6, ry_5, rz_5, ry_6, rz_6 );
    drawCrossSection( cs_6, cs_7, ry_6, rz_6, ry_7, rz_7 );
    drawCrossSection( cs_7, cs_8, ry_7, rz_7, ry_8, rz_8 );
}

////////////////////////////////////////////////////////////////////////////////

void SceneRoot::createSceneLight()
{
    osg::ref_ptr<osg::PositionAttitudeTransform> patSun = new osg::PositionAttitudeTransform();
    _root->addChild( patSun.get() );

    patSun->setAttitude( osg::Quat( M_PI_2 - M_PI_4, osg::Z_AXIS, -M_PI_4, osg::Y_AXIS, 0.0, osg::X_AXIS ) );

    osg::ref_ptr<osg::LightSource> lightSourceSun = new osg::LightSource();
    patSun->addChild( lightSourceSun.get() );

    osg::ref_ptr<osg::Light> lightSun = new osg::Light();

    lightSun->setLightNum( 0 );

    double x = 1000000.0;
    lightSun->setPosition( osg::Vec4d( x, 0.0, 0.0, 0.0 ) );

    osg::Vec4 lightColor( 255.0/255.0, 255.0/255.0, 250.0/255.0, 1.0 );

    lightSun->setAmbient( lightColor );
    lightSun->setDiffuse( lightColor );
    lightSun->setSpecular( lightColor );

    lightSun->setConstantAttenuation( 1.0 );

    lightSourceSun->setLight( lightSun.get() );

    lightSourceSun->setLocalStateSetModes( osg::StateAttribute::ON );
    lightSourceSun->setStateSetModes( *_root->getOrCreateStateSet(), osg::StateAttribute::ON );
}

////////////////////////////////////////////////////////////////////////////////

void SceneRoot::drawStartSection( const osg::Vec3d &p0, const osg::Vec3d &p1,
                                  const double ry, const double rz,
                                  const int nodes )
{
    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    _root->addChild( geode.get() );

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // vertices
    osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();  // colors

    osg::Vec3d r1;
    osg::Vec3d r2;

    osg::Vec3d norm;

    r1.x() = p1.x();
    r1.y() = ry + p1.y();
    r1.z() = 0.0;

    r2.x() = p1.x();
    r2.y() = ry * cos( ( 2.0*M_PI ) / (double)nodes ) + p1.y();
    r2.z() = rz * sin( ( 2.0*M_PI ) / (double)nodes ) + p1.z();

    norm = getNormal( p0, r1, r2 );

    v->push_back( p0 );
    v->push_back( r1 );
    v->push_back( r2 );
    n->push_back( norm );
    n->push_back( norm );
    n->push_back( norm );

    for ( int i = 1; i <= nodes + 1; i++ )
    {
        double tht = (double)i * ( ( 2.0*M_PI ) / (double)nodes );
        double cosTht = cos( tht );
        double sinTht = sin( tht );

        r2.x() = p1.x();
        r2.y() = ry * cosTht + p1.y();
        r2.z() = rz * sinTht + p1.z();

        norm = getNormal( p0, r1, r2 );

        v->push_back( p0 );
        v->push_back( r1 );
        v->push_back( r2 );
        n->push_back( norm );
        n->push_back( norm );
        n->push_back( norm );

        r1 = r2;
    }

    c->push_back( osg::Vec4( colorFuselage, 1.0 ) );

    geometry->setVertexArray( v.get() );
    geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::TRIANGLE_FAN, 0, v->size() ) );

    geometry->setNormalArray( n.get() );
    geometry->setNormalBinding( osg::Geometry::BIND_PER_VERTEX );

    geometry->setColorArray( c.get() );
    geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

    geode->addDrawable( geometry.get() );
}

////////////////////////////////////////////////////////////////////////////////

void SceneRoot::drawCrossSection( const osg::Vec3d &p0, const osg::Vec3d &p1,
                                  const double ry_1, const double rz_1,
                                  const double ry_2, const double rz_2,
                                  const int nodes )
{
    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    _root->addChild( geode.get() );

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // vertices
    osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();  // colors

    double tht = 0.0;
    double cosTht = cos( tht );
    double sinTht = sin( tht );

    osg::Vec3d r_10;
    osg::Vec3d r_11;
    osg::Vec3d r_20;
    osg::Vec3d r_21;

    osg::Vec3d norm;

    r_10.x() = p0.x();
    r_10.y() = ry_1 * cosTht + p0.y();
    r_10.z() = rz_1 * sinTht + p0.z();

    r_11.x() = p1.x();
    r_11.y() = ry_2 * cosTht + p1.y();
    r_11.z() = rz_2 * sinTht + p1.z();

    norm = getNormal( r_10, r_11, r_20 );

    v->push_back( r_10 );
    v->push_back( r_11 );
    v->push_back( r_21 );
    v->push_back( r_20 );

    n->push_back( norm );
    n->push_back( norm );
    n->push_back( norm );
    n->push_back( norm );

    for ( int i = 1; i <= nodes; i++ )
    {
        tht = (double)i * ( ( 2.0*M_PI ) / (double)nodes );
        cosTht = cos( tht );
        sinTht = sin( tht );

        r_20.x() = p0.x();
        r_20.y() = ry_1 * cosTht + p0.y();
        r_20.z() = rz_1 * sinTht + p0.z();

        r_21.x() = p1.x();
        r_21.y() = ry_2 * cosTht + p1.y();
        r_21.z() = rz_2 * sinTht + p1.z();

        norm = getNormal( r_10, r_11, r_20 );

        v->push_back( r_10 );
        v->push_back( r_11 );
        v->push_back( r_21 );
        v->push_back( r_20 );

        n->push_back( norm );
        n->push_back( norm );
        n->push_back( norm );
        n->push_back( norm );

        r_10 = r_20;
        r_11 = r_21;
    }

    c->push_back( osg::Vec4( colorFuselage, 1.0 ) );

    geometry->setVertexArray( v.get() );
    geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::QUADS, 0, v->size() ) );

    geometry->setNormalArray( n.get() );
    geometry->setNormalBinding( osg::Geometry::BIND_PER_VERTEX );

    geometry->setColorArray( c.get() );
    geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

    geode->addDrawable( geometry.get() );
}

////////////////////////////////////////////////////////////////////////////////

void SceneRoot::drawRotor( osg::Group *parent, const double coning,
                           const double r, const osg::Vec4d &color,
                           const int nodes )
{
    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    parent->addChild( geode.get() );

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // vertices
    osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();  // colors

    double beta = coning;// + cosPsi * pitch + sinPsi * roll;

    double effective_r = cos( beta ) * r;
    double z = sin( beta ) * r;

    osg::Vec3d r1;
    osg::Vec3d r2;

    r2.x() = effective_r;
    r2.y() = 0.0;
    r2.z() = z;

    v->push_back( osg::Vec3d() );
    v->push_back( r1 );
    v->push_back( r2 );
    n->push_back( getNormal( osg::Vec3d(), r1, r2 ) );
    n->push_back( getNormal( osg::Vec3d(), r1, r2 ) );
    n->push_back( getNormal( osg::Vec3d(), r1, r2 ) );

    for ( int i = 1; i <= nodes + 1; i++ )
    {
        double psi = (double)i * ( ( 2.0*M_PI ) / (double)nodes );
        double cosPsi = cos( psi );
        double sinPsi = sin( psi );

        beta = coning;// + cosPsi * pitch + sinPsi * roll;

        effective_r = cos( beta ) * r;
        z = sin( beta ) * r;

        r2.x() = effective_r * cosPsi;
        r2.y() = effective_r * sinPsi;
        r2.z() = z;

        v->push_back( osg::Vec3d( 0.0, 0.0, 0.0 ) );
        v->push_back( r1 );
        v->push_back( r2 );
        n->push_back( getNormal( osg::Vec3d(), r1, r2 ) );
        n->push_back( getNormal( osg::Vec3d(), r1, r2 ) );
        n->push_back( getNormal( osg::Vec3d(), r1, r2 ) );

        r1 = r2;
    }

    c->push_back( color );

    geometry->setVertexArray( v.get() );
    geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::TRIANGLES, 0, v->size() ) );

    geometry->setNormalArray( n.get() );
    geometry->setNormalBinding( osg::Geometry::BIND_PER_VERTEX );

    geometry->setColorArray( c.get() );
    geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

    geode->addDrawable( geometry.get() );

    osg::ref_ptr<osg::StateSet> stateSet = geometry->getOrCreateStateSet();

    //stateSet->setAttribute( material );
    stateSet->setMode( GL_BLEND, osg::StateAttribute::ON );
    stateSet->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
    stateSet->setRenderBinDetails( 11, "RenderBin" );
}
