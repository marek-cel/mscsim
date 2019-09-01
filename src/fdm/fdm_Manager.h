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

#include <fdm/main/fdm_Aircraft.h>

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

    inline bool getVerbose() const { return _verbose; }

    inline void setVerbose( bool verbose ) { _verbose = verbose; }

private:

    typedef DataInp::AircraftType AircraftType;

    typedef DataInp::PhaseInp PhaseInp;
    typedef DataOut::StateOut StateOut;

    Aircraft *_aircraft;            ///< aircraft simulation object

    DataInp _dataInp;               ///< input data
    DataOut _dataOut;               ///< output data

    AircraftType _aircraftType;     ///< aircraft type

    PhaseInp _phaseInp;             ///< internal phase input
    StateOut _stateOut;             ///< internal state output

    Vector3    _init_pos_wgs;       ///< [m] initial position expressed in WGS
    Quaternion _init_att_wgs;       ///< initial attitude expressed as quaternion of rotation from WGS to BAS

    UInt32 _initStep;               ///< initialization step number

    double _init_phi;               ///< [rad] initial roll angle
    double _init_tht;               ///< [rad] initial pitch angle
    double _init_alt;               ///< [m] initial altitude above ground level

    double _timeStep;               ///< [s] simulation time step
    double _realTime;               ///< [s] simulation real time

    unsigned int _timeSteps;        ///< number of time steps

    bool _verbose;                  ///< specify if extra information should be printed

    /**
     * Computes aircraft equilibrium in flight.
     */
    void initEquilibriumInFlight();

    /**
     * Computes aircraft equilibrium on ground.
     */
    void initEquilibriumOnGround();

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

    void printState();
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_MANAGER_H
