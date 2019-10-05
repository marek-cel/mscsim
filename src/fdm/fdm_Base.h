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
#ifndef FDM_BASE_H
#define FDM_BASE_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Defines.h>
#include <fdm/fdm_Path.h>
#include <fdm/fdm_Types.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

class XmlNode;  ///< XmlNode forward declaration

/**
 * @brief Base class.
 */
class FDMEXPORT Base
{
public:

    static const UInt8 _ix;     ///< index of vector x-coordinate
    static const UInt8 _iy;     ///< index of vector x-coordinate
    static const UInt8 _iz;     ///< index of vector x-coordinate

    static const UInt8 _iu;     ///< index of vector u-coordinate (velocity)
    static const UInt8 _iv;     ///< index of vector v-coordinate (velocity)
    static const UInt8 _iw;     ///< index of vector w-coordinate (velocity)

    static const UInt8 _ip;     ///< index of vector p-coordinate (angular velocity)
    static const UInt8 _iq;     ///< index of vector q-coordinate (angular velocity)
    static const UInt8 _ir;     ///< index of vector r-coordinate (angular velocity)

    static const UInt8 _is_x;   ///< index of aircraft location x-coordinate (origin of BAS axis system) expressed in WGS axis system
    static const UInt8 _is_y;   ///< index of aircraft location y-coordinate (origin of BAS axis system) expressed in WGS axis system
    static const UInt8 _is_z;   ///< index of aircraft location z-coordinate (origin of BAS axis system) expressed in WGS axis system
    static const UInt8 _is_e0;  ///< index of aircraft attitude quaternion e0-coordinate
    static const UInt8 _is_ex;  ///< index of aircraft attitude quaternion ex-coordinate
    static const UInt8 _is_ey;  ///< index of aircraft attitude quaternion ey-coordinate
    static const UInt8 _is_ez;  ///< index of aircraft attitude quaternion ez-coordinate
    static const UInt8 _is_u;   ///< index of aircraft linear velocity x-coordinate expressed in BAS axis system
    static const UInt8 _is_v;   ///< index of aircraft linear velocity y-coordinate expressed in BAS axis system
    static const UInt8 _is_w;   ///< index of aircraft linear velocity z-coordinate expressed in BAS axis system
    static const UInt8 _is_p;   ///< index of aircraft angular velocity x-coordinate expressed in BAS axis system
    static const UInt8 _is_q;   ///< index of aircraft angular velocity y-coordinate expressed in BAS axis system
    static const UInt8 _is_r;   ///< index of aircraft angular velocity z-coordinate expressed in BAS axis system
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_BASE_H
