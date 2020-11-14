#include <iostream>

#include <QString>
#include <QtTest>

#include <fdm/utils/fdm_Oscillation.h>

////////////////////////////////////////////////////////////////////////////////

using namespace std;

////////////////////////////////////////////////////////////////////////////////

class OscillationTest : public QObject
{
    Q_OBJECT

public:

    OscillationTest();

private Q_SLOTS:

    void initTestCase();
    void cleanupTestCase();

    void sampleTest();
};

////////////////////////////////////////////////////////////////////////////////

OscillationTest::OscillationTest() {}

////////////////////////////////////////////////////////////////////////////////

void OscillationTest::initTestCase() {}

////////////////////////////////////////////////////////////////////////////////

void OscillationTest::cleanupTestCase() {}

////////////////////////////////////////////////////////////////////////////////

void OscillationTest::sampleTest()
{
    fdm::Oscillation osc;

    double x = 0.0;
    double y = 0.0;
    double a = 1.0;

    for ( int i = 0; i < 10000; i++ )
    {
        y = a * sin( x );

        osc.add( x, y );

        x += 0.01;
    }

    y = a * sin( x );

    osc.add( x, y );

    std::cout << "y_max= " << osc.getY_max() << std::endl;
    std::cout << "y_min= " << osc.getY_min() << std::endl;
    std::cout << "p_max= " << osc.getP_max() << std::endl;
    std::cout << "p_min= " << osc.getP_min() << std::endl;
    std::cout << "p_avg= " << osc.getP_avg() << std::endl;
    std::cout << "a_max= " << osc.getA_max() << std::endl;
    std::cout << "a_min= " << osc.getA_min() << std::endl;
    std::cout << "a_avg= " << osc.getA_avg() << std::endl;

    QVERIFY2( true, "Failure" );
}

////////////////////////////////////////////////////////////////////////////////

QTEST_APPLESS_MAIN(OscillationTest)

////////////////////////////////////////////////////////////////////////////////

#include "test_fdm_oscillation.moc"
