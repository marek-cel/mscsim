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
#ifndef FDM_XMLNODE_H
#define FDM_XMLNODE_H

////////////////////////////////////////////////////////////////////////////////

#include <limits>
#include <map>
#include <string>

#include <libxml/tree.h>

#include <fdm/fdm_Defines.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief XML node class.
 */
class FDMEXPORT XmlNode
{
public:

    typedef std::map<std::string,std::string> Attributes;

    /** Constructor. */
    XmlNode();

    /** Copy constructor. */
    XmlNode( const XmlNode &node );

    /** Constructor. */
    XmlNode( xmlNodePtr node, const char *file );

    /** Destrcutor. */
    virtual ~XmlNode() {}

    /**
     * Returns the value of the attribute or an empty string if the attribute
     * has not been specified.
     */
    std::string getAttribute( const char *name ) const;

    /**
     * Returns element attributes list or empty list if the node is not
     * an element or element does not has any attributes.
     */
    Attributes getAttributes() const;

    /**
     * Returns first child node.
     */
    XmlNode getFirstChild() const;

    /**
     * Returns first child element node of the given name.
     * @param name element name
     */
    XmlNode getFirstChildElement( const char *name = "" ) const;

    /**
     * Returns file name.
     */
    inline std::string getFile() const
    {
        return _file;
    }

    /**
     * Returns file name and line number.
     */
    std::string getFileAndLine() const;

    /**
     * Returns line number.
     */
    inline int getLine() const
    {
        if ( isValid() )
        {
            return (int)_node->line;
        }

        return std::numeric_limits< int >::quiet_NaN();
    }

    /**
     * Returns node name.
     */
    std::string getName() const
    {
        if ( isValid() )
        {
            return std::string( (const char*)_node->name );
        }

        return std::string();
    }

    /**
     * Returns next sibling node.
     */
    XmlNode getNextSibling() const;

    /**
     * Returns next sibling element node of the given name.
     * @param name element name
     */
    XmlNode getNextSiblingElement( const char *name = "" ) const;

    /** */
    std::string getText() const;

    /**
     * Returns true if node has an attribute of a given name.
     */
    bool hasAttribute( const char *name ) const;

    /**
     * Returns true if node has attributes.
     */
    inline bool hasAttributes() const
    {
        if ( isValid() )
        {
            return ( _node->properties != 0 );
        }

        return false;
    }

    /**
     * Returns true if node has children.
     */
    inline bool hasChildren() const
    {
        if ( isValid() )
        {
            return ( _node->children != 0 );
        }

        return false;
    }

    /**
     * Returns true if node is an attribute.
     */
    inline bool isAttribute() const
    {
        if ( isValid() )
        {
            return ( _node->type == XML_ATTRIBUTE_NODE );
        }

        return false;
    }

    /**
     * Returns true if node is a comment.
     */
    inline bool isComment() const
    {
        if ( isValid() )
        {
            return ( _node->type == XML_COMMENT_NODE );
        }

        return false;
    }

    /**
     * Returns true if node is an element.
     */
    inline bool isElement() const
    {
        if ( isValid() )
        {
            return ( _node->type == XML_ELEMENT_NODE );
        }

        return false;
    }

    /**
     * Returns true if node is a text.
     */
    inline bool isText() const
    {
        if ( isValid() )
        {
            return ( _node->type == XML_TEXT_NODE );
        }

        return false;
    }

    /**
     * Returns true if node is valid.
     */
    inline bool isValid() const
    {
        return ( _node ) ? true : false;
    }

    /** Assignment operator. */
    inline const XmlNode& operator= ( const XmlNode &node )
    {
        _file = node._file;
        _node = node._node;

        return (*this);
    }

private:

    std::string _file;  ///< XML file name
    xmlNodePtr  _node;  ///< XML node pointer
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_XMLNODE_H
