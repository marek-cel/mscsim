/****************************************************************************//*
 * Copyright (C) 2020 Marek M. Cel
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
#ifndef AIRPORTS_H
#define AIRPORTS_H

////////////////////////////////////////////////////////////////////////////////

#include <QDomElement>
#include <QVector>

#include <sim/Singleton.h>

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Airports class.
 */
class Airports : public Singleton< Airports >
{
    friend class Singleton< Airports >;

public:

    /** */
    struct Location
    {
        QString name;                   ///< name

        double lat;                     ///< [rad]
        double lon;                     ///< [rad]
        double alt;                     ///< [m]
        double hdg;                     ///< [rad]

        double elev;                    ///< [m]
        double slope;                   ///< [rad]

        bool runway;                    ///<
    };

    /** */
    struct Airport
    {
        QString name;                   ///< name
        QVector< Location > locations;  ///< locations
    };

    /** Destructor. */
    virtual ~Airports();

    inline Airport getAirport( int index ) const
    {
        return _airports.at( index );
    }

    inline int getCount() const { return _airports.size(); }

    inline Location getDefault() const { return _default; }

private:

    QVector< Airport > _airports;       ///< airports

    Location _default;                  ///< default location

    /**
     * You should use static function instance() due to get refernce
     * to Airports class instance.
     */
    Airports();

    /** Using this constructor is forbidden. */
    Airports( const Airports & ) {}

    void parseAirport( QDomElement &node );
    void parseLocation( QDomElement &node, Airport &airport );
};

////////////////////////////////////////////////////////////////////////////////

#endif // AIRPORTS_H
