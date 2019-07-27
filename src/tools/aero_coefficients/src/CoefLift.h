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
#ifndef COEFLIFT_H
#define COEFLIFT_H

////////////////////////////////////////////////////////////////////////////////

/**
 * Lift coefficient approximation class.
 * @see Takahashi M.: A Flight-Dynamic Helicopter Mathematical Model with a Single Flap-Lag-Torsion Main Rotor, NASA, TM-102267, p.68
 */
class CoefLift
{
public:

    CoefLift( double cl_s, double cl_1, double cl_2,
              double al_s, double al_1, double al_2 );

    double get( double aoa );

private:

    double m_cl_0;      ///< [-]

    double m_cl_s;      ///< [-]
    double m_cl_1;      ///< [-]
    double m_cl_2;      ///< [-]

    double m_al_s;      ///< [rad]
    double m_al_1;      ///< [rad]
    double m_al_2;      ///< [rad]

    double m_dcl_da_s;  ///< [1/rad]
    double m_dcl_da_1;  ///< [1/rad]

    double m_div_l1_1;  ///<
    double m_div_l1_2;  ///<
};

////////////////////////////////////////////////////////////////////////////////

#endif // COEFLIFT_H
