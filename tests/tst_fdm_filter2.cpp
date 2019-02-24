#include <iostream>

#include <QString>
#include <QtTest>

#include <fdmSys/fdm_Filter2.h>

////////////////////////////////////////////////////////////////////////////////

#define TIME_STEP 0.1

#define C_1 2.0
#define C_2 2.0
#define C_3 2.0
#define C_4 1.0
#define C_5 2.0
#define C_6 4.0

////////////////////////////////////////////////////////////////////////////////

using namespace std;

////////////////////////////////////////////////////////////////////////////////

class Filter2Test : public QObject
{
    Q_OBJECT

public:

    Filter2Test();

private:

    std::vector< double > m_y;

    fdm::Filter2 *m_filter;

private Q_SLOTS:

    void initTestCase();
    void cleanupTestCase();

    void sampleTest();
};

////////////////////////////////////////////////////////////////////////////////

Filter2Test::Filter2Test() : m_filter ( 0 ) {}

////////////////////////////////////////////////////////////////////////////////

void Filter2Test::initTestCase()
{
    m_filter = new fdm::Filter2( C_1, C_2, C_3, C_4, C_5, C_6 );

    FILE *file = fopen( "data/tst_fdm_filter2.bin", "r" );

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

void Filter2Test::cleanupTestCase()
{
    if ( m_filter ) delete m_filter;
    m_filter = 0;
}

////////////////////////////////////////////////////////////////////////////////

void Filter2Test::sampleTest()
{
    double t = 0.0;
    double y = 0.0;

    int devider = 10;
    int index = 0;
    double dt = TIME_STEP / (double)devider;

    for ( unsigned int i = 0; i < devider * m_y.size(); i++ )
    {
        double u = ( t < 0.99 ) ? 0.0 : 1.0;

        m_filter->update( u, dt );
        y = m_filter->getValue();

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

QTEST_APPLESS_MAIN(Filter2Test)

////////////////////////////////////////////////////////////////////////////////

#include "tst_fdm_filter2.moc"
