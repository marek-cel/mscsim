#include <iostream>

#include <QString>
#include <QtTest>

#include <fdm/utils/fdm_Angles.h>
#include <fdm/utils/fdm_Units.h>

#define DOUBLE_NAN std::numeric_limits< double >::quiet_NaN()

////////////////////////////////////////////////////////////////////////////////

using namespace std;

////////////////////////////////////////////////////////////////////////////////

class AnglesTest : public QObject
{
    Q_OBJECT

public:

    AnglesTest();

private Q_SLOTS:

    void initTestCase();
    void cleanupTestCase();

    void static_normalize();
    void static_toDegMinSec();
    void constructor();
    void isValid();
    void normalize();

    void operator_assign();
    void operator_equal();
    void operator_notequal();
};

////////////////////////////////////////////////////////////////////////////////

AnglesTest::AnglesTest() {}

////////////////////////////////////////////////////////////////////////////////

void AnglesTest::initTestCase() {}

////////////////////////////////////////////////////////////////////////////////

void AnglesTest::cleanupTestCase() {}

////////////////////////////////////////////////////////////////////////////////

void AnglesTest::static_normalize()
{
    double result1 = fdm::Angles::normalize( 2.0*M_PI );
    QVERIFY2( fabs( result1 - 2.0*M_PI ) < 1.0e-9, "Failure 1" );

    double result2 = fdm::Angles::normalize( 2.0*M_PI, -M_PI );
    QVERIFY2( fabs( result2 - 0.0 ) < 1.0e-9, "Failure 2" );

    double result3 = fdm::Angles::normalize( 3.0*M_PI );
    QVERIFY2( fabs( result3 - M_PI ) < 1.0e-9, "Failure 3" );

    double result4 = fdm::Angles::normalize( 3.0*M_PI, -M_PI );
    QVERIFY2( fabs( result4 - M_PI ) < 1.0e-9, "Failure 4" );

    double result5 = fdm::Angles::normalize( 4.0*M_PI );
    QVERIFY2( fabs( result5 - 2.0*M_PI ) < 1.0e-9, "Failure 5" );
}

////////////////////////////////////////////////////////////////////////////////

void AnglesTest::static_toDegMinSec()
{
    fdm::Angles::DegMinSec dms1 = fdm::Angles::toDegMinSec( 0.0 );
    QVERIFY2( fabs( dms1.deg - 0.0 ) < 1.0e-9, "Failure deg 1" );
    QVERIFY2( fabs( dms1.min - 0.0 ) < 1.0e-9, "Failure min 1" );
    QVERIFY2( fabs( dms1.sec - 0.0 ) < 1.0e-9, "Failure sec 1" );

    fdm::Angles::DegMinSec dms2 = fdm::Angles::toDegMinSec( M_PI_2 );
    QVERIFY2( fabs( dms2.deg - 90.0 ) < 1.0e-9, "Failure deg 2" );
    QVERIFY2( fabs( dms2.min -  0.0 ) < 1.0e-9, "Failure min 2" );
    QVERIFY2( fabs( dms2.sec -  0.0 ) < 1.0e-9, "Failure sec 2" );

    fdm::Angles::DegMinSec dms3 = fdm::Angles::toDegMinSec( fdm::Units::deg2rad( 37.5 ) );
    //qDebug() << QString::number( dms3.sec, 'f', 12 );
    QVERIFY2( fabs( dms3.deg - 37.0 ) < 1.0e-9, "Failure deg 3" );
    QVERIFY2( fabs( dms3.min - 30.0 ) < 1.0e-9, "Failure min 3" );
    QVERIFY2( fabs( dms3.sec -  0.0 ) < 1.0e-9, "Failure sec 3" );

    fdm::Angles::DegMinSec dms4 = fdm::Angles::toDegMinSec( fdm::Units::deg2rad( 1.5 + 1.0 / 3600.0 ) );
    QVERIFY2( fabs( dms4.deg -  1.0 ) < 1.0e-9, "Failure deg 4" );
    QVERIFY2( fabs( dms4.min - 30.0 ) < 1.0e-9, "Failure min 4" );
    QVERIFY2( fabs( dms4.sec -  1.0 ) < 1.0e-9, "Failure sec 4" );

    fdm::Angles::DegMinSec dms5 = fdm::Angles::toDegMinSec( fdm::Units::deg2rad( -1.5 - 1.0 / 3600.0 ) );
    QVERIFY2( fabs( dms5.deg +  1.0 ) < 1.0e-9, "Failure deg 5" );
    QVERIFY2( fabs( dms5.min - 30.0 ) < 1.0e-9, "Failure min 5" );
    QVERIFY2( fabs( dms5.sec -  1.0 ) < 1.0e-9, "Failure sec 5" );
}

