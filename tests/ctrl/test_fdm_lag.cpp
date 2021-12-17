#include <iostream>

#include <QString>
#include <QtTest>

#include <fdm/ctrl/fdm_Lag.h>

////////////////////////////////////////////////////////////////////////////////

#define TIME_STEP 0.1
#define TIME_CONSTANT 2.0

////////////////////////////////////////////////////////////////////////////////

using namespace std;

////////////////////////////////////////////////////////////////////////////////

class LagTest : public QObject
{
    Q_OBJECT

public:

    LagTest();

private:

    std::vector< double > _y;

    fdm::Lag *_lag;

private Q_SLOTS:

    void initTestCase();
    void cleanupTestCase();

    void test_update();
};

////////////////////////////////////////////////////////////////////////////////

LagTest::LagTest() {}

////////////////////////////////////////////////////////////////////////////////

void LagTest::initTestCase()
{
    _lag = new fdm::Lag( TIME_CONSTANT );

    FILE *file = fopen( "../ctrl/data/test_fdm_lag.bin", "r" );

    if ( file )
    {
        char buffer[4];

        while ( fread( buffer, 1, 4, file ) == 4 )
        {
            float *y = (float*)(buffer);
            _y.push_back( *y );
        }

        fclose( file );
    }
    else
    {
        QFAIL( "Cannot open file" );
    }
}

////////////////////////////////////////////////////////////////////////////////

void LagTest::cleanupTestCase()
{
    if ( _lag ) delete _lag;
    _lag = 0;
}

////////////////////////////////////////////////////////////////////////////////

void LagTest::test_update()
{
    double t = 0.0;
    double y = 0.0;

    double u_prev = 0.0;
    double y_prev = 0.0;

    for ( unsigned int i = 0; i < _y.size(); i++ )
    {
        double u = ( t < 0.99 ) ? 0.0 : 1.0;

        int steps = 10;
        for ( int j = 0; j < steps; j++ )
        {
            double dt = TIME_STEP / (double)steps;
            _lag->update( dt, u );
            y = _lag->getValue();

            if ( 0 )
            {
                double c1 = 1.0 / TIME_CONSTANT;
                double denom = 2.0 + dt * c1;
                double ca = dt * c1 / denom;
                double cb = ( 2.0 - dt * c1 ) / denom;

                y = ( u + u_prev ) * ca + y_prev * cb;

                u_prev = u;
                y_prev = y;
            }
        }

        //cout << y << " " << _y.at( i ) << endl;

        QVERIFY2( fabs( y - _y.at( i ) ) < 1.0e-3, "Failure" );

        t += TIME_STEP;
    }
}

////////////////////////////////////////////////////////////////////////////////

QTEST_APPLESS_MAIN(LagTest)

////////////////////////////////////////////////////////////////////////////////

#include "test_fdm_lag.moc"
