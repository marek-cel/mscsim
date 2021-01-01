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
#ifndef NAV_DATABASE_H
#define NAV_DATABASE_H

////////////////////////////////////////////////////////////////////////////////

#include <string>
#include <vector>

#include <fdm/utils/fdm_Vector3.h>

#include <sim/Singleton.h>

////////////////////////////////////////////////////////////////////////////////

namespace nav
{

/**
 * @brief Navigation data base class.
 */
class DataBase : public Singleton< DataBase >
{
    friend class Singleton< DataBase >;

public:

    /** */
    struct APT
    {
        std::string ident;      ///<
        std::string name;       ///<
        std::string icao;       ///<

        double lat;             ///< [rad] latitude
        double lon;             ///< [rad] longitude
    };

    /** */
    struct ILS
    {
        enum Type
        {
            NONE = 0,           ///<
            DME,                ///< DME
            LOC,                ///< localizer
            GS,                 ///< glide slope
            BCM,                ///< back course marker
            IM,                 ///< inner marker
            MM,                 ///< middle marker
            OM                  ///< outer marker
        };

        Type type;              ///< type

        double lat;             ///< [rad] latitude
        double lon;             ///< [rad] longitude

        double elev;            ///< [m] elevation

        double true_bearing;    ///< [rad] true bearing
        double glide_slope;     ///< [rad] glide slope angle

        int freq;               ///< [kHz] frequency

        fdm::Vector3 pos_wgs;   ///< [m] position expressed in WGS
    };

    /** */
    struct NAV
    {
        enum Type
        {
            NONE    = 0,        ///<
            VOR     = 1,        ///<
            VORTAC  = 2,        ///<
            TACAN   = 3,        ///<
            VOR_DME = 4,        ///<
            NDB     = 5,        ///<
            NDB_DME = 7,        ///<
            LOCATOR = 8,        ///<
            DME     = 9         ///<
        };

        Type type;              ///< type

        double lat;             ///< [rad] latitude
        double lon;             ///< [rad] longitude

        double elev;            ///< [m] elevation

        int freq;               ///< [kHz] frequency

        fdm::Vector3 pos_wgs;   ///< [m] position expressed in WGS
    };

    /** */
    struct RWY
    {
        double he_lat;          ///< [rad] latitude
        double he_lon;          ///< [rad] longitude
        double he_elev;         ///< [m] elevation

        double le_lat;          ///< [rad] latitude
        double le_lon;          ///< [rad] longitude
        double le_elev;         ///< [m] elevation

        double true_hdg;        ///< [rad] true heading

        double length;          ///< [m] length
        double width;           ///< [m] width
    };

    typedef std::vector< APT > ListAPT;
    typedef std::vector< ILS > ListILS;
    typedef std::vector< NAV > ListNAV;
    typedef std::vector< RWY > ListRWY;

    static double getTrueBearing( const char *str_bear, const char *str_magvar );

    /** @brief Destructor. */
    virtual ~DataBase();

    const ListAPT& getListAPT() const { return _list_apt; }
    const ListILS& getListILS() const { return _list_ils; }
    const ListNAV& getListNAV() const { return _list_nav; }
    const ListRWY& getListRWY() const { return _list_rwy; }

private:

    ListAPT _list_apt;          ///<
    ListILS _list_ils;          ///<
    ListNAV _list_nav;          ///<
    ListRWY _list_rwy;          ///<

    /**
     * You should use static function instance() due to get refernce
     * to DataBase class instance.
     */
    DataBase();

    /** Using this constructor is forbidden. */
    DataBase( const DataBase & ) {}

    void parseAPT( const char *path );
    void parseILS( const char *path );
    void parseNAV( const char *path );
    void parseRWY( const char *path );
};

} // end of nav namepsace

////////////////////////////////////////////////////////////////////////////////

#endif // NAV_DATABASE_H
