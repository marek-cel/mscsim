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

#include <gui/WidgetPFD.h>
#include <ui_WidgetPFD.h>

#include <QSettings>

#include <Common.h>

#include <gui/gui_Defines.h>

////////////////////////////////////////////////////////////////////////////////

WidgetPFD::WidgetPFD( QWidget *parent ) :
    QWidget ( parent ),
    _ui ( new Ui::WidgetPFD ),
    _pfd ( NULLPTR ),
    _timerId ( 0 )
{
    _ui->setupUi( this );

    settingsRead();
}

////////////////////////////////////////////////////////////////////////////////

WidgetPFD::~WidgetPFD()
{
    settingsSave();

    DELPTR( _pfd );

    DELPTR( _ui );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetPFD::init( Autopilot *ap, g1000::IFD *ifd )
{
    _ui->widgetPFD->setAutopilot( ap );

    _pfd = new g1000::PFD( ifd, Path::get( "g1000/pfd.xml" ).c_str() );
    _ui->widgetPFD->setSceneRoot( _pfd->getRoot() );

    _timerId = startTimer( 1000.0 * GUI_TIME_STEP );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetPFD::closeEvent( QCloseEvent *event )
{
    /////////////////////////////
    QWidget::closeEvent( event );
    /////////////////////////////

    emit closed();
}

////////////////////////////////////////////////////////////////////////////////

void WidgetPFD::timerEvent( QTimerEvent *event )
{
    /////////////////////////////
    QWidget::timerEvent( event );
    /////////////////////////////

    _pfd->update();
    _ui->widgetPFD->update();
}

////////////////////////////////////////////////////////////////////////////////

void WidgetPFD::settingsRead()
{
    QSettings settings( SIM_ORG_NAME, SIM_APP_NAME );

    settings.beginGroup( "widget_pfd" );
    restoreGeometry( settings.value( "geometry" ).toByteArray() );
    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void WidgetPFD::settingsSave()
{
    QSettings settings( SIM_ORG_NAME, SIM_APP_NAME );

    settings.beginGroup( "widget_pfd" );
    settings.setValue( "geometry", saveGeometry() );
    settings.endGroup();
}
