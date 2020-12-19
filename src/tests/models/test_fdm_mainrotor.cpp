#include <QString>
#include <QtTest>

#include <cmath>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////

using namespace std;

////////////////////////////////////////////////////////////////////////////////

class MainRotorTest : public QObject
{
    Q_OBJECT

public:

    MainRotorTest();

    bool testFun( bool dir_ccw );

private Q_SLOTS:

    void initTestCase();
    void cleanupTestCase();

    void test();
};

////////////////////////////////////////////////////////////////////////////////

MainRotorTest::MainRotorTest() {}

////////////////////////////////////////////////////////////////////////////////

bool MainRotorTest::testFun( bool dir_ccw )
{
    bool _ccw = dir_ccw;

    int _nb = 4;

    double _r = 10.0;
    double _c = 0.2;
    double _e = 0.2;

    double _a = 5.0;
    double _b = 0.9;

    double _r2 = _r * _r;
    double _r3 = _r * _r * _r;
    double _r4 = _r * _r * _r * _r;
    double _b2 = _b * _b;
    double _b3 = _b * _b * _b;
    double _b4 = _b * _b * _b * _b;
    double _ad = M_PI * _r2;
    double _s = ( (double)_nb ) * _c * _r / _ad;

    double blade_mass = 100.0;

    double _sb = blade_mass * _r / 2.0;
    double _ib = blade_mass * _r2 / 3.0;

    double _omega = 30.0;

    double _theta_0 = M_PI_2 / 10.0;

    double _vel_i = 0.0;

    double omega2 = _omega * _omega;
    double omegaR = _omega * _r;

    // Lock number
    double airDensity = 1.225;
    double gamma = airDensity * _a * _c * _r4 / _ib;

    double airspeed = 10.0;

    double alpha = -M_PI_4;

    const double mu   = airspeed * cos( alpha ) / omegaR;
    const double mu2  = mu * mu;
    const double mu_z = airspeed * sin( alpha ) / omegaR;

    const double a_z = -9.81;

    // rotor inflow
    double lambda_i = _vel_i / omegaR;

    double lambda = mu_z - lambda_i;

    double p = 0.1;
    double q = 0.2;
    double r = 0.3;

    double beta_0 = 0.0;
    double beta_1c_cwas = 0.0;
    double beta_1s_cwas = 0.0;

    double t_beta_0 = 0.0;
    double t_beta_1c_cwas = 0.0;
    double t_beta_1s_cwas = 0.0;

    ///////////////////////////////


    double cd = _ccw ? 1.0 : -1.0;

    t_beta_0 = ( gamma / 2.0 ) * ( _b3 * lambda / 3.0 + cd * _b3 * p * mu / ( 6.0 * _omega ) + _b4 * _theta_0 / 4.0 + _b2 * _theta_0 * mu2 / 4.0 )
            - a_z * _sb / ( _ib * omega2 );

    t_beta_1c_cwas = 2.0 * mu * ( lambda + 4.0 * _b * _theta_0 / 3.0 ) / ( mu2 / 2.0 - _b2 )
            + cd * ( _b4 * p / _omega - cd * 16.0 * q / ( gamma * _omega ) ) / ( _b2 * ( mu2 / 2.0 - _b2 ) );

    t_beta_1s_cwas = - 4.0 * t_beta_0 * mu * _b / ( mu2 / 2.0 + _b2 ) / 3.0
            + ( _b4 * q / _omega + cd * 16.0 * p / ( gamma * _omega ) ) / ( _b2 * ( mu2 / 2.0 + _b2 ) );

    // flapping coefficients
    if ( _ccw )
    {
        beta_0 = ( gamma / 2.0 ) * ( _b3 * lambda / 3.0 + _b3 * p * mu / ( 6.0 * _omega ) + _b4 * _theta_0 / 4.0 + _b2 * _theta_0 * mu2 / 4.0 )
                - a_z * _sb / ( _ib * omega2 );

        beta_1c_cwas = 2.0 * mu * ( lambda + 4.0 * _b * _theta_0 / 3.0 ) / ( mu2 / 2.0 - _b2 )
                + ( _b4 * p / _omega - 16.0 * q / ( gamma * _omega ) ) / ( _b2 * ( mu2 / 2.0 - _b2 ) );

        beta_1s_cwas = - 4.0 * beta_0 * mu * _b / ( mu2 / 2.0 + _b2 ) / 3.0
                + ( _b4 * q / _omega + 16.0 * p / ( gamma * _omega ) ) / ( _b2 * ( mu2 / 2.0 + _b2 ) );
    }
    else
    {
        beta_0 = ( gamma / 2.0 ) * ( _b3 * lambda / 3.0 - _b3 * p * mu / ( 6.0 * _omega ) + _b4 * _theta_0 / 4.0 + _b2 * _theta_0 * mu2 / 4.0 )
                - a_z * _sb / ( _ib * omega2 );

        beta_1c_cwas = 2.0 * mu * ( lambda + 4.0 * _b * _theta_0 / 3.0 ) / ( mu2 / 2.0 - _b2 )
                - ( _b4 * p / _omega + 16.0 * q / ( gamma * _omega ) ) / ( _b2 * ( mu2 / 2.0 - _b2 ) );

        beta_1s_cwas = -4.0 * beta_0 * mu * _b / ( mu2 / 2.0 + _b2 ) / 3.0
                + ( _b4 * q / _omega - 16.0 * p / ( gamma * _omega ) ) / ( _b2 * ( mu2 / 2.0 + _b2 ) );
    }

    std::cout << ( _ccw ? "CCW" : "CW" ) << std::endl;
    std::cout << t_beta_0       << " " <<  beta_0       << std::endl;
    std::cout << t_beta_1c_cwas << " " <<  beta_1c_cwas << std::endl;
    std::cout << t_beta_1s_cwas << " " <<  beta_1s_cwas << std::endl;

    return ( fabs( t_beta_0       - beta_0       ) < 1.0e-9 )
        && ( fabs( t_beta_1c_cwas - beta_1c_cwas ) < 1.0e-9 )
        && ( fabs( t_beta_1s_cwas - beta_1s_cwas ) < 1.0e-9 );
}

////////////////////////////////////////////////////////////////////////////////

void MainRotorTest::initTestCase() {}

////////////////////////////////////////////////////////////////////////////////

void MainRotorTest::cleanupTestCase() {}

////////////////////////////////////////////////////////////////////////////////

void MainRotorTest::test()
{
    QVERIFY2( testFun( true  ) , "CCW" );
    QVERIFY2( testFun( false ) , "CW"  );
}

////////////////////////////////////////////////////////////////////////////////

QTEST_APPLESS_MAIN(MainRotorTest)

////////////////////////////////////////////////////////////////////////////////

#include "test_fdm_mainrotor.moc"
