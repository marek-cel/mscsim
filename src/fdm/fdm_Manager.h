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
        // controls inputs

        DataRef ctrlRoll;       ///< roll control data reference
        DataRef ctrlPitch;      ///< pitch control data reference
        DataRef ctrlYaw;        ///< yaw control data reference

        DataRef trimRoll;       ///< roll trim data reference
        DataRef trimPitch;      ///< pitch trim data reference
        DataRef trimYaw;        ///< yaw trim data reference

        DataRef brakeLeft;      ///< left brake data reference
        DataRef brakeRight;     ///< right brake data reference

        DataRef noseWheel;      ///< nose wheel data reference

        DataRef lgHandle;       ///< landing gear handle
        DataRef nwSteering;     ///< nose wheel steering data reference

        DataRef flaps;          ///< flaps data reference
        DataRef airbrake;       ///< airbrake data reference
        DataRef spoilers;       ///< spoilers data reference

        DataRef collective;     ///< collective data reference

        // controls outputs

        DataRef outAilerons;    ///<
        DataRef outElevator;    ///<
        DataRef outElevons;     ///<
        DataRef outRudder;      ///<
        DataRef outFlaps;       ///<
        DataRef outFlaperons;   ///<
        DataRef outLEF;         ///<
        DataRef outAirbrake;    ///<

        // landing gear inputs

        DataRef wheelN;         ///<
        DataRef wheelL;         ///<
        DataRef wheelR;         ///<

        // masses inputs

        DataRef pilot;          ///< pilot
        DataRef pilotL;         ///< left pilot
        DataRef pilotR;         ///< right pilot
        DataRef pilotF;         ///< forward pilot
        DataRef pilotA;         ///< aft pilot

        DataRef fuelTank;       ///< internal fuel
        DataRef fuelTankL;      ///< fuel tank left
        DataRef fuelTankR;      ///< fuel tank right
        DataRef fuelTankF;      ///< fuel tank forward
        DataRef fuelTankA;      ///< fuel tank aft

        DataRef cabinLoad;      ///< cabin load
        DataRef cargoTrunk;     ///< cargo trunk

        // engine inputs

        DataRef throttle  [ FDM_MAX_ENGINES ];  ///< engine throttle data references
        DataRef mixture   [ FDM_MAX_ENGINES ];  ///< engine mixture lever data references
        DataRef propeller [ FDM_MAX_ENGINES ];  ///< engine propeller lever data references
        DataRef fuel      [ FDM_MAX_ENGINES ];  ///< engine fuel data references
        DataRef ignition  [ FDM_MAX_ENGINES ];  ///< engine ignition data references
        DataRef starter   [ FDM_MAX_ENGINES ];  ///< engine starter data references

        // engine outputs

        DataRef engineOn   [ FDM_MAX_ENGINES ]; ///< engine state data references
        DataRef engineRPM  [ FDM_MAX_ENGINES ]; ///< engine RPM data references
        DataRef engineProp [ FDM_MAX_ENGINES ]; ///< propeller RPM data references
        DataRef engineNG   [ FDM_MAX_ENGINES ]; ///< NG data references
        DataRef engineN1   [ FDM_MAX_ENGINES ]; ///< N1 data references
        DataRef engineN2   [ FDM_MAX_ENGINES ]; ///< N2 data references
        DataRef engineTRQ  [ FDM_MAX_ENGINES ]; ///< engine TRQ data references
        DataRef engineEPR  [ FDM_MAX_ENGINES ]; ///< engine EPR data references
        DataRef engineMAP  [ FDM_MAX_ENGINES ]; ///< engine MAP data references
        DataRef engineEGT  [ FDM_MAX_ENGINES ]; ///< engine EGT data references
        DataRef engineITT  [ FDM_MAX_ENGINES ]; ///< engine ITT data references
        DataRef engineFF   [ FDM_MAX_ENGINES ]; ///< engine fuel flow data references

        DataRef mainRotorAzimuth;       ///< main rotor rotation angle (azimuth) data references
        DataRef mainRotorConingAngle;   ///< main rotor coning angle data references
        DataRef mainRotorDiskRoll;      ///< main rotor longitudinal flapping angle data references
        DataRef mainRotorDiskPitch;     ///< main rotor lateral flapping angle data references
        DataRef mainRotorCollective;    ///< main rotor collective pitch angle data references
        DataRef mainRotorCyclicLon;     ///< main rotor longitudinal cyclic pitch angle data references
        DataRef mainRotorCyclicLat;     ///< main rotor lateral cyclic pitch angle data references
        DataRef tailRotorAzimuth;       ///< tail rotor rotation angle data references
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

    bool m_verbose;                 ///< specify if extra information should be printed

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

    /**
     * Updates FDM output data.
     */
    void updateDataOutput();

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
