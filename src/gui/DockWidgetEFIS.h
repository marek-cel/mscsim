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
#ifndef DOCKWIDGETEFIS_H
#define DOCKWIDGETEFIS_H

////////////////////////////////////////////////////////////////////////////////

#include <QDockWidget>

#include "gui_Defines.h"

#include "GraphicsEADI.h"
#include "GraphicsEHSI.h"

////////////////////////////////////////////////////////////////////////////////

namespace Ui
{
    class DockWidgetEFIS;
}

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief EFIS dock widget class.
 */
class DockWidgetEFIS : public QDockWidget
{
    Q_OBJECT

public:

    /** Constructor. */
    explicit DockWidgetEFIS( QWidget *parent = NULLPTR );

    /** Destructor. */
    virtual ~DockWidgetEFIS();

    /** */
    inline void setFlightMode( GraphicsEADI::FlightMode flightMode )
    {
        _graphicsEADI->setFlightMode( flightMode );
    }

    /** */
    inline void setSpeedMode( GraphicsEADI::SpeedMode speedMode )
    {
        _graphicsEADI->setSpeedMode( speedMode );
    }

    /** */
    inline void setLNAV( GraphicsEADI::LNAV lnav )
    {
        _graphicsEADI->setLNAV( lnav );
    }

    /** */
    inline void setVNAV( GraphicsEADI::VNAV vnav )
    {
        _graphicsEADI->setVNAV( vnav );
    }

    /** @param roll angle [deg] */
    inline void setRoll( float roll )
    {
        _graphicsEADI->setRoll( roll );
    }

    /** @param pitch angle [deg] */
    inline void setPitch( float pitch )
    {
        _graphicsEADI->setPitch( pitch );
    }

    /** @param normalized slip or skid (range from -1.0 to 1.0) */
    inline void setSlipSkid( float slipSkid )
    {
        _graphicsEADI->setSlipSkid( slipSkid );
    }

    /**
     * @param normalized turn rate (range from -1.0 to 1.0),
     * hash marks positions are set to be -0.5 and 0.5
     */
    inline void setTurnRate( float turnRate )
    {
        _graphicsEADI->setTurnRate( turnRate );
    }

    /**
     * @param dotH [-] normalized horizontal deviation position (range from -1.0 to 1.0)
     * @param dotV [-] normalized vertical deviation position (range from -1.0 to 1.0)
     * @param visibleH deviation horizontal visibility
     * @param visibleV deviation vertical visibility */
    inline void setDots( float dotH, float dotV,
                         bool visibleH, bool visibleV )
    {
        _graphicsEADI->setDots( dotH, dotV, visibleH, visibleV );
    }

    /**
     * @param FD roll angle [deg]
     * @param FD pitch angle [deg]
     * @param FD visibility */
    inline void setFD( float roll, float pitch, bool visible = true )
    {
        _graphicsEADI->setFD( roll, pitch, visible );
    }

    /** @param stall flag */
    inline void setStall( bool stall )
    {
        _graphicsEADI->setStall( stall );
    }

    /** @param altitude (dimensionless numeric value) */
    inline void setAltitude( float altitude )
    {
        _graphicsEADI->setAltitude( altitude );
    }

    /** @param airspeed (dimensionless numeric value) */
    inline void setAirspeed( float airspeed )
    {
        _graphicsEADI->setAirspeed( airspeed );
    }

    /** @param Mach number */
    inline void setMachNo( float machNo )
    {
        _graphicsEADI->setMachNo( machNo );
    }

    /** @param heading [deg] */
    inline void setHeading( float heading )
    {
        _graphicsEADI->setHeading( heading );
        _graphicsEHSI->setHeading( heading );
    }

    /** @param climb rate (dimensionless numeric value)  */
    inline void setClimbRate( float climbRate )
    {
        _graphicsEADI->setClimbRate( climbRate );
    }

    /** @param course [deg] */
    inline void setCourse( float course )
    {
        _graphicsEHSI->setCourse( course );
    }

    /**
     * setDistance
     * @param distance [nmi] distance
     * @param visible
     */
    inline void setDistance( float distance, bool visible )
    {
        _graphicsEHSI->setDistance( distance, visible );
    }

    /**
     * setBearing
     * @param bearing [deg] bearing
     * @param visible
     */
    inline void setBearing( float bearing, bool visible )
    {
        _graphicsEHSI->setBearing( bearing, visible );
    }

    /**
     * setDeviation
     * @param deviation [-]
     * @param visible
     */
    inline void setDeviation( float deviation, bool visible )
    {
        _graphicsEHSI->setDeviation( deviation, visible );
    }

    /** @param airspeed (dimensionless numeric value) */
    inline void setAirspeedSet( double airspeed )
    {
        _graphicsEADI->setAirspeedSet( airspeed );
    }

    /** @param altitude (dimensionless numeric value) */
    inline void setAltitudeSet( double altitude )
    {
        _graphicsEADI->setAltitudeSet( altitude );
    }

    /** @param heading [deg] */
    inline void setHeadingSet( float heading )
    {
        _graphicsEHSI->setHeadingSet( heading );
    }

signals:

    void closed();

protected:

    void closeEvent( QCloseEvent *event );

private:

    Ui::DockWidgetEFIS *_ui;        ///<

    GraphicsEADI *_graphicsEADI;    ///<
    GraphicsEHSI *_graphicsEHSI;    ///<
};

////////////////////////////////////////////////////////////////////////////////

#endif // DOCKWIDGETEFIS_H
