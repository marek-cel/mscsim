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

#include <fdmXml/fdm_XmlDoc.h>

#include <fdmMain/fdm_Defines.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

XmlDoc::XmlDoc( const std::string &fileName ) :
    m_doc  ( 0 ),
    m_open ( false ),
    m_root ( 0 )
{
    readFile( fileName );
}

////////////////////////////////////////////////////////////////////////////////

XmlDoc::~XmlDoc()
{
    if ( m_root ) delete m_root;
    m_root = 0;

    xmlFreeDoc( m_doc );
}

////////////////////////////////////////////////////////////////////////////////

int XmlDoc::readFile( const std::string &fileName )
{
    m_doc = xmlParseFile( fileName.c_str() );

    if ( m_doc == 0 )
    {
        xmlFreeDoc( m_doc );
        return FDM_FAILURE;
    }

    xmlNodePtr root = xmlDocGetRootElement( m_doc );

    if ( root == 0 )
    {
        xmlFreeNode( root );
        xmlFreeDoc( m_doc );
        return FDM_FAILURE;
    }

    m_root = new XmlNode( root, fileName );

    m_open = true;

    return FDM_SUCCESS;
}
