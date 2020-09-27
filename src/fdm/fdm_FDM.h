/***************************************************************************//**
 *
 * @author Marek M. Cel <marekcel@marekcel.pl>
 *
 * @section LICENSE
 *
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
#ifndef FDM_FDM_H
#define FDM_FDM_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_DataInp.h>
#include <fdm/fdm_DataOut.h>

#include <fdm/fdm_Recorder.h>

#include <fdm/main/fdm_Aircraft.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/** Flight dynamics modelflight dynamics model wrapper class. */
class FDMEXPORT FDM : public Base
{
public:

    /** Constructor. */
    FDM( const DataInp *dataInpPtr, DataOut *dataOutPtr, bool verbose = false );

    /** Destructor. */
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

    /** Data references. */
    struct DataRefs
    {
        /** */
        struct Input
        {
            /** */
            struct Controls
            {
                DataRef roll;                       ///< roll controls data reference
                DataRef pitch;                      ///< pitch control data reference
                DataRef yaw;                        ///< yaw control data reference

                DataRef trim_roll;                  ///< roll trim data reference
                DataRef trim_pitch;                 ///< pitch trim data reference
                DataRef trim_yaw;                   ///< yaw trim data reference

                DataRef brake_l;                    ///< left brake data reference
                DataRef brake_r;                    ///< right brake data reference

                DataRef landing_gear;               ///< landing gear data reference
                DataRef wheel_nose;                 ///< nose wheel steering data reference

                DataRef flaps;                      ///< flaps data reference
                DataRef airbrake;                   ///< airbrake data reference
                DataRef spoilers;                   ///< spoilers data reference

                DataRef collective;                 ///< collective data reference

                DataRef lgh;                        ///< landing gear handle data reference
                DataRef nws;                        ///< nose wheel steering data reference
                DataRef abs;                        ///< anti-skid braking system data reference
            };

            /** */
            struct Engine
            {
                DataRef  throttle;                  ///< throttle data reference
                DataRef  mixture;                   ///< mixture lever data reference
                DataRef  propeller;                 ///< propeller lever data reference

                DataRef  fuel;                      ///< fuel state data reference
                DataRef  ignition;                  ///< ignition state data reference
                DataRef  starter;                   ///< starter state data reference
            };

            /** */
            struct Masses
            {
                DataRef pilot [ FDM_MAX_PILOTS ];   ///< pilots data reference
                DataRef tank  [ FDM_MAX_TANKS ];    ///< fuel tanks data reference
                DataRef cabin;                      ///< cabin data reference
                DataRef trunk;                      ///< cargo trunk data reference
                DataRef slung;                      ///< slung load data reference
            };

            Controls controls;                      ///< controls data
            Engine   engine[ FDM_MAX_ENGINES ];     ///< engines data
            Masses   masses;                        ///< masses data
        };

        Input  input;                               ///< input data
    }
    _dataRefs;                                      ///< data references

    const DataInp *_dataInpPtr;                     ///< input data pointer
    DataOut       *_dataOutPtr;                     ///< output data pointer

    DataInp _dataInp;                               ///< input data (internal)
    DataOut _dataOut;                               ///< output data (internal)

    DataNode *_rootNode;                            ///< data tree root node

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

    virtual void updateInitialPositionAndAttitude();

private:

    /** Using this constructor is forbidden. */
    FDM( const FDM & ) : Base() {}

    /** Initializes basic data tree. */
    void initDataTreeBasic();
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_FDM_H
