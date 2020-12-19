#include <QString>
#include <QtTest>

#include <fdm/models/fdm_PistonEngine.h>
#include <fdm/utils/fdm_Misc.h>
#include <fdm/utils/fdm_Units.h>

////////////////////////////////////////////////////////////////////////////////

#define POWER_MAX 1000.0

////////////////////////////////////////////////////////////////////////////////

using namespace std;

////////////////////////////////////////////////////////////////////////////////

class PistonEngineTest : public QObject
{
    Q_OBJECT

public:

    class PistonEngine : public fdm::PistonEngine
    {
    public:

        double getManifoldAbsolutePressure( double throttle, double rpm,
                                            double airPressure )
        {
            return fdm::PistonEngine::getManifoldAbsolutePressure( throttle, rpm, airPressure );
        }
    };

    PistonEngineTest();

private:

    PistonEngine *_engine;

private Q_SLOTS:

    void initTestCase();
    void cleanupTestCase();

    void getManifoldAbsolutePressure();
    void getStaticPower();
};

////////////////////////////////////////////////////////////////////////////////

PistonEngineTest::PistonEngineTest() : _engine ( 0 ) {}

////////////////////////////////////////////////////////////////////////////////

void PistonEngineTest::initTestCase()
{
    _engine = new PistonEngine();
    // TODO
}

////////////////////////////////////////////////////////////////////////////////

void PistonEngineTest::cleanupTestCase()
{
    if ( _engine ) delete _engine;
    _engine = 0;
}

////////////////////////////////////////////////////////////////////////////////

void PistonEngineTest::getManifoldAbsolutePressure()
{
    double throttle = 0.0;      // <0.0;1.0> throttle
    double rpm = 0.0;           // [rpm] engine rpm
    double airPressure = 0.0;   // [Pa] air pressure

    for ( int i = 0; i < 10; i++ )
    {
        throttle = i * 0.1;
        rpm = 1000.0 + i * 100.0;
        airPressure = 1.0 + i * 0.05;

        double map_e = _engine->getManifoldAbsolutePressure( throttle, rpm, airPressure );
        double map_t = airPressure + fdm::Units::inhg2pa( ( 0.04635 * throttle - 0.0469 ) * rpm );
        map_t = fdm::Misc::max( 0.0, map_t );

        QVERIFY2( fabs( map_e - map_t ) < 1.0e-6, "Failure" );
    }
}

////////////////////////////////////////////////////////////////////////////////

void PistonEngineTest::getStaticPower()
{
    // TODO
}

////////////////////////////////////////////////////////////////////////////////

QTEST_APPLESS_MAIN(PistonEngineTest)

////////////////////////////////////////////////////////////////////////////////

#include "test_fdm_pistonengine.moc"
