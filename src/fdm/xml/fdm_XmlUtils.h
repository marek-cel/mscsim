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
#ifndef FDM_XMLUTILS_H
#define FDM_XMLUTILS_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/utils/fdm_Matrix3x3.h>
#include <fdm/utils/fdm_Table.h>
#include <fdm/utils/fdm_Table2D.h>
#include <fdm/utils/fdm_Vector3.h>

#include <fdm/xml/fdm_XmlNode.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief XML utilities class.
 */
class FDMEXPORT XmlUtils
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
     * @return FDM_SUCCESS on success or FDM_FAILURE on failure.
     */
    static int read( const XmlNode &node, std::string &str );

    /**
     * @brief Reads value from XML file.
     * @param node XML node
     * @param data destination
     * @return FDM_SUCCESS on success or FDM_FAILURE on failure.
     */
    static int read( const XmlNode &node, int &data );

    /**
     * @brief Reads value from XML file.
     * @param node XML node
     * @param data destination
     * @return FDM_SUCCESS on success or FDM_FAILURE on failure.
     *
     * @code
     * <tag_name [factor="{ value }"] [unit="{ unit string }"]> { value } </tag_name>
     * @endcode
     *
     * @see fdm::Units::getFactorSI()
     */
    static int read( const XmlNode &node, double &data );

    /**
     * @brief Reads value from XML file.
     * @param node XML node
     * @param data destination
     * @return FDM_SUCCESS on success or FDM_FAILURE on failure.
     *
     * @code
     * <tag_name [factor="{ value }"] [unit="{ unit string }"]>
     *   { i_xx } { i_xy } { i_xz }
     *   { i_yx } { i_yy } { i_yz }
     *   { i_zx } { i_zy } { i_zz }
     * </tag_name>
     * @endcode
     *
     * @see fdm::Units::getFactorSI()
     */
    static int read( const XmlNode &node, Matrix3x3 &data );

    /**
     * @brief Reads value from XML file.
     * @param node XML node
     * @param data destination
     * @return FDM_SUCCESS on success or FDM_FAILURE on failure.
     *
     * @code
     * <tag_name [factor="{ value }"] [unit="{ unit string }"]>
     *   { x-value } { y-value } { z-value }
     * </tag_name>
     * @endcode
     *
     * @see fdm::Units::getFactorSI()
     */
    static int read( const XmlNode &node, Vector3 &data );

    /**
     * @brief Reads value from XML file.
     * @param node XML node
     * @param data destination
     * @return FDM_SUCCESS on success or FDM_FAILURE on failure.
     *
     * @code
     * <tag_name [factor="{ value }] [unit="{ unit string }"] [keys_factor="{ value }] [keys_unit="{ unit string }"]>
     *   { key } { value }
     *   ... { more entries }
     * </tag_name>
     * @endcode
     *
     * @see fdm::Units::getFactorSI()
     */
    static int read( const XmlNode &node, Table &data );

    /**
     * @brief Reads value from XML file.
     * @param node XML node
     * @param data destination
     * @return FDM_SUCCESS on success or FDM_FAILURE on failure.
     *
     * @code
     * <tag_name [factor="{ value }] [unit="{ unit string }"] [cols_factor="{ value }] [cols_unit="{ unit string }] [rows_factor="{ value }] [rows_unit="{ unit string }"]>
     *   { column key } ... { more column keys }
     *    { row key } { value } ... { more values }
     *   ... { more entries }
     * </tag_name>
     * @endcode
     *
     * @see fdm::Units::getFactorSI()
     */
    static int read( const XmlNode &node, Table2D &data );

    /**
     * @brief Reads value from XML file.
     * @param parent parent XML node
     * @param data destination
     * @param name element containing data name
     * @param optional if true then FDM_SUCCESS will be returned if element of given name doesn't exist
     * @return FDM_SUCCESS on success or FDM_FAILURE on failure.
     */
    template < class TYPE >
    static int read( const XmlNode &parent, TYPE &data, const std::string name,
                     bool optional = false )
    {
        // empty aircraft mass
        XmlNode node = parent.getFirstChildElement( name );

        if ( node.isValid() )
        {
            TYPE data_temp;

            if ( FDM_SUCCESS == read( node, data_temp ) )
            {
                data = data_temp;
                return FDM_SUCCESS;
            }
        }
        else if ( optional )
        {
            return FDM_SUCCESS;
        }

        return FDM_FAILURE;
    }

    /**
     * @brief Throws FileReadingError error
     * @param file
     * @param line
     * @param node
     */
    static void throwError( const char *file, int line, const XmlNode &node );
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_XMLUTILS_H
