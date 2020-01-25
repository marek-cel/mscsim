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
#ifndef DOCKWIDGETTEST_H
#define DOCKWIDGETTEST_H

////////////////////////////////////////////////////////////////////////////////

#include <QDockWidget>
#include <QElapsedTimer>

#include <Common.h>

////////////////////////////////////////////////////////////////////////////////

namespace Ui
{
    class DockWidgetTest;
}

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Controls dock widget class.
 */
class DockWidgetTest : public QDockWidget
{
    Q_OBJECT

public:

    /** Constructor. */
    explicit DockWidgetTest( QWidget *parent = NULLPTR );

    /** Destructor. */
    ~DockWidgetTest();

    void update( double realTime );

    inline double getAlpha     () const { return _alpha;     }
    inline double getBeta      () const { return _beta;      }
    inline double getRoll      () const { return _roll;      }
    inline double getPitch     () const { return _pitch;     }
    inline double getHeading   () const { return _heading;   }
    inline double getSlipSkid  () const { return _slipSkid;  }
    inline double getTurnRate  () const { return _turnRate;  }
    inline double getCourse    () const { return _course;    }
    inline double getDevHor    () const { return _devH;      }
    inline double getDevVer    () const { return _devV;      }
    inline double getAirspeed  () const { return _airspeed;  }
    inline double getAltitude  () const { return _altitude;  }
    inline double getPressure  () const { return _pressure;  }
    inline double getClimbRate () const { return _climbRate; }
    inline double getMachNo    () const { return _machNo;    }
    inline double getADF       () const { return _adf;       }
    inline double getDME       () const { return _dme;       }

private:

    Ui::DockWidgetTest *_ui;        ///<

    double _alpha;
    double _beta;
    double _roll;
    double _pitch;
    double _heading;
    double _slipSkid;
    double _turnRate;
    double _course;
    double _devH;
    double _devV;
    double _airspeed;
    double _altitude;
    double _pressure;
    double _climbRate;
    double _machNo;
    double _adf;
    double _dme;
};

////////////////////////////////////////////////////////////////////////////////

#endif // DOCKWIDGETTEST_H
