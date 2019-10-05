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

#include <cgi/hud/cgi_HUD.h>

#include <sstream>

#include <osg/Drawable>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Light>
#include <osg/LineWidth>
#include <osg/Material>
#include <osg/PositionAttitudeTransform>
#include <osg/Switch>

#include <Data.h>

#include <cgi/cgi_Color.h>
#include <cgi/cgi_Defines.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

const float HUD::_charSize  = 5.0f;
const float HUD::_angleCoef = CGI_HUD_Y / CGI_FOV_Y;
const float HUD::_rollLimit = osg::DegreesToRadians( 75.0 );

////////////////////////////////////////////////////////////////////////////////

HUD::HUD() :
    _color( Color::_hud, 1.0f )
{
    _root = new osg::Group();

    _switch = new osg::Switch();
    _root->addChild( _switch.get() );

    createWaterLine();

    createRollIndicator();
    createPitchLadder();
    createHeadingScale();

    createTextL();
    createTextR();

    createFPM();
    createILS();

    createStall();

    osg::ref_ptr<osg::StateSet> stateSet = _root->getOrCreateStateSet();

    stateSet->setMode( GL_RESCALE_NORMAL , osg::StateAttribute::ON  );
    stateSet->setMode( GL_LIGHTING       , osg::StateAttribute::OFF );
    stateSet->setMode( GL_LIGHT0         , osg::StateAttribute::ON  );
    stateSet->setMode( GL_BLEND          , osg::StateAttribute::ON  );
    stateSet->setMode( GL_ALPHA_TEST     , osg::StateAttribute::ON  );
    stateSet->setMode( GL_DEPTH_TEST     , osg::StateAttribute::ON  );
    stateSet->setMode( GL_DITHER         , osg::StateAttribute::OFF );
    stateSet->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
    stateSet->setRenderBinDetails( CGI_DEPTH_SORTED_BIN_HUD, "RenderBin" );

    // material
    _material = new osg::Material();
    _material->setColorMode( osg::Material::AMBIENT_AND_DIFFUSE );
    _material->setAmbient( osg::Material::FRONT, _color );
    _material->setDiffuse( osg::Material::FRONT, _color );

    stateSet->setAttribute( _material.get(), osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON );

    // lines
    osg::ref_ptr<osg::LineWidth> lineWidth = new osg::LineWidth();
    lineWidth->setWidth( 2.0f );

    stateSet->setAttributeAndModes( lineWidth, osg::StateAttribute::ON );
}

////////////////////////////////////////////////////////////////////////////////

HUD::~HUD() {}

////////////////////////////////////////////////////////////////////////////////

void HUD::update()
{
    if ( Data::get()->cgi.hud.enabled )
    {
        _switch->setAllChildrenOn();

        _sideslipAngleFPM_deg = osg::RadiansToDegrees( Data::get()->cgi.hud.sideslipAngle );
        _angleOfAttackFPM_deg = osg::RadiansToDegrees( Data::get()->cgi.hud.angleOfAttack );

        _validFPM = true;

        if ( fabs( _sideslipAngleFPM_deg ) > 8.0f )
        {
            _validFPM = false;
            _sideslipAngleFPM_deg = ( _sideslipAngleFPM_deg < 0.0f ) ? -8.0f : 8.0f;
        }

        if ( _angleOfAttackFPM_deg > 10.0f || _angleOfAttackFPM_deg < -10.0f )
        {
            _validFPM = false;
            _angleOfAttackFPM_deg = ( _angleOfAttackFPM_deg < 0.0f ) ? -10.0f : 10.0f;
        }

        if ( Data::get()->cgi.hud.airspeed < 0.5f )
        {
            _validFPM = false;
            _angleOfAttackFPM_deg = osg::RadiansToDegrees( Data::get()->cgi.hud.pitch );
            _sideslipAngleFPM_deg = 0.0f;
        }

        updateRollIndicator();
        updatePitchLadder();
        updateHeadingScale();

        updateFPM();
        updateILS();

        updateStall();

        updateTextL();
        updateTextR();

        // update color
        osg::Vec4 color( Data::get()->cgi.hud.color_r,
                         Data::get()->cgi.hud.color_g,
                         Data::get()->cgi.hud.color_b,
                         Data::get()->cgi.hud.opacity );

        if ( color != _color )
        {
            _color = color;

            for ( std::vector< osg::ref_ptr<osgText::Text> >::iterator it = _texts.begin(); it != _texts.end(); it++ )
            {
                (*it)->setColor( _color );
            }

            _material->setAmbient( osg::Material::FRONT, _color );
            _material->setDiffuse( osg::Material::FRONT, _color );
            //_material->setTransparency( osg::Material::FRONT_AND_BACK, 1.0f - Data::get()->hud.opacity );
        }
    }
    else
    {
        _switch->setAllChildrenOff();
    }
}

////////////////////////////////////////////////////////////////////////////////

void HUD::createWaterLine()
{
    const float w = 20.0f;
    const float h =  3.0f;

    const float d = w / 6.0f;

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    _switch->addChild( geode.get() );

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // vertices
    osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals

    v->push_back( osg::Vec3( -d * 3.0f,  0.0f, -1.0f ) ); // 1
    v->push_back( osg::Vec3( -d * 2.0f,  0.0f, -1.0f ) ); // 2
    v->push_back( osg::Vec3( -d * 1.0f,    -h, -1.0f ) ); // 3
    v->push_back( osg::Vec3(      0.0f,  0.0f, -1.0f ) ); // 4
    v->push_back( osg::Vec3(  d * 1.0f,    -h, -1.0f ) ); // 5
    v->push_back( osg::Vec3(  d * 2.0f,  0.0f, -1.0f ) ); // 6
    v->push_back( osg::Vec3(  d * 3.0f,  0.0f, -1.0f ) ); // 7

    n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );

    geometry->setVertexArray( v.get() );
    geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINE_STRIP, 0, v->size() ) );

    geometry->setNormalArray( n.get() );
    geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );

