#include <iostream>

#include <QString>
#include <QtTest>

#include <fdm/ctrl/fdm_LeadLag.h>

////////////////////////////////////////////////////////////////////////////////

#define TIME_STEP 0.1

#define C_1 1.0
#define C_2 0.0
#define C_3 1.0
#define C_4 1.0

////////////////////////////////////////////////////////////////////////////////

using namespace std;

////////////////////////////////////////////////////////////////////////////////

class LeadLagTest : public QObject
{
    Q_OBJECT

public:

    LeadLagTest();

private:

    std::vector< double > _y;

    fdm::LeadLag *_leadLag;

private Q_SLOTS:

    void initTestCase();
    void cleanupTestCase();

    void test_update();
};

////////////////////////////////////////////////////////////////////////////////

LeadLagTest::LeadLagTest() : _leadLag ( 0 ) {}

////////////////////////////////////////////////////////////////////////////////

void LeadLagTest::initTestCase()
{
    _leadLag = new fdm::LeadLag( C_1, C_2, C_3, C_4 );

    FILE *file = fopen( "../ctrl/data/test_fdm_lead_lag.bin", "r" );

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

void LeadLagTest::cleanupTestCase()
{
    if ( _leadLag ) delete _leadLag;
    _leadLag = 0;
}

////////////////////////////////////////////////////////////////////////////////

void LeadLagTest::test_update()
{
    double t = 0.0;
    double y = 0.0;

    int devider = 10;
    int index = 0;
    double dt = TIME_STEP / (double)devider;

    for ( unsigned int i = 0; i < devider * _y.size(); i++ )
    {
        double u = ( t < 0.99 ) ? 0.0 : 1.0;

        _leadLag->update( dt, u );
        y = _leadLag->getValue();

        if ( i % devider == 0 )
        {
            if ( index > 0 )
            {
                //cout << y << " " << _y.at( index - 1 ) << endl;
                QVERIFY2( fabs( y - _y.at( index - 1 ) ) < 1.0e-1, "Failure" );
            }

            index++;
        }

        t += dt;
    }
}

////////////////////////////////////////////////////////////////////////////////

QTEST_APPLESS_MAIN(LeadLagTest)

////////////////////////////////////////////////////////////////////////////////

#include "test_fdm_lead_lag.moc"
