#include <cmath>
#include <iostream>

#include <QString>
#include <QtTest>

#include <fdm/utils/fdm_RungeKutta4.h>

////////////////////////////////////////////////////////////////////////////////

#define ZERO 1.0e-9

#define T_MAX 10.0

#define T_STEP 1.0e-2

// max error due to T_STEP for 4th order method ( 10^-2 )^4 = 10^-6
#define DELTA_MAX 1.0e-6

////////////////////////////////////////////////////////////////////////////////

using namespace std;

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief The RungeKutta4Test class, a fdm::RungeKutta4 integrator unit test class.
 *
 * Linear homogeneous ordinary differential (Cauchy–Euler) equation is used to
 * test RungeKutta4 numerical integration class. Results obtained using a
 * fdm::RungeKutta4 are going to be compared with results of differential equation
 * analytical solution.
 *
 * Mass-Spring-Damper (MSD) model is used as an example. MSD is described
 * by the following differential equation:
 * m * (d^2 x)/(d t^2)  =  -k * x  -  c * dx/dt
 * Where:
 * m  -  mass
 * k  -  stiffness
 * c  -  damping
 *
 * This equation can be transformed to the following form:
 * m * (d^2 x)/(d t^2)  +  c * dx/dt  +  k * x  =  0                [1]
 *
 * Initial values are given as follows:
 * x_0 = x ( t_0 = 0 )
 * x_1 = x'( t_0 = 0 )
 *
 * Assuming that:
 * x = e^( r * t )
 * Then:
 * x'  = r   * e^( r * t )
 * x'' = r^2 * e^( r * t )
 *
 * Subsituting this into equation [1] gives:
 * m * r^2 * e^( r * t )  +  c * r * e^( r * t )  +  k * e^( r * t )  =  0
 *
 * Dividing this equation by e^( r * t ) gives:
 * m * r^2  +  c * r  +  k  =  0
 *
 * Discriminant of this equation is:
 * Delta  =  c^2 - 4 * m * k
 *
 * If Delta > 0 then equation [1] has solution in the following form:
 * x( t )  =  C_1 * e^( r_1 * t )  +  C_2 * e^( r_2 * t )
 * Where:
 * r_1  =  ( -c - sqrt( Delta ) ) / ( 2 * m )
 * r_2  =  ( -c + sqrt( Delta ) ) / ( 2 * m )
 * Then:
 * x'( t )  =  C_1 * r_1 * e^( r_1 * t )  +  C_2 * r_2 * e^( r_2 * t )
 * x'( 0 )  =  C_1 * r_1  +  C_2 * r_2
 *
 * If Delta = 0 then equation [1] has solution in the following form:
 * x( t )  =  ( C_1 * t + C_2 ) * e^( r_1 * t )
 * Where:
 * r_1  =  -c / ( 2 * m )
 * Then:
 * x'( t )  =  C_1  * e^( r_1 * t ) + ( C_1 * t + C_2 ) * r_1 * e^( r_1 * t )
 * x'( 0 )  =  C_1  +  C_2 * r_1
 *
 * If Delta < 0 then equation [1] has solution in the following form:
 * x( t )  =  e^( a * t ) * (  C_1 * cos( b*t )  +  C_2 * sin( b*t )  )
 * Where:
 * a  =  -c / ( 2 * m )
 * b  =  sqrt( 4 * m * k  -  c^2 ) / ( 2 * m )
 * Then:
 * x'( t )  =  a * e^( a * t ) * (  C_1 * cos( b*t )  +  C_2 * sin( b*t )  )
 *          +  e^( a * t ) * ( -C_1 * b * sin( b*t )  + C_2 * b * cos( b*t ) )
 * x'( 0 )  =  C_1 * a  +  C_2 * b
 *
 * @see Krysicki W., Wlodarski L.: Analiza matematyczna w zadaniach, Tom II. PWN, Ed. XXVII, 2018 [in Polish], p.287
 */
class RungeKutta4Test : public QObject
{
    Q_OBJECT

public:

