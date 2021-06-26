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
#ifndef WIDGETPFD_H
#define WIDGETPFD_H

////////////////////////////////////////////////////////////////////////////////

#include <QWidget>

#include <Autopilot.h>
#include <defs.h>

#include <g1000/cgi/g1000_PFD.h>

////////////////////////////////////////////////////////////////////////////////

namespace Ui
{
    class WidgetPFD;
}

////////////////////////////////////////////////////////////////////////////////

/** */
class WidgetPFD : public QWidget
{
    Q_OBJECT

public:

    explicit WidgetPFD( QWidget *parent = NULLPTR );

    virtual ~WidgetPFD();

    void init( Autopilot *ap, g1000::IFD *ifd );

signals:

    void closed();

protected:

    void closeEvent( QCloseEvent *event );

    /** */
    void timerEvent( QTimerEvent *event );

private:

    Ui::WidgetPFD *_ui;

    g1000::PFD *_pfd;

    int _timerId;

    void settingsRead();
    void settingsSave();
};

////////////////////////////////////////////////////////////////////////////////

#endif // WIDGETPFD_H