//    geometry->setColorArray( c.get() );
//    geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

    geode->addDrawable( geometry.get() );
}

////////////////////////////////////////////////////////////////////////////////

void HUD::createRollIndicator()
{
    const float r0 = 80.0f;
    const float r1 = r0 + 2.5f;
    const float r2 = r0 + 5.0f;

    osg::ref_ptr<osg::PositionAttitudeTransform> patTemp = new osg::PositionAttitudeTransform();
    _switch->addChild( patTemp.get() );

    patTemp->setPosition( osg::Vec3( 0.0f, 0.0f, 0.0f ) );

    _switchRollIndicator = new osg::Switch();
    patTemp->addChild( _switchRollIndicator.get() );

    // Scale
    {
        osg::ref_ptr<osg::Geode> geode = new osg::Geode();
        patTemp->addChild( geode.get() );

        osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

        osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // vertices
        osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals

        float a = 0.0;

        float x0 = 0.0f;
        float x1 = 0.0f;
        float x2 = 0.0f;

        float y0 = 0.0f;
        float y1 = 0.0f;
        float y2 = 0.0f;

        // -60
        a = osg::DegreesToRadians( -60.0f );

        x0 = r0 * sin( a );
        x1 = r1 * sin( a );

        y0 = r0 * cos( a );
        y1 = r1 * cos( a );

        v->push_back( osg::Vec3( -x0, y0, -1.0f ) );
        v->push_back( osg::Vec3( -x1, y1, -1.0f ) );

        v->push_back( osg::Vec3(  x0, y0, -1.0f ) );
        v->push_back( osg::Vec3(  x1, y1, -1.0f ) );

        // -45
        a = osg::DegreesToRadians( -45.0f );

        x0 = r0 * sin( a );
        x1 = r1 * sin( a );

        y0 = r0 * cos( a );
        y1 = r1 * cos( a );

        v->push_back( osg::Vec3( -x0, y0, -1.0f ) );
        v->push_back( osg::Vec3( -x1, y1, -1.0f ) );

        v->push_back( osg::Vec3(  x0, y0, -1.0f ) );
        v->push_back( osg::Vec3(  x1, y1, -1.0f ) );

        // -30
        a = osg::DegreesToRadians( -30.0f );

        x0 = r0 * sin( a );
        x2 = r2 * sin( a );

        y0 = r0 * cos( a );
        y2 = r2 * cos( a );

        v->push_back( osg::Vec3( -x0, y0, -1.0f ) );
        v->push_back( osg::Vec3( -x2, y2, -1.0f ) );

        v->push_back( osg::Vec3(  x0, y0, -1.0f ) );
        v->push_back( osg::Vec3(  x2, y2, -1.0f ) );

        // -20
        a = osg::DegreesToRadians( -20.0f );

        x0 = r0 * sin( a );
        x1 = r1 * sin( a );

        y0 = r0 * cos( a );
        y1 = r1 * cos( a );

        v->push_back( osg::Vec3( -x0, y0, -1.0f ) );
        v->push_back( osg::Vec3( -x1, y1, -1.0f ) );

        v->push_back( osg::Vec3(  x0, y0, -1.0f ) );
        v->push_back( osg::Vec3(  x1, y1, -1.0f ) );

        // -10
        a = osg::DegreesToRadians( -10.0f );

        x0 = r0 * sin( a );
        x1 = r1 * sin( a );

        y0 = r0 * cos( a );
        y1 = r1 * cos( a );

        v->push_back( osg::Vec3( -x0, y0, -1.0f ) );
        v->push_back( osg::Vec3( -x1, y1, -1.0f ) );

        v->push_back( osg::Vec3(  x0, y0, -1.0f ) );
        v->push_back( osg::Vec3(  x1, y1, -1.0f ) );

        // 0
        v->push_back( osg::Vec3(  0.0f, r0, -1.0f ) );
        v->push_back( osg::Vec3( -2.0f, r0 + 5.0f, -1.0f ) );
        v->push_back( osg::Vec3( -2.0f, r0 + 5.0f, -1.0f ) );
        v->push_back( osg::Vec3(  2.0f, r0 + 5.0f, -1.0f ) );
        v->push_back( osg::Vec3(  2.0f, r0 + 5.0f, -1.0f ) );
        v->push_back( osg::Vec3(  0.0f, r0, -1.0f ) );

        n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );

        geometry->setVertexArray( v.get() );
        geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, v->size() ) );

        geometry->setNormalArray( n.get() );
        geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );

        geode->addDrawable( geometry.get() );
    }

    // Marker
    {
        _patRollIndicator = new osg::PositionAttitudeTransform();
        _switchRollIndicator->addChild( _patRollIndicator.get() );

        osg::ref_ptr<osg::Geode> geode = new osg::Geode();
        _patRollIndicator->addChild( geode.get() );

        osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

        osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // vertices
        osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals

        v->push_back( osg::Vec3(  0.0f, r0, -1.0f ) );
        v->push_back( osg::Vec3( -2.0f, r0 - 5.0f, -1.0f ) );
        v->push_back( osg::Vec3(  2.0f, r0 - 5.0f, -1.0f ) );

        n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );

        geometry->setVertexArray( v.get() );
        geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINE_LOOP, 0, v->size() ) );

        geometry->setNormalArray( n.get() );
        geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );

        geode->addDrawable( geometry.get() );
    }
}

////////////////////////////////////////////////////////////////////////////////

