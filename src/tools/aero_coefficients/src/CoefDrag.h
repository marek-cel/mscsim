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
#ifndef COEFDRAG_H
#define COEFDRAG_H

////////////////////////////////////////////////////////////////////////////////

/**
 * Drag coefficient approximation class.
 * @see Takahashi M.: A Flight-Dynamic Helicopter Mathematical Model with a Single Flap-Lag-Torsion Main Rotor, NASA, TM-102267, p.67
 */
class CoefDrag
{
public:

    CoefDrag( double cd_0, double cd_1, double cd_2, double cd_3, double cd_4, double cd_5,
              double ad_1, double ad_2, double ad_3, double ad_4 );

    double get( double aoa );

private:

    double _cd_0;       ///< [-]
    double _cd_1;       ///< [-]
    double _cd_2;       ///< [-]
    double _cd_3;       ///< [-]
    double _cd_4;       ///< [-]
    double _cd_5;       ///< [-]

    double _ad_1;       ///< [rad]
    double _ad_2;       ///< [rad]
    double _ad_3;       ///< [rad]
    double _ad_4;       ///< [rad]

    double _ad_1_2;     ///< [rad^2]
    double _ad_2_2;     ///< [rad^2]

    double _div_d1_0;   ///<
    double _div_d1_1;   ///<
    double _div_d1_2;   ///<

    double _div_d2_2;   ///<
    double _div_d2_3;   ///<
    double _div_d2_4;   ///<
    double _div_d2_5;   ///<
};

////////////////////////////////////////////////////////////////////////////////

#endif // COEFDRAG_H
