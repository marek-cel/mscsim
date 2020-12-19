#include <iostream>

#include <QString>
#include <QtTest>

#include <fdm/ctrl/fdm_HPF.h>

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

    std::vector< double > _y;
    std::vector< double > _y2;

    fdm::HPF *_hpf;
    fdm::HPF *_hpf2;

private Q_SLOTS:

    void initTestCase();
    void cleanupTestCase();

    void test_update1();
    void test_update2();
};

////////////////////////////////////////////////////////////////////////////////

HPFTest::HPFTest() :
    _hpf ( 0 ),
    _hpf2 ( 0 )
{}

////////////////////////////////////////////////////////////////////////////////

void HPFTest::initTestCase()
{
    _hpf = new fdm::HPF( OMEGA );
    _hpf2 = new fdm::HPF( OMEGA );

    FILE *file = fopen( "../ctrl/data/test_fdm_hpf.bin", "r" );

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

    FILE *file2 = fopen( "../ctrl/data/test_fdm_hpf_2.bin", "r" );

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

void HPFTest::cleanupTestCase()
{
    if ( _hpf ) delete _hpf;
    _hpf = 0;

    if ( _hpf2 ) delete _hpf2;
    _hpf2 = 0;
}

////////////////////////////////////////////////////////////////////////////////

void HPFTest::test_update1()
{
    double t = 0.0;
    double y = 0.0;

    int devider = 10;
    int index = 0;
    double dt = TIME_STEP / (double)devider;

    for ( unsigned int i = 0; i < devider * _y.size(); i++ )
    {
        double u = ( t < 0.99 ) ? 0.0 : 1.0;

        _hpf->update( dt, u );
        y = _hpf->getValue();

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

void HPFTest::test_update2()
{
    double t = 0.0;
    double y = 0.0;

    int devider = 10;
    int index = 0;
    double dt = TIME_STEP / (double)devider;

    for ( unsigned int i = 0; i < devider * _y2.size(); i++ )
    {
        double u = sin( t );

        _hpf2->update( dt, u );
        y = _hpf2->getValue();

        if ( i % devider == 0 )
        {
            if ( index > 0 )
            {
                //cout << y << " " << _y2.at( index - 1 ) << endl;
                QVERIFY2( fabs( y - _y2.at( index - 1 ) ) < 1.0e-1, "Failure" );
            }

            index++;
        }

        t += dt;
    }
}

////////////////////////////////////////////////////////////////////////////////

QTEST_APPLESS_MAIN(HPFTest)

////////////////////////////////////////////////////////////////////////////////

#include "test_fdm_hpf.moc"
