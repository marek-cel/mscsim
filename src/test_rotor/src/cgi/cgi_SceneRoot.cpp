/****************************************************************************//*
 * Copyright (C) 2020 Marek M. Cel
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

#include <cgi/cgi_SceneRoot.h>

#include <iostream>

#include <osg/LightSource>
#include <osg/PositionAttitudeTransform>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

SceneRoot::SceneRoot() :
    _helicopter ( nullptr ),
    _terrain    ( nullptr ),
    _vector     ( nullptr )
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

    createSceneLight();

    _helicopter = new Helicopter();
    _terrain    = new Terrain();
    _vector     = new Vector();

    _root->addChild( _helicopter ->getRoot() );
    _root->addChild( _terrain    ->getRoot() );
    _root->addChild( _vector     ->getRoot() );
}

////////////////////////////////////////////////////////////////////////////////

SceneRoot::~SceneRoot()
{
    if ( _helicopter ) delete _helicopter;
    _helicopter = nullptr;

    if ( _terrain ) delete _terrain;
    _terrain = nullptr;

    if ( _vector ) delete _vector;
    _vector = nullptr;
}

////////////////////////////////////////////////////////////////////////////////

void SceneRoot::update()
{
    _helicopter ->update();
    _terrain    ->update();
    _vector     ->update();
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

    lightSun->setPosition( osg::Vec4d( 1000.0, 1000.0, 1000.0, 0.0 ) );

    osg::Vec4 lightColor( 255.0/255.0, 255.0/255.0, 250.0/255.0, 1.0 );

    lightSun->setAmbient( lightColor );
    lightSun->setDiffuse( lightColor );
    lightSun->setSpecular( lightColor );

    lightSun->setConstantAttenuation( 1.0 );

    lightSourceSun->setLight( lightSun.get() );

    lightSourceSun->setLocalStateSetModes( osg::StateAttribute::ON );
    lightSourceSun->setStateSetModes( *_root->getOrCreateStateSet(), osg::StateAttribute::ON );
}
