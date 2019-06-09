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

#include <ostream>
#include <stdio.h>
#include <string>

#include <fdm/fdm_Defines.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief The Exception class.
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
        m_cause ( 0 )
    {
        m_type = UnknownException;
        m_line = 0;
        m_file = "";
        m_info = "";
    }

    /** Copy constructor. */
    Exception( const Exception &e ) :
        m_cause ( ( e.m_cause ) ? new Exception( *e.m_cause ) : 0 )
    {
        m_type = e.m_type;
        m_line = e.m_line;
        m_file = e.m_file;
        m_info = e.m_info;
    }

    /** Destructor. */
    virtual ~Exception()
    {
        removeCause();
    }

    /** */
    inline bool hasCause() const
    {
        return ( m_cause ) ? true : false;
    }

    /** */
    inline const Exception& getCause() const
    {
        return (*m_cause);
    }

    /** */
    inline std::string getFile() const
    {
        return m_file;
    }

    /** */
    inline std::string getFileAndLine() const
    {
        if ( m_file.size() > 0 && m_line > 0 )
        {
            char line[255];
            sprintf( line, "%d", m_line );

            return m_file + "(" + line + ")";
        }
        else
        {
            return std::string();
        }
    }

    /** */
    inline std::string getInfo() const
    {
        return m_info;
    }

    /** */
    inline int getLine() const
    {
        return m_line;
    }

    /** */
    inline Type getType() const
    {
        return m_type;
    }

    /** */
    inline void removeCause()
    {
        if ( m_cause ) delete m_cause;
        m_cause = 0;
    }

    /** */
    inline void setCause( Exception &cause )
    {
        removeCause();

        m_cause = new Exception( cause );
    }

    /** */
    inline void setFile( const char *file )
    {
        if ( file != 0 )
        {
            m_file = file;
        }
    }

    /** */
    inline void setInfo( const char *info )
    {
        m_info = info;
    }

    /** */
    inline void setInfo( std::string info )
    {
        m_info = info;
    }

    /** */
    inline void setLine( int line )
    {
        m_line = line;
    }

    /** */
    inline void setType( Type type )
    {
        m_type = type;
    }

private:

    Exception *m_cause;         ///< exception cause (if exception triggered by catching other exception)

    Type m_type;                ///< exception type
    int m_line;                 ///< line number
    std::string m_file;         ///< file name
    std::string m_info;         ///< extra informations
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

/** */
inline std::ostream& operator<< ( std::ostream &s, const fdm::Exception &e )
{
    if ( e.hasCause() )
    {
        s << e.getCause();
    }

    s << "ERROR!";
    s << " ";
    s << e.getInfo();
    s << " ";
    s << e.getFileAndLine();
    s << std::endl;

    return s;
}

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_EXCEPTION_H
