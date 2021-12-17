#include <QString>
#include <QtTest>

#include <iostream>

#include <fdm/models/fdm_Atmosphere.h>

////////////////////////////////////////////////////////////////////////////////

#define NUM 9

////////////////////////////////////////////////////////////////////////////////

using namespace std;

////////////////////////////////////////////////////////////////////////////////

class AtmosphereTest : public QObject
{
    Q_OBJECT

public:

    static const double _h   [ NUM ];   // [m] altitude
    static const double _t   [ NUM ];   // [K] temperature
    static const double _p   [ NUM ];   // [Pa] pressure
    static const double _rho [ NUM ];   // [kg/m^3] density
    static const double _c   [ NUM ];   // [m/s] speed of sound
    static const double _mu  [ NUM ];   // [Pa*s] dynamic viscosity

    AtmosphereTest();

private:

    fdm::Atmosphere *_atmosphere;

private Q_SLOTS:

    void initTestCase();
    void cleanupTestCase();

    void stdConditions();

    void modifiedSeaLevelConditions();
};

////////////////////////////////////////////////////////////////////////////////


const double AtmosphereTest::_h[] = {
    0.0,
    5000.0,
    11000.0,
    15000.0,
    20000.0,
    32000.0,
    47000.0,
    51000.0,
    71000.0
};

// from Table I
const double AtmosphereTest::_t[] = {
    288.15,
    255.65,
    216.65,
    216.65,
    216.65,
    228.65,
    270.65,
    270.65,
    214.65
};

// from Table I
const double AtmosphereTest::_p[] = {
    101325.0,
     54019.0,
     22632.0,
     12044.0,
      5474.8,
       868.01,
       110.9,
        66.938,
         3.9564
};

// from Table I
const double AtmosphereTest::_rho[] = {
    1.225,
    0.73612,
    0.36392,
    0.19367,
    0.088035,
    0.013225,
    0.0014275,
    0.0008616,
    0.000064211
};

// from Table III
const double AtmosphereTest::_c[] = {
    340.29,
    320.53,
    295.07,
    295.07,
    295.07,
    303.13,
    329.8,
    329.8,
    293.7
};

// from Table III
const double AtmosphereTest::_mu[] = {
    1.7894e-5,
    1.6281e-5,
    1.4216e-5,
    1.4216e-5,
    1.4216e-5,
    1.4868e-5,
    1.7037e-5,
    1.7037e-5,
    1.4106e-5
};

////////////////////////////////////////////////////////////////////////////////

AtmosphereTest::AtmosphereTest() : _atmosphere ( 0 ) {}

////////////////////////////////////////////////////////////////////////////////

void AtmosphereTest::initTestCase()
{
    _atmosphere = new fdm::Atmosphere();

//    double h = 0.0;

//    _atmosphere->update( h );

//    double t   = _atmosphere->getTemperature();
//    double p   = _atmosphere->getPressure();
//    double rho = _atmosphere->getDensity();
//    double c   = _atmosphere->getSpeedOfSound();
//    double mu  = _atmosphere->getDynViscosity();
//    double nu  = _atmosphere->getKinViscosity();

//    std::cout << "h: "   << h   << std::endl;
//    std::cout << "t: "   << t   << std::endl;
//    std::cout << "p: "   << p   << std::endl;
//    std::cout << "rho: " << rho << std::endl;
//    std::cout << "c: "   << c   << std::endl;
//    std::cout << "mu: "  << mu  << std::endl;
//    std::cout << "nu: "  << nu  << std::endl;
}

////////////////////////////////////////////////////////////////////////////////

void AtmosphereTest::cleanupTestCase()
{
    if ( _atmosphere ) delete _atmosphere;
    _atmosphere = 0;
}

////////////////////////////////////////////////////////////////////////////////

void AtmosphereTest::stdConditions()
{
    _atmosphere->setPressureSL(    fdm::Atmosphere::_std_sl_p );
    _atmosphere->setTemperatureSL( fdm::Atmosphere::_std_sl_t );

    for ( int i = 0; i < NUM; i++ )
    {
        _atmosphere->update( _h[ i ] );

        double h   = _h[ i ];
        double t   = _atmosphere->getTemperature();
        double p   = _atmosphere->getPressure();
        double rho = _atmosphere->getDensity();
        double c   = _atmosphere->getSpeedOfSound();
        double mu  = _atmosphere->getDynViscosity();

        QVERIFY2( fabs( t   - _t   [ i ] ) < 1.0e-2 * fabs( _t   [ i ] ) , "Failure" );
        QVERIFY2( fabs( p   - _p   [ i ] ) < 1.0e-2 * fabs( _p   [ i ] ) , "Failure" );
        QVERIFY2( fabs( rho - _rho [ i ] ) < 1.0e-2 * fabs( _rho [ i ] ) , "Failure" );
        QVERIFY2( fabs( c   - _c   [ i ] ) < 1.0e-2 * fabs( _c   [ i ] ) , "Failure" );
        QVERIFY2( fabs( mu  - _mu  [ i ] ) < 1.0e-2 * fabs( _mu  [ i ] ) , "Failure" );
    }
}

////////////////////////////////////////////////////////////////////////////////

void AtmosphereTest::modifiedSeaLevelConditions()
{
    _atmosphere->setPressureSL(    fdm::Atmosphere::_std_sl_p + 1000.0 );
    _atmosphere->setTemperatureSL( fdm::Atmosphere::_std_sl_t + 10.0 );

    for ( int i = 2; i < NUM; i++ )
    {
        _atmosphere->update( _h[ i ] );

        double t   = _atmosphere->getTemperature();
        double p   = _atmosphere->getPressure();
        double rho = _atmosphere->getDensity();
        double c   = _atmosphere->getSpeedOfSound();
        double mu  = _atmosphere->getDynViscosity();

        QVERIFY2( fabs( t   - _t   [ i ] ) < 1.0e-2 * fabs( _t   [ i ] ) , "Failure" );
        QVERIFY2( fabs( p   - _p   [ i ] ) < 1.0e-2 * fabs( _p   [ i ] ) , "Failure" );
        QVERIFY2( fabs( rho - _rho [ i ] ) < 1.0e-2 * fabs( _rho [ i ] ) , "Failure" );
        QVERIFY2( fabs( c   - _c   [ i ] ) < 1.0e-2 * fabs( _c   [ i ] ) , "Failure" );
        QVERIFY2( fabs( mu  - _mu  [ i ] ) < 1.0e-2 * fabs( _mu  [ i ] ) , "Failure" );
    }
}

////////////////////////////////////////////////////////////////////////////////

QTEST_APPLESS_MAIN(AtmosphereTest)

////////////////////////////////////////////////////////////////////////////////

#include "test_fdm_atmospheretest.moc"
