#include <iostream>

#include <QString>
#include <QtTest>

#include <fdm/sys/fdm_HPF.h>

////////////////////////////////////////////////////////////////////////////////

#define TIME_STEP 0.1

#define OMEGA 2.0

////////////////////////////////////////////////////////////////////////////////

using namespace std;

////////////////////////////////////////////////////////////////////////////////

class HPFTest : public QObject
{
    Q_OBJECT

public:

    HPFTest();

private:

    std::vector< double > m_y;
    std::vector< double > m_y2;

    fdm::HPF *m_hpf;
    fdm::HPF *m_hpf2;

private Q_SLOTS:

    void initTestCase();
    void cleanupTestCase();

    void sampleTest();
    void sampleTest2();
};

////////////////////////////////////////////////////////////////////////////////

HPFTest::HPFTest() :
    m_hpf ( 0 ),
    m_hpf2 ( 0 )
{}

////////////////////////////////////////////////////////////////////////////////

void HPFTest::initTestCase()
{
    m_hpf = new fdm::HPF( OMEGA );
    m_hpf2 = new fdm::HPF( OMEGA );

    FILE *file = fopen( "data/tst_fdm_hpf.bin", "r" );

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

    FILE *file2 = fopen( "data/tst_fdm_hpf_2.bin", "r" );

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

void HPFTest::cleanupTestCase()
{
    if ( m_hpf ) delete m_hpf;
    m_hpf = 0;

    if ( m_hpf2 ) delete m_hpf2;
    m_hpf2 = 0;
}

////////////////////////////////////////////////////////////////////////////////

void HPFTest::sampleTest()
{
    double t = 0.0;
    double y = 0.0;

    int devider = 10;
    int index = 0;
    double dt = TIME_STEP / (double)devider;

    for ( unsigned int i = 0; i < devider * m_y.size(); i++ )
    {
        double u = ( t < 0.99 ) ? 0.0 : 1.0;

        m_hpf->update( u, dt );
        y = m_hpf->getValue();

        if ( i % devider == 0 )
        {
            if ( index > 0 )
            {
                cout << y << " " << m_y.at( index - 1 ) << endl;
                QVERIFY2( fabs( y - m_y.at( index - 1 ) ) < 1.0e-1, "Failure" );
            }

            index++;
        }

        t += dt;
    }
}

////////////////////////////////////////////////////////////////////////////////

void HPFTest::sampleTest2()
{
    double t = 0.0;
    double y = 0.0;

    int devider = 10;
    int index = 0;
    double dt = TIME_STEP / (double)devider;

    for ( unsigned int i = 0; i < devider * m_y2.size(); i++ )
    {
        double u = sin( t );

        m_hpf2->update( u, dt );
        y = m_hpf2->getValue();

        if ( i % devider == 0 )
        {
            if ( index > 0 )
            {
                cout << y << " " << m_y2.at( index - 1 ) << endl;
                QVERIFY2( fabs( y - m_y2.at( index - 1 ) ) < 1.0e-1, "Failure" );
            }

            index++;
        }

        t += dt;
    }
}

////////////////////////////////////////////////////////////////////////////////

QTEST_APPLESS_MAIN(HPFTest)

////////////////////////////////////////////////////////////////////////////////

#include "tst_fdm_hpf.moc"
