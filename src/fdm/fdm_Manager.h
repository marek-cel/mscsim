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
#ifndef FDM_MANAGER_H
#define FDM_MANAGER_H

////////////////////////////////////////////////////////////////////////////////

#include <fdmMain/fdm_Aircraft.h>

#include <fdm/fdm_DataInp.h>
#include <fdm/fdm_DataOut.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Simulation manager class.
 */
class Manager : public Base
{
public:

    /** Constructor. */
    Manager();

    /** Destructor. */
    virtual ~Manager();

    /**
     * Performs manager step.
     * @param timeStep [s] simulation time step
     * @param dataInp
     * @param dataOut
     */
    void step( double timeStep, const DataInp &dataInp, DataOut &dataOut );

    inline bool getVerbose() const { return m_verbose; }

    inline void setVerbose( bool verbose ) { m_verbose = verbose; }

private:

    typedef DataInp::AircraftType AircraftType;

    typedef DataInp::PhaseInp PhaseInp;
    typedef DataOut::StateOut StateOut;

    /** Common data references struct. */
    struct DataRefs
    {
        // flight controls inputs

        DataRef ctrlRoll;       ///< roll control data reference
        DataRef ctrlPitch;      ///< pitch control data reference
        DataRef ctrlYaw;        ///< yaw control data reference

        DataRef trimRoll;       ///< roll trim data reference
        DataRef trimPitch;      ///< pitch trim data reference
        DataRef trimYaw;        ///< yaw trim data reference

        DataRef brakeLeft;      ///< left brake data reference
        DataRef brakeRight;     ///< right brake data reference

        DataRef landingGear;    ///< landing gear data reference
        DataRef noseWheel;      ///< nose wheel data reference
        DataRef nwSteering;     ///< nose wheel steering data reference

        DataRef flaps;          ///< flaps data reference
        DataRef airbrake;       ///< airbrake data reference
        DataRef spoilers;       ///< spoilers data reference

        DataRef collective;     ///< collective data reference

        // engine inputs

        DataRef throttle  [ FDM_MAX_ENGINES ];  ///< engine throttle data references
        DataRef mixture   [ FDM_MAX_ENGINES ];  ///< engine mixture lever data references
        DataRef propeller [ FDM_MAX_ENGINES ];  ///< engine propeller lever data references
        DataRef fuel      [ FDM_MAX_ENGINES ];  ///< engine fuel data references
        DataRef ignition  [ FDM_MAX_ENGINES ];  ///< engine ignition data references
        DataRef starter   [ FDM_MAX_ENGINES ];  ///< engine starter data references

        // engine outputs

        DataRef engineOn  [ FDM_MAX_ENGINES ];  ///< engine state data references
        DataRef engineMAP [ FDM_MAX_ENGINES ];  ///< engine MAP data references
        DataRef engineRPM [ FDM_MAX_ENGINES ];  ///< engine RPM data references
        DataRef engineFF  [ FDM_MAX_ENGINES ];  ///< engine fuel flow data references
    };

    Aircraft *m_aircraft;           ///< aircraft simulation object

    DataInp m_dataInp;              ///< input data
    DataOut m_dataOut;              ///< output data

    AircraftType m_aircraftType;    ///< aircraft type

    PhaseInp m_phaseInp;            ///< internal phase input
    StateOut m_stateOut;            ///< internal state output

    DataRefs m_dr;                  ///< common data references

    Vector3    m_init_pos_wgs;      ///< [m] initial position expressed in WGS
    Quaternion m_init_att_wgs;      ///< initial attitude expressed as quaternion of rotation from WGS to BAS

    unsigned int m_initStep;        ///< initialization step number

    double m_init_phi;              ///< [rad] initial roll angle
    double m_init_tht;              ///< [rad] initial pitch angle
    double m_init_alt;              ///< [m] initial altitude above ground level

    double m_timeStep;              ///< [s] simulation time step
    double m_realTime;              ///< [s] simulation real time

    bool m_verbose;                 ///<

    /**
     * Initializes data references.
     */
    void dataRefsInit();

    /**
     * Resets data references.
     */
    void dataRefsReset();

    /**
     * Initializes aircraft.
     */
    void initAircraft();

    /**
     * Computes aircraft equilibrium in flight.
     */
    void initEquilibriumInFlight();

    /**
     * Computes aircraft equilibrium on ground.
     */
    void initEquilibriumOnGround();

    /**
     * Updates FDM input data.
     */
    void updateDataInput();
    void updateDataInput_Common();
    void updateDataInput_C130();
    void updateDataInput_C172();
    void updateDataInput_F16C();
    void updateDataInput_UH60();

    /**
     * Updates FDM output data.
     */
    void updateDataOutput();
    void updateDataOutput_Common();
    void updateDataOutput_C130();
    void updateDataOutput_C172();
    void updateDataOutput_F16C();
    void updateDataOutput_UH60();

    /**
     * Updates initial position and attitude.
     */
    void updateInitialPositionAndAttitude();

    /**
     * Updates internal phase input.
     */
    void updateInternalPhaseInp();

    void updatePhaseIdle();
    void updatePhaseInit();
    void updatePhaseWork();
    void updatePhasePause();
    void updatePhaseStop();
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_MANAGER_H
