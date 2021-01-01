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

#include <nav/nav_Manager.h>

#include <fdm/utils/fdm_Units.h>

#include <Data.h>

#include <nav/nav_Frequency.h>

#include <sim/Path.h>

////////////////////////////////////////////////////////////////////////////////

using namespace nav;

////////////////////////////////////////////////////////////////////////////////

const double Manager::_range_ils = fdm::Units::nmi2m( 18.0 );
const double Manager::_range_ndb = fdm::Units::nmi2m( 40.0 );
const double Manager::_range_vor = fdm::Units::nmi2m( 40.0 );

const double Manager::_ils_gs_max = fdm::Units::deg2rad( 0.7 );
const double Manager::_ils_lc_max = fdm::Units::deg2rad( 2.5 );

const double Manager::_vor_max = fdm::Units::deg2rad( 10.0 );

////////////////////////////////////////////////////////////////////////////////

Manager::Manager() :
    _ils_loc ( NULLPTR ),
    _ils_dme ( NULLPTR ),
    _ils_gs  ( NULLPTR ),
    _vor     ( NULLPTR ),
    _ndb     ( NULLPTR ),

    _course  ( 0.0 ),

    _adf_freq ( 190 ),
    _nav_freq ( 11800 ),

    _adf_visible ( false ),
    _adf_bearing ( 0.0 ),

    _dme_visible ( false ),
    _dme_distance ( 0.0 ),

    _ils_gs_visible ( false ),
    _ils_lc_visible ( false ),
    _ils_heading ( 0.0 ),
    _ils_gs_deviation ( 0.0 ),
    _ils_lc_deviation ( 0.0 ),
    _ils_gs_norm ( 0.0 ),
    _ils_lc_norm ( 0.0 ),

    _nav_cdi ( Data::Navigation::NONE ),
    _nav_bearing   ( 0.0 ),
    _nav_deviation ( 0.0 ),
    _nav_norm ( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

Manager::~Manager() {}

////////////////////////////////////////////////////////////////////////////////

void Manager::update()
{
    _aircraft_wgs.setPos_WGS( fdm::Vector3( Data::get()->ownship.pos_x_wgs,
                                            Data::get()->ownship.pos_y_wgs,
                                            Data::get()->ownship.pos_z_wgs ) );

    while ( _course < 0.0        ) _course += 2.0 * M_PI;
    while ( _course > 2.0 * M_PI ) _course -= 2.0 * M_PI;

    updateNavaids();

    Data::get()->navigation.adf_visible = _adf_visible;
    Data::get()->navigation.adf_bearing = _adf_bearing;

    Data::get()->navigation.dme_visible  = _dme_visible;
    Data::get()->navigation.dme_distance = _dme_distance;

    Data::get()->navigation.ils_visible      = _ils_gs_visible && _ils_lc_visible;
    Data::get()->navigation.ils_gs_visible   = _ils_gs_visible;
    Data::get()->navigation.ils_lc_visible   = _ils_lc_visible;
    Data::get()->navigation.ils_heading      = _ils_heading;
    Data::get()->navigation.ils_gs_deviation = _ils_gs_deviation;
    Data::get()->navigation.ils_lc_deviation = _ils_lc_deviation;
    Data::get()->navigation.ils_gs_norm      = _ils_gs_norm;
    Data::get()->navigation.ils_lc_norm      = _ils_lc_norm;

    Data::get()->navigation.nav_cdi       = _nav_cdi;
    Data::get()->navigation.nav_bearing   = _nav_bearing;
    Data::get()->navigation.nav_deviation = _nav_deviation;
    Data::get()->navigation.nav_norm      = _nav_norm;
}

////////////////////////////////////////////////////////////////////////////////

double Manager::getAzimuth( const fdm::Vector3 &pos_wgs )
{
    fdm::Vector3 pos_ned = _aircraft_wgs.getWGS2NED() * ( pos_wgs - _aircraft_wgs.getPos_WGS() );

    double azim = atan2(  pos_ned.y(), pos_ned.x() );

    if      ( azim < 0.0        ) azim += 2.0 * M_PI;
    else if ( azim > 2.0 * M_PI ) azim -= 2.0 * M_PI;

    return azim;
}

////////////////////////////////////////////////////////////////////////////////

double Manager::getDistance( const fdm::Vector3 &pos_wgs )
{
    // orthodromic distance
    return fdm::WGS84::_r1 * acos( pos_wgs.getNormalized() * _aircraft_wgs.getPos_WGS().getNormalized() );
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateNavaids()
{
    _adf_visible = false;
    _adf_bearing = 0.0;

    _dme_visible = false;
    _dme_distance  = 0.0;

    _ils_gs_visible = false;
    _ils_lc_visible = false;
    _ils_heading = 0.0;
    _ils_gs_deviation = 0.0;
    _ils_lc_deviation = 0.0;
    _ils_gs_norm = 0.0;
    _ils_lc_norm = 0.0;

    _nav_cdi = Data::Navigation::NONE;
    _nav_bearing   = 0.0;
    _nav_deviation = 0.0;
    _nav_norm      = 0.0;

    updateNavaidsActive();

    updateADF();
    updateILS();
    updateNAV();
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateNavaidsActive()
{
    if ( _ils_loc )
    {
        if ( _ils_loc->freq != _nav_freq || getDistance( _ils_loc->pos_wgs ) > _range_ils )
            _ils_loc = NULLPTR;
    }

    if ( _ils_dme )
    {
        if ( _ils_dme->freq != Frequency::getFreqDME( _nav_freq ) || getDistance( _ils_dme->pos_wgs ) > _range_ils )
            _ils_dme = NULLPTR;
    }

    if ( _ils_gs )
    {
        if ( _ils_gs->freq != Frequency::getFreqGS( _nav_freq ) || getDistance( _ils_gs->pos_wgs ) > _range_ils )
            _ils_gs = NULLPTR;
    }

    if ( _vor )
    {
        if ( _vor->freq != _nav_freq || getDistance( _vor->pos_wgs ) > _range_vor )
            _vor = NULLPTR;
    }

    if ( _ndb )
    {
        if ( _ndb->freq != _adf_freq || getDistance( _ndb->pos_wgs ) > _range_ndb )
            _ndb = NULLPTR;
    }

    if ( !_ils_loc )
    {
        for ( DataBase::ListILS::const_iterator it = DataBase::instance()->getListILS().begin();
              it != DataBase::instance()->getListILS().end();
              ++it )
        {
            if ( (*it).type == DataBase::ILS::LOC
              && (*it).freq == _nav_freq
              && getDistance( (*it).pos_wgs ) < _range_ils )
            {
                _ils_loc = &(*it);
                break;
            }
        }
    }

    if ( !_ils_dme )
    {
        for ( DataBase::ListILS::const_iterator it = DataBase::instance()->getListILS().begin();
              it != DataBase::instance()->getListILS().end();
              ++it )
        {
            if ( (*it).type == DataBase::ILS::DME
              && (*it).freq == Frequency::getFreqDME( _nav_freq )
              && getDistance( (*it).pos_wgs ) < _range_ils )
            {
                _ils_dme = &(*it);
                break;
            }
        }
    }

    if ( !_ils_gs )
    {
        for ( DataBase::ListILS::const_iterator it = DataBase::instance()->getListILS().begin();
              it != DataBase::instance()->getListILS().end();
              ++it )
        {
            if ( (*it).type == DataBase::ILS::GS
              && (*it).freq == Frequency::getFreqGS( _nav_freq )
              && getDistance( (*it).pos_wgs ) < _range_ils )
            {
                _ils_gs = &(*it);
                break;
            }
        }
    }

    if ( !_vor )
    {
        for ( DataBase::ListNAV::const_iterator it = DataBase::instance()->getListNAV().begin();
              it != DataBase::instance()->getListNAV().end();
              ++it )
        {
            if ( ( (*it).type == DataBase::NAV::VOR || (*it).type == DataBase::NAV::VOR_DME || (*it).type == DataBase::NAV::VORTAC )
              && (*it).freq == _nav_freq
              && getDistance( (*it).pos_wgs ) < _range_vor )
            {
                _vor = &(*it);
                break;
            }
        }
    }

    if ( !_ndb )
    {
        for ( DataBase::ListNAV::const_iterator it = DataBase::instance()->getListNAV().begin();
              it != DataBase::instance()->getListNAV().end();
              ++it )
        {
            if ( ( (*it).type == DataBase::NAV::NDB || (*it).type == DataBase::NAV::NDB_DME )
              && (*it).freq == _adf_freq
              && getDistance( (*it).pos_wgs ) < _range_ndb )
            {
                _ndb = &(*it);
                break;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateADF()
{
    if ( _ndb )
    {
        _adf_visible = true;
        _adf_bearing = getAzimuth( _ndb->pos_wgs );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateILS()
{
    if ( _ils_loc )
    {
        double azim = getAzimuth( _ils_loc->pos_wgs );

        if      ( azim < -M_PI ) azim += 2.0 * M_PI;
        else if ( azim >  M_PI ) azim -= 2.0 * M_PI;

        if ( fabs( azim - _ils_loc->true_bearing ) < fdm::Units::deg2rad(  30.0 )
          || fabs( azim - _ils_loc->true_bearing ) > fdm::Units::deg2rad( 150.0 ) )
        {
            _ils_lc_visible = true;

            updateNAV( azim, _ils_loc->true_bearing, _ils_lc_max );

            _nav_cdi = Data::Navigation::TO;

            _ils_heading = _ils_loc->true_bearing;
            _ils_lc_deviation = _nav_deviation;
            _ils_lc_norm = _nav_norm;

            if ( _ils_dme )
            {
                _dme_visible = true;
                _dme_distance = getDistance( _ils_dme->pos_wgs );
            }

            if ( _ils_gs )
            {
                fdm::Vector3 pos_ned_gs = _aircraft_wgs.getWGS2NED() * ( _ils_gs->pos_wgs - _aircraft_wgs.getPos_WGS() );
                fdm::Vector3 pos_rwy_gs = fdm::Matrix3x3( fdm::Angles( 0.0, 0.0, _ils_loc->true_bearing ) ) * pos_ned_gs;

                double elev = atan2( -pos_rwy_gs.z(), pos_rwy_gs.x() );

                double elev_rel = elev + _ils_gs->glide_slope;

                if ( pos_rwy_gs.x() > 0.0 )
                {
                    _ils_gs_visible = true;

                    _ils_gs_deviation = elev_rel;

                    _ils_gs_norm = _ils_gs_deviation / _ils_gs_max;
                    _ils_gs_norm = fdm::Misc::satur( -1.0, 1.0, _ils_gs_norm );
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateNAV()
{
    if ( _vor )
    {
        _nav_visible = true;

        double azim = getAzimuth( _vor->pos_wgs );

        if      ( azim < -M_PI ) azim += 2.0 * M_PI;
        else if ( azim >  M_PI ) azim -= 2.0 * M_PI;

        updateNAV( azim, _course, _vor_max );

        _adf_visible = true;
        _adf_bearing = _nav_bearing;

        if ( _vor->type == DataBase::NAV::VOR_DME )
        {
            _dme_visible = true;
            _dme_distance = getDistance( _vor->pos_wgs );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateNAV( double azim, double course, double max )
{
    double azim_rel = azim - course;

    while ( azim_rel < -M_PI ) azim_rel += 2.0 * M_PI;
    while ( azim_rel >  M_PI ) azim_rel -= 2.0 * M_PI;

    if ( azim_rel < -M_PI_2 || M_PI_2 < azim_rel )
    {
        _nav_cdi = Data::Navigation::FROM;

        if ( azim_rel < -M_PI_2 ) azim_rel += M_PI;
        if ( azim_rel >  M_PI_2 ) azim_rel -= M_PI;

        azim_rel *= -1.0;
    }
    else
    {
        _nav_cdi = Data::Navigation::TO;
    }

    _nav_bearing = azim;

    _nav_deviation = azim_rel;

    _nav_norm = _nav_deviation / max;
    _nav_norm = fdm::Misc::satur( -1.0, 1.0, _nav_norm );
}
