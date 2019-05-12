#include <QString>
#include <QtTest>

#include <iostream>

#include <fdmMain/fdm_Atmosphere.h>

////////////////////////////////////////////////////////////////////////////////

#define NUM 9

////////////////////////////////////////////////////////////////////////////////

using namespace std;

////////////////////////////////////////////////////////////////////////////////

class AtmosphereTest : public QObject
{
    Q_OBJECT

public:

    static const double m_h   [ NUM ];  // [m] altitude
    static const double m_t   [ NUM ];  // [K] temperature
    static const double m_p   [ NUM ];  // [Pa] pressure
    static const double m_rho [ NUM ];  // [kg/m^3] density
    static const double m_c   [ NUM ];  // [m/s] speed of sound
    static const double m_mu  [ NUM ];  // [Pa*s] dynamic viscosity

    AtmosphereTest();

private:

    fdm::Atmosphere *m_atmosphere;

private Q_SLOTS:

    void initTestCase();
    void cleanupTestCase();

    void stdConditions();

    void modifiedSeaLevelConditions();
};

////////////////////////////////////////////////////////////////////////////////


const double AtmosphereTest::m_h[] = {
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
const double AtmosphereTest::m_t[] = {
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
const double AtmosphereTest::m_p[] = {
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
const double AtmosphereTest::m_rho[] = {
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
const double AtmosphereTest::m_c[] = {
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
const double AtmosphereTest::m_mu[] = {
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

AtmosphereTest::AtmosphereTest() : m_atmosphere ( 0 ) {}

////////////////////////////////////////////////////////////////////////////////

void AtmosphereTest::initTestCase()
{
    m_atmosphere = new fdm::Atmosphere();

//    double h = 0.0;

//    m_atmosphere->update( h );

//    double t   = m_atmosphere->getTemperature();
//    double p   = m_atmosphere->getPressure();
//    double rho = m_atmosphere->getDensity();
//    double c   = m_atmosphere->getSpeedOfSound();
//    double mu  = m_atmosphere->getDynViscosity();
//    double nu  = m_atmosphere->getKinViscosity();

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
    if ( m_atmosphere ) delete m_atmosphere;
    m_atmosphere = 0;
}

////////////////////////////////////////////////////////////////////////////////

void AtmosphereTest::stdConditions()
{
    m_atmosphere->setPressureSL(    fdm::Atmosphere::m_std_sl_p );
    m_atmosphere->setTemperatureSL( fdm::Atmosphere::m_std_sl_t );

    for ( int i = 0; i < NUM; i++ )
    {
        m_atmosphere->update( m_h[ i ] );

        double h   = m_h[ i ];
        double t   = m_atmosphere->getTemperature();
        double p   = m_atmosphere->getPressure();
        double rho = m_atmosphere->getDensity();
        double c   = m_atmosphere->getSpeedOfSound();
        double mu  = m_atmosphere->getDynViscosity();

        QVERIFY2( fabs( t   - m_t   [ i ] ) < 1.0e-2 * fabs( m_t   [ i ] ) , "Failure" );
        QVERIFY2( fabs( p   - m_p   [ i ] ) < 1.0e-2 * fabs( m_p   [ i ] ) , "Failure" );
        QVERIFY2( fabs( rho - m_rho [ i ] ) < 1.0e-2 * fabs( m_rho [ i ] ) , "Failure" );
        QVERIFY2( fabs( c   - m_c   [ i ] ) < 1.0e-2 * fabs( m_c   [ i ] ) , "Failure" );
        QVERIFY2( fabs( mu  - m_mu  [ i ] ) < 1.0e-2 * fabs( m_mu  [ i ] ) , "Failure" );
    }
}

////////////////////////////////////////////////////////////////////////////////

void AtmosphereTest::modifiedSeaLevelConditions()
{
    m_atmosphere->setPressureSL(    fdm::Atmosphere::m_std_sl_p + 1000.0 );
    m_atmosphere->setTemperatureSL( fdm::Atmosphere::m_std_sl_t + 10.0 );

    for ( int i = 2; i < NUM; i++ )
    {
        m_atmosphere->update( m_h[ i ] );

        double t   = m_atmosphere->getTemperature();
        double p   = m_atmosphere->getPressure();
        double rho = m_atmosphere->getDensity();
        double c   = m_atmosphere->getSpeedOfSound();
        double mu  = m_atmosphere->getDynViscosity();

        QVERIFY2( fabs( t   - m_t   [ i ] ) < 1.0e-2 * fabs( m_t   [ i ] ) , "Failure" );
        QVERIFY2( fabs( p   - m_p   [ i ] ) < 1.0e-2 * fabs( m_p   [ i ] ) , "Failure" );
        QVERIFY2( fabs( rho - m_rho [ i ] ) < 1.0e-2 * fabs( m_rho [ i ] ) , "Failure" );
        QVERIFY2( fabs( c   - m_c   [ i ] ) < 1.0e-2 * fabs( m_c   [ i ] ) , "Failure" );
        QVERIFY2( fabs( mu  - m_mu  [ i ] ) < 1.0e-2 * fabs( m_mu  [ i ] ) , "Failure" );
    }
}

////////////////////////////////////////////////////////////////////////////////

QTEST_APPLESS_MAIN(AtmosphereTest)

////////////////////////////////////////////////////////////////////////////////

#include "tst_fdm_atmospheretest.moc"