void HUD::createPitchLadder()
{
    _patPitchLadderRoll = new osg::PositionAttitudeTransform();
    _switch->addChild( _patPitchLadderRoll.get() );

    _patPitchLadderPitch = new osg::PositionAttitudeTransform();
    _patPitchLadderRoll->addChild( _patPitchLadderPitch.get() );

    _patPitchLadderSlip = new osg::PositionAttitudeTransform();
    _patPitchLadderPitch->addChild( _patPitchLadderSlip.get() );

    _switchPitchLadder = new osg::Switch();
    _patPitchLadderSlip->addChild( _switchPitchLadder.get() );

    for ( int i = -90; i < 0; i++ )
    {
        if ( i % 5 == 0 )
        {
            osg::ref_ptr<osg::Geode> geode = new osg::Geode();
            _switchPitchLadder->addChild( geode.get() );

            createPitchLadderBar( geode.get(), ((float)i)*_angleCoef, i );
        }
    }

    // Hozrizon Line
    {
        const float w = 120.0f;
        const float g =  20.0f;

        osg::ref_ptr<osg::Geode> geode = new osg::Geode();
        _switchPitchLadder->addChild( geode.get() );

        osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

        osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // vertices
        osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals

        v->push_back( osg::Vec3( -w / 2.0f, 0.0f, -1.0f ) );
        v->push_back( osg::Vec3( -g / 2.0f, 0.0f, -1.0f ) );

        v->push_back( osg::Vec3(  w / 2.0f, 0.0f, -1.0f ) );
        v->push_back( osg::Vec3(  g / 2.0f, 0.0f, -1.0f ) );

        n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );

        geometry->setVertexArray( v.get() );
        geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, v->size() ) );

        geometry->setNormalArray( n.get() );
        geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );

        geode->addDrawable( geometry.get() );
    }

    for ( int i = 1; i <= 90; i++ )
    {
        if ( i % 5 == 0 )
        {
            osg::ref_ptr<osg::Geode> geode = new osg::Geode();
            _switchPitchLadder->addChild( geode.get() );

            createPitchLadderBar( geode.get(), ((float)i)*_angleCoef, i );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void HUD::createHeadingScale()
{
    const float y_del = -80.0f;

    const float scale_w = 100.0f;

    const float box_w = 12.0f;
    const float box_h =  7.5f;

    const float px_deg = scale_w / 50.0;

    _patHeadingScale = new osg::PositionAttitudeTransform();
    _switch->addChild( _patHeadingScale.get() );

    _switchHeadingScale = new osg::Switch();
    _patHeadingScale->addChild( _switchHeadingScale.get() );

    // Box
    {
        osg::ref_ptr<osg::Geode> geode = new osg::Geode();
        _switch->addChild( geode.get() );

        osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

        osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // vertices
        osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals

        // marker
        v->push_back( osg::Vec3( -box_w / 6.0f, y_del + box_h * 0.5f, -1.0f ) );
        v->push_back( osg::Vec3(          0.0f, y_del               , -1.0f ) );
        v->push_back( osg::Vec3(  box_w / 6.0f, y_del + box_h * 0.5f, -1.0f ) );

        // box
        v->push_back( osg::Vec3(  box_w / 2.0f, y_del + box_h * 0.5f, -1.0f ) );
        v->push_back( osg::Vec3(  box_w / 2.0f, y_del + box_h * 1.5f, -1.0f ) );
        v->push_back( osg::Vec3( -box_w / 2.0f, y_del + box_h * 1.5f, -1.0f ) );
        v->push_back( osg::Vec3( -box_w / 2.0f, y_del + box_h * 0.5f, -1.0f ) );

        n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );

        geometry->setVertexArray( v.get() );
        geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINE_LOOP, 0, v->size() ) );

        geometry->setNormalArray( n.get() );
        geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );

        geode->addDrawable( geometry.get() );
    }

    // Text Heading
    {
        _heading = new osg::Geode();
        _switch->addChild( _heading.get() );

        osg::ref_ptr<osgText::Text> text = new osgText::Text();
        _texts.push_back( text );
        //text->setFont(font);
        text->setColor( _color );
        text->setCharacterSize( _charSize );
        text->setAxisAlignment( osgText::TextBase::XY_PLANE );
        text->setPosition( osg::Vec3( 0.0f, y_del + box_h * 0.5f + 1.5f, -1.0f ) );
        text->setLayout( osgText::Text::LEFT_TO_RIGHT );
        text->setAlignment( osgText::Text::CENTER_BASE_LINE );
        text->setText( "360" );
        _heading->addDrawable( text );
    }

    // Scale
    {
        osg::ref_ptr<osg::Geode> geode = new osg::Geode();
        _switch->addChild( geode.get() );

        osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

        osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // vertices
        osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals

        // marker
        v->push_back( osg::Vec3( -scale_w / 2.0f, y_del, -1.0f ) );
        v->push_back( osg::Vec3(  scale_w / 2.0f, y_del, -1.0f ) );

        n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );

        geometry->setVertexArray( v.get() );
        geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, v->size() ) );

        geometry->setNormalArray( n.get() );
        geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );

        geode->addDrawable( geometry.get() );

        for ( int i = -360; i <= 360; i++ )
        {
            if ( i % 5 == 0 )
            {
                osg::ref_ptr<osg::Geode> geode = new osg::Geode();
                _switchHeadingScale->addChild( geode.get() );

                if ( i % 10 == 0 )
                {
                    int deg10 = ( i < 0 ) ? ( 360 + i ) / 10 : i / 10;

                    createHeadingScaleBar( geode.get(), y_del, ((float)i)*px_deg, deg10 );
                }
                else
                {
                    createHeadingScaleBar( geode.get(), y_del, ((float)i)*px_deg );
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void HUD::createFPM()
{
    const float r0 = 3.0f;
    const float r1 = 6.0f;

    _patFPM = new osg::PositionAttitudeTransform();
    _switch->addChild( _patFPM.get() );

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    _patFPM->addChild( geode.get() );

    // Lines
    {
        osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

        osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // vertices
        osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals

        v->push_back( osg::Vec3( -r1, 0.0f, -1.0f ) );
        v->push_back( osg::Vec3( -r0, 0.0f, -1.0f ) );

        v->push_back( osg::Vec3(  0.0f, r1, -1.0f ) );
        v->push_back( osg::Vec3(  0.0f, r0, -1.0f ) );

        v->push_back( osg::Vec3(  r1, 0.0f, -1.0f ) );
        v->push_back( osg::Vec3(  r0, 0.0f, -1.0f ) );

        n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );

        geometry->setVertexArray( v.get() );
        geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, v->size() ) );

        geometry->setNormalArray( n.get() );
        geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );

        geode->addDrawable( geometry.get() );
    }

    // Circle
    {
        osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

        osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // vertices
        osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals

        for ( int i = 0; i < 360; i = i + 10 )
        {
            float a = osg::DegreesToRadians( (float)i );

            float x = r0 * sin( a );
            float y = r0 * cos( a );

            v->push_back( osg::Vec3( x, y, -1.0f ) );
        }

        n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );

        geometry->setVertexArray( v.get() );
        geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINE_LOOP, 0, v->size() ) );

        geometry->setNormalArray( n.get() );
        geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );

        geode->addDrawable( geometry.get() );
    }

    // invalid mark
    {
        _switchFPM = new osg::Switch();
        _patFPM->addChild( _switchFPM.get() );

        osg::ref_ptr<osg::Geode> geodeCross = new osg::Geode();
        _switchFPM->addChild( geodeCross.get() );

        osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

        osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // vertices
        osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals

        v->push_back( osg::Vec3( -r0,  r0, -1.0f ) );
        v->push_back( osg::Vec3(  r0, -r0, -1.0f ) );

        v->push_back( osg::Vec3(  r0,  r0, -1.0f ) );
        v->push_back( osg::Vec3( -r0, -r0, -1.0f ) );

        n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );

        geometry->setVertexArray( v.get() );
        geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, v->size() ) );

        geometry->setNormalArray( n.get() );
        geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );

        geodeCross->addDrawable( geometry.get() );
    }
}

