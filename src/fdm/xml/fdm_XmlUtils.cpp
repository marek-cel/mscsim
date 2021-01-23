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

#include <fdm/xml/fdm_XmlUtils.h>

//#include <cstdio>

#include <fdm/utils/fdm_String.h>
#include <fdm/utils/fdm_Units.h>

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

int XmlUtils::read( const XmlNode &node, std::string *str )
{
    if ( node.isValid() )
    {
        XmlNode textNode = node.getFirstChild();

        if ( textNode.isValid() && textNode.isText() )
        {
            (*str) = textNode.getText();
            return FDM_SUCCESS;
        }
    }

    return FDM_FAILURE;
}

////////////////////////////////////////////////////////////////////////////////

int XmlUtils::read( const XmlNode &node, int *data )
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
                (*data) = data_temp;
                return FDM_SUCCESS;
            }
        }
    }

    return FDM_FAILURE;
}

////////////////////////////////////////////////////////////////////////////////

int XmlUtils::read( const XmlNode &node, double *data )
{
    if ( node.isValid() )
    {
        XmlNode textNode = node.getFirstChild();

        double factor = String::toDouble( node.getAttribute( "factor" ), 1.0 );

        if ( textNode.isValid() && textNode.isText() )
        {
            std::string text = String::stripLeadingSpaces( textNode.getText() );

            double data_temp = String::toDouble( text );

            if ( Misc::isValid( data_temp ) )
            {
                if ( node.hasAttribute( "unit" ) )
                {
                    Units::fptr converter = Units::getConverter( node.getAttribute( "unit" ).c_str() );

                    if ( converter )
                        data_temp = (*converter)( data_temp );
                    else
                        return FDM_FAILURE;
                }

                (*data) = data_temp * factor;
                return FDM_SUCCESS;
            }
        }
    }

    return FDM_FAILURE;
}

////////////////////////////////////////////////////////////////////////////////

int XmlUtils::read( const XmlNode &node, Matrix3x3 *data )
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

            int val_read = sscanf( text.c_str(),
                                   "%lf %lf %lf %lf %lf %lf %lf %lf %lf",
                                   &xx, &xy, &xz,
                                   &yx, &yy, &yz,
                                   &zx, &zy, &zz );

            if ( val_read == 9 )
            {
                data->xx() = xx;
                data->xy() = xy;
                data->xz() = xz;

                data->yx() = yx;
                data->yy() = yy;
                data->yz() = yz;

                data->zx() = zx;
                data->zy() = zy;
                data->zz() = zz;

                return FDM_SUCCESS;
            }
        }
    }

    return FDM_FAILURE;
}

////////////////////////////////////////////////////////////////////////////////

int XmlUtils::read( const XmlNode &node, Vector3 *data )
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

            int val_read = sscanf( text.c_str(), "%lf %lf %lf", &x, &y, &z );

            if ( val_read == 3 )
            {
                if ( node.hasAttribute( "unit" ) )
                {
                    Units::fptr converter = Units::getConverter( node.getAttribute( "unit" ).c_str() );

                    if ( converter )
                    {
                        x = (*converter)( x );
                        y = (*converter)( y );
                        z = (*converter)( z );
                    }
                    else
                    {
                        return FDM_FAILURE;
                    }
                }

                data->x() = x;
                data->y() = y;
                data->z() = z;

                return FDM_SUCCESS;
            }
        }
    }

    return FDM_FAILURE;
}

////////////////////////////////////////////////////////////////////////////////

int XmlUtils::read( const XmlNode &node, Table1 *table )
{
    std::vector< double > keyValues;
    std::vector< double > tableData;

    if ( node.isValid() )
    {
        XmlNode textNode = node.getFirstChild();

        double factor = String::toDouble( node.getAttribute( "factor" ), 1.0 );
        bool has_unit = node.hasAttribute( "unit" );
        Units::fptr converter = Units::getConverter( node.getAttribute( "unit" ).c_str() );

        if ( textNode.isValid() && textNode.isText() )
        {
            unsigned int offset = 0;
            unsigned int result = 0;

            std::string text = String::stripLeadingSpaces( textNode.getText() );

            do
            {
                double key = 0.0;
                double val = 0.0;

                unsigned int ch_read = 0;

                result = sscanf( text.c_str() + offset,
                                 "%lf %lf \n%n", &key, &val, &ch_read );

                offset += ch_read;

                if ( result == 2 )
                {
                    if ( has_unit )
                    {
                        if ( converter )
                            val = (*converter)( val );
                        else
                            return FDM_FAILURE;
                    }

                    keyValues.push_back( key );
                    tableData.push_back( val * factor );
                }
            }
            while ( result == 2 );

            if ( keyValues.size() == tableData.size() && keyValues.size() )
            {
                (*table) = Table1( keyValues, tableData );

                return FDM_SUCCESS;
            }
        }
    }

    return FDM_FAILURE;
}

////////////////////////////////////////////////////////////////////////////////

int XmlUtils::read( const XmlNode &node, Table2 *table )
{
    std::vector< double > colValues;
    std::vector< double > rowValues;
    std::vector< double > tableData;

    if ( node.isValid() )
    {
        XmlNode textNode = node.getFirstChild();

        double factor = String::toDouble( node.getAttribute( "factor" ), 1.0 );
        bool has_unit = node.hasAttribute( "unit" );
        Units::fptr converter = Units::getConverter( node.getAttribute( "unit" ).c_str() );

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
                    colValues.push_back( key );
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
                    rowValues.push_back( key );

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
                            if ( has_unit )
                            {
                                if ( converter )
                                    val = (*converter)( val );
                                else
                                    return FDM_FAILURE;
                            }

                            tableData.push_back( val * factor );
                        }
                    }
                }
            }
            while ( result == 1 );

            if ( rowValues.size() * colValues.size() == tableData.size() )
            {
                (*table) = Table2( rowValues, colValues, tableData );
                return FDM_SUCCESS;
            }
        }
    }

    return FDM_FAILURE;
}

////////////////////////////////////////////////////////////////////////////////

int XmlUtils::read( const XmlNode &node, PID *pid, double min, double max )
{
    if ( node.isValid() )
    {
        int result = FDM_SUCCESS;

        double kp = 0.0;
        double ki = 0.0;
        double kd = 0.0;

        double kaw = 0.0;

        double temp_min = min;
        double temp_max = max;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( node, &kp, "kp", true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( node, &ki, "ki", true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( node, &kd, "kd", true );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( node, &kaw, "kaw", true );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( node, &temp_min, "min", true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( node, &temp_max, "max", true );

        if ( result == FDM_SUCCESS )
        {
            if ( temp_min < min ) temp_min = min;
            if ( temp_max > max ) temp_max = max;

            pid->setKp( kp );
            pid->setKi( ki );
            pid->setKd( kd );

            pid->setKaw( kaw );

            pid->setMin( temp_min );
            pid->setMax( temp_max );

            return FDM_SUCCESS;
        }
    }

    return FDM_FAILURE;
}

////////////////////////////////////////////////////////////////////////////////

void XmlUtils::throwError( const char *file, int line, const XmlNode &node )
{
    Exception e;

    e.setType( Exception::FileReadingError );
    e.setInfo( "Reading XML file failed. " + XmlUtils::getErrorInfo( node ) );

    e.setFile( file );
    e.setLine( line );

    throw e;
}
