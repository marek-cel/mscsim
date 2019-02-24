#include <iostream>

#include <QString>
#include <QtTest>

#include <fdmSys/fdm_LPF.h>

////////////////////////////////////////////////////////////////////////////////

#define TIME_STEP 0.1

#define OMEGA 0.5

////////////////////////////////////////////////////////////////////////////////

using namespace std;

////////////////////////////////////////////////////////////////////////////////

class LPFTest : public QObject
{
    Q_OBJECT

public:

    LPFTest();

private:

    std::vector< double > m_y;
    std::vector< double > m_y2;

    fdm::LPF *m_lpf;
    fdm::LPF *m_lpf2;

private Q_SLOTS:

    void initTestCase();
    void cleanupTestCase();

    void sampleTest();
    void sampleTest2();
};

////////////////////////////////////////////////////////////////////////////////

LPFTest::LPFTest() : m_lpf ( 0 ), m_lpf2 ( 0 ) {}

////////////////////////////////////////////////////////////////////////////////

void LPFTest::initTestCase()
{
    m_lpf = new fdm::LPF( OMEGA );
    m_lpf2 = new fdm::LPF( OMEGA );

    FILE *file = fopen( "data/tst_fdm_lpf.bin", "r" );

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

    FILE *file2 = fopen( "data/tst_fdm_lpf_2.bin", "r" );

    if ( file2 )
    {
        char buffer[4];

        while ( fread( buffer, 1, 4, file2 ) == 4 )
        {
            float *y = (float*)(buffer);
            m_y2.push_back( *y );
        }

        fclose( file2 );
    }
}

////////////////////////////////////////////////////////////////////////////////

void LPFTest::cleanupTestCase()
{
    if ( m_lpf ) delete m_lpf;
    m_lpf = 0;

    if ( m_lpf2 ) delete m_lpf2;
    m_lpf2 = 0;
}

////////////////////////////////////////////////////////////////////////////////

void LPFTest::sampleTest()
{
    double t = 0.0;
    double y = 0.0;

    int devider = 10;
    int index = 0;
    double dt = TIME_STEP / (double)devider;

    for ( unsigned int i = 0; i < devider * m_y.size(); i++ )
    {
        double u = ( t < 0.99 ) ? 0.0 : 1.0;

        m_lpf->update( u, dt );
        y = m_lpf->getValue();

        if ( i % devider == 0 )
        {
            if ( index > 0 )
            {
                cout << y << " " << m_y.at( index - 1 ) << endl;
                QVERIFY2( fabs( y - m_y.at( index - 1 ) ) < 1.0e-2, "Failure" );
            }

            index++;
        }

        t += dt;
    }
}

////////////////////////////////////////////////////////////////////////////////

void LPFTest::sampleTest2()
{
    double t = 0.0;
    double y = 0.0;

    int devider = 10;
    int index = 0;
    double dt = TIME_STEP / (double)devider;

    //cout << "################################################" << endl;

    for ( unsigned int i = 0; i < devider * m_y2.size(); i++ )
    {
        double u = sin( t );

        m_lpf2->update( u, dt );
        y = m_lpf2->getValue();

        if ( i % devider == 0 )
        {
            if ( index > 0 )
            {
                cout << y << " " << m_y2.at( index - 1 ) << endl;
                QVERIFY2( fabs( y - m_y2.at( index - 1 ) ) < 1.0e-2, "Failure" );
            }

            index++;
        }

        t += dt;
    }
}

////////////////////////////////////////////////////////////////////////////////

QTEST_APPLESS_MAIN(LPFTest)

////////////////////////////////////////////////////////////////////////////////

#include "tst_fdm_lpf.moc"
