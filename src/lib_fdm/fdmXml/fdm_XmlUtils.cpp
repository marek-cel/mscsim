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

#include <fdmXml/fdm_XmlUtils.h>

#include <stdio.h>

#include <fdmUtils/fdm_String.h>
#include <fdmUtils/fdm_Units.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

std::string XmlUtils::getErrorInfo( const XmlNode &node )
{
    std::string info;

    info.clear();

    if ( node.isValid() )
    {
        info += "Reading file: \"";
        info += node.getFile();
        info += "\" near line: ";
        info += String::toString( (int)node.getLine() );
        info += " failed.";
    }
    else
    {
        info += "Invalid node.";
    }

    return info;
}

////////////////////////////////////////////////////////////////////////////////

int XmlUtils::read( const XmlNode &node, std::string &str )
{
    if ( node.isValid() )
    {
        XmlNode textNode = node.getFirstChild();

        if ( textNode.isValid() && textNode.isText() )
        {
            str = textNode.getText();
            return FDM_SUCCESS;
        }
    }

    return FDM_FAILURE;
}

////////////////////////////////////////////////////////////////////////////////

int XmlUtils::read( const XmlNode &node, int &data )
{
    if ( node.isValid() )
    {
        XmlNode textNode = node.getFirstChild();

        if ( textNode.isValid() && textNode.isText() )
        {
            std::string text = String::stripLeadingSpaces( textNode.getText() );

            int data_temp = String::toInt( text );

            if ( Misc::isValid( data_temp ) )
            {
                data = data_temp;
                return FDM_SUCCESS;
            }
        }
    }

    return FDM_FAILURE;
}

////////////////////////////////////////////////////////////////////////////////

int XmlUtils::read( const XmlNode &node, double &data )
{
    if ( node.isValid() )
    {
        XmlNode textNode = node.getFirstChild();

        double factor = String::toDouble( node.getAttribute( "factor" ), 1.0 );
        Units::fptr converter = Units::getConverter( node.getAttribute( "unit" ).c_str() );

        if ( textNode.isValid() && textNode.isText() )
        {
            std::string text = String::stripLeadingSpaces( textNode.getText() );

            double data_temp = String::toDouble( text );

            if ( Misc::isValid( data_temp ) )
            {
                data = (*converter)( data_temp ) * factor;
                return FDM_SUCCESS;
            }
        }
    }

    return FDM_FAILURE;
}

////////////////////////////////////////////////////////////////////////////////

int XmlUtils::read( const XmlNode &node, Matrix3x3 &data )
{
    if ( node.isValid() )
    {
        XmlNode textNode = node.getFirstChild();

        if ( textNode.isValid() && textNode.isText() )
        {
            double xx = std::numeric_limits< double >::quiet_NaN();
            double xy = std::numeric_limits< double >::quiet_NaN();
            double xz = std::numeric_limits< double >::quiet_NaN();

            double yx = std::numeric_limits< double >::quiet_NaN();
            double yy = std::numeric_limits< double >::quiet_NaN();
            double yz = std::numeric_limits< double >::quiet_NaN();

            double zx = std::numeric_limits< double >::quiet_NaN();
            double zy = std::numeric_limits< double >::quiet_NaN();
            double zz = std::numeric_limits< double >::quiet_NaN();

            std::string text = String::stripLeadingSpaces( textNode.getText() );

            int valRead = sscanf( text.c_str(),
                                  "%lf %lf %lf %lf %lf %lf %lf %lf %lf",
                                  &xx, &xy, &xz,
                                  &yx, &yy, &yz,
                                  &zx, &zy, &zz );

            if ( valRead == 9 )
            {
                data(0,0) = xx;
                data(0,1) = xy;
                data(0,2) = xz;

                data(1,0) = yx;
                data(1,1) = yy;
                data(1,2) = yz;

                data(2,0) = zx;
                data(2,1) = zy;
                data(2,2) = zz;

                return FDM_SUCCESS;
            }
        }
    }

    return FDM_FAILURE;
}

////////////////////////////////////////////////////////////////////////////////

int XmlUtils::read( const XmlNode &node, Vector3 &data )
{
    if ( node.isValid() )
    {
        XmlNode textNode = node.getFirstChild();

        if ( textNode.isValid() && textNode.isText() )
        {
            double x = std::numeric_limits< double >::quiet_NaN();
            double y = std::numeric_limits< double >::quiet_NaN();
            double z = std::numeric_limits< double >::quiet_NaN();

            std::string text = String::stripLeadingSpaces( textNode.getText() );

            int valRead = sscanf( text.c_str(), "%lf %lf %lf", &x, &y, &z );

            if ( valRead == 3 )
            {
                data.x() = x;
                data.y() = y;
                data.z() = z;

                return FDM_SUCCESS;
            }
        }
    }

    return FDM_FAILURE;
}

