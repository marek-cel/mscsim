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

#include <g1000/xml/g1000_XmlDoc.h>

#include <g1000/g1000_Defines.h>

////////////////////////////////////////////////////////////////////////////////

using namespace g1000;

////////////////////////////////////////////////////////////////////////////////

XmlDoc::XmlDoc( const std::string &fileName ) :
    _doc  ( 0 ),
    _open ( false ),
    _root ( NULLPTR )
{
    readFile( fileName );
}

////////////////////////////////////////////////////////////////////////////////

XmlDoc::~XmlDoc()
{
	DELPTR( _root );

    xmlFreeDoc( _doc );
}

////////////////////////////////////////////////////////////////////////////////

bool XmlDoc::readFile( const std::string &fileName )
{
    _doc = xmlParseFile( fileName.c_str() );

    if ( _doc == 0 )
    {
        xmlFreeDoc( _doc );
        return false;
    }

    xmlNodePtr root = xmlDocGetRootElement( _doc );

    if ( root == 0 )
    {
        xmlFreeNode( root );
        xmlFreeDoc( _doc );
        return false;
    }

    _root = new XmlNode( root, fileName );

    _open = true;

    return true;
}