////////////////////////////////////////////////////////////////////////////////

void HUD::createILS()
{
    const float bar_w = 40.0f;
    const float mrk_w =  5.0f;

    _patILS = new osg::PositionAttitudeTransform();
    _switch->addChild( _patILS.get() );

    _switchILS = new osg::Switch();
    _patILS->addChild( _switchILS.get() );

    // glide slope
    {
        _patILS_GS = new osg::PositionAttitudeTransform();
        _switchILS->addChild( _patILS_GS.get() );

        osg::ref_ptr<osg::Geode> geode = new osg::Geode();
        _patILS_GS->addChild( geode.get() );

        osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

        osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // vertices
        osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals

        // bar
        v->push_back( osg::Vec3( -bar_w/2.0f, 0.0f, -1.0f ) );
        v->push_back( osg::Vec3(  bar_w/2.0f, 0.0f, -1.0f ) );

        // marking full left
        v->push_back( osg::Vec3( -bar_w/2.0f, -mrk_w/2.0f, -1.0f ) );
        v->push_back( osg::Vec3( -bar_w/2.0f,  mrk_w/2.0f, -1.0f ) );

        // marking half left
        v->push_back( osg::Vec3( -bar_w/4.0f, -mrk_w/2.0f, -1.0f ) );
        v->push_back( osg::Vec3( -bar_w/4.0f,  mrk_w/2.0f, -1.0f ) );

        // marking center
        v->push_back( osg::Vec3(        0.0f, -mrk_w/2.0f, -1.0f ) );
        v->push_back( osg::Vec3(        0.0f,  mrk_w/2.0f, -1.0f ) );

        // marking half right
        v->push_back( osg::Vec3(  bar_w/4.0f, -mrk_w/2.0f, -1.0f ) );
        v->push_back( osg::Vec3(  bar_w/4.0f,  mrk_w/2.0f, -1.0f ) );

        // marking full right
        v->push_back( osg::Vec3(  bar_w/2.0f, -mrk_w/2.0f, -1.0f ) );
        v->push_back( osg::Vec3(  bar_w/2.0f,  mrk_w/2.0f, -1.0f ) );

        n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );

        geometry->setVertexArray( v.get() );
        geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, v->size() ) );

        geometry->setNormalArray( n.get() );
        geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );

        geode->addDrawable( geometry.get() );
    }

    // localizer
    {
        _patILS_LC = new osg::PositionAttitudeTransform();
        _switchILS->addChild( _patILS_LC.get() );

        osg::ref_ptr<osg::Geode> geode = new osg::Geode();
        _patILS_LC->addChild( geode.get() );

        osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

        osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // vertices
        osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals

        // bar
        v->push_back( osg::Vec3( 0.0f, -bar_w/2.0f, -1.0f ) );
        v->push_back( osg::Vec3( 0.0f,  bar_w/2.0f, -1.0f ) );

        // marking full bottom
        v->push_back( osg::Vec3( -mrk_w/2.0f, -bar_w/2.0f, -1.0f ) );
        v->push_back( osg::Vec3(  mrk_w/2.0f, -bar_w/2.0f, -1.0f ) );

        // marking half bottom
        v->push_back( osg::Vec3( -mrk_w/2.0f, -bar_w/4.0f, -1.0f ) );
        v->push_back( osg::Vec3(  mrk_w/2.0f, -bar_w/4.0f, -1.0f ) );

        // marking center
        v->push_back( osg::Vec3( -mrk_w/2.0f,        0.0f, -1.0f ) );
        v->push_back( osg::Vec3(  mrk_w/2.0f,        0.0f, -1.0f ) );

        // marking half top
        v->push_back( osg::Vec3( -mrk_w/2.0f,  bar_w/4.0f, -1.0f ) );
        v->push_back( osg::Vec3(  mrk_w/2.0f,  bar_w/4.0f, -1.0f ) );

        // marking full top
        v->push_back( osg::Vec3( -mrk_w/2.0f,  bar_w/2.0f, -1.0f ) );
        v->push_back( osg::Vec3(  mrk_w/2.0f,  bar_w/2.0f, -1.0f ) );

        n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );

        geometry->setVertexArray( v.get() );
        geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, v->size() ) );

        geometry->setNormalArray( n.get() );
        geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );

        geode->addDrawable( geometry.get() );
    }
}

