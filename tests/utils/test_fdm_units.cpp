#include <iostream>

#include <QString>
#include <QtTest>

#include <fdm/utils/fdm_Units.h>

////////////////////////////////////////////////////////////////////////////////

using namespace std;

////////////////////////////////////////////////////////////////////////////////

class UnitsTest : public QObject
{
    Q_OBJECT

public:

    UnitsTest();

private Q_SLOTS:

    void initTestCase();
    void cleanupTestCase();

    void deg2rad();
    void rad2deg();
    void m2ft();
    void m2mi();
    void m2nmi();
    void m2km();
    void ft2m();
    void ft2km();
    void ft2mi();
    void ft2nmi();
    void km2m();
    void km2ft();
    void km2mi();
    void km2nmi();
    void mi2m();
    void mi2ft();
    void mi2km();
    void mi2nmi();
    void nmi2m();
    void nmi2ft();
    void nmi2km();
    void nmi2mi();
    void cuin2m3();
    void l2m3();
    void mps2fpm();
    void mps2fps();
    void mps2kmh();
    void mps2kts();
    void mps2mph();
    void fpm2fps();
    void fpm2kmh();
    void fpm2kts();
    void fpm2mph();
    void fpm2mps();
    void fps2fpm();
    void fps2kmh();
    void fps2kts();
    void fps2mph();
    void fps2mps();
    void kmh2fpm();
    void kmh2fps();
    void kmh2kts();
    void kmh2mph();
    void kmh2mps();
    void kts2fpm();
    void kts2fps();
    void kts2kmh();
    void kts2mph();
    void kts2mps();
    void mph2fpm();
    void mph2fps();
    void mph2kmh();
    void mph2kts();
    void mph2mps();
    void rpm2rad_s();
    void rad_s2rpm();
    void kg2lb();
    void lb2kg();
    void n2lbf();
    void lbf2n();
    void pa2psi();
    void pa2inhg();
    void pa2mb();
    void psi2pa();
    void psi2inhg();
    void psi2mb();
    void inhg2pa();
    void inhg2psi();
    void inhg2mb();
    void mb2pa();
    void mb2psi();
    void mb2inhg();
    void ps2hp();
    void ps2w();
    void ps2kw();
    void hp2ps();
    void hp2w();
    void hp2kw();
    void w2ps();
    void w2hp();
    void w2kw();
    void kw2ps();
    void kw2hp();
    void kw2w();
    void c2f();
    void c2k();
    void f2c();
    void f2k();
    void k2c();
    void k2f();
    void g_kWh_2_kg_Ws();
    void g_kNs_2_kg_Ns();
};

////////////////////////////////////////////////////////////////////////////////

