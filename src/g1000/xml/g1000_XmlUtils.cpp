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

#include <g1000/xml/g1000_XmlUtils.h>

#include <cstdio>

#include <g1000/utils/g1000_Misc.h>
#include <g1000/utils/g1000_String.h>

////////////////////////////////////////////////////////////////////////////////

using namespace g1000;

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
        //info += String::toString( (int)node.getLine() );
        info += " failed.";
    }
    else
    {
        info += "Invalid node.";
    }

    return info;
}

////////////////////////////////////////////////////////////////////////////////

bool XmlUtils::read( const XmlNode &node, std::string &str )
{
    if ( node.isValid() )
    {
        XmlNode textNode = node.getFirstChild();

        if ( textNode.isValid() && textNode.isText() )
        {
            str = textNode.getText();
            return true;
        }
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool XmlUtils::read( const XmlNode &node, int &data )
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
                return true;
            }
        }
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool XmlUtils::read( const XmlNode &node, double &data )
{
    if ( node.isValid() )
    {
        XmlNode textNode = node.getFirstChild();

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
                        return false;
                }

                data = data_temp;
                return true;
            }
        }
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool XmlUtils::read( const XmlNode &node, Units::fptr *fptr )
{
    if ( node.isValid() )
    {
        Units::fptr converter = Units::getConverter( node.getAttribute( "unit" ).c_str() );

        if ( converter )
        {
            (*fptr) = converter;
            return true;
        }
    }

    return false;
}
