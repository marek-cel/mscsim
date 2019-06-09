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

#include <fdm/utils/fdm_Vector6.h>

#include <math.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Vector6::Vector6() :
    Vector< 6 >()
{}

////////////////////////////////////////////////////////////////////////////////

Vector6::Vector6( const Vector6 &vect ) :
    Vector< 6 >( vect )
{}

////////////////////////////////////////////////////////////////////////////////

Vector6::Vector6( const double items[] ) :
    Vector< 6 >( items )
{}

////////////////////////////////////////////////////////////////////////////////

Vector6 Vector6::getNormalized() const
{
    Vector6 result( *this );

    result.normalize();

    return result;
}

////////////////////////////////////////////////////////////////////////////////

const Vector6& Vector6::operator= ( const Vector6 &vect )
{
    setArray( vect.m_items );
    
    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

Vector6 Vector6::operator+ ( const Vector6 &vect ) const
{
    Vector6 result;
    
    for ( unsigned int i = 0; i < m_size; i++ )
    {
        result.m_items[ i ] = m_items[ i ] + vect.m_items[ i ];
    }
    
    return result;
}

////////////////////////////////////////////////////////////////////////////////

Vector6 Vector6::operator- () const
{
    Vector6 result;
    
    for ( unsigned int i = 0; i < m_size; i++ )
    {
        result.m_items[ i ] = - m_items[ i ];
    }
    
    return result;
}

////////////////////////////////////////////////////////////////////////////////

Vector6 Vector6::operator- ( const Vector6 &vect ) const
{
    Vector6 result;
    
    for ( unsigned int i = 0; i < m_size; i++ )
    {
        result.m_items[ i ] = m_items[ i ] - vect.m_items[ i ];
    }
    
    return result;
}

////////////////////////////////////////////////////////////////////////////////

Vector6 Vector6::operator* ( double val ) const
{
    Vector6 result;
    
    for ( unsigned int i = 0; i < m_size; i++ )
    {
        result.m_items[ i ] = m_items[ i ] * val;
    }
    
    return result;
}

////////////////////////////////////////////////////////////////////////////////

Vector6 Vector6::operator/ ( double val ) const
{
    Vector6 result;
    
    for ( unsigned int i = 0; i < m_size; i++ )
    {
        result.m_items[ i ] = m_items[ i ] / val;
    }
    
    return result;
}

////////////////////////////////////////////////////////////////////////////////

Vector6& Vector6::operator+= ( const Vector6 &vect )
{
    for ( unsigned int i = 0; i < m_size; i++ )
    {
        m_items[ i ] += vect.m_items[ i ];
    }
    
    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

Vector6& Vector6::operator-= ( const Vector6 &vect )
{
    for ( unsigned int i = 0; i < m_size; i++ )
    {
        m_items[ i ] -= vect.m_items[ i ];
    }
    
    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

Vector6& Vector6::operator*= ( double val )
{
    for ( unsigned int i = 0; i < m_size; i++ )
    {
        m_items[ i ] *= val;
    }
    
    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

Vector6& Vector6::operator/= ( double val )
{
    for ( unsigned int i = 0; i < m_size; i++ )
    {
        m_items[ i ] /= val;
    }
    
    return (*this);
}
