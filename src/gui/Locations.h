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

        float elev;     ///< [m]
        float slope;    ///< [rad]

        bool runway;    ///<
    };

    /** */
    static inline Locations* instance()
    {
        if ( !_instance )
        {
            _instance = new Locations();
        }

        return _instance;
    }

    /** Destructor. */
    virtual ~Locations();

    inline Location getLocation( int index ) const
    {
        return _locations.at( index );
    }

    inline int getCount() const { return _locations.size(); }

private:

    static Locations *_instance;        ///< instance of Locations singleton class

    QVector< Location > _locations;     ///<

    /**
     * You should use static function instance() due to get refernce
     * to Locations class instance.
     */
    Locations();

    /** Using this constructor is forbidden. */
    Locations( const Locations & ) {}

    void parseLocation( QDomElement &node );
};

////////////////////////////////////////////////////////////////////////////////

#endif // LOCATIONS_H
