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

#include <gui/DockWidgetMap.h>
#include <ui_DockWidgetMap.h>

#include <gui/gui_Defines.h>

////////////////////////////////////////////////////////////////////////////////

DockWidgetMap::DockWidgetMap( QWidget *parent ) :
    QDockWidget ( parent ),
    m_ui ( new Ui::DockWidgetMap )
{
    m_ui->setupUi( this );

    //m_ui->widgetBar->hide();
    on_widgetMap_positionChanged( 0.0, 0.0 );
}

////////////////////////////////////////////////////////////////////////////////

DockWidgetMap::~DockWidgetMap()
{
    if ( m_ui ) delete m_ui;
    m_ui = 0;
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMap::closeEvent( QCloseEvent *event )
{
    /////////////////////////////////
    QDockWidget::closeEvent( event );
    /////////////////////////////////

    emit closed();
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMap::on_widgetMap_positionChanged( double lat, double lon )
{
    double lat_deg = osg::RadiansToDegrees( fabs( lat ) );
    double lon_deg = osg::RadiansToDegrees( fabs( lon ) );

    int lat_d = floor( lat_deg );
    int lat_m = floor( 60.0 * ( lat_deg - lat_d ) );
    double lat_s = 3600.0 * ( lat_deg - lat_d - lat_m / 60.0 );

    int lon_d = floor( lon_deg );
    int lon_m = floor( 60.0 * ( lon_deg - lon_d ) );
    double lon_s = 3600.0 * ( lon_deg - lon_d - lon_m / 60.0 );

    QString str;

    str += QString("%1").arg( lat_d, 2, 'f', 0, QChar(' ') );
    str += QString::fromUtf8( "°" );
    str += QString("%1").arg( lat_m, 2, 'f', 0, QChar('0') );
    str += "'";
    str += QString("%1").arg( lat_s, 5, 'f', 2, QChar('0') );
    str += "\"";
    str += ( lat > 0.0 ) ? "N" : "S";
    str += " ";
    str += QString("%1").arg( lon_d, 3, 'f', 0, QChar(' ') );
    str += QString::fromUtf8( "°" );
    str += QString("%1").arg( lon_m, 2, 'f', 0, QChar('0') );
    str += "'";
    str += QString("%1").arg( lon_s, 5, 'f', 2, QChar('0') );
    str += "\"";
    str += ( lon > 0.0 ) ? "E" : "W";

    m_ui->labelBar->setText( str );
}
