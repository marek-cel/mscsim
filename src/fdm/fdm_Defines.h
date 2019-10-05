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
#ifndef FDM_DEFINES_H
#define FDM_DEFINES_H

////////////////////////////////////////////////////////////////////////////////

#if defined(_MSC_VER)
#   if defined(FDM_DLL_EXPORTS)
#       define FDM_DLL_SPEC __declspec(dllexport)
#   elif defined(FDM_DLL_IMPORTS)
#       define FDM_DLL_SPEC __declspec(dllimport)
#   else
#       define FDM_DLL_SPEC
#   endif
#else
#   define FDM_DLL_SPEC
#endif

#if defined(__cplusplus)
#   define FDMEXPORT FDM_DLL_SPEC
#endif

#if !defined(FDMEXPORT)
#   define FDMEXPORT
#endif

////////////////////////////////////////////////////////////////////////////////

#define FDM_TIME_STEP 0.01  /* 100 Hz */

#define FDM_TIME_STEP_MIN 0.001
#define FDM_TIME_STEP_MAX 0.1

////////////////////////////////////////////////////////////////////////////////

#define FDM_MAX_ENGINES 4

////////////////////////////////////////////////////////////////////////////////

#define FDM_MIN_INIT_ALTITUDE 30.0
#define FDM_MAX_INIT_STEPS 20000

////////////////////////////////////////////////////////////////////////////////

#define FDM_SUCCESS 0
#define FDM_FAILURE 1

////////////////////////////////////////////////////////////////////////////////

#define FDM_STATE_DIMENSION 13

////////////////////////////////////////////////////////////////////////////////

#if defined(_MSC_VER) && _MSC_VER < 1600
#   define FDM_NULLPTR 0
#else
#   define FDM_NULLPTR nullptr
#endif

////////////////////////////////////////////////////////////////////////////////

#define FDM_COUT( x ) \
{ \
    std::cout << __FILE__ << "(" << __LINE__ << ") "#x"= " << x << std::endl; \
}

////////////////////////////////////////////////////////////////////////////////

#define FDM_COUT_FL \
{ \
    std::cout << __FILE__ << "(" << __LINE__ << ")" << std::endl; \
}

////////////////////////////////////////////////////////////////////////////////

#define FDM_DELETE( ptr ) \
{ \
    if ( ptr ) delete ptr; \
    ptr = FDM_NULLPTR; \
}

////////////////////////////////////////////////////////////////////////////////

#define FDM_DELETE_TAB( ptr ) \
{ \
    if ( ptr ) delete [] ptr; \
    ptr = FDM_NULLPTR; \
}

////////////////////////////////////////////////////////////////////////////////

#define FDM_THROW( e ) \
{ \
    e.setFile( __FILE__ ); \
    e.setLine( __LINE__ ); \
    throw e; \
}

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_DEFINES_H
