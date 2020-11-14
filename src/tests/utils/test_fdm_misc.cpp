#include <iostream>

#include <QString>
#include <QtTest>

#include <cfloat>

#include <fdm/utils/fdm_Misc.h>

////////////////////////////////////////////////////////////////////////////////

using namespace std;

////////////////////////////////////////////////////////////////////////////////

class MiscTest : public QObject
{
    Q_OBJECT

public:

    MiscTest();

private Q_SLOTS:

    void initTestCase();
    void cleanupTestCase();

    void inertia();
    void isInf();
    void isNaN();
    void isValid();
    void isInside();
    void isOutside();
    void max();
    void min();
    void pow2();
    void pow3();
    void pow4();
    void pow5();
    void rate();
    void satur();
    void deadband();
    void pacejka();
    void sign();
    void sigmoid();
};

////////////////////////////////////////////////////////////////////////////////

MiscTest::MiscTest() {}

////////////////////////////////////////////////////////////////////////////////

void MiscTest::initTestCase() {}

////////////////////////////////////////////////////////////////////////////////

void MiscTest::cleanupTestCase() {}

////////////////////////////////////////////////////////////////////////////////

void MiscTest::inertia()
{
    // TODO

    QVERIFY2( true, "Failure" );
}

////////////////////////////////////////////////////////////////////////////////

void MiscTest::isInf()
{
    QVERIFY2( !fdm::Misc::isInf( 0.0       ) , "isInff( 0.0       ) failed!" );
    QVERIFY2( !fdm::Misc::isInf( DBL_MAX   ) , "isInff( DBL_MAX   ) failed!" );
    QVERIFY2(  fdm::Misc::isInf( 1.0 / 0.0 ) , "isInff( 1.0 / 0.0 ) failed!" );
}

////////////////////////////////////////////////////////////////////////////////

void MiscTest::isNaN()
{
    double quiet_NaN = std::numeric_limits< double >::quiet_NaN();

    QVERIFY2( !fdm::Misc::isNaN( 0.0       ) , "isNaN( 0.0       ) failed!" );
    QVERIFY2( !fdm::Misc::isNaN( DBL_MAX   ) , "isNaN( DBL_MAX   ) failed!" );
    QVERIFY2(  fdm::Misc::isNaN( quiet_NaN ) , "isNaN( quiet_NaN ) failed!" );
}

////////////////////////////////////////////////////////////////////////////////

void MiscTest::isValid()
{
    double quiet_NaN = std::numeric_limits< double >::quiet_NaN();

    QVERIFY2(  fdm::Misc::isValid( 0.0       ) , "isValid( 0.0       ) failed!" );
    QVERIFY2(  fdm::Misc::isValid( DBL_MAX   ) , "isValid( DBL_MAX   ) failed!" );
    QVERIFY2( !fdm::Misc::isValid( 1.0 / 0.0 ) , "isValid( 1.0 / 0.0 ) failed!" );
    QVERIFY2( !fdm::Misc::isValid( quiet_NaN ) , "isValid( quiet_NaN ) failed!" );

    double a1[] = { 0.0, 1.0, 2.0 };
    double a2[] = { 0.0, 1.0, DBL_MAX };
    double a3[] = { 0.0, 1.0, 1.0 / 0.0 };
    double a4[] = { 0.0, 1.0, quiet_NaN };

    QVERIFY2(  fdm::Misc::isValid( a1, 3 ) , "isValid( a1, 3 ) failed!" );
    QVERIFY2(  fdm::Misc::isValid( a2, 3 ) , "isValid( a2, 3 ) failed!" );
    QVERIFY2( !fdm::Misc::isValid( a3, 3 ) , "isValid( a3, 3 ) failed!" );
    QVERIFY2( !fdm::Misc::isValid( a4, 3 ) , "isValid( a4, 3 ) failed!" );
}

////////////////////////////////////////////////////////////////////////////////

