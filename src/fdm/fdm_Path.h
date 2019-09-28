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
#ifndef FDM_PATH_H
#define FDM_PATH_H

////////////////////////////////////////////////////////////////////////////////

#include <string>

#include <fdm/fdm_Defines.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Files path class.
 */
class FDMEXPORT Path
{
public:

    /**
     * Returns file path prefixed with base path.
     * @param path file path relative to the base path.
     */
    inline static std::string get( const std::string &path = "" )
    {
#       if defined _LINUX_  && !defined SIM_LOCAL_DATA_DIR
        return "/usr/share/mscsim/" + path;
#       else
        return path;
#       endif
    }
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_PATH_H
