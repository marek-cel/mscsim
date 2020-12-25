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

#include <fdm/models/fdm_WingRunner.h>

#include <fdm/utils/fdm_String.h>
#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

WingRunner::WingRunner() :
    _k ( 0.0 ),
    _c ( 0.0 ),

    _active ( true )
{}

////////////////////////////////////////////////////////////////////////////////

WingRunner::~WingRunner() {}

////////////////////////////////////////////////////////////////////////////////

void WingRunner::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_r_w_bas, "wing" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_r_f_bas, "feet" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_k, "stiffness" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_c, "damping"   );

        if ( result != FDM_SUCCESS ) XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
    else
    {
        XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
}

////////////////////////////////////////////////////////////////////////////////

void WingRunner::computeForceAndMoment( const Vector3 &vel_bas,
                                        const Vector3 &omg_bas,
                                        const Vector3 &r_c_bas,
                                        const Vector3 &n_c_bas )
{
    _for_bas.zeroize();
    _mom_bas.zeroize();

    if ( _active )
    {
        double deflection_norm = n_c_bas * ( r_c_bas - _r_f_bas );

        if ( deflection_norm > 1.0e-6 )
        {
            // contact point velocities components
            Vector3 v_c_bas = vel_bas + ( omg_bas % r_c_bas );
            double v_norm = n_c_bas * v_c_bas;

            // normal force
            double for_norm = _k * deflection_norm - _c * v_norm;

            // resulting forces
            _for_bas = for_norm * n_c_bas;
            _mom_bas = r_c_bas % _for_bas;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void WingRunner::update( double timeStep, const Vector3 &vel_bas, bool onGround )
{
    if ( _active )
    {
        if ( timeStep > FDM_TIME_STEP_MIN )
        {
            if ( vel_bas.getLength() > 1.0 || ( !onGround ) )
            {
                _active = false;
            }
        }
    }
}
