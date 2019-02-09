#include <iostream>

#include <QString>
#include <QtTest>

#include <fdmSys/fdm_Inertia.h>

////////////////////////////////////////////////////////////////////////////////

#define TIME_STEP 0.1
#define TIME_CONSTANT 2.0

////////////////////////////////////////////////////////////////////////////////

using namespace std;

////////////////////////////////////////////////////////////////////////////////

class InertiaTest : public QObject
{
    Q_OBJECT

public:

    InertiaTest();

private:

    std::vector< double > m_y;

private Q_SLOTS:

    void initTestCase();
    void cleanupTestCase();

    void testUpdate();
};

////////////////////////////////////////////////////////////////////////////////

InertiaTest::InertiaTest() {}

////////////////////////////////////////////////////////////////////////////////

void InertiaTest::initTestCase()
{
    FILE *file = fopen( "data/tst_fdm_inertia.bin", "r" );

    if ( file )
    {
        char buffer[4];

        while ( fread( buffer, 1, 4, file ) == 4 )
        {
            float *y = (float*)(buffer);
            m_y.push_back( *y );
        }

        fclose( file );
    }
}

////////////////////////////////////////////////////////////////////////////////

void InertiaTest::cleanupTestCase() {}

////////////////////////////////////////////////////////////////////////////////

void InertiaTest::testUpdate()
{
    double t = 0.0;
    double y = 0.0;

    for ( unsigned int i = 0; i < m_y.size(); i++ )
    {
        double u = ( t < 0.99 ) ? 0.0 : 1.0;

        int steps = 10;
        for ( int j = 0; j < steps; j++ )
        {
            double dt = TIME_STEP / (double)steps;
            y = fdm::Inertia::update( u, y, dt, TIME_CONSTANT );
        }

        cout << y << " " << m_y.at( i ) << endl;

        QVERIFY2( fabs( y - m_y.at( i ) ) < 1.0e-3, "Failure" );

        t += TIME_STEP;
    }
}

////////////////////////////////////////////////////////////////////////////////

QTEST_APPLESS_MAIN(InertiaTest)

////////////////////////////////////////////////////////////////////////////////

#include "tst_fdm_inertia.moc"
