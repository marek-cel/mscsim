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
#ifndef DIALOGENVR_H
#define DIALOGENVR_H

////////////////////////////////////////////////////////////////////////////////

#include <QDialog>
#include <QSettings>

#include <Data.h>

////////////////////////////////////////////////////////////////////////////////

namespace Ui
{
    class DialogEnvr;
}

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief This is 'Environment' dialog class.
 */
class DialogEnvr : public QDialog
{
    Q_OBJECT

public:

    typedef fdm::DataInp::Environment::WindShear WindShear;
    typedef Data::Environment::Clouds::Type CloudsType;
    typedef Data::Environment::Clouds::Data::Block BlockClouds;
    typedef Data::Environment::Clouds::Data::Layer LayerClouds;

    /** Constructor. */
    explicit DialogEnvr( QWidget *parent = 0 );
    
    /** Destructor. */
    virtual ~DialogEnvr();

    inline double getTemperatureSL() const { return m_temperature_0; }
    inline double getPressureSL()    const { return m_pressure_0;    }

    inline double getWindDirection() const { return m_windDirection; }
    inline double getWindSpeed()     const { return m_windSpeed;     }
    inline double getTurbulence()    const { return m_turbulence;    }

    inline WindShear getWindShear() const { return m_windShear; }

    inline CloudsType getCloudsType() const { return m_cloudsType; }

    inline const BlockClouds& getBlockClouds() const { return m_blockClouds; }
    inline const LayerClouds& getLayerClouds() const { return m_layerClouds; }

    inline double getVisibility() const { return m_visibility; }

    void readData();
    void saveData();

private:

    Ui::DialogEnvr *m_ui;           ///<

    double m_temperature_0;         ///< [K] sea level temperature
    double m_pressure_0;            ///< [Pa] sea level pressure

    double m_windDirection;         ///< [rad] wind direction
    double m_windSpeed;             ///< [m/s] wind speed
    double m_turbulence;            ///< [-] turbulence intensity

    WindShear m_windShear;          ///< active wind shear model

    double m_visibility;            ///< [m]

    CloudsType m_cloudsType;

    BlockClouds m_blockClouds;
    LayerClouds m_layerClouds;

    void settingsRead();
    void settingsRead_EnvrData( QSettings &settings );
    void settingsRead_UnitCombos( QSettings &settings );

    void settingsSave();
    void settingsSave_EnvrData( QSettings &settings );
    void settingsSave_UnitCombos( QSettings &settings );

private slots:

    void on_comboSeaLevelTemp_currentIndexChanged( int index );
    void on_comboSeaLevelPress_currentIndexChanged( int index );

    void on_comboWindDir_currentIndexChanged( int index );
    void on_comboWindSpeed_currentIndexChanged( int index );

    void on_comboVisibility_currentIndexChanged( int index );

    void on_sliderTurbulence_valueChanged( int value );

    void on_sliderVisibility_valueChanged( int value );

    void on_comboCloudsBlockBaseASL_currentIndexChanged( int index );
    void on_comboCloudsBlockThickness_currentIndexChanged( int index );

    void on_radioButtonCloudsTypeBlock_clicked( bool checked );
    void on_radioButtonCloudsTypeLayer_clicked( bool checked );
};

////////////////////////////////////////////////////////////////////////////////

#endif // DIALOGENVR_H
