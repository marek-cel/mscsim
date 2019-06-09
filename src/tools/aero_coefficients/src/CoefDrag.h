/***************************************************************************//**
 * @author  Marek M. Cel <marekcel@marekcel.pl>
 *
 * @section LICENSE
 *
 * Copyright (c) 2019 Marek M. Cel
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

    double m_cd_0;      ///< [-]
    double m_cd_1;      ///< [-]
    double m_cd_2;      ///< [-]
    double m_cd_3;      ///< [-]
    double m_cd_4;      ///< [-]
    double m_cd_5;      ///< [-]

    double m_ad_1;      ///< [rad]
    double m_ad_2;      ///< [rad]
    double m_ad_3;      ///< [rad]
    double m_ad_4;      ///< [rad]

    double m_ad_1_2;    ///< [rad^2]
    double m_ad_2_2;    ///< [rad^2]

    double m_div_d1_0;  ///<
    double m_div_d1_1;  ///<
    double m_div_d1_2;  ///<

    double m_div_d2_2;  ///<
    double m_div_d2_3;  ///<
    double m_div_d2_4;  ///<
    double m_div_d2_5;  ///<
};

////////////////////////////////////////////////////////////////////////////////

#endif // COEFDRAG_H
