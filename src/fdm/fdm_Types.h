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
#ifndef FDM_TYPES_H
#define FDM_TYPES_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Defines.h>

#if __cplusplus > 199711L
#   include <stdint.h>
#endif

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

#if __cplusplus > 199711L
typedef uint8_t        UInt8;   ///< 8-bits unsigned integer type
typedef uint16_t       UInt16;  ///< 16-bits unsigned integer type
typedef uint32_t       UInt32;  ///< 32-bits unsigned integer type
#else
typedef unsigned char  UInt8;   ///< 8-bits unsigned integer type
typedef unsigned short UInt16;  ///< 16-bits unsigned integer type
typedef unsigned int   UInt32;  ///< 32-bits unsigned integer type
#endif

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_TYPES_H
