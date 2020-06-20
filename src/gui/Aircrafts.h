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
#ifndef AIRCRAFTS_H
#define AIRCRAFTS_H

////////////////////////////////////////////////////////////////////////////////

#include <QDomElement>
#include <QVector>

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Aircrafts class.
 */
class Aircrafts
{
public:

    /** Axis tuning data. */
    struct Axis
    {
        double curvature;       ///< curvature
        double deadzone;        ///< deadzone expressed as fraction of input
    };

    /** Axes tuning data. */
    struct Axes
    {
        Axis roll;              ///< roll axis tuning data
        Axis pitch;             ///< pitch axis tuning data
        Axis yaw;               ///< yaw axis tuning data
    };

    /** Aircraft controls data struct. */
    struct Controls
    {
        bool collective;        ///<
        bool landingGear;       ///<
        bool flaps;             ///<
        int  throttle;          ///<
        int  propeller;         ///<
        int  mixture;           ///<
        bool airbrake;          ///<
        bool spoilers;          ///<

        QList<double> notches;  ///<
    };

    /** Aircraft propulsion data struct. */
    struct Propulsion
    {
        int engines;            ///<

        bool ab;                ///<
        bool rpm;               ///<
        bool prop;              ///<
        bool ng;                ///<
        bool n1;                ///<
        bool n2;                ///<
        bool trq;               ///<
        bool epr;               ///<
        bool map;               ///<
        bool egt;               ///<
        bool tit;               ///<
        bool tot;               ///<
        bool itt;               ///<
    };

    /** Aircraft variable masses data struct. */
    struct Masses
    {
        /** Variable mass data struct. */
        struct Mass
        {
            bool enabled;       ///< specifies if mass is enabled
            QString name;       ///< name
            double max;         ///< [kg] maximum mass
            double def;         ///< [kg] default mass
        };

        Mass pilot_1;           ///<
        Mass pilot_2;           ///<

        Mass fuel_tank_1;       ///<
        Mass fuel_tank_2;       ///<
        Mass fuel_tank_3;       ///<
        Mass fuel_tank_4;       ///<
        Mass fuel_tank_5;       ///<
        Mass fuel_tank_6;       ///<
        Mass fuel_tank_7;       ///<
        Mass fuel_tank_8;       ///<

        Mass cabin;             ///<
        Mass trunk;             ///<
    };

    /** */
    struct Aircraft
    {
        QString name;           ///< name
        int     type;           ///< type
        QString file;           ///< file

        float distance_def;     ///< [m] minimum distance
        float distance_min;     ///< [m] minimum distance

        float offset_x;         ///< [m] camera position x-offset expressed in BAS
        float offset_y;         ///< [m] camera position y-offset expressed in BAS
        float offset_z;         ///< [m] camera position z-offset expressed in BAS

        float vfe;              ///< [m/s] maximum flap extension speed
        float vne;              ///< [m/s] never exceed speed

        Axes       axes;        ///< axes tuning data
        Controls   controls;    ///< controls data
        Propulsion propulsion;  ///< propulsion data
        Masses     masses;      ///< masses data

        float mainRotorCoef;    ///< [-] main rotor rotation coefficient (sign)
        float tailRotorCoef;    ///< [-] tail rotor rotation coefficient (sign)
    };

    /** */
    static inline Aircrafts* instance()
    {
        if ( !_instance )
        {
            _instance = new Aircrafts();
        }

        return _instance;
    }

    /** Destructor. */
    virtual ~Aircrafts();

    inline Aircraft getAircraft( int index ) const
    {
        return _aircrafts.at( index );
    }

    inline int getCount() const { return _aircrafts.size(); }

private:

    static Aircrafts *_instance;        ///< instance of Aircrafts singleton class

    QVector< Aircraft > _aircrafts;     ///<

    /**
     * You should use static function instance() due to get refernce
     * to Aircrafts class instance.
     */
    Aircrafts();

    /** Using this constructor is forbidden. */
    Aircrafts( const Aircrafts & ) {}

    void parseAircraft( const QDomElement &node );
    void parseAircraftAxes( const QDomElement &node, Axes &axes );
    void parseAircraftAxis( const QDomElement &node, Axis &axis );
    void parseAircraftControls( const QDomElement &node, Controls &controls );
    void parseAircraftPropulsion( const QDomElement &node, Propulsion &propulsion );
    void parseAircraftMasses( const QDomElement &node, Masses &masses );
    void parseAircraftMass( QDomElement &massNode, Masses::Mass &mass );
};

////////////////////////////////////////////////////////////////////////////////

#endif // AIRCRAFTS_H