////////////////////////////////////////////////////////////////////////////////

void HUD::createStall()
{
    _switchStall = new osg::Switch();
    _switch->addChild( _switchStall.get() );

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    _switchStall->addChild( geode.get() );

    osg::ref_ptr<osgText::Text> text = new osgText::Text();
    _texts.push_back( text );
    //text->setFont(font);
    text->setColor( _color );
    text->setCharacterSize( 2 * _charSize );
    text->setAxisAlignment( osgText::TextBase::XY_PLANE );
    text->setPosition( osg::Vec3( 0.0f, 20.0f, -1.0f ) );
    text->setLayout( osgText::Text::LEFT_TO_RIGHT );
    text->setAlignment( osgText::Text::CENTER_BASE_LINE );
    text->setText( "STALL" );
    geode->addDrawable( text );
}

////////////////////////////////////////////////////////////////////////////////

void HUD::createTextL()
{
    const float x = 60.0f;
    const float y =  0.0f;

    const float w = 16.0f;
    const float h =  9.0f;

    // Box
    {
        osg::ref_ptr<osg::Geode> geode = new osg::Geode();
        _switch->addChild( geode.get() );

        osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

        osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // vertices
        osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals

        v->push_back( osg::Vec3( -x     , y     , -1.0f ) );
        v->push_back( osg::Vec3( -x     , y + h , -1.0f ) );
        v->push_back( osg::Vec3( -x - w , y + h , -1.0f ) );
        v->push_back( osg::Vec3( -x - w , y     , -1.0f ) );

        n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );

        geometry->setVertexArray( v.get() );
        geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINE_LOOP, 0, v->size() ) );

        geometry->setNormalArray( n.get() );
        geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );

        geode->addDrawable( geometry.get() );
    }

    //osgText::Font* font = osgText::readFontFile( m_fontPath.c_str() );

    // Text Airspeed
    {
        _airspeed = new osg::Geode();
        _switch->addChild( _airspeed.get() );

        osg::ref_ptr<osgText::Text> text = new osgText::Text();
        _texts.push_back( text );
        //text->setFont(font);
        text->setColor( _color );
        text->setCharacterSize( _charSize );
        text->setAxisAlignment( osgText::TextBase::XY_PLANE );
        text->setPosition( osg::Vec3( -x - 1.0f, y + 2.5f, -1.0f ) );
        text->setLayout( osgText::Text::LEFT_TO_RIGHT );
        text->setAlignment( osgText::Text::RIGHT_BASE_LINE );
        text->setText( "9999" );
        _airspeed->addDrawable( text );
    }

    // Text Mach No
    {
        _machNo = new osg::Geode();
        _switch->addChild( _machNo.get() );

        osg::ref_ptr<osgText::Text> text = new osgText::Text();
        _texts.push_back( text );
        //text->setFont(font);
        text->setColor( _color );
        text->setCharacterSize( _charSize );
        text->setAxisAlignment( osgText::TextBase::XY_PLANE );
        text->setPosition( osg::Vec3( -x - 1.0f, y - 7.5f, -1.0f ) );
        text->setLayout( osgText::Text::LEFT_TO_RIGHT );
        text->setAlignment( osgText::Text::RIGHT_BASE_LINE );
        text->setText( "2.15" );
        _machNo->addDrawable( text );
    }

    // Text G-Force
    {
        _gForce = new osg::Geode();
        _switch->addChild( _gForce.get() );

        osg::ref_ptr<osgText::Text> text = new osgText::Text();
        _texts.push_back( text );
        //text->setFont(font);
        text->setColor( _color );
        text->setCharacterSize( _charSize );
        text->setAxisAlignment( osgText::TextBase::XY_PLANE );
        text->setPosition( osg::Vec3( -x - 1.0f, y - 15.0f, -1.0f ) );
        text->setLayout( osgText::Text::LEFT_TO_RIGHT );
        text->setAlignment( osgText::Text::RIGHT_BASE_LINE );
        text->setText( "1.0G" );
        _gForce->addDrawable( text );
    }
}

////////////////////////////////////////////////////////////////////////////////