void MiscTest::isInside()
{
    QVERIFY2( !fdm::Misc::isInside( 0.0, 1.0, -2.0 ), "isInside( 0.0, 1.0, -2.0 ) failed!" );
    QVERIFY2(  fdm::Misc::isInside( 0.0, 1.0,  0.0 ), "isInside( 0.0, 1.0,  0.0 ) failed!" );
    QVERIFY2(  fdm::Misc::isInside( 0.0, 1.0,  0.1 ), "isInside( 0.0, 1.0,  0.1 ) failed!" );
    QVERIFY2(  fdm::Misc::isInside( 0.0, 1.0,  0.5 ), "isInside( 0.0, 1.0,  0.5 ) failed!" );
    QVERIFY2(  fdm::Misc::isInside( 0.0, 1.0,  0.9 ), "isInside( 0.0, 1.0,  0.9 ) failed!" );
    QVERIFY2(  fdm::Misc::isInside( 0.0, 1.0,  1.0 ), "isInside( 0.0, 1.0,  1.0 ) failed!" );
    QVERIFY2( !fdm::Misc::isInside( 0.0, 1.0,  2.0 ), "isInside( 0.0, 1.0,  2.0 ) failed!" );
}

////////////////////////////////////////////////////////////////////////////////

void MiscTest::isOutside()
{
    QVERIFY2(  fdm::Misc::isOutside( 0.0, 1.0, -2.0 ), "isOutside( 0.0, 1.0, -2.0 ) failed!" );
    QVERIFY2( !fdm::Misc::isOutside( 0.0, 1.0,  0.0 ), "isOutside( 0.0, 1.0,  0.0 ) failed!" );
    QVERIFY2( !fdm::Misc::isOutside( 0.0, 1.0,  0.1 ), "isOutside( 0.0, 1.0,  0.1 ) failed!" );
    QVERIFY2( !fdm::Misc::isOutside( 0.0, 1.0,  0.5 ), "isOutside( 0.0, 1.0,  0.5 ) failed!" );
    QVERIFY2( !fdm::Misc::isOutside( 0.0, 1.0,  0.9 ), "isOutside( 0.0, 1.0,  0.9 ) failed!" );
    QVERIFY2( !fdm::Misc::isOutside( 0.0, 1.0,  1.0 ), "isOutside( 0.0, 1.0,  1.0 ) failed!" );
    QVERIFY2(  fdm::Misc::isOutside( 0.0, 1.0,  2.0 ), "isOutside( 0.0, 1.0,  2.0 ) failed!" );
}

////////////////////////////////////////////////////////////////////////////////

void MiscTest::max()
{
    QVERIFY2( fdm::Misc::max( 0.0, 1.0 ) == 1.0 , "max( 0.0, 1.0 ) failed!" );
    QVERIFY2( fdm::Misc::max( 1.0, 0.0 ) == 1.0 , "max( 1.0, 0.0 ) failed!" );

    QVERIFY2( fdm::Misc::max( 0.0, -1.0 ) == 0.0 , "max( 0.0, -1.0 ) failed!" );
    QVERIFY2( fdm::Misc::max( -1.0, 0.0 ) == 0.0 , "max( -1.0, 0.0 ) failed!" );
}

////////////////////////////////////////////////////////////////////////////////

void MiscTest::min()
{
    QVERIFY2( fdm::Misc::min( 0.0, 1.0 ) == 0.0 , "max( 0.0, 1.0 ) failed!" );
    QVERIFY2( fdm::Misc::min( 1.0, 0.0 ) == 0.0 , "max( 1.0, 0.0 ) failed!" );

    QVERIFY2( fdm::Misc::min( 0.0, -1.0 ) == -1.0 , "max( 0.0, -1.0 ) failed!" );
    QVERIFY2( fdm::Misc::min( -1.0, 0.0 ) == -1.0 , "max( -1.0, 0.0 ) failed!" );
}

////////////////////////////////////////////////////////////////////////////////

void MiscTest::pow2()
{
    QVERIFY2( fabs( fdm::Misc::pow2( 0.0 ) - 0.0 ) < 1.0e-9, "pow2( 0.0 ) failed!" );
    QVERIFY2( fabs( fdm::Misc::pow2( 1.0 ) - 1.0 ) < 1.0e-9, "pow2( 1.0 ) failed!" );
    QVERIFY2( fabs( fdm::Misc::pow2( 2.0 ) - 4.0 ) < 1.0e-9, "pow2( 2.0 ) failed!" );
    QVERIFY2( fabs( fdm::Misc::pow2( 3.0 ) - 9.0 ) < 1.0e-9, "pow2( 3.0 ) failed!" );
}

