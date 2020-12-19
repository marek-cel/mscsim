#include <iostream>

#include <QString>
#include <QtTest>

#include <fdm/ctrl/fdm_Lag.h>
#include <fdm/ctrl/fdm_PID.h>

////////////////////////////////////////////////////////////////////////////////

#define DT 0.1
#define TC 1.0
#define KP 5.0
#define KI 0.5
#define KD 0.1

////////////////////////////////////////////////////////////////////////////////

using namespace std;

////////////////////////////////////////////////////////////////////////////////

class TestPID : public QObject
{
    Q_OBJECT

public:

    TestPID();

private:

    std::vector< double > _y;

private Q_SLOTS:

    void initTestCase();
    void cleanupTestCase();

    void test_update();
};

////////////////////////////////////////////////////////////////////////////////

TestPID::TestPID() {}

////////////////////////////////////////////////////////////////////////////////

void TestPID::initTestCase()
{
    FILE *file = fopen( "../ctrl/data/test_fdm_pid.bin", "r" );

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

void TestPID::cleanupTestCase() {}

////////////////////////////////////////////////////////////////////////////////

void TestPID::test_update()
{
    double t = 0.0;
    double y = 0.0;

    fdm::PID *pid = new fdm::PID( KP, KI, KD );

    for ( unsigned int i = 0; i < _y.size(); i++ )
    {
        double u = ( t < 4.99 ) ? 0.0 : 1.0;

        int steps = 10;
        for ( int j = 0; j < steps; j++ )
        {
            double dt = DT / (double)steps;
            double e = u - y;
            pid->update( dt, e );
            y = fdm::Lag::update( pid->getValue(), y, dt, TC );
        }

        if ( t >= 5.0 )
        {
            //cout << y << " " << _y.at( i ) << endl;
            QVERIFY2( fabs( y - _y.at( i ) ) < 1.0e-1, "Failure" );
        }

        t += DT;
    }

    delete pid; pid = 0;
}

////////////////////////////////////////////////////////////////////////////////

QTEST_APPLESS_MAIN(TestPID)

////////////////////////////////////////////////////////////////////////////////

#include "test_fdm_pid.moc"
