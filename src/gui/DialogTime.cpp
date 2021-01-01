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

#include <gui/DialogTime.h>
#include <ui_DialogTime.h>

#include <cmath>

////////////////////////////////////////////////////////////////////////////////

QTime DialogTime::getTimeUTC( double lon_deg, const QTime &time_utc, QWidget *parent )
{
    DialogTime *dialog = NULLPTR;

    dialog = new DialogTime( parent );
    dialog->init( lon_deg, time_utc );

    if ( dialog->exec() == QDialog::Accepted )
    {
        return QTime( dialog->_time_utc );
    }

    return QTime( time_utc );
}

////////////////////////////////////////////////////////////////////////////////

DialogTime::DialogTime( QWidget *parent ) :
    QDialog( parent ),
    _ui( new Ui::DialogTime ),
    _lon_deg ( 0.0 ),
    _offset ( 0.0 )
{
    _ui->setupUi( this );
}

////////////////////////////////////////////////////////////////////////////////

DialogTime::~DialogTime()
{
    DELPTR( _ui );
}

////////////////////////////////////////////////////////////////////////////////

void DialogTime::init( double lon_deg, const QTime &time_utc )
{
    _lon_deg = lon_deg;

    if ( _lon_deg < -180.0 ) _lon_deg = -180.0;
    if ( _lon_deg >  180.0 ) _lon_deg =  180.0;

    _time_utc = time_utc;

    int lon_temp = floor( _lon_deg + 180.0 + 7.5 );
    int tz_index = lon_temp / 15;

    _ui->comboBoxZone->setCurrentIndex( tz_index );
}

////////////////////////////////////////////////////////////////////////////////

void DialogTime::on_comboBoxZone_currentIndexChanged( int index )
{
    _offset = -12.0 + (double)index;

    QTime time_local = _time_utc;
    time_local = time_local.addSecs( 3600.0 * _offset );

    _ui->timeEdit->setTime( time_local );
}

////////////////////////////////////////////////////////////////////////////////

void DialogTime::on_timeEdit_timeChanged( const QTime &time )
{
    _time_utc = time.addSecs( -3600.0 * _offset );
}

////////////////////////////////////////////////////////////////////////////////

void DialogTime::on_pushButton_00_clicked()
{
    _ui->timeEdit->setTime( QTime( 0, 0 ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogTime::on_pushButton_06_clicked()
{
    _ui->timeEdit->setTime( QTime( 6, 0 ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogTime::on_pushButton_12_clicked()
{
    _ui->timeEdit->setTime( QTime( 12, 0 ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogTime::on_pushButton_18_clicked()
{
    _ui->timeEdit->setTime( QTime( 18, 0 ) );
}
