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

#include <gui/PageGeneral.h>
#include <ui_PageGeneral.h>

#include <QColorDialog>
#include <QSettings>

#include <gui/gui_Defines.h>

////////////////////////////////////////////////////////////////////////////////

PageGeneral::PageGeneral( QWidget *parent ) :
    QWidget ( parent ),
    _ui ( new Ui::PageGeneral ),

    _hud_color( 0, 185, 0 ),
    _hud_color_temp( _hud_color ),
    _hud_opacity  ( 100 ),
    _hud_factor_alt ( 1.0 ),
    _hud_factor_vel ( 1.0 ),

    _widescreen ( true ),

    _sound_volume ( 100 )
{
    _ui->setupUi( this );

    settingsRead();
}

////////////////////////////////////////////////////////////////////////////////

PageGeneral::~PageGeneral()
{
    settingsSave();

    DELPTR( _ui );
}

////////////////////////////////////////////////////////////////////////////////

void PageGeneral::readData()
{
    settingsRead();

    setFrameHudColor( _hud_color );
    _ui->sliderHudOpacity->setValue( _hud_opacity );
    _ui->checkBoxWidescreen->setChecked( _widescreen );
    _ui->sliderSoundVol->setValue( _sound_volume );
}

////////////////////////////////////////////////////////////////////////////////

void PageGeneral::saveData()
{
    _hud_color    = _hud_color_temp;
    _hud_opacity  = _ui->sliderHudOpacity->value();
    _widescreen   = _ui->checkBoxWidescreen->isChecked();
    _sound_volume = _ui->sliderSoundVol->value();

    settingsSave();
}

////////////////////////////////////////////////////////////////////////////////

void PageGeneral::setFrameHudColor( const QColor &color )
{
    QPalette pal = palette();
    pal.setColor( QPalette::Background, color );
    _ui->frameHudColor->setAutoFillBackground( true );
    _ui->frameHudColor->setPalette( pal );
}

////////////////////////////////////////////////////////////////////////////////

void PageGeneral::settingsRead()
{
    QSettings settings( SIM_ORG_NAME, SIM_APP_NAME );

    settings.beginGroup( "page_general" );
    {
        _hud_color.setRed   ( settings.value( "hud_color_r",   0 ).toInt() );
        _hud_color.setGreen ( settings.value( "hud_color_g", 185 ).toInt() );
        _hud_color.setBlue  ( settings.value( "hud_color_b",   0 ).toInt() );

        _hud_color_temp = _hud_color;

        _hud_opacity = settings.value( "hud_opacity", 100 ).toInt();

        int hud_factor_alt_index = settings.value( "hud_factor_alt", 1 ).toInt();
        int hud_factor_vel_index = settings.value( "hud_factor_vel", 4 ).toInt();

        _ui->comboBoxHudAltitude->setCurrentIndex( hud_factor_alt_index );
        _ui->comboBoxHudVelocity->setCurrentIndex( hud_factor_vel_index );

        _widescreen = settings.value( "widescreen_layout", 1 ).toInt() != 0;

        _sound_volume = settings.value( "sound_volume", 100 ).toInt();
    }
    settings.endGroup(); // page_general
}

////////////////////////////////////////////////////////////////////////////////

void PageGeneral::settingsSave()
{
    QSettings settings( SIM_ORG_NAME, SIM_APP_NAME );

    settings.beginGroup( "page_general" );
    {
        settings.setValue( "hud_color_r" , _hud_color.red()   );
        settings.setValue( "hud_color_g" , _hud_color.green() );
        settings.setValue( "hud_color_b" , _hud_color.blue()  );

        settings.setValue( "hud_opacity" , _hud_opacity );

        settings.setValue( "hud_factor_alt", _ui->comboBoxHudAltitude->currentIndex() );
        settings.setValue( "hud_factor_vel", _ui->comboBoxHudVelocity->currentIndex() );

        settings.setValue( "widescreen_layout" , _widescreen ? 1 : 0 );

        settings.setValue( "sound_volume" , _sound_volume );
    }
    settings.endGroup(); // page_general
}

////////////////////////////////////////////////////////////////////////////////

void PageGeneral::on_pushButtonHudColor_clicked()
{
    QColor color = QColorDialog::getColor( _hud_color_temp, this );

    if ( color.isValid() )
    {
        _hud_color_temp = color;
        setFrameHudColor( _hud_color_temp );
    }
}

////////////////////////////////////////////////////////////////////////////////

void PageGeneral::on_comboBoxHudAltitude_currentIndexChanged( int index )
{
    _hud_factor_alt = _ui->comboBoxHudAltitude->getCoef( index );
}

////////////////////////////////////////////////////////////////////////////////

void PageGeneral::on_comboBoxHudVelocity_currentIndexChanged( int index )
{
    _hud_factor_vel = _ui->comboBoxHudVelocity->getCoef( index );
}
