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
#ifndef DIALOGTIME_H
#define DIALOGTIME_H

////////////////////////////////////////////////////////////////////////////////

#include <QDialog>
#include <QTime>

#include <hid/hid_Manager.h>

#include "gui_Defines.h"

////////////////////////////////////////////////////////////////////////////////

namespace Ui
{
    class DialogTime;
}

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Local time pick dialog class.
 */
class DialogTime : public QDialog
{
    Q_OBJECT

public:

    static QTime getTimeUTC( double lon_deg, const QTime &time_utc, QWidget *parent = NULLPTR );

    explicit DialogTime( QWidget *parent = NULLPTR );

    ~DialogTime();

    void init( double lon_deg, const QTime &time_utc );

private:

    Ui::DialogTime *_ui;

    QTime _time_utc;

    double _lon_deg;
    double _offset;

private slots:

    void on_comboBoxZone_currentIndexChanged( int index );
    void on_timeEdit_timeChanged( const QTime &time );
};

////////////////////////////////////////////////////////////////////////////////

#endif // DIALOGTIME_H
