#include <iostream>

#include <QString>
#include <QtTest>

#include <fdmSys/fdm_Inertia2.h>

////////////////////////////////////////////////////////////////////////////////

#define TIME_STEP 0.1

#define TIME_CONSTANT_1 2.0
#define TIME_CONSTANT_2 3.0

////////////////////////////////////////////////////////////////////////////////

using namespace std;

////////////////////////////////////////////////////////////////////////////////

class Inertia2Test : public QObject
{
    Q_OBJECT

public:

    Inertia2Test();

private:

    std::vector< double > m_y;

    fdm::Inertia2 *m_inertia;

private Q_SLOTS:

    void initTestCase();
    void cleanupTestCase();

    void testUpdate();
};

////////////////////////////////////////////////////////////////////////////////

Inertia2Test::Inertia2Test() {}

////////////////////////////////////////////////////////////////////////////////

void Inertia2Test::initTestCase()
{
    m_inertia = new fdm::Inertia2( TIME_CONSTANT_1, TIME_CONSTANT_2 );

    FILE *file = fopen( "data/tst_fdm_inertia2.bin", "r" );

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

void Inertia2Test::cleanupTestCase()
{
    if ( m_inertia ) delete m_inertia;
    m_inertia = 0;
}

////////////////////////////////////////////////////////////////////////////////

void Inertia2Test::testUpdate()
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
            m_inertia->update( u, dt );
            y = m_inertia->getValue();
        }

        cout << y << " " << m_y.at( i ) << endl;

        QVERIFY2( fabs( y - m_y.at( i ) ) < 1.0e-3, "Failure" );

        t += TIME_STEP;
    }
}

////////////////////////////////////////////////////////////////////////////////

QTEST_APPLESS_MAIN(Inertia2Test)

////////////////////////////////////////////////////////////////////////////////

#include "tst_fdm_inertia2.moc"