    typedef fdm::RungeKutta4< RungeKutta4Test > Integrator;

    RungeKutta4Test();


    void computeStateDeriv( const fdm::VectorN &state,
                                  fdm::VectorN *deriv );

private:

    double _m;      ///< [kg]       mass
    double _k;      ///< [N/m]      stiffness
    double _c;      ///< [N/(m/s)]  damping`

    bool solve( double m,
                double k,
                double c,
                double x_0,
                double x_1 );

private Q_SLOTS:

    void initTestCase();
    void cleanupTestCase();

    void test1_1();
    void test1_2();
    void test1_3();

    void test2_1();
    void test2_2();
    void test2_3();

    void test3_1();
    void test3_2();
    void test3_3();
};

////////////////////////////////////////////////////////////////////////////////

double calcDelta( double a, double b, double c )
{
    return b*b - 4.0 * a * c;
}

////////////////////////////////////////////////////////////////////////////////

void RungeKutta4Test::computeStateDeriv( const fdm::VectorN &state,
                                               fdm::VectorN *deriv )
{
    (*deriv)( 0 ) = state( 1 );
    (*deriv)( 1 ) = -_k * state( 0 ) - _c * state( 1 );
}

////////////////////////////////////////////////////////////////////////////////

bool RungeKutta4Test::solve( double m,
                             double k,
                             double c,
                             double x_0,
                             double x_1 )
{
    _m = m;
    _k = k;
    _c = c;

    // state vector
    // index 0: x
    // index 1: dx/dt
    fdm::VectorN s( 2 );

    // initial conditions
    s( 0 ) = x_0;
    s( 1 ) = x_1;

    Integrator *integrator = new Integrator( this, &RungeKutta4Test::computeStateDeriv );

    double t = 0.0;
    double x = 0.0;

    // m * r^2  +  c * r  +  k  =  0
    double delta = calcDelta( _m, _c, _k );

    //std::cout << "Delta= " << delta << std::endl;

    if ( delta < -ZERO ) // numerical zero
    {
        // x( t )  =  e^( a * t ) * (  C_1 * cos( b*t )  +  C_2 * sin( b*t )  )

        // a  =  -c / ( 2 * m )
        // b  =  sqrt( 4 * m * k  -  c^2 ) / ( 2 * m )
        double a = -c / ( 2.0 * m );
        double b = sqrt( 4.0*m*k - c*c ) / ( 2.0 * m );

        // x ( t )  =  e^( a * t ) * (  C_1 * cos( b*t )  +  C_2 * sin( b*t )  )
        // x'( t )  =  a * e^( a * t ) * (  C_1 * cos( b*t )  +  C_2 * sin( b*t )  )
        //          +  e^( a * t ) * ( -C_1 * b * sin( b*t )  + C_2 * b * cos( b*t ) )
        //
        // x ( 0 )  =  C_1
        // x'( 0 )  =  C_1 * a  +  C_2 * b
        //
        // x_0  =  C_1
        // x_1  =  C_1 * a  +  C_2 * b
        //
        // C_1  =  x_0
        // C_2  = ( x_1 - C_1 * a ) / b
        double c_1 = x_0;
        double c_2 = ( x_1 - c_1 * a ) / b;

        while ( t <= T_MAX )
        {
            x = exp( a * t ) * (  c_1 * cos( b*t )  +  c_2 * sin( b*t )  );

            //std::cout << t << "," << x << "," << s( 0 ) << std::endl;

            if ( fabs( s( 0 ) - x ) > DELTA_MAX ) return false;

            t += T_STEP;

            integrator->integrate( T_STEP, &s );
        }
    }
    else if ( delta > ZERO ) // numerical zero
    {
        // x( t )  =  C_1 * e^( r_1 * t )  +  C_2 * e^( r_2 * t )

        //  r_1  =  ( -c - sqrt( Delta ) ) / ( 2 * m )
        //  r_2  =  ( -c + sqrt( Delta ) ) / ( 2 * m )
        double sqrt_delta = sqrt( delta );
        double r_1 = ( -_c - sqrt_delta ) / ( 2.0 * _m );
        double r_2 = ( -_c + sqrt_delta ) / ( 2.0 * _m );

        // x ( t )  =  C_1 * e^( r_1 * t )  +  C_2 * e^( r_2 * t )
        // x'( t )  =  C_1 * r_1 * e^( r_1 * t )  +  C_2 * r_2 * e^( r_2 * t )
        //
        // x ( 0 )  =  C_1  +  C_2
        // x'( 0 )  =  C_1 * r_1  +  C_2 * r_2
        //
        // x_0  =  C_1  +  C_2
        // x_1  =  C_1 * r_1  +  C_2 * r_2
        //
        // C_2  =  ( x_1 - x_0 * r_1 ) / ( r_2 - r_1 )
        // C_1  =  x_0 - C_2
        double c_2 = ( x_1 - x_0 * r_1 ) / ( r_2 - r_1 );
        double c_1 = x_0 - c_2;

        while ( t <= T_MAX )
        {
            x = c_1 * exp( r_1 * t ) + c_2 * exp( r_2 * t );

            //std::cout << t << "," << x << "," << s( 0 ) << std::endl;

            if ( fabs( s( 0 ) - x ) > DELTA_MAX ) return false;

            t += T_STEP;

            integrator->integrate( T_STEP, &s );
        }
    }
    else // delta == numerical zero
    {
        // x( t )  =  ( C_1 * t + C_2 ) * e^( r_1 * t )

        // r_1  =  -c / ( 2 * m )
        double r_1 = -c / ( 2.0 * m );

        // x ( t )  =  ( C_1 * t + C_2 ) * e^( r_1 * t )
        // x'( t )  =  C_1  * e^( r_1 * t ) + ( C_1 * t + C_2 ) * r_1 * e^( r_1 * t )
        //
        // x ( 0 )  =  C_2
        // x'( 0 )  =  C_1  +  C_2 * r_1
        //
        // x_0  =  C_2
        // x_1  =  C_1  +  C_2 * r_1
        //
        // C_2  = x_0
        // C_1  = x_1  -  C_2 * r_1
        double c_2  = x_0;
        double c_1  = x_1 - c_2 * r_1;

        while ( t <= T_MAX )
        {
            x = ( c_1 * t + c_2 ) * exp( r_1 * t );

            //std::cout << t << "," << x << "," << s( 0 ) << std::endl;

            if ( fabs( s( 0 ) - x ) > DELTA_MAX ) return false;

            t += T_STEP;

            integrator->integrate( T_STEP, &s );
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////

RungeKutta4Test::RungeKutta4Test() {}

////////////////////////////////////////////////////////////////////////////////

void RungeKutta4Test::initTestCase() {}

////////////////////////////////////////////////////////////////////////////////

void RungeKutta4Test::cleanupTestCase() {}

////////////////////////////////////////////////////////////////////////////////

void RungeKutta4Test::test1_1()
{
    //std::cout << "test1_1()" << std::endl;

    // model parameters:
    const double m = 1.0;       // [kg]
    const double k = 1.0;       // [N/m]
    const double c = 3.0;       // [N/(m/s)]

    // initial conditions:
    const double x_0 = 0.0;     // [m]
    const double x_1 = 1.0;     // [m/s]

    QVERIFY2( solve( m, k, c, x_0, x_1 ), "Failure" );
}

////////////////////////////////////////////////////////////////////////////////

void RungeKutta4Test::test1_2()
{
    //std::cout << "test1_2()" << std::endl;

    // model parameters:
    const double m = 1.0;       // [kg]
    const double k = 1.0;       // [N/m]
    const double c = 3.0;       // [N/(m/s)]

    // initial conditions:
    const double x_0 = 1.0;     // [m]
    const double x_1 = 0.0;     // [m/s]

    QVERIFY2( solve( m, k, c, x_0, x_1 ), "Failure" );
}

////////////////////////////////////////////////////////////////////////////////

void RungeKutta4Test::test1_3()
{
    //std::cout << "test1_3()" << std::endl;

    // model parameters:
    const double m = 1.0;       // [kg]
    const double k = 1.0;       // [N/m]
    const double c = 3.0;       // [N/(m/s)]

    // initial conditions:
    const double x_0 = 1.0;     // [m]
    const double x_1 = 1.0;     // [m/s]

    QVERIFY2( solve( m, k, c, x_0, x_1 ), "Failure" );
}

////////////////////////////////////////////////////////////////////////////////

void RungeKutta4Test::test2_1()
{
    //std::cout << "test2_1()" << std::endl;

    // model parameters:
    const double m = 1.0;       // [kg]
    const double k = 1.0;       // [N/m]
    const double c = 1.0;       // [N/(m/s)]

    // initial conditions:
    const double x_0 = 0.0;     // [m]
    const double x_1 = 1.0;     // [m/s]

    QVERIFY2( solve( m, k, c, x_0, x_1 ), "Failure" );
}

////////////////////////////////////////////////////////////////////////////////

void RungeKutta4Test::test2_2()
{
    //std::cout << "test2_2()" << std::endl;

    // model parameters:
    const double m = 1.0;       // [kg]
    const double k = 1.0;       // [N/m]
    const double c = 1.0;       // [N/(m/s)]

    // initial conditions:
    const double x_0 = 1.0;     // [m]
    const double x_1 = 0.0;     // [m/s]

    QVERIFY2( solve( m, k, c, x_0, x_1 ), "Failure" );
}

////////////////////////////////////////////////////////////////////////////////

void RungeKutta4Test::test2_3()
{
    //std::cout << "test2_3()" << std::endl;

    // model parameters:
    const double m = 1.0;       // [kg]
    const double k = 1.0;       // [N/m]
    const double c = 1.0;       // [N/(m/s)]

    // initial conditions:
    const double x_0 = 1.0;     // [m]
    const double x_1 = 1.0;     // [m/s]

    QVERIFY2( solve( m, k, c, x_0, x_1 ), "Failure" );
}

////////////////////////////////////////////////////////////////////////////////

void RungeKutta4Test::test3_1()
{
    //std::cout << "test3_1()" << std::endl;

    // model parameters:
    const double m = 1.0;       // [kg]
    const double k = 1.0;       // [N/m]
    const double c = 2.0;       // [N/(m/s)]

    // initial conditions:
    const double x_0 = 0.0;     // [m]
    const double x_1 = 1.0;     // [m/s]

    QVERIFY2( solve( m, k, c, x_0, x_1 ), "Failure" );
}

////////////////////////////////////////////////////////////////////////////////

void RungeKutta4Test::test3_2()
{
    //std::cout << "test3_2()" << std::endl;

    // model parameters:
    const double m = 1.0;       // [kg]
    const double k = 1.0;       // [N/m]
    const double c = 2.0;       // [N/(m/s)]

    // initial conditions:
    const double x_0 = 1.0;     // [m]
    const double x_1 = 0.0;     // [m/s]

    QVERIFY2( solve( m, k, c, x_0, x_1 ), "Failure" );
}

////////////////////////////////////////////////////////////////////////////////

void RungeKutta4Test::test3_3()
{
    //std::cout << "test3_3()" << std::endl;

    // model parameters:
    const double m = 1.0;       // [kg]
    const double k = 1.0;       // [N/m]
    const double c = 2.0;       // [N/(m/s)]

    // initial conditions:
    const double x_0 = 1.0;     // [m]
    const double x_1 = 1.0;     // [m/s]

    QVERIFY2( solve( m, k, c, x_0, x_1 ), "Failure" );
}

////////////////////////////////////////////////////////////////////////////////

QTEST_APPLESS_MAIN(RungeKutta4Test)

////////////////////////////////////////////////////////////////////////////////

#include "test_fdm_rungekutta4.moc"
