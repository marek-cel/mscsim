#include <iostream>

#include <QString>
#include <QtTest>

#include <fdm/ctrl/fdm_Lead.h>

////////////////////////////////////////////////////////////////////////////////

#define TIME_STEP 0.1
#define TIME_CONSTANT 0.3

////////////////////////////////////////////////////////////////////////////////

using namespace std;

////////////////////////////////////////////////////////////////////////////////

class LeadTest : public QObject
{
    Q_OBJECT

public:

    LeadTest();

private:

    std::vector< double > _y;

    fdm::Lead *_lead;

private Q_SLOTS:

    void initTestCase();
    void cleanupTestCase();

    void test_update();
};

////////////////////////////////////////////////////////////////////////////////

LeadTest::LeadTest() {}

////////////////////////////////////////////////////////////////////////////////

void LeadTest::initTestCase()
{
    _lead = new fdm::Lead( TIME_CONSTANT );

    FILE *file = fopen( "../ctrl/data/test_fdm_lead.bin", "r" );

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

void LeadTest::cleanupTestCase()
{
    if ( _lead ) delete _lead;
    _lead = 0;
}

////////////////////////////////////////////////////////////////////////////////

void LeadTest::test_update()
{
    double t = 0.0;
    double y = 0.0;

    double u_prev = 0.0;
    double y_prev = 0.0;

    for ( unsigned int i = 0; i < _y.size(); i++ )
    {
        //double u = sin( t );

        int steps = 10;
        for ( int j = 0; j < steps; j++ )
        {
            double dt = TIME_STEP / (double)steps;
            double tt = t + (double)j * dt;

            double u = sin( tt );

            _lead->update( dt, u );
            y = _lead->getValue();

            //std::cout << sin( t ) << " " << sin( tt ) << " y= " << y << std::endl;

            if ( 0 )
            {
                double c1 = 1.0 / TIME_CONSTANT;
                double denom = 2.0 + dt * c1;
                double ca = dt * c1 / denom;
                double cb = ( 2.0 - dt * c1 ) / denom;

                y = ( u + u_prev ) * ca + y_prev * cb;

                u_prev = u;
                y_prev = y;
            }
        }

        //cout << y << " " << _y.at( i ) << endl;
        QVERIFY2( fabs( y - _y.at( i ) ) < 2.0e-2, "Failure" );

        t += TIME_STEP;
    }
}

////////////////////////////////////////////////////////////////////////////////

QTEST_APPLESS_MAIN(LeadTest)

////////////////////////////////////////////////////////////////////////////////

#include "test_fdm_lead.moc"
