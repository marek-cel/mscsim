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
#ifndef CGI_ENTITIES_H
#define CGI_ENTITIES_H

////////////////////////////////////////////////////////////////////////////////

#include <fdmUtils/fdm_Table.h>

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

    typedef std::vector< osg::ref_ptr<osg::PositionAttitudeTransform> > Blades;

    /** Constructor. */
    Ownship( Module *parent = 0 );

    /** Destructor. */
    virtual ~Ownship();

    /** Updates ownship. */
    void update();

private:

    osg::ref_ptr<osg::PositionAttitudeTransform> m_pat;         ///<

    osg::ref_ptr<osg::Switch> m_switch;                         ///<

    osg::ref_ptr<osg::PositionAttitudeTransform> m_aileronL;    ///< left aileron deflection
    osg::ref_ptr<osg::PositionAttitudeTransform> m_aileronR;    ///< right aileron deflection
    osg::ref_ptr<osg::PositionAttitudeTransform> m_elevatorL;   ///< left elevator deflection
    osg::ref_ptr<osg::PositionAttitudeTransform> m_elevatorR;   ///< right elevator deflection
    osg::ref_ptr<osg::PositionAttitudeTransform> m_rudderL;     ///< left rudder deflection
    osg::ref_ptr<osg::PositionAttitudeTransform> m_rudderR;     ///< right rudder deflection
    osg::ref_ptr<osg::PositionAttitudeTransform> m_flaperonL;   ///< left flaperon deflection
    osg::ref_ptr<osg::PositionAttitudeTransform> m_flaperonR;   ///< right flaperon deflection
    osg::ref_ptr<osg::PositionAttitudeTransform> m_elevonL;     ///< left elevon deflection
    osg::ref_ptr<osg::PositionAttitudeTransform> m_elevonR;     ///< right elevon deflection
    osg::ref_ptr<osg::PositionAttitudeTransform> m_flapLEL;     ///< left leading edge flap deflection
    osg::ref_ptr<osg::PositionAttitudeTransform> m_flapLER;     ///< right leading edge flap deflection
    osg::ref_ptr<osg::PositionAttitudeTransform> m_flapTEL;     ///< left trailing edge flap deflection
    osg::ref_ptr<osg::PositionAttitudeTransform> m_flapTER;     ///< right trailing edge flap deflection
    osg::ref_ptr<osg::PositionAttitudeTransform> m_airbrakeP;   ///< positive airbrake
    osg::ref_ptr<osg::PositionAttitudeTransform> m_airbrakeN;   ///< negative airbrake

    osg::ref_ptr<osg::Switch> m_landingGear;                    ///<

    osg::ref_ptr<osg::PositionAttitudeTransform> m_propeller1;  ///<
    osg::ref_ptr<osg::PositionAttitudeTransform> m_propeller2;  ///<
    osg::ref_ptr<osg::PositionAttitudeTransform> m_propeller3;  ///<
    osg::ref_ptr<osg::PositionAttitudeTransform> m_propeller4;  ///<

    osg::ref_ptr<osg::PositionAttitudeTransform> m_mainRotor;   ///<
    osg::ref_ptr<osg::PositionAttitudeTransform> m_tailRotor;   ///<

    Blades m_mainRotorBlades;

    std::string m_aircraftFile;

    fdm::Table m_ailerons;
    fdm::Table m_elevator;
    fdm::Table m_rudder;
    fdm::Table m_flaps;

    double m_airbrake;
    double m_coefElev;
    double m_coefFlap;

    void loadModel( const std::string &modelFile );

    void reload();
    void reset();
};

} // end of cgi namespace

////////////////////////////////////////////////////////////////////////////////

#endif // CGI_ENTITIES_H