void HUD::createTextR()
{
    const float x = 60.0f;
    const float y =  0.0f;

    const float w = 23.0f;
    const float h =  9.0f;

    // Box
    {
        osg::ref_ptr<osg::Geode> geode = new osg::Geode();
        _switch->addChild( geode.get() );

        osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

        osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // vertices
        osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals

        v->push_back( osg::Vec3( x     , y     , -1.0f ) );
        v->push_back( osg::Vec3( x     , y + h , -1.0f ) );
        v->push_back( osg::Vec3( x + w , y + h , -1.0f ) );
        v->push_back( osg::Vec3( x + w , y     , -1.0f ) );

        n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );

        geometry->setVertexArray( v.get() );
        geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINE_LOOP, 0, v->size() ) );

        geometry->setNormalArray( n.get() );
        geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );

        geode->addDrawable( geometry.get() );
    }

    //osgText::Font* font = osgText::readFontFile( m_fontPath.c_str() );

    // Text Altitude
    {
        _altitude = new osg::Geode();
        _switch->addChild( _altitude.get() );

        osg::ref_ptr<osgText::Text> text = new osgText::Text();
        _texts.push_back( text );
        //text->setFont(font);
        text->setColor( _color );
        text->setCharacterSize( _charSize );
        text->setAxisAlignment( osgText::TextBase::XY_PLANE );
        text->setPosition( osg::Vec3( x + w - 1.0f, y + 2.5f, -1.0f ) );
        text->setLayout( osgText::Text::LEFT_TO_RIGHT );
        text->setAlignment( osgText::Text::RIGHT_BASE_LINE );
        text->setText( "99,999" );
        _altitude->addDrawable( text );
    }

    // Text Climb Rate
    {
        _climbRate = new osg::Geode();
        _switch->addChild( _climbRate.get() );

        osg::ref_ptr<osgText::Text> text = new osgText::Text();
        _texts.push_back( text );
        //text->setFont(font);
        text->setColor( _color );
        text->setCharacterSize( _charSize );
        text->setAxisAlignment( osgText::TextBase::XY_PLANE );
        text->setPosition( osg::Vec3( x + w - 1.0f, y - 7.5f, -1.0f ) );
        text->setLayout( osgText::Text::LEFT_TO_RIGHT );
        text->setAlignment( osgText::Text::RIGHT_BASE_LINE );
        text->setText( "6000" );
        _climbRate->addDrawable( text );
    }

    // Text Radio Altitude
    {
        _radioAlt = new osg::Geode();
        _switch->addChild( _radioAlt.get() );

        osg::ref_ptr<osgText::Text> text = new osgText::Text();
        _texts.push_back( text );
        //text->setFont(font);
        text->setColor( _color );
        text->setCharacterSize( _charSize );
        text->setAxisAlignment( osgText::TextBase::XY_PLANE );
        text->setPosition( osg::Vec3( x + w - 1.0f, y - 15.0f, -1.0f ) );
        text->setLayout( osgText::Text::LEFT_TO_RIGHT );
        text->setAlignment( osgText::Text::RIGHT_BASE_LINE );
        text->setText( "RA 9999" );
        _radioAlt->addDrawable( text );
    }
}

////////////////////////////////////////////////////////////////////////////////

void HUD::createPitchLadderBar( osg::Geode *geode, int y, int deg )
{
    const float w = 60.0f; // width
    const float h =  3.0f; // height
    const float g = 20.0f; // gap

    // Bar
    {
        osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

        osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // vertices
        osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals

        if ( deg < 0 )
        {
            float l = w - g;
            float d = l * 0.2f;

            // left
            v->push_back( osg::Vec3( -w / 2.0f    , y, -1.0f ) );
            v->push_back( osg::Vec3( -w / 2.0f + d, y, -1.0f ) );

            v->push_back( osg::Vec3( -g / 2.0f - d, y, -1.0f ) );
            v->push_back( osg::Vec3( -g / 2.0f    , y, -1.0f ) );

            v->push_back( osg::Vec3( -g / 2.0f,     y, -1.0f ) );
            v->push_back( osg::Vec3( -g / 2.0f, y + h, -1.0f ) );

            // right
            v->push_back( osg::Vec3(  w / 2.0f    , y, -1.0f ) );
            v->push_back( osg::Vec3(  w / 2.0f - d, y, -1.0f ) );

            v->push_back( osg::Vec3(  g / 2.0f + d, y, -1.0f ) );
            v->push_back( osg::Vec3(  g / 2.0f    , y, -1.0f ) );

            v->push_back( osg::Vec3(  g / 2.0f,     y, -1.0f ) );
            v->push_back( osg::Vec3(  g / 2.0f, y + h, -1.0f ) );
        }
        else
        {
            // left
            v->push_back( osg::Vec3( -w / 2.0f,     y, -1.0f ) );
            v->push_back( osg::Vec3( -g / 2.0f,     y, -1.0f ) );

            v->push_back( osg::Vec3( -g / 2.0f,     y, -1.0f ) );
            v->push_back( osg::Vec3( -g / 2.0f, y - h, -1.0f ) );

            // right
            v->push_back( osg::Vec3(  w / 2.0f,     y, -1.0f ) );
            v->push_back( osg::Vec3(  g / 2.0f,     y, -1.0f ) );

            v->push_back( osg::Vec3(  g / 2.0f,     y, -1.0f ) );
            v->push_back( osg::Vec3(  g / 2.0f, y - h, -1.0f ) );
        }

        n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );

        geometry->setVertexArray( v.get() );

        if ( deg < 0 )
        {
            geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, v->size() ) );
        }
        else
        {
            geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, v->size() ) );
        }

        geometry->setNormalArray( n.get() );
        geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );

        geode->addDrawable( geometry.get() );
    }

    std::stringstream deg_str;
    deg_str << ( deg < 0 ? -deg : deg );

    // TEXT
    float y_del = _charSize - 3.0;

    if ( deg < 0 )
    {
        y_del = -_charSize - 0.5;
    }

    //osgText::Font* font = osgText::readFontFile( m_fontPath.c_str() );

    // text L
    {
        osg::ref_ptr<osgText::Text> text = new osgText::Text();
        _texts.push_back( text );
        //text->setFont(font);
        text->setColor( _color );
        text->setCharacterSize( _charSize );
        text->setAxisAlignment( osgText::TextBase::XY_PLANE );
        text->setPosition( osg::Vec3( -w / 2.0f, y + y_del, -1.0f ) );
        text->setLayout( osgText::Text::LEFT_TO_RIGHT );
        text->setAlignment( osgText::Text::LEFT_BASE_LINE );
        text->setText( deg_str.str() );
        geode->addDrawable( text );
    }

    // text R
    {
        osg::ref_ptr<osgText::Text> text = new osgText::Text();
        _texts.push_back( text );
        //text->setFont(font);
        text->setColor( _color );
        text->setCharacterSize( _charSize );
        text->setAxisAlignment( osgText::TextBase::XY_PLANE );
        text->setPosition( osg::Vec3( w / 2.0f, y + y_del, -1.0f ) );
        text->setLayout( osgText::Text::LEFT_TO_RIGHT );
        text->setAlignment( osgText::Text::RIGHT_BASE_LINE );
        text->setText( deg_str.str() );
        geode->addDrawable( text );
    }
}

