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

#include <cgi/cgi_Module.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

Module::Module( const Module *parent ) :
    _parent ( parent )
{
    _root = new osg::Group();

    _children.clear();
}

////////////////////////////////////////////////////////////////////////////////

Module::~Module()
{
    removeAllChildren();
}

////////////////////////////////////////////////////////////////////////////////

void Module::addChild( Module *child )
{
    if ( child )
    {
        _children.push_back( child );
        _root->addChild( child->getNode() );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Module::update()
{
    for ( List::iterator it = _children.begin(); it != _children.end(); ++it )
    {
        (*it)->update();
    }
}

////////////////////////////////////////////////////////////////////////////////

void Module::removeAllChildren()
{
    if ( _root->getNumChildren() > 0 )
    {
        _root->removeChildren( 0, _root->getNumChildren() );
    }

    List::iterator it = _children.begin();
    while ( it != _children.end() )
    {
        DELPTR( (*it) );
        it = _children.erase( it );
    }
}
