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
#ifndef FDM_WRAPPER_H
#define FDM_WRAPPER_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_DataInp.h>
#include <fdm/fdm_DataOut.h>

#include <fdm/fdm_Aircraft.h>
#include <fdm/fdm_Recorder.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/** Fight dynamics model wrapper class. */
class FDMEXPORT FDM : public Base
{
public:

    /** @brief Constructor. */
    FDM( const DataInp *dataInpPtr, DataOut *dataOutPtr, bool verbose = false );

    /** @brief Destructor. */
    virtual ~FDM();

    /** */
    virtual void initialize();

    /** */
    virtual void update( double timeStep );

    /** */
    virtual void printInitialConditions();

    /** */
    virtual void printState();

    inline DataOut::Crash getCrash() const { return _aircraft->getCrash(); }

    inline bool isReady() const { return _ready; }

    inline bool isReplaying() const { return _recorder->isReplaying(); }

protected:

    Input::DataRefs _dataRefs;                      ///< data references

    const DataInp *_dataInpPtr;                     ///< input data pointer
    DataOut       *_dataOutPtr;                     ///< output data pointer

    DataInp _dataInp;                               ///< input data (internal)
    DataOut _dataOut;                               ///< output data (internal)

    Input *_input;                                  ///< input data tree root node

    Aircraft *_aircraft;                            ///< aircraft model
    Recorder *_recorder;                            ///< recorder object

    Vector3    _init_pos_wgs;                       ///< [m] initial position expressed in WGS
    Quaternion _init_att_wgs;                       ///< initial attitude expressed as quaternion of rotation from WGS to BAS

    UInt32 _initStep;                               ///< initialization step number

    double _init_g_coef_p;                          ///< initialization iteration coefficient
    double _init_g_coef_q;                          ///< initialization iteration coefficient
    double _init_g_coef_n;                          ///< initialization iteration coefficient

    double _init_phi;                               ///< [rad] initial roll angle
    double _init_tht;                               ///< [rad] initial pitch angle
    double _init_alt;                               ///< [m] initial altitude above ground level

    bool _initialized;                              ///< specifies if flight dynamics model is initialized
    bool _ready;                                    ///< specifies if flight dynamics model is ready
    bool _verbose;                                  ///< specifies if extra information should be printed

    virtual void initializeOnGround();
    virtual void initializeInFlight();

    virtual void initializeRecorder();

    virtual void updateDataInp();
    virtual void updateDataOut();

    virtual void updateAndSetDataInp();
    virtual void updateAndSetDataOut();

    virtual void updateEnvironment();

    virtual void updateInitialPositionAndAttitude();

private:

    /** Using this constructor is forbidden. */
    FDM( const FDM & ) : Base() {}

    /** Initializes basic data tree. */
    void initDataTreeBasic();
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_WRAPPER_H
