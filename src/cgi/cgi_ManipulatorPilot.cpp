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

#include <cgi/cgi_ManipulatorPilot.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

ManipulatorPilot::ManipulatorPilot() :
    inherited()
{}

////////////////////////////////////////////////////////////////////////////////

ManipulatorPilot::ManipulatorPilot( const ManipulatorPilot &cmi,
                                    const osg::CopyOp &copyOp ) :
                                    osg::Object( cmi, copyOp ),
                                    inherited( cmi, copyOp ),
                                    m_matrix( cmi.m_matrix )
{}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorPilot::setByMatrix( const osg::Matrixd &matrix )
{
    m_matrix = matrix;
}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorPilot::setByInverseMatrix( const osg::Matrixd &matrix )
{
    setByMatrix( osg::Matrixd::inverse( matrix ) );
}

////////////////////////////////////////////////////////////////////////////////

osg::Matrixd ManipulatorPilot::getMatrix() const
{
    return m_matrix;
}

////////////////////////////////////////////////////////////////////////////////

osg::Matrixd ManipulatorPilot::getInverseMatrix() const
{
    return osg::Matrixd::inverse( m_matrix );
}

////////////////////////////////////////////////////////////////////////////////

bool ManipulatorPilot::handle( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &/*us*/ )
{
    switch ( ea.getEventType() )
    {
    case osgGA::GUIEventAdapter::FRAME:
        updateMatrix();
        return false;
        break;
    default:
        break;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorPilot::updateMatrix() {}
