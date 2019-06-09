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

#include <gui/PageGeneral.h>
#include <ui_PageGeneral.h>

#include <QColorDialog>
#include <QSettings>

#include <gui/gui_Defines.h>

////////////////////////////////////////////////////////////////////////////////

PageGeneral::PageGeneral( QWidget *parent ) :
    QWidget ( parent ),
    m_ui ( new Ui::PageGeneral ),

    m_hud_color( 0, 170, 0 ),
    m_hud_color_temp( m_hud_color ),
    m_hud_opacity ( 100 ),
    m_widescreen ( true )
{
    m_ui->setupUi( this );

    settingsRead();
}

////////////////////////////////////////////////////////////////////////////////

PageGeneral::~PageGeneral()
{
    settingsSave();

    if ( m_ui ) delete m_ui;
    m_ui = 0;
}

////////////////////////////////////////////////////////////////////////////////

void PageGeneral::readData()
{
    settingsRead();

    setFrameHudColor( m_hud_color );
    m_ui->sliderHudOpacity->setValue( m_hud_opacity );
    m_ui->checkBoxWidescreen->setChecked( m_widescreen );
}

////////////////////////////////////////////////////////////////////////////////

void PageGeneral::saveData()
{
    m_hud_color = m_hud_color_temp;
    m_hud_opacity = m_ui->sliderHudOpacity->value();
    m_widescreen = m_ui->checkBoxWidescreen->isChecked();

    settingsSave();
}

////////////////////////////////////////////////////////////////////////////////

void PageGeneral::setFrameHudColor( const QColor &color )
{
    QPalette pal = palette();
    pal.setColor( QPalette::Background, color );
    m_ui->frameHudColor->setAutoFillBackground( true );
    m_ui->frameHudColor->setPalette( pal );
}

////////////////////////////////////////////////////////////////////////////////

void PageGeneral::settingsRead()
{
    QSettings settings( GUI_ORG_NAME, GUI_APP_NAME );

    settings.beginGroup( "page_general" );
    {
        m_hud_color.setRed   ( settings.value( "hud_color_r",   0 ).toInt() );
        m_hud_color.setGreen ( settings.value( "hud_color_g", 170 ).toInt() );
        m_hud_color.setBlue  ( settings.value( "hud_color_b",   0 ).toInt() );

        m_hud_color_temp = m_hud_color;

        m_hud_opacity = settings.value( "hud_opacity", 100 ).toInt();

        m_widescreen = settings.value( "widescreen_layout", 1 ).toInt() != 0;
    }
    settings.endGroup(); // page_general
}

////////////////////////////////////////////////////////////////////////////////

void PageGeneral::settingsSave()
{
    QSettings settings( GUI_ORG_NAME, GUI_APP_NAME );

    settings.beginGroup( "page_general" );
    {
        settings.setValue( "hud_color_r" , m_hud_color.red()   );
        settings.setValue( "hud_color_g" , m_hud_color.green() );
        settings.setValue( "hud_color_b" , m_hud_color.blue()  );

        settings.setValue( "hud_opacity" , m_hud_opacity );

        settings.setValue( "widescreen_layout" , m_widescreen ? 1 : 0 );
    }
    settings.endGroup(); // page_general
}

////////////////////////////////////////////////////////////////////////////////

void PageGeneral::on_pushButtonHudColor_clicked()
{
    QColor color = QColorDialog::getColor( m_hud_color_temp, this );

    if ( color.isValid() )
    {
        m_hud_color_temp = color;
        setFrameHudColor( m_hud_color_temp );
    }
}
