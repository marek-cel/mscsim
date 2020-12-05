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
#ifndef FDM_MANAGER_H
#define FDM_MANAGER_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_DataInp.h>
#include <fdm/fdm_DataOut.h>

#include <fdm/main/fdm_FDM.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Simulation manager class.
 */
class Manager
{
public:

    /** @brief Constructor. */
    Manager( const DataInp *dataInpPtr, DataOut *dataOutPtr );

    /** @brief Destructor. */
    virtual ~Manager();

    /**
     * @brief Performs manager step.
     * @param timeStep [s] simulation time step
     */
    void step( double timeStep );

    inline bool getVerbose() const { return _verbose; }

    inline void setVerbose( bool verbose ) { _verbose = verbose; }

private:

    typedef DataInp::AircraftType AircraftType;

    typedef DataInp::StateInp StateInp;
    typedef DataOut::StateOut StateOut;

    const DataInp *_dataInpPtr;     ///< input data pointer
    DataOut       *_dataOutPtr;     ///< output data pointer

    FDM *_fdm;                      ///< flight dynamics model object

    AircraftType _aircraftType;     ///< aircraft type

    StateInp _stateInp;             ///< internal state input
    StateOut _stateOut;             ///< internal state output

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
     * @brief Creates flight dynamics model object.
     * @param aircraftType aircraft type
     * @return aircraft object on success null pointer on failure
     */
    FDM* createFDM( AircraftType aircraftType );

    /**
     * @brief Updates internal state input.
     */
    void updateStateInp();

    void updateStateIdle();
    void updateStateInit();
    void updateStateWork();
    void updateStatePause();
    void updateStateStop();

    void updateTimeStepStats( double compTime_0 );

    void printFlightEndInfo();
    void printState();
    void printTimeStepStats();
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_MANAGER_H
