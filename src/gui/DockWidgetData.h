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
#ifndef DOCKWIDGETDATA_H
#define DOCKWIDGETDATA_H

////////////////////////////////////////////////////////////////////////////////

#include <QDockWidget>
#include <QSettings>

////////////////////////////////////////////////////////////////////////////////

namespace Ui
{
    class DockWidgetData;
}

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Data dock widget class.
 */
class DockWidgetData : public QDockWidget
{
    Q_OBJECT

public:

    /** Constructor. */
    explicit DockWidgetData( QWidget *parent = 0 );

    /** Destructor. */
    virtual ~DockWidgetData();

    /** */
    void reset();

    void setAltitudeASL( double altitude_asl );
    void setAltitudeAGL( double altitude_agl );

    void setAirspeed( double airspeed );
    void setClimbRate( double climbRate );
    void setMachNumber( double machNumber );

    void setRollAngle( double rollAngle );
    void setPitchAngle( double pitchAngle );
    void setHeading( double heading );

    void setAngleOfAttack( double angleOfAttack );
    void setSideslipAngle( double sideslipAngle );
    void setCourse( double course );
    void setPathAngle( double pathAngle );

    void setRollRate( double rollRate );
    void setPitchRate( double pitchRate );
    void setYawRate( double yawRate );

    void setGx( double Gx );
    void setGy( double Gy );
    void setGz( double Gz );

private:

    Ui::DockWidgetData *m_ui;

    double m_altitude_asl;  ///< [m] altitude above sea level
    double m_altitude_agl;  ///< [m] altitude above ground level

    double m_airspeed;      ///< [m/s] airspeed
    double m_climbRate;     ///< [m/s] climb rate
    double m_machNumber;    ///< [-] Mach number

    double m_rollAngle;     ///< [rad] roll angle
    double m_pitchAngle;    ///< [rad] pitch angle
    double m_heading;       ///< [rad] true heading

    double m_angleOfAttack; ///< [rad] angle of attack
    double m_sideslipAngle; ///< [rad] angle of sideslip

    double m_course;        ///< [rad] course
    double m_pathAngle;     ///< [rad] path angle

    double m_rollRate;      ///< [rad/s] roll rate
    double m_pitchRate;     ///< [rad/s] pitch rate
    double m_yawRate;       ///< [rad/s] yaw rate

    double m_Gx;            ///< [-] X component of G-Force vector expressed in BAS
    double m_Gy;            ///< [-] Y component of G-Force vector expressed in BAS
    double m_Gz;            ///< [-] Z component of G-Force vector expressed in BAS

    double m_maxGz;         ///< [-]

    void settingsRead();
    void settingsRead_UnitCombos( QSettings &settings );

    void settingsSave();
    void settingsSave_UnitCombos( QSettings &settings );

private slots:

    void on_comboAltitudeASL_currentIndexChanged( int /*index*/ );
    void on_comboAltitudeAGL_currentIndexChanged( int /*index*/ );
    void on_comboAirspeed_currentIndexChanged( int /*index*/ );
    void on_comboClimbRate_currentIndexChanged( int /*index*/ );
    void on_comboMachNumber_currentIndexChanged( int /*index*/ );
    void on_comboRollAngle_currentIndexChanged( int /*index*/ );
    void on_comboPitchAngle_currentIndexChanged( int /*index*/ );
    void on_comboHeading_currentIndexChanged( int /*index*/ );
    void on_comboAngleOfAttack_currentIndexChanged( int /*index*/ );
    void on_comboSideslipAngle_currentIndexChanged( int /*index*/ );
    void on_comboCourse_currentIndexChanged( int /*index*/ );
    void on_comboPathAngle_currentIndexChanged( int /*index*/ );
    void on_comboRollRate_currentIndexChanged( int /*index*/ );
    void on_comboPitchRate_currentIndexChanged( int /*index*/ );
    void on_comboYawRate_currentIndexChanged( int /*index*/ );
    void on_comboGx_currentIndexChanged( int /*index*/ );
    void on_comboGy_currentIndexChanged( int /*index*/ );
    void on_comboGz_currentIndexChanged( int /*index*/ );
    void on_comboMaxGz_currentIndexChanged( int /*index*/ );
};

////////////////////////////////////////////////////////////////////////////////

#endif // DOCKWIDGETDATA_H
