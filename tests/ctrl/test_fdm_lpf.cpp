#include <iostream>

#include <QString>
#include <QtTest>

#include <fdm/ctrl/fdm_LPF.h>

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

    std::vector< double > _y;
    std::vector< double > _y2;

    fdm::LPF *_lpf;
    fdm::LPF *_lpf2;

private Q_SLOTS:

    void initTestCase();
    void cleanupTestCase();

    void test_update1();
    void test_update2();
};

////////////////////////////////////////////////////////////////////////////////

LPFTest::LPFTest() : _lpf ( 0 ), _lpf2 ( 0 ) {}

////////////////////////////////////////////////////////////////////////////////

void LPFTest::initTestCase()
{
    _lpf = new fdm::LPF( OMEGA );
    _lpf2 = new fdm::LPF( OMEGA );

    FILE *file = fopen( "../ctrl/data/test_fdm_lpf.bin", "r" );

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

    FILE *file2 = fopen( "../ctrl/data/test_fdm_lpf_2.bin", "r" );

    if ( file2 )
    {
        char buffer[4];

        while ( fread( buffer, 1, 4, file2 ) == 4 )
        {
            float *y = (float*)(buffer);
            _y2.push_back( *y );
        }

        fclose( file2 );
    }
    else
    {
        QFAIL( "Cannot open file" );
    }
}

////////////////////////////////////////////////////////////////////////////////

void LPFTest::cleanupTestCase()
{
    if ( _lpf ) delete _lpf;
    _lpf = 0;

    if ( _lpf2 ) delete _lpf2;
    _lpf2 = 0;
}

////////////////////////////////////////////////////////////////////////////////

void LPFTest::test_update1()
{
    double t = 0.0;
    double y = 0.0;

    int devider = 10;
    int index = 0;
    double dt = TIME_STEP / (double)devider;

    for ( unsigned int i = 0; i < devider * _y.size(); i++ )
    {
        double u = ( t < 0.99 ) ? 0.0 : 1.0;

        _lpf->update( dt, u );
        y = _lpf->getValue();

        if ( i % devider == 0 )
        {
            if ( index > 0 )
            {
                //cout << y << " " << _y.at( index - 1 ) << endl;
                QVERIFY2( fabs( y - _y.at( index - 1 ) ) < 1.0e-2, "Failure" );
            }

            index++;
        }

        t += dt;
    }
}

////////////////////////////////////////////////////////////////////////////////

void LPFTest::test_update2()
{
    double t = 0.0;
    double y = 0.0;

    int devider = 10;
    int index = 0;
    double dt = TIME_STEP / (double)devider;

    //cout << "################################################" << endl;

    for ( unsigned int i = 0; i < devider * _y2.size(); i++ )
    {
        double u = sin( t );

        _lpf2->update( dt, u );
        y = _lpf2->getValue();

        if ( i % devider == 0 )
        {
            if ( index > 0 )
            {
                //cout << y << " " << _y2.at( index - 1 ) << endl;
                QVERIFY2( fabs( y - _y2.at( index - 1 ) ) < 1.0e-2, "Failure" );
            }

            index++;
        }

        t += dt;
    }
}

////////////////////////////////////////////////////////////////////////////////

QTEST_APPLESS_MAIN(LPFTest)

////////////////////////////////////////////////////////////////////////////////

#include "test_fdm_lpf.moc"
