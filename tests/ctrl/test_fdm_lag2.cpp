#include <iostream>

#include <QString>
#include <QtTest>

#include <fdm/ctrl/fdm_Lag2.h>

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

    std::vector< double > _y;

    fdm::Lag2 *_lag;

private Q_SLOTS:

    void initTestCase();
    void cleanupTestCase();

    void test_update();
};

////////////////////////////////////////////////////////////////////////////////

Lag2Test::Lag2Test() {}

////////////////////////////////////////////////////////////////////////////////

void Lag2Test::initTestCase()
{
    _lag = new fdm::Lag2( TIME_CONSTANT_1, TIME_CONSTANT_2 );

    FILE *file = fopen( "../ctrl/data/test_fdm_lag2.bin", "r" );

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

void Lag2Test::cleanupTestCase()
{
    if ( _lag ) delete _lag;
    _lag = 0;
}

////////////////////////////////////////////////////////////////////////////////

void Lag2Test::test_update()
{
    double t = 0.0;
    double y = 0.0;

    for ( unsigned int i = 0; i < _y.size(); i++ )
    {
        double u = ( t < 0.99 ) ? 0.0 : 1.0;

        int steps = 10;
        for ( int j = 0; j < steps; j++ )
        {
            double dt = TIME_STEP / (double)steps;
            _lag->update( dt, u );
            y = _lag->getValue();
        }

        //cout << y << " " << _y.at( i ) << endl;

        QVERIFY2( fabs( y - _y.at( i ) ) < 1.0e-3, "Failure" );

        t += TIME_STEP;
    }
}

////////////////////////////////////////////////////////////////////////////////

QTEST_APPLESS_MAIN(Lag2Test)

////////////////////////////////////////////////////////////////////////////////

#include "test_fdm_lag2.moc"