////////////////////////////////////////////////////////////////////////////////

void AnglesTest::constructor()
{
    fdm::Angles a0( 0.0, 0.0, 0.0 );
    QVERIFY2( fabs( a0.phi() -  0.0 ) < 1.0e-9, "Failure phi a0" );
    QVERIFY2( fabs( a0.tht() -  0.0 ) < 1.0e-9, "Failure tht a0" );
    QVERIFY2( fabs( a0.psi() -  0.0 ) < 1.0e-9, "Failure psi a0" );

    fdm::Angles a1( M_PI, 0.0, 0.0 );
    QVERIFY2( fabs( a1.phi() - M_PI ) < 1.0e-9, "Failure phi a1" );
    QVERIFY2( fabs( a1.tht() -  0.0 ) < 1.0e-9, "Failure tht a1" );
    QVERIFY2( fabs( a1.psi() -  0.0 ) < 1.0e-9, "Failure psi a1" );

    fdm::Angles a2( 0.0, M_PI, 0.0 );
    QVERIFY2( fabs( a2.phi() -  0.0 ) < 1.0e-9, "Failure phi a2" );
    QVERIFY2( fabs( a2.tht() - M_PI ) < 1.0e-9, "Failure tht a2" );
    QVERIFY2( fabs( a2.psi() -  0.0 ) < 1.0e-9, "Failure psi a2" );

    fdm::Angles a3( 0.0, 0.0, M_PI );
    QVERIFY2( fabs( a3.phi() -  0.0 ) < 1.0e-9, "Failure phi a3" );
    QVERIFY2( fabs( a3.tht() -  0.0 ) < 1.0e-9, "Failure tht a3" );
    QVERIFY2( fabs( a3.psi() - M_PI ) < 1.0e-9, "Failure psi a3" );

    fdm::Angles a4( 1.0, 2.0, 3.0 );
    QVERIFY2( fabs( a4.phi() - 1.0 ) < 1.0e-9, "Failure phi a4" );
    QVERIFY2( fabs( a4.tht() - 2.0 ) < 1.0e-9, "Failure tht a4" );
    QVERIFY2( fabs( a4.psi() - 3.0 ) < 1.0e-9, "Failure psi a4" );

    fdm::Angles b0( a0 );
    QVERIFY2( fabs( b0.phi() - b0.phi() ) < 1.0e-9, "Failure phi b0" );
    QVERIFY2( fabs( b0.tht() - b0.tht() ) < 1.0e-9, "Failure tht b0" );
    QVERIFY2( fabs( b0.psi() - b0.psi() ) < 1.0e-9, "Failure psi b0" );

    fdm::Angles b4( a4 );
    QVERIFY2( fabs( b4.phi() - b4.phi() ) < 1.0e-9, "Failure phi b4" );
    QVERIFY2( fabs( b4.tht() - b4.tht() ) < 1.0e-9, "Failure tht b4" );
    QVERIFY2( fabs( b4.psi() - b4.psi() ) < 1.0e-9, "Failure psi b4" );
}

////////////////////////////////////////////////////////////////////////////////

void AnglesTest::isValid()
{
    fdm::Angles a( 0.0, 0.0, 0.0 );
    QVERIFY2( a.isValid(), "Failure 1" );

    a.set( DOUBLE_NAN, 0.0, 0.0 );
    QVERIFY2( !a.isValid(), "Failure 2" );

    a.set( 0.0, DOUBLE_NAN, 0.0 );
    QVERIFY2( !a.isValid(), "Failure 3" );

    a.set( 0.0, 0.0, DOUBLE_NAN );
    QVERIFY2( !a.isValid(), "Failure 4" );

    a.set( 0.0, 0.0, 0.0 );
    QVERIFY2( a.isValid(), "Failure 5" );
}

////////////////////////////////////////////////////////////////////////////////

void AnglesTest::normalize()
{
    fdm::Angles ang;
    // TODO
    ang.normalize();
    // TODO
}