////////////////////////////////////////////////////////////////////////////////

void MiscTest::pow3()
{
    QVERIFY2( fabs( fdm::Misc::pow3( 0.0 ) -  0.0 ) < 1.0e-9, "pow3( 0.0 ) failed!" );
    QVERIFY2( fabs( fdm::Misc::pow3( 1.0 ) -  1.0 ) < 1.0e-9, "pow3( 1.0 ) failed!" );
    QVERIFY2( fabs( fdm::Misc::pow3( 2.0 ) -  8.0 ) < 1.0e-9, "pow3( 2.0 ) failed!" );
    QVERIFY2( fabs( fdm::Misc::pow3( 3.0 ) - 27.0 ) < 1.0e-9, "pow3( 3.0 ) failed!" );
}

////////////////////////////////////////////////////////////////////////////////

void MiscTest::pow4()
{
    QVERIFY2( fabs( fdm::Misc::pow4( 0.0 ) -  0.0 ) < 1.0e-9, "pow4( 0.0 ) failed!" );
    QVERIFY2( fabs( fdm::Misc::pow4( 1.0 ) -  1.0 ) < 1.0e-9, "pow4( 1.0 ) failed!" );
    QVERIFY2( fabs( fdm::Misc::pow4( 2.0 ) - 16.0 ) < 1.0e-9, "pow4( 2.0 ) failed!" );
    QVERIFY2( fabs( fdm::Misc::pow4( 3.0 ) - 81.0 ) < 1.0e-9, "pow4( 3.0 ) failed!" );
}

////////////////////////////////////////////////////////////////////////////////

void MiscTest::pow5()
{
    QVERIFY2( fabs( fdm::Misc::pow5( 0.0 ) -   0.0 ) < 1.0e-9, "pow5( 0.0 ) failed!" );
    QVERIFY2( fabs( fdm::Misc::pow5( 1.0 ) -   1.0 ) < 1.0e-9, "pow5( 1.0 ) failed!" );
    QVERIFY2( fabs( fdm::Misc::pow5( 2.0 ) -  32.0 ) < 1.0e-9, "pow5( 2.0 ) failed!" );
    QVERIFY2( fabs( fdm::Misc::pow5( 3.0 ) - 243.0 ) < 1.0e-9, "pow5( 3.0 ) failed!" );
}

////////////////////////////////////////////////////////////////////////////////

void MiscTest::rate()
{
    // TODO

    QVERIFY2( true, "Failure" );
}

////////////////////////////////////////////////////////////////////////////////

