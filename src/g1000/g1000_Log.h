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
#ifndef G1000_LOG_H
#define G1000_LOG_H

////////////////////////////////////////////////////////////////////////////////

#include <sstream>

////////////////////////////////////////////////////////////////////////////////

namespace g1000
{

/**
 * @brief Logging class.
 */
class Log
{
public:

    static std::ostream &_out;  ///< log output stream

    inline static std::ostream& i() { return ( timeTag() << "[INFO] "    ); }
    inline static std::ostream& w() { return ( timeTag() << "[WARNING] " ); }
    inline static std::ostream& e() { return ( timeTag() << "[ERROR] "   ); }

    inline static std::ostream& out() { return _out; }

    static std::ostream& timeTag();
};

} // end of g1000 namespace

////////////////////////////////////////////////////////////////////////////////

#endif // G1000_LOG_H
