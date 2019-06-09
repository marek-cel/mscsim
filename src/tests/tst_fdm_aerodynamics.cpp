#include <QString>
#include <QtTest>

#include <fdm/main/fdm_Aerodynamics.h>
#include <fdm/utils/fdm_Units.h>

////////////////////////////////////////////////////////////////////////////////

using namespace std;

////////////////////////////////////////////////////////////////////////////////

class AerodynamicsTest : public QObject
{
    Q_OBJECT

public:

    AerodynamicsTest();

private Q_SLOTS:

    void getAero2BAS();
    void getStab2BAS();
};

////////////////////////////////////////////////////////////////////////////////

AerodynamicsTest::AerodynamicsTest() {}

////////////////////////////////////////////////////////////////////////////////

void AerodynamicsTest::getStab2BAS()
{
    QVERIFY2( true, "Failure" );
}

////////////////////////////////////////////////////////////////////////////////

void AerodynamicsTest::getAero2BAS()
{
    const double delta_max = 1.0e-5;

    double alpha = 0.0;
    double beta  = 0.0;

    fdm::Vector3 v_aero;
    fdm::Vector3 v_bas_test;
    fdm::Vector3 v_bas_norm;

    ////////////////////////////////////////

    alpha = fdm::Units::deg2rad( 0.0 );
    beta  = fdm::Units::deg2rad( 0.0 );

    v_aero = fdm::Vector3( 1.0, 1.0, 1.0 );
    v_bas_test = fdm::Aerodynamics::getAero2BAS( alpha, beta ) * v_aero;
    v_bas_norm = fdm::Vector3( -1.0, 1.0, -1.0 );

    QVERIFY2( fabs( v_bas_test.x() - v_bas_norm.x() ) < delta_max, "Failure x" );
    QVERIFY2( fabs( v_bas_test.y() - v_bas_norm.y() ) < delta_max, "Failure y" );
    QVERIFY2( fabs( v_bas_test.z() - v_bas_norm.z() ) < delta_max, "Failure z" );

    ////////////////////////////////////////

    for ( int i = 1; i < 4; i++ )
    {
        alpha = fdm::Units::deg2rad( i * 30.0 );
        beta  = fdm::Units::deg2rad( 0.0 );

        v_aero = fdm::Vector3( 1.0, 0.0, 0.0 );
        v_bas_test = fdm::Aerodynamics::getAero2BAS( alpha, beta ) * v_aero;
        v_bas_norm = fdm::Vector3( -cos( alpha ), 0.0, -sin( alpha ) );

        QVERIFY2( fabs( v_bas_test.x() - v_bas_norm.x() ) < delta_max, "Failure x" );
        QVERIFY2( fabs( v_bas_test.y() - v_bas_norm.y() ) < delta_max, "Failure y" );
        QVERIFY2( fabs( v_bas_test.z() - v_bas_norm.z() ) < delta_max, "Failure z" );
    }

    ////////////////////////////////////////

    for ( int i = 1; i < 4; i++ )
    {
        alpha = fdm::Units::deg2rad( 0.0 );
        beta  = fdm::Units::deg2rad( i * 30.0 );

        v_aero = fdm::Vector3( 1.0, 0.0, 0.0 );
        v_bas_test = fdm::Aerodynamics::getAero2BAS( alpha, beta ) * v_aero;
        v_bas_norm = fdm::Vector3( -cos( -beta ), sin( -beta ), 0.0 );

        QVERIFY2( fabs( v_bas_test.x() - v_bas_norm.x() ) < delta_max, "Failure x" );
        QVERIFY2( fabs( v_bas_test.y() - v_bas_norm.y() ) < delta_max, "Failure y" );
        QVERIFY2( fabs( v_bas_test.z() - v_bas_norm.z() ) < delta_max, "Failure z" );
    }

    ////////////////////////////////////////

    alpha = fdm::Units::deg2rad( 10.0 );
    beta  = fdm::Units::deg2rad( -10.0 );

    v_aero = fdm::Vector3( -1.0, 0.0, 0.0 );
    v_bas_test = fdm::Aerodynamics::getAero2BAS( alpha, beta ) * v_aero;
    v_bas_norm = fdm::Vector3( 0.969848, -0.173644, 0.171007 );

    QVERIFY2( fabs( v_bas_test.x() - v_bas_norm.x() ) < delta_max, "Failure x" );
    QVERIFY2( fabs( v_bas_test.y() - v_bas_norm.y() ) < delta_max, "Failure y" );
    QVERIFY2( fabs( v_bas_test.z() - v_bas_norm.z() ) < delta_max, "Failure z" );
}

////////////////////////////////////////////////////////////////////////////////

QTEST_APPLESS_MAIN(AerodynamicsTest)

////////////////////////////////////////////////////////////////////////////////

#include "tst_fdm_aerodynamics.moc"
