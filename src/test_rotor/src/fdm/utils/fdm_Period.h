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
#ifndef FDM_PERIOD_H
#define FDM_PERIOD_H

////////////////////////////////////////////////////////////////////////////////

#include <vector>

#include <fdm/fdm_Defines.h>
#include <fdm/fdm_Types.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Function period class.
 *
 * UNDER CONSTRUCTION!
 *
 * @todo There are some problems with calculations. More testing is required!
 */
class FDMEXPORT Period
{
public:

    /** Constructor. */
    Period();

    /** Destructor. */
    virtual ~Period();

    void update( double arg, double val, double min = 0.0 );

    void reset();

    inline double getAmplitude()    const { return _amplitude; }
    inline double getAmplitudeAvg() const { return _amplitude_avg; }
    inline double getAmplitudeMin() const { return _amplitude_min; }
    inline double getAmplitudeMax() const { return _amplitude_max; }

    inline double getValueAvg() const { return _value_avg; }
    inline double getValueMin() const { return _value_min; }
    inline double getValueMax() const { return _value_max; }

    inline double getPeriod()    const { return _period; }
    inline double getPeriodAvg() const { return _period_avg; }
    inline double getPeriodMin() const { return _period_min; }
    inline double getPeriodMax() const { return _period_max; }

    inline UInt32 getCount() const { return _count; }

private:

    typedef std::vector< double > Values;

    Values _values;         ///< last period values

    double _arg_0;          ///< period start argument

    double _value_avg;      ///< average value
    double _value_min;      ///< minimum value
    double _value_max;      ///< maximum value

    double _value_avg_min;  ///< average minimum value
    double _value_avg_max;  ///< averaga maximum value

    double _amplitude;      ///< amplitude (last computed)
    double _amplitude_avg;  ///< average amplitude
    double _amplitude_min;  ///< minimum amplitude
    double _amplitude_max;  ///< maximum amplitude

    double _period;         ///< period (last computed)
    double _period_avg;     ///< average period
    double _period_min;     ///< minimum period
    double _period_max;     ///< maximum period

    UInt32 _count;          ///< periods count

    bool _even;             ///< specifies is pass through average vale is even
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_PERIOD_H
