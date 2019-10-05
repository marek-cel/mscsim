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
#ifndef FDM_EXCEPTION_H
#define FDM_EXCEPTION_H

////////////////////////////////////////////////////////////////////////////////

#include <sstream>
#include <string>

#include <fdm/fdm_Defines.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Exception class.
 */
class FDMEXPORT Exception
{
public:

    /** Exceptions types. */
    enum Type
    {
        UnknownException    = 0,    ///< undefined exception
        ExceptionCatched    = 1,    ///< another exception catched
        NullPointer         = 2,    ///< unexpected null pointer
        ArrayIndexOverLimit = 3,    ///< wrong value of array index
        UnexpectedNaN       = 4,    ///< unexpected Not-a-Number value
        FileReadingError    = 5     ///< file reading error
    };

    /** Constructor. */
    Exception() :
        _cause ( FDM_NULLPTR )
    {
        _type = UnknownException;
        _line = 0;
        _file = "";
        _info = "";
    }

    /** Copy constructor. */
    Exception( const Exception &e ) :
        _cause ( ( e._cause ) ? new Exception( *e._cause ) : 0 )
    {
        _type = e._type;
        _line = e._line;
        _file = e._file;
        _info = e._info;
    }

    /** Destructor. */
    virtual ~Exception()
    {
        removeCause();
    }

    /** */
    inline bool hasCause() const
    {
        return ( _cause ) ? true : false;
    }

    /** */
    inline const Exception& getCause() const
    {
        return (*_cause);
    }

    /** */
    inline std::string getFile() const
    {
        return _file;
    }

    /** */
    inline std::string getFileAndLine() const
    {
        std::stringstream ss;
        ss << _file << "(" << _line << ")";
        return ss.str();
    }

    /** */
    inline std::string getInfo() const
    {
        return _info;
    }

    /** */
    inline int getLine() const
    {
        return _line;
    }

    /** */
    inline Type getType() const
    {
        return _type;
    }

    /** */
    inline void removeCause()
    {
        FDM_DELETE( _cause );
    }

    /** */
    inline void setCause( Exception &cause )
    {
        removeCause();

        _cause = new Exception( cause );
    }

    /** */
    inline void setFile( const char *file )
    {
        if ( file != 0 )
        {
            _file = file;
        }
    }

    /** */
    inline void setInfo( const char *info )
    {
        _info = info;
    }

    /** */
    inline void setInfo( std::string info )
    {
        _info = info;
    }

    /** */
    inline void setLine( int line )
    {
        _line = line;
    }

    /** */
    inline void setType( Type type )
    {
        _type = type;
    }

    /** */
    inline std::string toString() const
    {
        std::stringstream ss;
        if ( hasCause() ) ss << _cause->toString();
        ss << _info << " " << getFileAndLine() << std::endl;
        return ss.str();
    }

private:

    Exception *_cause;          ///< exception cause (if exception triggered by catching other exception)

    Type _type;                 ///< exception type
    int _line;                  ///< line number
    std::string _file;          ///< file name
    std::string _info;          ///< extra informations
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_EXCEPTION_H