////////////////////////////////////////////////////////////////////////////////

int XmlUtils::read( const XmlNode &node, Table &data )
{
    std::vector< double > keyValues;
    std::vector< double > tableData;

    if ( node.isValid() )
    {
        XmlNode textNode = node.getFirstChild();

        double factor = String::toDouble( node.getAttribute( "factor" ), 1.0 );
        Units::fptr converter = Units::getConverter( node.getAttribute( "unit" ).c_str() );

        double factor_keys = String::toDouble( node.getAttribute( "keys_factor" ), 1.0 );
        Units::fptr converter_keys = Units::getConverter( node.getAttribute( "keys_unit" ).c_str() );

        if ( textNode.isValid() && textNode.isText() )
        {
            unsigned int offset = 0;
            unsigned int result = 0;

            std::string text = String::stripLeadingSpaces( textNode.getText() );

            do
            {
                double key = 0.0;
                double val = 0.0;

                unsigned int charsRead = 0;

                result = sscanf( text.c_str() + offset,
                                 "%lf %lf \n%n", &key, &val, &charsRead );

                offset += charsRead;

                if ( result == 2 )
                {
                    keyValues.push_back( (*converter_keys)( key ) * factor_keys );
                    tableData.push_back( (*converter)( val ) * factor );
                }
            }
            while ( result == 2 );

            if ( keyValues.size() == tableData.size() && keyValues.size() )
            {
                data = Table( keyValues, tableData );

                return FDM_SUCCESS;
            }
        }
    }

    return FDM_FAILURE;
}

////////////////////////////////////////////////////////////////////////////////

int XmlUtils::read( const XmlNode &node, Table2D &data )
{
    std::vector< double > colValues;
    std::vector< double > rowValues;
    std::vector< double > tableData;

    if ( node.isValid() )
    {
        XmlNode textNode = node.getFirstChild();

        double factor = String::toDouble( node.getAttribute( "factor" ), 1.0 );
        Units::fptr converter = Units::getConverter( node.getAttribute( "unit" ).c_str() );

        double factor_cols = String::toDouble( node.getAttribute( "cols_factor" ), 1.0 );
        Units::fptr converter_cols = Units::getConverter( node.getAttribute( "cols_unit" ).c_str() );

        double factor_rows = String::toDouble( node.getAttribute( "rows_factor" ), 1.0 );
        Units::fptr converter_rows = Units::getConverter( node.getAttribute( "rows_unit" ).c_str() );

        if ( textNode.isValid() && textNode.isText() )
        {
            unsigned int offset = 0;
            unsigned int result = 0;

            std::string text = String::stripLeadingSpaces( textNode.getText() );

            // column keys
            std::string line = String::getFirstLine( text );

            do
            {
                unsigned int charsRead = 0;

                double key = 0.0;

                result = sscanf( line.c_str() + offset,
                                 "%lf%n", &key, &charsRead );

                offset += charsRead;

                if ( result == 1 )
                {
                    colValues.push_back( (*converter_cols)( key ) * factor_cols );
                }
            }
            while ( result == 1 );

            // table rows
            do
            {
                unsigned int charsRead = 0;

                double key = 0.0;

                result = sscanf( text.c_str() + offset,
                                 "%lf%n", &key, &charsRead );

                offset += charsRead;

                if ( result == 1 )
                {
                    rowValues.push_back( (*converter_rows)( key ) * factor_rows );

                    // table data
                    for ( unsigned int i = 0; i < colValues.size(); i++ )
                    {
                        double val = 0.0;

                        if ( i < colValues.size() - 1 )
                        {
                            result = sscanf( text.c_str() + offset,
                                             "%lf%n", &val, &charsRead );
                        }
                        else
                        {
                            result = sscanf( text.c_str() + offset,
                                             "%lf \n%n", &val, &charsRead );
                        }

                        offset += charsRead;

                        if ( result == 1 )
                        {
                            tableData.push_back( (*converter)( val ) * factor );
                        }
                    }
                }
            }
            while ( result == 1 );

            if ( rowValues.size() * colValues.size() == tableData.size() )
            {
                data = Table2D( rowValues, colValues, tableData );
                return FDM_SUCCESS;
            }
        }
    }

    return FDM_FAILURE;
}
