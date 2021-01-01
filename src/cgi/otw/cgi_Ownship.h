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
#ifndef CGI_OWNSHIP_H
#define CGI_OWNSHIP_H

////////////////////////////////////////////////////////////////////////////////

#include <osg/PositionAttitudeTransform>
#include <osg/Switch>

#include <fdm/utils/fdm_Table1.h>
#include <fdm/xml/fdm_XmlNode.h>

#include <cgi/cgi_Module.h>

#include <cgi/otw/cgi_Rotor.h>
#include <cgi/otw/cgi_Scenery.h>
#include <cgi/otw/cgi_Vector.h>

////////////////////////////////////////////////////////////////////////////////

namespace cgi
{

/**
 * @brief Ownship entity class.
 */
class Ownship : public Module
{
public:

    struct ElementData
    {
        struct AxisData
        {
            double input_min;   ///< [-]
            double input_max;   ///< [-]

            double angle;       ///< [rad]
            double angle_min;   ///< [rad]
            double angle_max;   ///< [rad]

            double shift;       ///< [m]
            double shift_min;   ///< [m]
            double shift_max;   ///< [m]
        };

        AxisData x;
        AxisData y;
        AxisData z;
    };

    typedef std::vector< osg::ref_ptr<osg::PositionAttitudeTransform> > Elements;
    typedef std::vector< ElementData > ElementsData;

    /** @brief Constructor. */
    Ownship( const Module *parent = NULLPTR, Scenery *scenery = NULLPTR );

    /** @brief Destructor. */
    virtual ~Ownship();

    /** @brief Updates ownship. */
    void update();

private:

    Scenery *_scenery;      ///< scenery

    Rotor  *_rotor;         ///< rotor
    Vector *_vector;        ///< vector

    osg::Quat  _att_wgs;    ///< aircraft attitude
    osg::Vec3d _pos_wgs;    ///< aircraft position

    osg::ref_ptr<osg::PositionAttitudeTransform> _pat;          ///< aircraft position and attitude
    osg::ref_ptr<osg::PositionAttitudeTransform> _patOffset;    ///< model offset
    osg::ref_ptr<osg::PositionAttitudeTransform> _patRibbons;   ///< ribbons

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

    osg::ref_ptr<osg::PositionAttitudeTransform> _propeller1;   ///<
    osg::ref_ptr<osg::PositionAttitudeTransform> _propeller2;   ///<
    osg::ref_ptr<osg::PositionAttitudeTransform> _propeller3;   ///<
    osg::ref_ptr<osg::PositionAttitudeTransform> _propeller4;   ///<

    osg::ref_ptr<osg::PositionAttitudeTransform> _mainRotor;    ///<
    osg::ref_ptr<osg::PositionAttitudeTransform> _tailRotor;    ///<

    osg::ref_ptr<osg::PositionAttitudeTransform> _exhaust1;     ///< afterburner exhaust
    osg::ref_ptr<osg::PositionAttitudeTransform> _exhaust2;     ///< afterburner exhaust
    osg::ref_ptr<osg::PositionAttitudeTransform> _exhaust3;     ///< afterburner exhaust
    osg::ref_ptr<osg::PositionAttitudeTransform> _exhaust4;     ///< afterburner exhaust

    osg::ref_ptr<osg::Switch> _afterburner1;                    ///<
    osg::ref_ptr<osg::Switch> _afterburner2;                    ///<
    osg::ref_ptr<osg::Switch> _afterburner3;                    ///<
    osg::ref_ptr<osg::Switch> _afterburner4;                    ///<

    osg::ref_ptr<osg::Switch> _landingGear;                     ///<

    Elements _gearElements;                                     ///< landing gear elements
    Elements _flapElements;                                     ///< flaps elements
    Elements _abrkElements;                                     ///< airbrake elements
    Elements _rotorBlades;                                      ///< main rotor blades

    ElementsData _gearData;                                     ///< landing gear elements data
    ElementsData _flapData;                                     ///< flap elements data
    ElementsData _abrkData;                                     ///< airbrake elements data

    std::string _aircraftFile;                                  ///< aircraft file

    osg::Vec3d _pos_0_wgs;                                      ///< [m] initial position

    osg::Vec3 _wing_tip_l;                                      ///< [m] coordinates of left wing tip
    osg::Vec3 _wing_tip_r;                                      ///< [m] coordinates of right wing tip

    osg::Vec3 _rotor_center;                                    ///< [m] rotor hub center

    osg::ref_ptr<osg::Vec3Array> _trace_1;                      ///<
    osg::ref_ptr<osg::Vec3Array> _trace_2;                      ///<

    double _ab_angle;                                           ///< [rad]

    double _hinge_offset;                                       ///< [m] flapping hinge offset from rotor shaft
    double _rotor_radius;                                       ///< [m] rotor radius
    double _inclination;                                        ///< [rad] rotor inclination angle

    bool _double_trace;                                         ///<
    bool _helicopter;                                           ///< specifies if current aircraft is a helicopter

    void updateAxis( double input, ElementData::AxisData *axisData );

    void loadModel( const char *modelFile );

    osg::PositionAttitudeTransform* initAfterburnerAndGetExhaust( osg::Switch *afterburer );

    void readElementsData( const fdm::XmlNode &rootNode, ElementsData *elementsData );
    void readElementAxisData( const fdm::XmlNode &node, ElementData::AxisData *axisData );

    void readVec3( const fdm::XmlNode &node, osg::Vec3 *vec );

    void reload();
    void reset();

    void updateModel();
    void updateAfterburner( double value, osg::Switch *afterburner,
                            osg::PositionAttitudeTransform *exhaust );

    void updateTraces();
    void updateTrace( osg::Group *parent, osg::Vec3Array *positions,
                      const osg::Vec3 &color );
};

} // end of cgi namespace

////////////////////////////////////////////////////////////////////////////////

#endif // CGI_OWNSHIP_H
