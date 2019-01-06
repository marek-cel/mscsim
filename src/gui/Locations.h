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
#ifndef LOCATIONS_H
#define LOCATIONS_H

////////////////////////////////////////////////////////////////////////////////

#include <QDomElement>
#include <QVector>

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Locations class.
 */
class Locations
{
public:

    /** */
    struct Location
    {
        QString name;   ///< name

        float lat;      ///< [rad]
        float lon;      ///< [rad]
        float alt;      ///< [m]
        float hdg;      ///< [rad]
    };

    /** */
    static inline Locations* instance()
    {
        if ( !m_instance )
        {
            m_instance = new Locations();
        }

        return m_instance;
    }

    /** Destructor. */
    virtual ~Locations();

    inline Location getLocation( int index ) const
    {
        return m_locations.at( index );
    }

    inline int getCount() const { return m_locations.size(); }

private:

    static Locations *m_instance;       ///< instance of Locations singleton class

    QVector< Location > m_locations;    ///<

    /**
     * You should use static function instance() due to get refernce
     * to Locations class instance.
     */
    Locations();

    /** Using this constructor is forbidden. */
    Locations( const Locations & ) {}

    void parseLocation( const QDomElement &node );
};

////////////////////////////////////////////////////////////////////////////////

#endif // LOCATIONS_H
