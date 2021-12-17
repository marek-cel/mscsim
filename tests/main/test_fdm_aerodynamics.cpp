#include <QString>
#include <QtTest>

#include <iostream>

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

    void getAngleOfAttack();
    void getSideslipAngle();

    void getAero2BAS();
    void getStab2BAS();
};

////////////////////////////////////////////////////////////////////////////////

AerodynamicsTest::AerodynamicsTest() {}

////////////////////////////////////////////////////////////////////////////////

void AerodynamicsTest::getAngleOfAttack()
{
    QVERIFY2( true, "Failure" );
}

////////////////////////////////////////////////////////////////////////////////

void AerodynamicsTest::getSideslipAngle()
{
    const fdm::Vector3 vel_ned( 1.0, 0.0, 0.0 );

    // >>>> alpha = 0 deg ; phi = 0 deg <<<<

    // alpha = 0 deg ; phi = 0 deg ; beta = 0 deg
    fdm::Matrix3x3 ned2bas = fdm::Matrix3x3::identityMatrix();
    fdm::Vector3 vel_bas = ned2bas * vel_ned;

    double beta = fdm::Aerodynamics::getSideslipAngle( vel_bas );

    QVERIFY2( fabs( beta - fdm::Units::deg2rad( 0.0 ) ) < 1.0e-9, "Failure alpha=0 ; phi=0; beta=0" );

    // alpha = 0 deg ; phi = 0 deg ; beta = +45 deg
    ned2bas = fdm::Matrix3x3( fdm::Angles( 0.0, 0.0, fdm::Units::deg2rad( -45.0 ) ) );
    vel_bas = ned2bas * vel_ned;

    beta = fdm::Aerodynamics::getSideslipAngle( vel_bas );

    QVERIFY2( fabs( beta - fdm::Units::deg2rad( +45.0 ) ) < 1.0e-9, "Failure alpha=0 ; phi=0; beta=+45" );

    // alpha = 0 deg ; phi = 0 deg ; beta = -45 deg
    ned2bas = fdm::Matrix3x3( fdm::Angles( 0.0, 0.0, fdm::Units::deg2rad( 45.0 ) ) );
    vel_bas = ned2bas * vel_ned;

    beta = fdm::Aerodynamics::getSideslipAngle( vel_bas );

    QVERIFY2( fabs( beta - fdm::Units::deg2rad( -45.0 ) ) < 1.0e-9, "Failure alpha=0 ; phi=0; beta=-45" );

    // >>>> alpha = +10 deg ; phi = 0 deg <<<<

    // alpha = +10 deg ; phi = 0 deg ; beta = 0 deg
    ned2bas = fdm::Matrix3x3( fdm::Angles( 0.0, fdm::Units::deg2rad( 10.0 ), 0.0 ) );
    vel_bas = ned2bas * vel_ned;

    beta = fdm::Aerodynamics::getSideslipAngle( vel_bas );

    QVERIFY2( fabs( beta - fdm::Units::deg2rad( 0.0 ) ) < 1.0e-9, "Failure alpha=+10 ; phi=0; beta=0" );

    // alpha = +10 deg ; phi = 0 deg ; beta = +45 deg
    ned2bas = fdm::Matrix3x3( fdm::Angles( 0.0, fdm::Units::deg2rad( 10.0 ), fdm::Units::deg2rad( -45.0 ) ) );
    vel_bas = ned2bas * vel_ned;

    beta = fdm::Aerodynamics::getSideslipAngle( vel_bas );

    QVERIFY2( fabs( beta - fdm::Units::deg2rad( +45.0 ) ) < 1.0e-9, "Failure alpha=+10 ; phi=0; beta=+45" );

    // alpha = +10 deg ; phi = 0 deg ; beta = -45 deg
    ned2bas = fdm::Matrix3x3( fdm::Angles( 0.0, fdm::Units::deg2rad( 10.0 ), fdm::Units::deg2rad( 45.0 ) ) );
    vel_bas = ned2bas * vel_ned;

    beta = fdm::Aerodynamics::getSideslipAngle( vel_bas );

    QVERIFY2( fabs( beta - fdm::Units::deg2rad( -45.0 ) ) < 1.0e-9, "Failure alpha=+10 ; phi=0; beta=-45" );

    // >>>> alpha = -10 deg ; phi = 0 deg <<<<

    // alpha = -10 deg ; phi = 0 deg ; beta = 0 deg
    ned2bas = fdm::Matrix3x3( fdm::Angles( 0.0, fdm::Units::deg2rad( -10.0 ), 0.0 ) );
    vel_bas = ned2bas * vel_ned;

    beta = fdm::Aerodynamics::getSideslipAngle( vel_bas );

    QVERIFY2( fabs( beta - fdm::Units::deg2rad( 0.0 ) ) < 1.0e-9, "Failure alpha=-10 ; phi=0; beta=0" );

    // alpha = -10 deg ; phi = 0 deg ; beta = +45 deg
    ned2bas = fdm::Matrix3x3( fdm::Angles( 0.0, fdm::Units::deg2rad( -10.0 ), fdm::Units::deg2rad( -45.0 ) ) );
    vel_bas = ned2bas * vel_ned;

    beta = fdm::Aerodynamics::getSideslipAngle( vel_bas );

    QVERIFY2( fabs( beta - fdm::Units::deg2rad( +45.0 ) ) < 1.0e-9, "Failure alpha=-10 ; phi=0; beta=+45" );

    // alpha = -10 deg ; phi = 0 deg ; beta = -45 deg
    ned2bas = fdm::Matrix3x3( fdm::Angles( 0.0, fdm::Units::deg2rad( -10.0 ), fdm::Units::deg2rad( 45.0 ) ) );
    vel_bas = ned2bas * vel_ned;

    beta = fdm::Aerodynamics::getSideslipAngle( vel_bas );

    QVERIFY2( fabs( beta - fdm::Units::deg2rad( -45.0 ) ) < 1.0e-9, "Failure alpha=-10 ; phi=0; beta=-45" );

    // >>>> alpha = 0 deg ; phi = +10 deg <<<<

    // alpha = 0 deg ; phi = +10 deg ; beta = 0 deg
    ned2bas = fdm::Matrix3x3( fdm::Angles( fdm::Units::deg2rad( 10.0 ), 0.0, 0.0 ) );
    vel_bas = ned2bas * vel_ned;

    beta = fdm::Aerodynamics::getSideslipAngle( vel_bas );

    QVERIFY2( fabs( beta - fdm::Units::deg2rad( 0.0 ) ) < 1.0e-9, "Failure alpha=0 ; phi=+10; beta=0" );

    // alpha = 0 deg ; phi = +10 deg ; beta = +45 deg
    ned2bas = fdm::Matrix3x3( fdm::Angles( fdm::Units::deg2rad( 10.0 ), 0.0, fdm::Units::deg2rad( -45.0 ) ) );
    vel_bas = ned2bas * vel_ned;

    beta = fdm::Aerodynamics::getSideslipAngle( vel_bas ); cout << fdm::Units::rad2deg( beta ) << endl; // ????????????????

    QVERIFY2( fabs( beta - fdm::Units::deg2rad( +45.0 ) ) < 1.0e-9, "Failure alpha=0 ; phi=+10; beta=+45" );

    // alpha = 0 deg ; phi = +10 deg ; beta = -45 deg
    ned2bas = fdm::Matrix3x3( fdm::Angles(fdm::Units::deg2rad( 10.0 ), 0.0, fdm::Units::deg2rad( 45.0 ) ) );
    vel_bas = ned2bas * vel_ned;

    beta = fdm::Aerodynamics::getSideslipAngle( vel_bas );

    QVERIFY2( fabs( beta - fdm::Units::deg2rad( -45.0 ) ) < 1.0e-9, "Failure alpha=0 ; phi=+10; beta=-45" );
}

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

#include "test_fdm_aerodynamics.moc"