////////////////////////////////////////////////////////////////////////////////

void HUD::createHeadingScaleBar( osg::Geode *geode, float y_del, int x, int deg10 )
{
    /////////////////////////////////////////
    createHeadingScaleBar( geode, y_del, x );
    /////////////////////////////////////////

    char deg10_srt[256];

    sprintf( deg10_srt, "%02d", deg10 );

    osg::ref_ptr<osgText::Text> text = new osgText::Text();
    _texts.push_back( text );
    //text->setFont(font);
    text->setColor( _color );
    text->setCharacterSize( _charSize );
    text->setAxisAlignment( osgText::TextBase::XY_PLANE );
    text->setPosition( osg::Vec3( x, y_del - _charSize - 3.0f, -1.0f ) );
    text->setLayout( osgText::Text::LEFT_TO_RIGHT );
    text->setAlignment( osgText::Text::CENTER_BASE_LINE );
    text->setText( deg10_srt );
    geode->addDrawable( text );
}

////////////////////////////////////////////////////////////////////////////////

void HUD::createHeadingScaleBar( osg::Geode *geode, float y_del, int x )
{
    const float h = 2.5f;

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // vertices
    osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals

    // bar
    v->push_back( osg::Vec3( x, y_del    , -1.0f ) );
    v->push_back( osg::Vec3( x, y_del - h, -1.0f ) );

    n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );

    geometry->setVertexArray( v.get() );
    geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, v->size() ) );

    geometry->setNormalArray( n.get() );
    geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );

    geode->addDrawable( geometry.get() );
}

////////////////////////////////////////////////////////////////////////////////

void HUD::updateRollIndicator()
{
    if ( fabs( Data::get()->cgi.hud.roll ) < _rollLimit )
    {
        _switchRollIndicator->setAllChildrenOn();

        osg::Quat q( Data::get()->cgi.hud.roll, osg::Z_AXIS );

        _patRollIndicator->setAttitude( q );
    }
    else
    {
        _switchRollIndicator->setAllChildrenOff();
    }
}

////////////////////////////////////////////////////////////////////////////////