void MiscTest::satur()
{
    QVERIFY2( fabs( fdm::Misc::satur(  0.0, 1.0, -2.0 ) - 0.0 ) < 1.0e-9, "satur(  0.0, 1.0, -2.0 ) failed!" );
    QVERIFY2( fabs( fdm::Misc::satur(  0.0, 1.0, -0.1 ) - 0.0 ) < 1.0e-9, "satur(  0.0, 1.0, -0.1 ) failed!" );
    QVERIFY2( fabs( fdm::Misc::satur(  0.0, 1.0,  0.0 ) - 0.0 ) < 1.0e-9, "satur(  0.0, 1.0,  0.0 ) failed!" );
    QVERIFY2( fabs( fdm::Misc::satur(  0.0, 1.0,  0.1 ) - 0.1 ) < 1.0e-9, "satur(  0.0, 1.0,  0.1 ) failed!" );
    QVERIFY2( fabs( fdm::Misc::satur(  0.0, 1.0,  0.5 ) - 0.5 ) < 1.0e-9, "satur(  0.0, 1.0,  0.5 ) failed!" );
    QVERIFY2( fabs( fdm::Misc::satur(  0.0, 1.0,  0.9 ) - 0.9 ) < 1.0e-9, "satur(  0.0, 1.0,  0.9 ) failed!" );
    QVERIFY2( fabs( fdm::Misc::satur(  0.0, 1.0,  1.0 ) - 1.0 ) < 1.0e-9, "satur(  0.0, 1.0,  1.0 ) failed!" );
    QVERIFY2( fabs( fdm::Misc::satur(  0.0, 1.0,  1.1 ) - 1.0 ) < 1.0e-9, "satur(  0.0, 1.0,  1.1 ) failed!" );
    QVERIFY2( fabs( fdm::Misc::satur(  0.0, 1.0,  2.0 ) - 1.0 ) < 1.0e-9, "satur(  0.0, 1.0,  2.0 ) failed!" );

    QVERIFY2( fabs( fdm::Misc::satur( -1.0, 1.0, -2.0 ) + 1.0 ) < 1.0e-9, "satur( -1.0, 1.0, -2.0 ) failed!" );
    QVERIFY2( fabs( fdm::Misc::satur( -1.0, 1.0, -1.5 ) + 1.0 ) < 1.0e-9, "satur( -1.0, 1.0, -1.5 ) failed!" );
    QVERIFY2( fabs( fdm::Misc::satur( -1.0, 1.0, -1.0 ) + 1.0 ) < 1.0e-9, "satur( -1.0, 1.0, -1.0 ) failed!" );
    QVERIFY2( fabs( fdm::Misc::satur( -1.0, 1.0, -0.5 ) + 0.5 ) < 1.0e-9, "satur( -1.0, 1.0, -0.5 ) failed!" );
    QVERIFY2( fabs( fdm::Misc::satur( -1.0, 1.0,  0.0 ) - 0.0 ) < 1.0e-9, "satur( -1.0, 1.0,  0.0 ) failed!" );
    QVERIFY2( fabs( fdm::Misc::satur( -1.0, 1.0,  0.5 ) - 0.5 ) < 1.0e-9, "satur( -1.0, 1.0,  0.5 ) failed!" );
    QVERIFY2( fabs( fdm::Misc::satur( -1.0, 1.0,  1.0 ) - 1.0 ) < 1.0e-9, "satur( -1.0, 1.0,  1.0 ) failed!" );
    QVERIFY2( fabs( fdm::Misc::satur( -1.0, 1.0,  1.5 ) - 1.0 ) < 1.0e-9, "satur( -1.0, 1.0,  1.5 ) failed!" );
    QVERIFY2( fabs( fdm::Misc::satur( -1.0, 1.0,  2.0 ) - 1.0 ) < 1.0e-9, "satur( -1.0, 1.0,  2.0 ) failed!" );
}

////////////////////////////////////////////////////////////////////////////////

void MiscTest::deadband()
{
    // TODO

    QVERIFY2( true, "Failure" );
}

////////////////////////////////////////////////////////////////////////////////

void MiscTest::pacejka()
{
    // TODO

    QVERIFY2( true, "Failure" );
}

////////////////////////////////////////////////////////////////////////////////

void MiscTest::sign()
{
    QVERIFY2( fdm::Misc::sign( -2.0 ) == -1.0, "sign( -2.0 ) failed!" );
    QVERIFY2( fdm::Misc::sign( -1.0 ) == -1.0, "sign( -1.0 ) failed!" );
    QVERIFY2( fdm::Misc::sign( -0.1 ) == -1.0, "sign( -0.1 ) failed!" );
    QVERIFY2( fdm::Misc::sign(  0.0 ) ==  0.0, "sign(  0.0 ) failed!" );
    QVERIFY2( fdm::Misc::sign(  0.1 ) ==  1.0, "sign(  0.1 ) failed!" );
    QVERIFY2( fdm::Misc::sign(  1.0 ) ==  1.0, "sign(  1.0 ) failed!" );
    QVERIFY2( fdm::Misc::sign(  2.0 ) ==  1.0, "sign(  2.0 ) failed!" );
}

////////////////////////////////////////////////////////////////////////////////

void MiscTest::sigmoid()
{
    // TODO

    QVERIFY2( true, "Failure" );
}

////////////////////////////////////////////////////////////////////////////////

QTEST_APPLESS_MAIN(MiscTest)

////////////////////////////////////////////////////////////////////////////////

#include "test_fdm_misc.moc"