UnitsTest::UnitsTest() {}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::initTestCase() {}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::cleanupTestCase() {}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::deg2rad()
{
    double v1 = fdm::Units::deg2rad(  90.0 );
    double v2 = fdm::Units::deg2rad( 180.0 );
    double v3 = fdm::Units::deg2rad( 360.0 );

    QVERIFY2( fabs( v1 -       M_PI_2 ) < 1.0e-9, "Failure v1" );
    QVERIFY2( fabs( v2 -       M_PI   ) < 1.0e-9, "Failure v2" );
    QVERIFY2( fabs( v3 - 2.0 * M_PI   ) < 1.0e-9, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::rad2deg()
{
    double v1 = fdm::Units::rad2deg(       M_PI_2 );
    double v2 = fdm::Units::rad2deg(       M_PI   );
    double v3 = fdm::Units::rad2deg( 2.0 * M_PI   );

    QVERIFY2( fabs( v1 -  90.0 ) < 1.0e-9, "Failure v1" );
    QVERIFY2( fabs( v2 - 180.0 ) < 1.0e-9, "Failure v2" );
    QVERIFY2( fabs( v3 - 360.0 ) < 1.0e-9, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::m2ft()
{
    double v1 = fdm::Units::m2ft( 1.0 );
    double v2 = fdm::Units::m2ft( 2.0 );
    double v3 = fdm::Units::m2ft( 3.0 );

    QVERIFY2( fabs( v1 - 3.2808399 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 6.5616798 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 9.8425197 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::m2mi()
{
    double v1 = fdm::Units::m2mi( 1000.0 );
    double v2 = fdm::Units::m2mi( 2000.0 );
    double v3 = fdm::Units::m2mi( 3000.0 );

    QVERIFY2( fabs( v1 - 0.62137119 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 1.24274238 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 1.86411358 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::m2nmi()
{
    double v1 = fdm::Units::m2nmi( 1000.0 );
    double v2 = fdm::Units::m2nmi( 2000.0 );
    double v3 = fdm::Units::m2nmi( 3000.0 );

    QVERIFY2( fabs( v1 - 0.53995680 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 1.07991361 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 1.61987041 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::m2km()
{
    double v1 = fdm::Units::m2km( 1000.0 );
    double v2 = fdm::Units::m2km( 2000.0 );
    double v3 = fdm::Units::m2km( 3000.0 );

    QVERIFY2( fabs( v1 - 1.0 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 2.0 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 3.0 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::ft2m()
{
    double v1 = fdm::Units::ft2m( 1.0 );
    double v2 = fdm::Units::ft2m( 2.0 );
    double v3 = fdm::Units::ft2m( 3.0 );

    QVERIFY2( fabs( v1 - 0.3048 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 0.6096 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 0.9144 ) < 1.0e-4, "Failure v3" );
}
////////////////////////////////////////////////////////////////////////////////

void UnitsTest::ft2km()
{
    double v1 = fdm::Units::ft2km( 1000.0 );
    double v2 = fdm::Units::ft2km( 2000.0 );
    double v3 = fdm::Units::ft2km( 3000.0 );

    QVERIFY2( fabs( v1 - 0.3048 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 0.6096 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 0.9144 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::ft2mi()
{
    double v1 = fdm::Units::ft2mi( 1000.0 );
    double v2 = fdm::Units::ft2mi( 2000.0 );
    double v3 = fdm::Units::ft2mi( 3000.0 );

    QVERIFY2( fabs( v1 - 0.189393939 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 0.378787879 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 0.568181818 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::ft2nmi()
{
    double v1 = fdm::Units::ft2nmi( 1000.0 );
    double v2 = fdm::Units::ft2nmi( 2000.0 );
    double v3 = fdm::Units::ft2nmi( 3000.0 );

    QVERIFY2( fabs( v1 - 0.164578834 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 0.329157667 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 0.493736501 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::km2m()
{
    double v1 = fdm::Units::km2m( 1.0 );
    double v2 = fdm::Units::km2m( 2.0 );
    double v3 = fdm::Units::km2m( 3.0 );

    QVERIFY2( fabs( v1 - 1000.0 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 2000.0 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 3000.0 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::km2ft()
{
    double v1 = fdm::Units::km2ft( 0.001 );
    double v2 = fdm::Units::km2ft( 0.002 );
    double v3 = fdm::Units::km2ft( 0.003 );

    QVERIFY2( fabs( v1 - 3.2808399 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 6.5616798 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 9.8425197 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::km2mi()
{
    double v1 = fdm::Units::km2mi( 1.0 );
    double v2 = fdm::Units::km2mi( 2.0 );
    double v3 = fdm::Units::km2mi( 3.0 );

    QVERIFY2( fabs( v1 - 0.62137119 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 1.24274238 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 1.86411358 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::km2nmi()
{
    double v1 = fdm::Units::km2nmi( 1.0 );
    double v2 = fdm::Units::km2nmi( 2.0 );
    double v3 = fdm::Units::km2nmi( 3.0 );

    QVERIFY2( fabs( v1 - 0.53995680 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 1.07991361 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 1.61987041 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::mi2m()
{
    double v1 = fdm::Units::mi2m( 0.001 );
    double v2 = fdm::Units::mi2m( 0.002 );
    double v3 = fdm::Units::mi2m( 0.003 );

    QVERIFY2( fabs( v1 - 1.609344 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 3.218688 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 4.828032 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::mi2ft()
{
    double v1 = fdm::Units::mi2ft( 0.001 );
    double v2 = fdm::Units::mi2ft( 0.002 );
    double v3 = fdm::Units::mi2ft( 0.003 );

    QVERIFY2( fabs( v1 -  5.28 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 10.56 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 15.84 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::mi2km()
{
    double v1 = fdm::Units::mi2km( 1.0 );
    double v2 = fdm::Units::mi2km( 2.0 );
    double v3 = fdm::Units::mi2km( 3.0 );

    QVERIFY2( fabs( v1 - 1.609344 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 3.218688 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 4.828032 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::mi2nmi()
{
    double v1 = fdm::Units::mi2nmi( 1.0 );
    double v2 = fdm::Units::mi2nmi( 2.0 );
    double v3 = fdm::Units::mi2nmi( 3.0 );

    QVERIFY2( fabs( v1 - 0.86897624 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 1.73795248 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 2.60692873 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::nmi2m()
{
    double v1 = fdm::Units::nmi2m( 0.001 );
    double v2 = fdm::Units::nmi2m( 0.002 );
    double v3 = fdm::Units::nmi2m( 0.003 );

    QVERIFY2( fabs( v1 - 1.85200 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 3.70400 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 5.55600 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::nmi2ft()
{
    double v1 = fdm::Units::nmi2ft( 0.001 );
    double v2 = fdm::Units::nmi2ft( 0.002 );
    double v3 = fdm::Units::nmi2ft( 0.003 );

    QVERIFY2( fabs( v1 - 6.07611549 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 12.152231  ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 18.2283465 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::nmi2km()
{
    double v1 = fdm::Units::nmi2km( 1.0 );
    double v2 = fdm::Units::nmi2km( 2.0 );
    double v3 = fdm::Units::nmi2km( 3.0 );

    QVERIFY2( fabs( v1 - 1.85200 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 3.70400 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 5.55600 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::nmi2mi()
{
    double v1 = fdm::Units::nmi2mi( 1.0 );
    double v2 = fdm::Units::nmi2mi( 2.0 );
    double v3 = fdm::Units::nmi2mi( 3.0 );

    QVERIFY2( fabs( v1 - 1.15077945 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 2.3015589  ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 3.45233834 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::cuin2m3()
{
    double v1 = fdm::Units::cuin2m3( 1000.0 );
    double v2 = fdm::Units::cuin2m3( 2000.0 );
    double v3 = fdm::Units::cuin2m3( 3000.0 );

    QVERIFY2( fabs( v1 - 0.016387064 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 0.032774128 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 0.049161192 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::l2m3()
{
    double v1 = fdm::Units::l2m3( 1000.0 );
    double v2 = fdm::Units::l2m3( 2000.0 );
    double v3 = fdm::Units::l2m3( 3000.0 );

    QVERIFY2( fabs( v1 - 1.0 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 2.0 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 3.0 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::mps2fpm()
{
    double v1 = fdm::Units::mps2fpm( 1.0 );
    double v2 = fdm::Units::mps2fpm( 2.0 );
    double v3 = fdm::Units::mps2fpm( 3.0 );

    QVERIFY2( fabs( v1 - 196.850394 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 393.700787 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 590.551181 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::mps2fps()
{
    double v1 = fdm::Units::mps2fps( 1.0 );
    double v2 = fdm::Units::mps2fps( 2.0 );
    double v3 = fdm::Units::mps2fps( 3.0 );

    QVERIFY2( fabs( v1 - 3.2808399  ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 6.56167979 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 9.84251969 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::mps2kmh()
{
    double v1 = fdm::Units::mps2kmh( 1.0 );
    double v2 = fdm::Units::mps2kmh( 2.0 );
    double v3 = fdm::Units::mps2kmh( 3.0 );

    QVERIFY2( fabs( v1 -  3.6 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 -  7.2 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 10.8 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::mps2kts()
{
    double v1 = fdm::Units::mps2kts( 1.0 );
    double v2 = fdm::Units::mps2kts( 2.0 );
    double v3 = fdm::Units::mps2kts( 3.0 );

    QVERIFY2( fabs( v1 - 1.94384449 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 3.88768898 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 5.83153348 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::mps2mph()
{
    double v1 = fdm::Units::mps2mph( 1.0 );
    double v2 = fdm::Units::mps2mph( 2.0 );
    double v3 = fdm::Units::mps2mph( 3.0 );

    QVERIFY2( fabs( v1 - 2.23693629 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 4.47387258 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 6.71080888 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::fpm2fps()
{
    double v1 = fdm::Units::fpm2fps( 100.0 );
    double v2 = fdm::Units::fpm2fps( 200.0 );
    double v3 = fdm::Units::fpm2fps( 300.0 );

    QVERIFY2( fabs( v1 - 1.66666667 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 3.33333333 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 5.0        ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::fpm2kmh()
{
    double v1 = fdm::Units::fpm2kmh( 100.0 );
    double v2 = fdm::Units::fpm2kmh( 200.0 );
    double v3 = fdm::Units::fpm2kmh( 300.0 );

    QVERIFY2( fabs( v1 - 1.8288 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 3.6576 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 5.4864 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::fpm2kts()
{
    double v1 = fdm::Units::fpm2kts( 100.0 );
    double v2 = fdm::Units::fpm2kts( 200.0 );
    double v3 = fdm::Units::fpm2kts( 300.0 );

    QVERIFY2( fabs( v1 - 0.987473002 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 1.974946    ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 2.96241901  ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::fpm2mph()
{
    double v1 = fdm::Units::fpm2mph( 100.0 );
    double v2 = fdm::Units::fpm2mph( 200.0 );
    double v3 = fdm::Units::fpm2mph( 300.0 );

    QVERIFY2( fabs( v1 - 1.13636364 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 2.27272727 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 3.40909091 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::fpm2mps()
{
    double v1 = fdm::Units::fpm2mps( 100.0 );
    double v2 = fdm::Units::fpm2mps( 200.0 );
    double v3 = fdm::Units::fpm2mps( 300.0 );

    QVERIFY2( fabs( v1 - 0.508 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 1.016 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 1.524 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::fps2fpm()
{
    double v1 = fdm::Units::fps2fpm( 1.0 );
    double v2 = fdm::Units::fps2fpm( 2.0 );
    double v3 = fdm::Units::fps2fpm( 3.0 );

    QVERIFY2( fabs( v1 -  60.0 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 120.0 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 180.0 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::fps2kmh()
{
    double v1 = fdm::Units::fps2kmh( 1.0 );
    double v2 = fdm::Units::fps2kmh( 2.0 );
    double v3 = fdm::Units::fps2kmh( 3.0 );

    QVERIFY2( fabs( v1 - 1.09728 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 2.19456 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 3.29184 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::fps2kts()
{
    double v1 = fdm::Units::fps2kts( 1.0 );
    double v2 = fdm::Units::fps2kts( 2.0 );
    double v3 = fdm::Units::fps2kts( 3.0 );

    QVERIFY2( fabs( v1 - 0.592483801 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 1.1849676   ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 1.7774514   ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::fps2mph()
{
    double v1 = fdm::Units::fps2mph( 1.0 );
    double v2 = fdm::Units::fps2mph( 2.0 );
    double v3 = fdm::Units::fps2mph( 3.0 );

    QVERIFY2( fabs( v1 - 0.681818182 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 1.36363636  ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 2.04545455  ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::fps2mps()
{
    double v1 = fdm::Units::fps2mps( 1.0 );
    double v2 = fdm::Units::fps2mps( 2.0 );
    double v3 = fdm::Units::fps2mps( 3.0 );

    QVERIFY2( fabs( v1 - 0.3048 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 0.6096 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 0.9144 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::kmh2fpm()
{
    double v1 = fdm::Units::kmh2fpm( 1.0 );
    double v2 = fdm::Units::kmh2fpm( 2.0 );
    double v3 = fdm::Units::kmh2fpm( 3.0 );

    QVERIFY2( fabs( v1 - 54.6806649 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 109.36133  ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 164.041995 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::kmh2fps()
{
    double v1 = fdm::Units::kmh2fps( 1.0 );
    double v2 = fdm::Units::kmh2fps( 2.0 );
    double v3 = fdm::Units::kmh2fps( 3.0 );

    QVERIFY2( fabs( v1 - 0.911344415 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 1.82268883  ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 2.73403325  ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::kmh2kts()
{
    double v1 = fdm::Units::kmh2kts( 1.0 );
    double v2 = fdm::Units::kmh2kts( 2.0 );
    double v3 = fdm::Units::kmh2kts( 3.0 );

    QVERIFY2( fabs( v1 - 0.539956803 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 1.07991361  ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 1.61987041  ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::kmh2mph()
{
    double v1 = fdm::Units::kmh2mph( 1.0 );
    double v2 = fdm::Units::kmh2mph( 2.0 );
    double v3 = fdm::Units::kmh2mph( 3.0 );

    QVERIFY2( fabs( v1 - 0.621371192 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 1.24274238  ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 1.86411358  ) < 1.0e-4, "Failure v3" );
}


////////////////////////////////////////////////////////////////////////////////

void UnitsTest::kmh2mps()
{
    double v1 = fdm::Units::kmh2mps( 1.0 );
    double v2 = fdm::Units::kmh2mps( 2.0 );
    double v3 = fdm::Units::kmh2mps( 3.0 );

    QVERIFY2( fabs( v1 - 0.277777778 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 0.555555556 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 0.833333333 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::kts2fpm()
{
    double v1 = fdm::Units::kts2fpm( 1.0 );
    double v2 = fdm::Units::kts2fpm( 2.0 );
    double v3 = fdm::Units::kts2fpm( 3.0 );

    QVERIFY2( fabs( v1 - 101.268591 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 202.537183 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 303.805774 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::kts2fps()
{
    double v1 = fdm::Units::kts2fps( 1.0 );
    double v2 = fdm::Units::kts2fps( 2.0 );
    double v3 = fdm::Units::kts2fps( 3.0 );

    QVERIFY2( fabs( v1 - 1.68780986 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 3.37561971 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 5.06342957 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::kts2kmh()
{
    double v1 = fdm::Units::kts2kmh( 1.0 );
    double v2 = fdm::Units::kts2kmh( 2.0 );
    double v3 = fdm::Units::kts2kmh( 3.0 );

    QVERIFY2( fabs( v1 - 1.852 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 3.704 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 5.556 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::kts2mph()
{
    double v1 = fdm::Units::kts2mph( 1.0 );
    double v2 = fdm::Units::kts2mph( 2.0 );
    double v3 = fdm::Units::kts2mph( 3.0 );

    QVERIFY2( fabs( v1 - 1.15077945 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 2.3015589  ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 3.45233834 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::kts2mps()
{
    double v1 = fdm::Units::kts2mps( 1.0 );
    double v2 = fdm::Units::kts2mps( 2.0 );
    double v3 = fdm::Units::kts2mps( 3.0 );

    QVERIFY2( fabs( v1 - 0.514444444 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 1.02888889  ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 1.54333333  ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::mph2fpm()
{
    double v1 = fdm::Units::mph2fpm( 1.0 );
    double v2 = fdm::Units::mph2fpm( 2.0 );
    double v3 = fdm::Units::mph2fpm( 3.0 );

    QVERIFY2( fabs( v1 -  88.0 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 176.0 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 264.0 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::mph2fps()
{
    double v1 = fdm::Units::mph2fps( 1.0 );
    double v2 = fdm::Units::mph2fps( 2.0 );
    double v3 = fdm::Units::mph2fps( 3.0 );

    QVERIFY2( fabs( v1 - 1.46666667 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 2.93333333 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 4.4        ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::mph2kmh()
{
    double v1 = fdm::Units::mph2kmh( 1.0 );
    double v2 = fdm::Units::mph2kmh( 2.0 );
    double v3 = fdm::Units::mph2kmh( 3.0 );

    QVERIFY2( fabs( v1 - 1.609344 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 3.218688 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 4.828032 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::mph2kts()
{
    double v1 = fdm::Units::mph2kts( 1.0 );
    double v2 = fdm::Units::mph2kts( 2.0 );
    double v3 = fdm::Units::mph2kts( 3.0 );

    QVERIFY2( fabs( v1 - 0.868976242 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 1.73795248  ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 2.60692873  ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::mph2mps()
{
    double v1 = fdm::Units::mph2mps( 1.0 );
    double v2 = fdm::Units::mph2mps( 2.0 );
    double v3 = fdm::Units::mph2mps( 3.0 );

    QVERIFY2( fabs( v1 - 0.44704 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 0.89408 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 1.34112 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::rpm2rad_s()
{
    double v1 = fdm::Units::rpm2rad_s( 1.0 );
    double v2 = fdm::Units::rpm2rad_s( 2.0 );
    double v3 = fdm::Units::rpm2rad_s( 3.0 );

    QVERIFY2( fabs( v1 - 0.10471975511966  ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 0.20943951023932  ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 0.314159265358979 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::rad_s2rpm()
{
    double v1 = fdm::Units::rad_s2rpm( 1.0 );
    double v2 = fdm::Units::rad_s2rpm( 2.0 );
    double v3 = fdm::Units::rad_s2rpm( 3.0 );

    QVERIFY2( fabs( v1 -  9.54929658551372 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 19.0985931710274  ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 28.6478897565412  ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::kg2lb()
{
    double v1 = fdm::Units::kg2lb( 1.0 );
    double v2 = fdm::Units::kg2lb( 2.0 );
    double v3 = fdm::Units::kg2lb( 3.0 );

    QVERIFY2( fabs( v1 - 2.20462262 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 4.40924524 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 6.61386787 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::lb2kg()
{
    double v1 = fdm::Units::lb2kg( 1.0 );
    double v2 = fdm::Units::lb2kg( 2.0 );
    double v3 = fdm::Units::lb2kg( 3.0 );

    QVERIFY2( fabs( v1 - 0.45359237 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 0.90718474 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 1.36077711 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::n2lbf()
{
    double v1 = fdm::Units::n2lbf( 1.0 );
    double v2 = fdm::Units::n2lbf( 2.0 );
    double v3 = fdm::Units::n2lbf( 3.0 );

    QVERIFY2( fabs( v1 - 0.224808943 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 0.449617886 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 0.674426829 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::lbf2n()
{
    double v1 = fdm::Units::lbf2n( 1.0 );
    double v2 = fdm::Units::lbf2n( 2.0 );
    double v3 = fdm::Units::lbf2n( 3.0 );

    QVERIFY2( fabs( v1 - 4.4482216152605  ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 8.896443230521   ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 13.3446648457815 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::pa2psi()
{
    double v1 = fdm::Units::pa2psi( 1000.0 );
    double v2 = fdm::Units::pa2psi( 2000.0 );
    double v3 = fdm::Units::pa2psi( 3000.0 );

    QVERIFY2( fabs( v1 - 0.145037738 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 0.290075475 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 0.435113213 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::pa2inhg()
{
    double v1 = fdm::Units::pa2inhg( 1000.0 );
    double v2 = fdm::Units::pa2inhg( 2000.0 );
    double v3 = fdm::Units::pa2inhg( 3000.0 );

    QVERIFY2( fabs( v1 - 0.295333727 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 0.590667454 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 0.886001181 ) < 1.0e-4, "Failure v3" );

    double vs = fdm::Units::pa2inhg( 101325.0 );

    QVERIFY2( fabs( vs - 29.9212 ) < 1.0e-2, "Failure vs" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::pa2mb()
{
    double v1 = fdm::Units::pa2mb( 100.0 );
    double v2 = fdm::Units::pa2mb( 200.0 );
    double v3 = fdm::Units::pa2mb( 300.0 );

    QVERIFY2( fabs( v1 - 1.0 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 2.0 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 3.0 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::psi2pa()
{
    double v1 = fdm::Units::psi2pa( 0.1 );
    double v2 = fdm::Units::psi2pa( 0.2 );
    double v3 = fdm::Units::psi2pa( 0.3 );

    QVERIFY2( fabs( v1 - 689.475729 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 1378.95146 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 2068.42719 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::psi2inhg()
{
    double v1 = fdm::Units::psi2inhg( 1.0 );
    double v2 = fdm::Units::psi2inhg( 2.0 );
    double v3 = fdm::Units::psi2inhg( 3.0 );

    QVERIFY2( fabs( v1 - 2.03625437 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 4.07250874 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 6.10876311 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::psi2mb()
{
    double v1 = fdm::Units::psi2mb( 1.0 );
    double v2 = fdm::Units::psi2mb( 2.0 );
    double v3 = fdm::Units::psi2mb( 3.0 );

    QVERIFY2( fabs( v1 - 68.9475729 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 137.895146 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 206.842719 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::inhg2pa()
{
    double v1 = fdm::Units::inhg2pa( 0.001 );
    double v2 = fdm::Units::inhg2pa( 0.002 );
    double v3 = fdm::Units::inhg2pa( 0.003 );

    QVERIFY2( fabs( v1 -  3.386 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 -  6.772 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 10.158 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::inhg2psi()
{
    double v1 = fdm::Units::inhg2psi( 1.0 );
    double v2 = fdm::Units::inhg2psi( 2.0 );
    double v3 = fdm::Units::inhg2psi( 3.0 );

    QVERIFY2( fabs( v1 - 0.49109778 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 0.98219556 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 1.47329334 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::inhg2mb()
{
    double v1 = fdm::Units::inhg2mb( 1.0 );
    double v2 = fdm::Units::inhg2mb( 2.0 );
    double v3 = fdm::Units::inhg2mb( 3.0 );

    QVERIFY2( fabs( v1 -  33.86 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 -  67.72 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 101.58 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::mb2pa()
{
    double v1 = fdm::Units::mb2pa( 1.0 );
    double v2 = fdm::Units::mb2pa( 2.0 );
    double v3 = fdm::Units::mb2pa( 3.0 );

    QVERIFY2( fabs( v1 - 100.0 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 200.0 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 300.0 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::mb2psi()
{
    double v1 = fdm::Units::mb2psi( 100.0 );
    double v2 = fdm::Units::mb2psi( 200.0 );
    double v3 = fdm::Units::mb2psi( 300.0 );

    QVERIFY2( fabs( v1 - 1.45037738 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 2.90075475 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 4.35113213 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::mb2inhg()
{
    double v1 = fdm::Units::mb2inhg( 100.0 );
    double v2 = fdm::Units::mb2inhg( 200.0 );
    double v3 = fdm::Units::mb2inhg( 300.0 );

    QVERIFY2( fabs( v1 - 2.95333727 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 5.90667454 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 8.86001181 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::ps2hp()
{
    double v1 = fdm::Units::ps2hp( 1.0 );
    double v2 = fdm::Units::ps2hp( 2.0 );
    double v3 = fdm::Units::ps2hp( 3.0 );

    QVERIFY2( fabs( v1 - 0.9863200706 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 1.9726401412 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 2.9589602119 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::ps2w()
{
    double v1 = fdm::Units::ps2w( 0.001 );
    double v2 = fdm::Units::ps2w( 0.002 );
    double v3 = fdm::Units::ps2w( 0.003 );

    QVERIFY2( fabs( v1 - 0.7354988 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 1.4709975 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 2.2064963 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::ps2kw()
{
    double v1 = fdm::Units::ps2kw( 1.0 );
    double v2 = fdm::Units::ps2kw( 2.0 );
    double v3 = fdm::Units::ps2kw( 3.0 );

    QVERIFY2( fabs( v1 - 0.7354988 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 1.4709975 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 2.2064963 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::hp2ps()
{
    double v1 = fdm::Units::hp2ps( 1.0 );
    double v2 = fdm::Units::hp2ps( 2.0 );
    double v3 = fdm::Units::hp2ps( 3.0 );

    QVERIFY2( fabs( v1 - 1.0138696654 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 2.0277393308 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 3.0416089963 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::hp2w()
{
    double v1 = fdm::Units::hp2w( 0.001 );
    double v2 = fdm::Units::hp2w( 0.002 );
    double v3 = fdm::Units::hp2w( 0.003 );

    QVERIFY2( fabs( v1 - 0.745699872 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 1.49139974  ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 2.23709961  ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::hp2kw()
{
    double v1 = fdm::Units::hp2kw( 1.0 );
    double v2 = fdm::Units::hp2kw( 2.0 );
    double v3 = fdm::Units::hp2kw( 3.0 );

    QVERIFY2( fabs( v1 - 0.745699872 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 1.49139974  ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 2.23709961  ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::w2ps()
{
    double v1 = fdm::Units::w2ps( 1000.0 );
    double v2 = fdm::Units::w2ps( 2000.0 );
    double v3 = fdm::Units::w2ps( 3000.0 );

    QVERIFY2( fabs( v1 - 1.3596216173039 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 2.7192432346078 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 4.0788648519117 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::w2hp()
{
    double v1 = fdm::Units::w2hp( 1000.0 );
    double v2 = fdm::Units::w2hp( 2000.0 );
    double v3 = fdm::Units::w2hp( 3000.0 );

    QVERIFY2( fabs( v1 - 1.34102209 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 2.68204418 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 4.02306627 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::w2kw()
{
    double v1 = fdm::Units::w2kw( 1000.0 );
    double v2 = fdm::Units::w2kw( 2000.0 );
    double v3 = fdm::Units::w2kw( 3000.0 );

    QVERIFY2( fabs( v1 - 1.0 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 2.0 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 3.0 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::kw2ps()
{
    double v1 = fdm::Units::kw2ps( 1.0 );
    double v2 = fdm::Units::kw2ps( 2.0 );
    double v3 = fdm::Units::kw2ps( 3.0 );

    QVERIFY2( fabs( v1 - 1.3596216173039 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 2.7192432346078 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 4.0788648519117 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::kw2hp()
{
    double v1 = fdm::Units::kw2hp( 1.0 );
    double v2 = fdm::Units::kw2hp( 2.0 );
    double v3 = fdm::Units::kw2hp( 3.0 );

    QVERIFY2( fabs( v1 - 1.34102209 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 2.68204418 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 4.02306627 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::kw2w()
{
    double v1 = fdm::Units::kw2w( 1.0 );
    double v2 = fdm::Units::kw2w( 2.0 );
    double v3 = fdm::Units::kw2w( 3.0 );

    QVERIFY2( fabs( v1 - 1000.0 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 2000.0 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 3000.0 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::c2f()
{
    double v1 = fdm::Units::c2f( -273.15 );
    double v2 = fdm::Units::c2f(    0.0  );
    double v3 = fdm::Units::c2f(  100.0  );

    QVERIFY2( fabs( v1 + 459.67 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 -  32.0  ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 212.0  ) < 1.0e-4, "Failure v3" );

    double vs = fdm::Units::c2f( 36.6 );

    QVERIFY2( fabs( vs - 97.88 ) < 1.0e-2, "Failure vs" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::c2k()
{
    double v1 = fdm::Units::c2k( -273.15 );
    double v2 = fdm::Units::c2k(    0.0  );
    double v3 = fdm::Units::c2k(  100.0  );

    QVERIFY2( fabs( v1 +   0.0  ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 273.15 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 373.15 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::f2c()
{
    double v1 = fdm::Units::f2c(   0.0 );
    double v2 = fdm::Units::f2c( 100.0 );
    double v3 = fdm::Units::f2c( 451.0 );

    QVERIFY2( fabs( v1 + 17.7777778 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 37.7777778 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 232.777778 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::f2k()
{
    double v1 = fdm::Units::f2k( -459.67 );
    double v2 = fdm::Units::f2k(   32.0 );
    double v3 = fdm::Units::f2k(  100.0 );

    QVERIFY2( fabs( v1 -   0.0      ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 273.15     ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 310.927778 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::k2c()
{
    double v1 = fdm::Units::k2c(    0.0  );
    double v2 = fdm::Units::k2c(  273.15 );
    double v3 = fdm::Units::k2c(  373.15 );

    QVERIFY2( fabs( v1 + 273.15 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 -   0.0  ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 100.0  ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::k2f()
{
    double v1 = fdm::Units::k2f(   0.0      );
    double v2 = fdm::Units::k2f( 273.15     );
    double v3 = fdm::Units::k2f( 310.927778 );

    QVERIFY2( fabs( v1 + 459.67 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 -  32.0  ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 100.0  ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::g_kWh_2_kg_Ws()
{
    double v1 = fdm::Units::g_kWh_2_kg_Ws( 1000000000.0 );
    double v2 = fdm::Units::g_kWh_2_kg_Ws( 2000000000.0 );
    double v3 = fdm::Units::g_kWh_2_kg_Ws( 3000000000.0 );

    QVERIFY2( fabs( v1 - 0.277777778 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 0.555555556 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 0.833333333 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

void UnitsTest::g_kNs_2_kg_Ns()
{
    double v1 = fdm::Units::g_kNs_2_kg_Ns( 1000000.0 );
    double v2 = fdm::Units::g_kNs_2_kg_Ns( 2000000.0 );
    double v3 = fdm::Units::g_kNs_2_kg_Ns( 3000000.0 );

    QVERIFY2( fabs( v1 - 1.0 ) < 1.0e-4, "Failure v1" );
    QVERIFY2( fabs( v2 - 2.0 ) < 1.0e-4, "Failure v2" );
    QVERIFY2( fabs( v3 - 3.0 ) < 1.0e-4, "Failure v3" );
}

////////////////////////////////////////////////////////////////////////////////

QTEST_APPLESS_MAIN(UnitsTest)

////////////////////////////////////////////////////////////////////////////////

#include "test_fdm_units.moc"
