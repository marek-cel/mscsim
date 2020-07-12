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
#ifndef FDM_XMLDOC_H
#define FDM_XMLDOC_H

////////////////////////////////////////////////////////////////////////////////

#include <string>

#include <libxml/tree.h>

#include <fdm/xml/fdm_XmlNode.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief XML Document class.
 */
class FDMEXPORT XmlDoc
{
public:

    /** Constrcutor. */
    XmlDoc( const char *fileName = "" );

    /** Destrcutor. */
    virtual ~XmlDoc();

    /**
     * @brief Gets XML document root node.
     * @return XML document root node
     */
    inline XmlNode getRootNode()
    {
        return XmlNode( *_root );
    }

    /**
     * @brief Checks if XML document is open.
     * @return returns true if XML document is open
     */
    inline bool isOpen() const
    {
        return _open;
    }

    /** @return FDM_SUCCESS on success, FDM_FAILURE on failure. */
    int readFile( const char *fileName );

private:

    xmlDocPtr _doc;     ///< XML document pointer
    bool _open;         ///< specifies if document is open
    XmlNode *_root;     ///< XML document root node
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_XMLDOC_H
