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

#include <fdm/xml/fdm_XmlNode.h>

#include <cstring>

#include <fdm/utils/fdm_String.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

XmlNode::XmlNode() :
    _node ( 0 )
{}

////////////////////////////////////////////////////////////////////////////////

XmlNode::XmlNode( const XmlNode &node ) :
    _file ( node._file ),
    _node ( node._node )
{}

////////////////////////////////////////////////////////////////////////////////

XmlNode::XmlNode( xmlNodePtr node, const char *file ) :
    _file ( file ),
    _node ( node )
{}

////////////////////////////////////////////////////////////////////////////////

XmlNode::~XmlNode() {}

////////////////////////////////////////////////////////////////////////////////

std::string XmlNode::getAttribute( const char *name ) const
{
    if ( hasAttributes() )
    {
        xmlAttrPtr attr = _node->properties;

        while ( attr != 0 )
        {
            if ( 0 == xmlStrcmp( attr->name, (const xmlChar*)name ) )
            {
                if ( attr->children != 0 )
                {
                    xmlChar* value = xmlNodeListGetString( attr->children->doc, attr->children, 1);

                    std::string result( (const char*)value );

                    xmlFree( value );

                    return result;
                }
                else
                {
                    return std::string();
                }
            }

            attr = attr->next;
        }
    }

    return std::string();
}

////////////////////////////////////////////////////////////////////////////////

XmlNode::Attributes XmlNode::getAttributes() const
{
    Attributes attributes;
    attributes.clear();

    if ( hasAttributes() )
    {
        xmlAttrPtr attr = _node->properties;

        while ( attr != 0 )
        {
            if ( attr->children != 0 )
            {
                xmlChar* value = xmlNodeListGetString( attr->children->doc, attr->children, 1);

                std::string strName( (const char*)attr->name );
                std::string strValue( (const char*)value );

                attributes.insert( std::pair<std::string,std::string>( strName, strValue ) );

                xmlFree( value );
            }

            attr = attr->next;
        }
    }

    return attributes;
}

////////////////////////////////////////////////////////////////////////////////

XmlNode XmlNode::getFirstChild() const
{
    XmlNode result;

    if ( isValid() )
    {
        if ( _node->children != 0 )
        {
            result._node = _node->children;
            result._file = _file;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

XmlNode XmlNode::getFirstChildElement( const char *name ) const
{
    XmlNode result;

    if ( isValid() )
    {
        xmlNodePtr child = _node->children;

        while ( child != 0 )
        {
            if ( child->type == XML_ELEMENT_NODE )
            {
                if ( 0 == xmlStrcmp( child->name, (const xmlChar*)name )
                  || strlen( name ) == 0 )
                {
                    result._node = child;
                    result._file = _file;
                    return result;
                }
            }

            child = child->next;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

std::string XmlNode::getFileAndLine() const
{
    std::string result;

    result.clear();

    result += _file;
    result += "(";
    result += String::toString( (int)_node->line );
    result += ")";

    return result;
}

////////////////////////////////////////////////////////////////////////////////

XmlNode XmlNode::getNextSibling() const
{
    XmlNode result;

    if ( isValid() )
    {
        if ( _node->next != 0 )
        {
            result._node = _node->next;
            result._file = _file;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

XmlNode XmlNode::getNextSiblingElement( const char *name ) const
{
    XmlNode result;

    if ( isValid() )
    {
        xmlNodePtr next = _node->next;

        while ( next != 0 )
        {
            if ( next->type == XML_ELEMENT_NODE )
            {
                if ( 0 == xmlStrcmp( next->name, (const xmlChar*)name )
                  || strlen( name ) == 0 )
                {
                    result._node = next;
                    result._file = _file;
                    return result;
                }
            }

            next = next->next;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

std::string XmlNode::getText() const
{
    switch ( _node->type )
    {
    case XML_TEXT_NODE:
        return std::string( (const char*)_node->content );
        break;

    default:
        return std::string();
        break;
    }

    return std::string();
}

////////////////////////////////////////////////////////////////////////////////

bool XmlNode::hasAttribute( const char *name ) const
{
    if ( hasAttributes() )
    {
        xmlAttrPtr attr = _node->properties;

        while ( attr != 0 )
        {
            if ( 0 == xmlStrcmp( attr->name, (const xmlChar*)name ) )
            {
                return true;
            }

            attr = attr->next;
        }
    }

    return false;
}
