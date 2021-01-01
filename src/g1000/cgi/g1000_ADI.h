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
#ifndef G1000_ADI_H
#define G1000_ADI_H

////////////////////////////////////////////////////////////////////////////////

#include <osg/PositionAttitudeTransform>
#include <osg/Switch>

#include <g1000/cgi/g1000_Module.h>

////////////////////////////////////////////////////////////////////////////////

namespace g1000
{

/**
 * @brief PFD Attitude Direction Indicator
 */
class ADI : public Module
{
public:

    static const double _x_offset;
    static const double _y_offset;

    static const osg::Vec3 _colorGround;
    static const osg::Vec3 _colorHorizon;
    static const osg::Vec3 _colorSky;

    static const double _z_aircraft_symbol;
    static const double _z_horizon_line;
    static const double _z_pitch_scale;
    static const double _z_roll_scale;
    static const double _z_sky_and_ground;

    static const double _deg2pt;
    static const double _rad2pt;

    static const double _fd_pitch_min;
    static const double _fd_pitch_max;

    static const double _max_slip_skid;

    static const int _depth_sorted_bin_pitch_scale;

    static inline double deg2pt( double angle ) { return _deg2pt * angle; }
    static inline double rad2pt( double angle ) { return _rad2pt * angle; }

    /** Constructor. */
    ADI( IFD *ifd );

    /** Destructor. */
    virtual ~ADI();

    /** Updates. */
    void update();

private:

    osg::ref_ptr<osg::PositionAttitudeTransform> _pat;
    osg::ref_ptr<osg::PositionAttitudeTransform> _patPitch;
    osg::ref_ptr<osg::PositionAttitudeTransform> _patRoll;
    osg::ref_ptr<osg::PositionAttitudeTransform> _patSlipSkid;
    osg::ref_ptr<osg::PositionAttitudeTransform> _patHorizon;
    osg::ref_ptr<osg::PositionAttitudeTransform> _patFlightDir;

    osg::ref_ptr<osg::Switch> _switchFlightDir;

    void createAircraftSymbol();
    void createFlightDirector();
    void createHorizonLine();
    void createPitchScale();
    void createPitchScaleBar( osg::Geode *geode, osg::Vec3Array *v,
                              double pitch, double width, bool label );
    void createPitchScaleMask();
    void createRollPointer();
    void createRollScale();
    void createSkyAndGround();
};

} // end of g1000 namespace

////////////////////////////////////////////////////////////////////////////////

#endif // G1000_ADI_H
