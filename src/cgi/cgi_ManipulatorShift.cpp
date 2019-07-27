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

#include <cgi/cgi_ManipulatorShift.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

ManipulatorShift::ManipulatorShift() :
    inherited(),
    m_distance ( 5.0 ),
    m_distance_min ( 5.0 ),
    m_distance_max ( 5000.0 )
{}

////////////////////////////////////////////////////////////////////////////////

ManipulatorShift::ManipulatorShift( const ManipulatorShift &cmi,
                                    const osg::CopyOp &copyOp ) :
    osg::Object( cmi, copyOp ),
    inherited( cmi, copyOp ),
    m_matrix( cmi.m_matrix ),
    m_distance ( cmi.m_distance ),
    m_distance_min ( cmi.m_distance_min ),
    m_distance_max ( cmi.m_distance_max )
{}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorShift::setByMatrix( const osg::Matrixd &matrix )
{
    m_matrix = matrix;
}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorShift::setByInverseMatrix( const osg::Matrixd &matrix )
{
    setByMatrix( osg::Matrixd::inverse( matrix ) );
}

////////////////////////////////////////////////////////////////////////////////

osg::Matrixd ManipulatorShift::getMatrix() const
{
    return m_matrix;
}

////////////////////////////////////////////////////////////////////////////////

osg::Matrixd ManipulatorShift::getInverseMatrix() const
{
    return osg::Matrixd::inverse( m_matrix );
}

////////////////////////////////////////////////////////////////////////////////

bool ManipulatorShift::handle( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &/*us*/ )
{
    switch ( ea.getEventType() )
    {
    case osgGA::GUIEventAdapter::FRAME:
        updateMatrix();
        return false;
        break;

    case osgGA::GUIEventAdapter::SCROLL:
        switch ( ea.getScrollingMotion() )
        {
            case osgGA::GUIEventAdapter::SCROLL_UP:   setDistance( m_distance - 0.1 * m_distance ); break;
            case osgGA::GUIEventAdapter::SCROLL_DOWN: setDistance( m_distance + 0.1 * m_distance ); break;
            default: break;
        }
        return false;
        break;

    default:
        break;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorShift::setDistance( double distance )
{
    if ( distance > m_distance_max )
    {
        m_distance = m_distance_max;
    }
    else if ( distance < m_distance_min )
    {
        m_distance = m_distance_min;
    }
    else
    {
        m_distance = distance;
    }
}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorShift::setDistanceMin( double distance_min )
{
    if ( distance_min > 0.0 && distance_min < m_distance_max )
    {
        m_distance_min = distance_min;
    }

    boundDistance();
}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorShift::setDistanceMax( double distance_max )
{
    if ( distance_max > 0.0 && distance_max > m_distance_min )
    {
        m_distance_max = distance_max;
    }

    boundDistance();
}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorShift::boundDistance()
{
    if ( getDistance() > m_distance_max )
    {
        setDistance( m_distance_max );
    }
    else if ( getDistance() < m_distance_min )
    {
        setDistance( m_distance_min );
    }
}


////////////////////////////////////////////////////////////////////////////////

void ManipulatorShift::updateMatrix() {}