void HUD::updatePitchLadder()
{
    float roll_deg  = osg::RadiansToDegrees( Data::get()->cgi.hud.roll  );
    float pitch_deg = osg::RadiansToDegrees( Data::get()->cgi.hud.pitch );

    osg::Quat q( Data::get()->cgi.hud.roll, osg::Z_AXIS );

    _patPitchLadderRoll->setAttitude( q );

    osg::Vec3 r( 0.0, -_angleCoef * pitch_deg, 0.0 );

    _patPitchLadderPitch->setPosition( r );

    float sinRoll = sin( Data::get()->cgi.hud.roll );
    float cosRoll = cos( Data::get()->cgi.hud.roll );

    float x = -_angleCoef * ( -_sideslipAngleFPM_deg * cosRoll + _angleOfAttackFPM_deg * sinRoll );

    _patPitchLadderSlip->setPosition( osg::Vec3( x, 0.0, 0.0 ) );

    short i_pitch = floor( ( 90.0 + pitch_deg ) / 5.0 + 0.5 );

    if ( i_pitch <  0 ) i_pitch =  0;
    if ( i_pitch > 36 ) i_pitch = 36;

    float pitch_i_deg = i_pitch * 5.0f - 90.0f;

    _switchPitchLadder->setAllChildrenOff();
    _switchPitchLadder->setValue( i_pitch, true );

    if ( i_pitch >  0 ) _switchPitchLadder->setValue( i_pitch - 1, true );
    if ( i_pitch < 36 ) _switchPitchLadder->setValue( i_pitch + 1, true );

    float roll_deg_abs = fabs( roll_deg );

    if ( roll_deg_abs < 30.0 || roll_deg_abs > 150.0 )
    {
        if ( i_pitch >  1 && pitch_deg - pitch_i_deg <  1.0f )
        {
            _switchPitchLadder->setValue( i_pitch - 2, true );
        }

        if ( i_pitch < 35 && pitch_deg - pitch_i_deg > -1.0f )
        {
            _switchPitchLadder->setValue( i_pitch + 2, true );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void HUD::updateHeadingScale()
{
    osg::ref_ptr<osgText::Text> text;

    float head_deg = osg::RadiansToDegrees( Data::get()->cgi.hud.heading );

    short head_ind = floor( ( ( ( head_deg < 180.0f ) ? 360.0f : 0.0f ) + head_deg ) / 5.0 + 0.5 );
    if ( head_ind <   0 ) head_ind =   0;
    if ( head_ind > 128 ) head_ind = 128;

    _switchHeadingScale->setSingleChildOn( head_ind );
    _switchHeadingScale->setValue( head_ind - 1, true );
    _switchHeadingScale->setValue( head_ind + 1, true );
    _switchHeadingScale->setValue( head_ind - 2, true );
    _switchHeadingScale->setValue( head_ind + 2, true );
    _switchHeadingScale->setValue( head_ind - 3, true );
    _switchHeadingScale->setValue( head_ind + 3, true );
    _switchHeadingScale->setValue( head_ind - 4, true );
    _switchHeadingScale->setValue( head_ind + 4, true );

    float x = ( 100.0f / 50.0f ) * ( ( head_deg < 180.0f ) ? head_deg : head_deg - 360.0f );
    _patHeadingScale->setPosition( osg::Vec3( -x, 0.0, 0.0 ) );

    text = dynamic_cast< osgText::Text* >( _heading->getDrawable( 0 ) );

    if ( text.valid() )
    {
        char heading_str[256];
        sprintf( heading_str, "%d", (int)( floor( head_deg + 0.5 ) ) );
        text->setText( heading_str );
    }
}

////////////////////////////////////////////////////////////////////////////////

void HUD::updateFPM()
{
    if ( _validFPM )
    {
        _switchFPM->setAllChildrenOff();
    }
    else
    {
        _switchFPM->setAllChildrenOn();
    }

    osg::Vec3 r(  _angleCoef * _sideslipAngleFPM_deg,
                 -_angleCoef * _angleOfAttackFPM_deg,
                  0.0 );

    _patFPM->setPosition( r );
}

////////////////////////////////////////////////////////////////////////////////

void HUD::updateILS()
{
    if ( Data::get()->cgi.hud.ils_visible )
    {
        _switchILS->setAllChildrenOn();

        osg::Quat q( Data::get()->cgi.hud.roll, osg::Z_AXIS );
        osg::Vec3 r(  _angleCoef * _sideslipAngleFPM_deg,
                     -_angleCoef * _angleOfAttackFPM_deg,
                      0.0 );

        _patILS->setAttitude( q );
        _patILS->setPosition( r );

        float ils_gs = Data::get()->cgi.hud.ils_gs_deviation;
        float ils_lc = Data::get()->cgi.hud.ils_lc_deviation;

        if ( ils_gs < -1.0f ) ils_gs = -1.0f;
        if ( ils_gs >  1.0f ) ils_gs =  1.0f;
        if ( ils_lc < -1.0f ) ils_lc = -1.0f;
        if ( ils_lc >  1.0f ) ils_lc =  1.0f;

        _patILS_GS->setPosition( osg::Vec3( 0.0, 20.0 * ils_gs, 0.0 ) );
        _patILS_LC->setPosition( osg::Vec3( 20.0 * ils_lc, 0.0, 0.0 ) );
    }
    else
    {
        _switchILS->setAllChildrenOff();
    }
}

////////////////////////////////////////////////////////////////////////////////

void HUD::updateStall()
{
    if ( Data::get()->cgi.hud.stall )
    {
        _switchStall->setAllChildrenOn();
    }
    else
    {
        _switchStall->setAllChildrenOff();
    }
}

////////////////////////////////////////////////////////////////////////////////

void HUD::updateTextL()
{
    osg::ref_ptr<osgText::Text> text;

    // airspeed
    text = dynamic_cast< osgText::Text* >( _airspeed->getDrawable( 0 ) );

    if ( text.valid() )
    {
        float airspeed = Data::get()->cgi.hud.airspeed * Data::get()->cgi.hud.factor_vel;

        char airspeed_str[256];
        sprintf( airspeed_str, "%d", (int)( floor( airspeed + 0.5 ) ) );
        text->setText( airspeed_str );
    }

    // Mach no
    text = dynamic_cast< osgText::Text* >( _machNo->getDrawable( 0 ) );

    if ( text.valid() )
    {
        char machNo_str[256];
        sprintf( machNo_str, "M %.2f", Data::get()->cgi.hud.machNumber );
        text->setText( machNo_str );
    }

    // G-Force
    text = dynamic_cast< osgText::Text* >( _gForce->getDrawable( 0 ) );

    if ( text.valid() )
    {
        char gForce_str[256];
        sprintf( gForce_str, "%.1fG", Data::get()->cgi.hud.g_force );
        text->setText( gForce_str );
    }
}

////////////////////////////////////////////////////////////////////////////////

void HUD::updateTextR()
{
    osg::ref_ptr<osgText::Text> text;

    // altitude
    text = dynamic_cast< osgText::Text* >( _altitude->getDrawable( 0 ) );

    if ( text.valid() )
    {
        float altitude = Data::get()->cgi.hud.altitude * Data::get()->cgi.hud.factor_alt;

        char altitude_str[256];
        sprintf( altitude_str, "%d", (int)( floor( altitude + 0.5 ) ) );
        text->setText( altitude_str );
    }

    // climb rate
    text = dynamic_cast< osgText::Text* >( _climbRate->getDrawable( 0 ) );

    if ( text.valid() )
    {
        float climbRate = 60.0f * Data::get()->cgi.hud.climbRate * Data::get()->cgi.hud.factor_alt;

        char climbRate_str[256];
        sprintf( climbRate_str, "%d", (int)( floor( climbRate + 0.5 ) ) );
        text->setText( climbRate_str );
    }

    // radio altitude
    text = dynamic_cast< osgText::Text* >( _radioAlt->getDrawable( 0 ) );

    if ( text.valid() )
    {
        float altitude = Data::get()->cgi.hud.radioAlt * Data::get()->cgi.hud.factor_alt;

        if ( altitude < 3000.0 )
        {
            char altitude_str[256];
            sprintf( altitude_str, "RA %d", (int)( floor( altitude + 0.5 ) ) );
            text->setText( altitude_str );
        }
        else
        {
            text->setText( "" );
        }
    }
}
