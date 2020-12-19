#include <iostream>

#include <QString>
#include <QtTest>

#include <fdm/ctrl/fdm_Filter2.h>

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

    std::vector< double > _y;

    fdm::Filter2 *_filter;

private Q_SLOTS:

    void initTestCase();
    void cleanupTestCase();

    void test_update();
};

////////////////////////////////////////////////////////////////////////////////

Filter2Test::Filter2Test() : _filter ( 0 ) {}

////////////////////////////////////////////////////////////////////////////////

void Filter2Test::initTestCase()
{
    _filter = new fdm::Filter2( C_1, C_2, C_3, C_4, C_5, C_6 );

    FILE *file = fopen( "../ctrl/data/test_fdm_filter2.bin", "r" );

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

void Filter2Test::cleanupTestCase()
{
    if ( _filter ) delete _filter;
    _filter = 0;
}

////////////////////////////////////////////////////////////////////////////////

void Filter2Test::test_update()
{
    double t = 0.0;
    double y = 0.0;

    int devider = 10;
    int index = 0;
    double dt = TIME_STEP / (double)devider;

    for ( unsigned int i = 0; i < devider * _y.size(); i++ )
    {
        double u = ( t < 0.99 ) ? 0.0 : 1.0;

        _filter->update( dt, u );
        y = _filter->getValue();

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

QTEST_APPLESS_MAIN(Filter2Test)

////////////////////////////////////////////////////////////////////////////////

#include "test_fdm_filter2.moc"
