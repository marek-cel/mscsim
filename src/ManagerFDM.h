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
#ifndef MANAGER_FDM_H
#define MANAGER_FDM_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/main/fdm_Aircraft.h>

#include <fdm/fdm_DataInp.h>
#include <fdm/fdm_DataOut.h>

#include <fdm/fdm_Recorder.h>

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

    inline bool getVerbose() const { return _verbose; }

    inline void setVerbose( bool verbose ) { _verbose = verbose; }

private:

    typedef DataInp::AircraftType AircraftType;

    typedef DataInp::StateInp StateInp;
    typedef DataOut::StateOut StateOut;

    Aircraft *_aircraft;            ///< aircraft simulation object
    Recorder *_recorder;            ///< recorder object

    DataInp _dataInp;               ///< input data
    DataOut _dataOut;               ///< output data

    AircraftType _aircraftType;     ///< aircraft type

    StateInp _stateInp;             ///< internal state input
    StateOut _stateOut;             ///< internal state output

    Vector3    _init_pos_wgs;       ///< [m] initial position expressed in WGS
    Quaternion _init_att_wgs;       ///< initial attitude expressed as quaternion of rotation from WGS to BAS

    UInt32 _initStep;               ///< initialization step number

    double _init_phi;               ///< [rad] initial roll angle
    double _init_tht;               ///< [rad] initial pitch angle
    double _init_alt;               ///< [m] initial altitude above ground level

    double _timeStep;               ///< [s] simulation time step
    double _realTime;               ///< [s] simulation real time

    double _compTimeMax;            ///< [s] maximum computations time
    double _compTimeSum;            ///< [s] sum of computations time
    double _compTimeSum2;           ///< [s] sum of computations time squared

    double _timeStepRaw;            ///< [s] simulation raw time step
    double _timeStepMin;            ///< [s] simulation minimum raw time step
    double _timeStepMax;            ///< [s] simulation maximum raw time step
    double _timeStepSum;            ///< [s] sum of simulation raw time step
    double _timeStepSum2;           ///< [s] sum of simulation raw time step squared

    unsigned int _timeSteps;        ///< number of time steps
    unsigned int _stepsLT_def;      ///< number of steps less than default time step
    unsigned int _stepsGT_def;      ///< number of steps greater than default time step

    bool _verbose;                  ///< specifies if extra information should be printed

    /**
     * Creates aircraft object.
     * @param aircraftType aircraft type
     * @return aircraft object on success null pointer on failure
     */
    Aircraft* createAircraft( AircraftType aircraftType );

    /**
     * Computes aircraft equilibrium in flight.
     */
    void initInFlight();

    /**
     * Computes aircraft equilibrium on ground.
     */
    void initOnGround();

    /**
     * Initializes recorder.
     */
    void initRecorder();

    /**
     * Updates initial position and attitude.
     */
    void updateInitialPositionAndAttitude();

    /**
     * Updates internal state input.
     */
    void updateStateInp();

    void updateStateIdle();
    void updateStateInit();
    void updateStateWork();
    void updateStateFreeze();
    void updateStatePause();
    void updateStateStop();

    void updateTimeStepStats( double compTime_0 );

    void printFlightEndInfo();
    void printState();
    void printTimeStepStats();
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

typedef fdm::Manager ManagerFDM;

////////////////////////////////////////////////////////////////////////////////

#endif // MANAGER_FDM_H