////////////////////////////////////////////////////////////////////////////////

void AnglesTest::operator_assign()
{
    fdm::Angles a;

    fdm::Angles a1( M_PI, 0.0, 0.0 );
    fdm::Angles a2( 0.0, M_PI, 0.0 );
    fdm::Angles a3( 0.0, 0.0, M_PI );
    fdm::Angles a4( 1.0, 2.0, 3.0 );

    a = a1;
    QVERIFY2( fabs( a.phi() - M_PI ) < 1.0e-9, "Failure phi a1" );
    QVERIFY2( fabs( a.tht() -  0.0 ) < 1.0e-9, "Failure tht a1" );
    QVERIFY2( fabs( a.psi() -  0.0 ) < 1.0e-9, "Failure psi a1" );

    a = a2;
    QVERIFY2( fabs( a.phi() -  0.0 ) < 1.0e-9, "Failure phi a2" );
    QVERIFY2( fabs( a.tht() - M_PI ) < 1.0e-9, "Failure tht a2" );
    QVERIFY2( fabs( a.psi() -  0.0 ) < 1.0e-9, "Failure psi a2" );

    a = a3;
    QVERIFY2( fabs( a.phi() -  0.0 ) < 1.0e-9, "Failure phi a3" );
    QVERIFY2( fabs( a.tht() -  0.0 ) < 1.0e-9, "Failure tht a3" );
    QVERIFY2( fabs( a.psi() - M_PI ) < 1.0e-9, "Failure psi a3" );

    a = a4;
    QVERIFY2( fabs( a.phi() - 1.0 ) < 1.0e-9, "Failure phi a4" );
    QVERIFY2( fabs( a.tht() - 2.0 ) < 1.0e-9, "Failure tht a4" );
    QVERIFY2( fabs( a.psi() - 3.0 ) < 1.0e-9, "Failure psi a4" );
}

////////////////////////////////////////////////////////////////////////////////

void AnglesTest::operator_equal()
{
    fdm::Angles a;

    fdm::Angles a1( M_PI, 0.0, 0.0 );
    fdm::Angles a2( 0.0, M_PI, 0.0 );
    fdm::Angles a3( 0.0, 0.0, M_PI );
    fdm::Angles a4( 1.0, 2.0, 3.0 );

    QVERIFY2( !( a == a1 ), "Failure NEQ a1" );
    a = a1;
    QVERIFY2(  ( a == a1 ), "Failure  EQ a1" );

    QVERIFY2( !( a == a2 ), "Failure NEQ a2" );
    a = a2;
    QVERIFY2(  ( a == a2 ), "Failure  EQ a2" );

    QVERIFY2( !( a == a3 ), "Failure NEQ a3" );
    a = a3;
    QVERIFY2(  ( a == a3 ), "Failure  EQ a3" );

    QVERIFY2( !( a == a4 ), "Failure NEQ a4" );
    a = a4;
    QVERIFY2(  ( a == a4 ), "Failure  EQ a4" );
}

////////////////////////////////////////////////////////////////////////////////

void AnglesTest::operator_notequal()
{
    fdm::Angles a;

    fdm::Angles a1( M_PI, 0.0, 0.0 );
    fdm::Angles a2( 0.0, M_PI, 0.0 );
    fdm::Angles a3( 0.0, 0.0, M_PI );
    fdm::Angles a4( 1.0, 2.0, 3.0 );

    QVERIFY2(  ( a != a1 ), "Failure NEQ a1" );
    a = a1;
    QVERIFY2( !( a != a1 ), "Failure  EQ a1" );

    QVERIFY2(  ( a != a2 ), "Failure NEQ a2" );
    a = a2;
    QVERIFY2( !( a != a2 ), "Failure  EQ a2" );

    QVERIFY2(  ( a != a3 ), "Failure NEQ a3" );
    a = a3;
    QVERIFY2( !( a != a3 ), "Failure  EQ a3" );

    QVERIFY2(  ( a != a4 ), "Failure NEQ a4" );
    a = a4;
    QVERIFY2( !( a != a4 ), "Failure  EQ a4" );
}

////////////////////////////////////////////////////////////////////////////////

QTEST_APPLESS_MAIN(AnglesTest)

////////////////////////////////////////////////////////////////////////////////

#include "test_fdm_angles.moc"
