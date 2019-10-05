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
#ifndef CGI_OWNSHIP_H
#define CGI_OWNSHIP_H

////////////////////////////////////////////////////////////////////////////////

#include <osg/PositionAttitudeTransform>
#include <osg/Switch>

#include <fdm/utils/fdm_Table.h>
#include <fdm/xml/fdm_XmlNode.h>

#include <cgi/cgi_Module.h>

////////////////////////////////////////////////////////////////////////////////

namespace cgi
{

/**
 * @brief Ownship entity class.
 */
class Ownship : public Module
{
public:

    struct LandingGearElementData
    {
        struct AxisData
        {
            double input_min;
            double input_max;
            double angle_min;   ///< [rad]
            double angle_max;   ///< [rad]
        };

        AxisData x;
        AxisData y;
        AxisData z;
    };

    typedef std::vector< osg::ref_ptr<osg::PositionAttitudeTransform> > Blades;
    typedef std::vector< osg::ref_ptr<osg::PositionAttitudeTransform> > LandingGearElements;
    typedef std::vector< LandingGearElementData > LandingGearElementsData;

    /** Constructor. */
    Ownship( Module *parent = 0 );

    /** Destructor. */
    virtual ~Ownship();

    /** Updates ownship. */
    void update();

private:

    osg::Quat  _att_wgs;    ///< aircraft attitude
    osg::Vec3d _pos_wgs;    ///< aircraft position

    osg::ref_ptr<osg::PositionAttitudeTransform> _pat;          ///<
    osg::ref_ptr<osg::PositionAttitudeTransform> _patRibbons;   ///<

    osg::ref_ptr<osg::Switch> _switch;                          ///<
    osg::ref_ptr<osg::Switch> _switchRibbons;                   ///<

    osg::ref_ptr<osg::PositionAttitudeTransform> _aileronL;     ///< left aileron deflection
    osg::ref_ptr<osg::PositionAttitudeTransform> _aileronR;     ///< right aileron deflection
    osg::ref_ptr<osg::PositionAttitudeTransform> _elevatorL;    ///< left elevator deflection
    osg::ref_ptr<osg::PositionAttitudeTransform> _elevatorR;    ///< right elevator deflection
    osg::ref_ptr<osg::PositionAttitudeTransform> _rudderL;      ///< left rudder deflection
    osg::ref_ptr<osg::PositionAttitudeTransform> _rudderR;      ///< right rudder deflection
    osg::ref_ptr<osg::PositionAttitudeTransform> _flaperonL;    ///< left flaperon deflection
    osg::ref_ptr<osg::PositionAttitudeTransform> _flaperonR;    ///< right flaperon deflection
    osg::ref_ptr<osg::PositionAttitudeTransform> _elevonL;      ///< left elevon deflection
    osg::ref_ptr<osg::PositionAttitudeTransform> _elevonR;      ///< right elevon deflection
    osg::ref_ptr<osg::PositionAttitudeTransform> _flapL;        ///< left trailing edge flap deflection
    osg::ref_ptr<osg::PositionAttitudeTransform> _flapR;        ///< right trailing edge flap deflection
    osg::ref_ptr<osg::PositionAttitudeTransform> _lefL;         ///< left leading edge flap deflection
    osg::ref_ptr<osg::PositionAttitudeTransform> _lefR;         ///< right leading edge flap deflection
    osg::ref_ptr<osg::PositionAttitudeTransform> _airbrakeP;    ///< positive airbrake
    osg::ref_ptr<osg::PositionAttitudeTransform> _airbrakeN;    ///< negative airbrake

    osg::ref_ptr<osg::Switch> _landingGear;                     ///<

    LandingGearElements _landingGearElements;                   ///<

    osg::ref_ptr<osg::PositionAttitudeTransform> _propeller1;   ///<
    osg::ref_ptr<osg::PositionAttitudeTransform> _propeller2;   ///<
    osg::ref_ptr<osg::PositionAttitudeTransform> _propeller3;   ///<
    osg::ref_ptr<osg::PositionAttitudeTransform> _propeller4;   ///<

    osg::ref_ptr<osg::PositionAttitudeTransform> _mainRotor;    ///<
    osg::ref_ptr<osg::PositionAttitudeTransform> _tailRotor;    ///<

    Blades _mainRotorBlades;                                    ///< main rotor blades

    std::string _aircraftFile;                                  ///< aircraft file

    LandingGearElementsData _landingGearElementsData;           ///< landing gear elements

    osg::Vec3d _pos_0_wgs;                                      ///< [m] initial position

    osg::Vec3 _wing_tip_l;                                      ///< [m] coordinates of left wing tip
    osg::Vec3 _wing_tip_r;                                      ///< [m] coordinates of right wing tip

    osg::ref_ptr<osg::Vec3Array> _trace_1;                      ///<
    osg::ref_ptr<osg::Vec3Array> _trace_2;                      ///<

    bool _double_trace;                                         ///<

    double getAngle( double input, LandingGearElementData::AxisData *axisData );

    void loadModel( const std::string &modelFile );

    void readLandingGearElementsData( const fdm::XmlNode &rootNode,
                                      LandingGearElementsData *landingGearElementsData );
    void readLandingGearElementAxisData( const fdm::XmlNode &node,
                                         LandingGearElementData::AxisData *axisData );

    void readWingTipData( const fdm::XmlNode &node, osg::Vec3 *wing_tip );

    void reload();
    void reset();

    void updateModel();

    void updateTraces();
    void updateTrace( osg::Group *parent, osg::Vec3Array *positions,
                      const osg::Vec3 &color );
};

} // end of cgi namespace

////////////////////////////////////////////////////////////////////////////////

#endif // CGI_OWNSHIP_H
