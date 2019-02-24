#include <iostream>

#include <QString>
#include <QtTest>

#include <fdmSys/fdm_Lag2.h>

////////////////////////////////////////////////////////////////////////////////

#define TIME_STEP 0.1

#define TIME_CONSTANT_1 2.0
#define TIME_CONSTANT_2 3.0

////////////////////////////////////////////////////////////////////////////////

using namespace std;

////////////////////////////////////////////////////////////////////////////////

class Lag2Test : public QObject
{
    Q_OBJECT

public:

    Lag2Test();

private:

    std::vector< double > m_y;

    fdm::Lag2 *m_lag;

private Q_SLOTS:

    void initTestCase();
    void cleanupTestCase();

    void testUpdate();
};

////////////////////////////////////////////////////////////////////////////////

Lag2Test::Lag2Test() {}

////////////////////////////////////////////////////////////////////////////////

void Lag2Test::initTestCase()
{
    m_lag = new fdm::Lag2( TIME_CONSTANT_1, TIME_CONSTANT_2 );

    FILE *file = fopen( "data/tst_fdm_lag2.bin", "r" );

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

void Lag2Test::cleanupTestCase()
{
    if ( m_lag ) delete m_lag;
    m_lag = 0;
}

////////////////////////////////////////////////////////////////////////////////

void Lag2Test::testUpdate()
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
            m_lag->update( u, dt );
            y = m_lag->getValue();
        }

        cout << y << " " << m_y.at( i ) << endl;

        QVERIFY2( fabs( y - m_y.at( i ) ) < 1.0e-3, "Failure" );

        t += TIME_STEP;
    }
}

////////////////////////////////////////////////////////////////////////////////

QTEST_APPLESS_MAIN(Lag2Test)

////////////////////////////////////////////////////////////////////////////////

#include "tst_fdm_lag2.moc"
