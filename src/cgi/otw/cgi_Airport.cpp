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

#include <cgi/otw/cgi_Airport.h>

#include <Data.h>

#include <cgi/cgi_FindNode.h>
#include <cgi/cgi_Models.h>
#include <cgi/cgi_WGS84.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

Airport::Airport( const char *file, double lat, double lon, double alt,
                  const Module *parent ) :
    Module( parent )
{
    _pat = new osg::PositionAttitudeTransform();
    _root->addChild( _pat.get() );

    osg::ref_ptr<osg::Node> node = Models::get( file );

    if ( node.valid() )
    {
        _pat->addChild( node.get() );

        WGS84::setLatLonAltHdg( _pat.get(), lat, lon, alt );

        switchesAdd( &_lightsRALS, node.get(), "RALS_L" );
        switchesAdd( &_lightsTDZL, node.get(), "TDZL_L" );
        switchesAdd( &_lightsVGSI, node.get(), "VGSI_L" );

        switchesAdd( &_lightsRALS, node.get(), "RALS_H" );
        switchesAdd( &_lightsTDZL, node.get(), "TDZL_H" );
        switchesAdd( &_lightsVGSI, node.get(), "VGSI_H" );

        switchesAdd( &_lightsRCLS, node.get(), "RCLS" );
        switchesAdd( &_lightsRELS, node.get(), "RELS" );

        switchesAdd( &_lightsHELI, node.get(), "HELI" );
        switchesAdd( &_lightsTELS, node.get(), "TELS" );
        switchesAdd( &_lightsTWRL, node.get(), "TWRL" );
    }
}

////////////////////////////////////////////////////////////////////////////////

Airport::~Airport() {}

////////////////////////////////////////////////////////////////////////////////

void Airport::update()
{
    /////////////////
    Module::update();
    /////////////////

    switchesSet( &_lightsRALS, Data::get()->cgi.airport.lightsRALS );
    switchesSet( &_lightsTDZL, Data::get()->cgi.airport.lightsTDZL );
    switchesSet( &_lightsVGSI, Data::get()->cgi.airport.lightsVGSI );

    switchesSet( &_lightsRCLS, Data::get()->cgi.airport.lightsRCLS );
    switchesSet( &_lightsRELS, Data::get()->cgi.airport.lightsRELS );

    switchesSet( &_lightsHELI, Data::get()->cgi.airport.lightsHELI );
    switchesSet( &_lightsTELS, Data::get()->cgi.airport.lightsTELS );
    switchesSet( &_lightsTWRL, Data::get()->cgi.airport.lightsTWRL );
}

////////////////////////////////////////////////////////////////////////////////

void Airport::switchesAdd( Switches *switches, osg::Node *node, const char *name )
{
    FindNode::Nodes nodes = FindNode::findNodes( node, name );

    for ( FindNode::Nodes::iterator it = nodes.begin(); it != nodes.end(); ++it )
    {
        osg::ref_ptr<osg::Switch> sw = dynamic_cast<osg::Switch*>( (*it).get() );

        if ( sw.valid() ) switches->push_back( sw.get() );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Airport::switchesSet( Switches *switches, bool enabled )
{
    for ( Switches::iterator it = switches->begin(); it != switches->end(); ++it )
    {
        if ( enabled )
            (*it)->setAllChildrenOn();
        else
            (*it)->setAllChildrenOff();
    }
}
