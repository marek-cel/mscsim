#include <iostream>

#include <QString>
#include <QtTest>

#include <fdm/sys/fdm_Lead.h>

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

    std::vector< double > m_y;

    fdm::Lead *m_lead;

private Q_SLOTS:

    void initTestCase();
    void cleanupTestCase();

    void testUpdate();
};

////////////////////////////////////////////////////////////////////////////////

LeadTest::LeadTest() {}

////////////////////////////////////////////////////////////////////////////////

void LeadTest::initTestCase()
{
    m_lead = new fdm::Lead( TIME_CONSTANT );

    FILE *file = fopen( "data/tst_fdm_lead.bin", "r" );

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

void LeadTest::cleanupTestCase()
{
    if ( m_lead ) delete m_lead;
    m_lead = 0;
}

////////////////////////////////////////////////////////////////////////////////

void LeadTest::testUpdate()
{
    double t = 0.0;
    double y = 0.0;

    double u_prev = 0.0;
    double y_prev = 0.0;

    for ( unsigned int i = 0; i < m_y.size(); i++ )
    {
        //double u = sin( t );

        int steps = 10;
        for ( int j = 0; j < steps; j++ )
        {
            double dt = TIME_STEP / (double)steps;
            double tt = t + (double)j * dt;

            double u = sin( tt );

            m_lead->update( u, dt );
            y = m_lead->getValue();

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

        cout << y << " " << m_y.at( i ) << endl;
        QVERIFY2( fabs( y - m_y.at( i ) ) < 2.0e-2, "Failure" );

        t += TIME_STEP;
    }
}

////////////////////////////////////////////////////////////////////////////////

QTEST_APPLESS_MAIN(LeadTest)

////////////////////////////////////////////////////////////////////////////////

#include "tst_fdm_lead.moc"
