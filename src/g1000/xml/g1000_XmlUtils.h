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
#ifndef G1000_XMLUTILS_H
#define G1000_XMLUTILS_H

////////////////////////////////////////////////////////////////////////////////

#include <g1000/xml/g1000_XmlNode.h>

#include <g1000/utils/g1000_Units.h>

////////////////////////////////////////////////////////////////////////////////

namespace g1000
{

/**
 * @brief XML utilities class.
 */
class XmlUtils
{
public:

    /**
     * @brief Returns XML parsing error info string.
     * @param node XML node
     * @return XML parsing error info string
     */
    static std::string getErrorInfo( const XmlNode &node );

    /**
     * @brief Reads value from XML file.
     * @param node XML node
     * @param str destination
     * @return true on success or false on failure.
     */
    static bool read( const XmlNode &node, std::string &str );

    /**
     * @brief Reads value from XML file.
     * @param node XML node
     * @param data destination
     * @return true on success or false on failure.
     */
    static bool read( const XmlNode &node, int &data );

    /**
     * @brief Reads value from XML file.
     * @param node XML node
     * @param data destination
     * @return true on success or false on failure.
     *
     * @code
     * <tag_name [unit="{ unit string }"]> { value } </tag_name>
     * @endcode
     *
     * @see fdm::Units::getConverter()
     */
    static bool read( const XmlNode &node, double &data );

    /**
     * @brief Reads value from XML file.
     * @param node XML node
     * @param converter function pointer
     * @return true on success or false on failure.
     *
     * @code
     * <tag_name [unit="{ unit string }"] />
     * @endcode
     *
     * @see fdm::Units::getConverterInv()
     */
    static bool read( const XmlNode &node, Units::fptr *fptr );

    /**
     * @brief Reads value from XML file.
     * @param parent parent XML node
     * @param data destination
     * @param name element containing data name
     * @param optional if true then true will be returned if element of given name doesn't exist
     * @return true on success or false on failure.
     */
    template < class TYPE >
    static bool read( const XmlNode &parent, TYPE &data, const std::string name,
                      bool optional = false )
    {
        // empty aircraft mass
        XmlNode node = parent.getFirstChildElement( name );

        if ( node.isValid() )
        {
            TYPE data_temp;

            if ( read( node, data_temp ) )
            {
                data = data_temp;
                return true;
            }
        }
        else if ( optional )
        {
            return true;
        }

        return false;
    }
};

} // end of g1000 namespace

////////////////////////////////////////////////////////////////////////////////

#endif // G1000_XMLUTILS_H
