#include <iostream>

#include <QString>
#include <QtTest>

#include <fdm/utils/fdm_VectorN.h>

////////////////////////////////////////////////////////////////////////////////

using namespace std;

////////////////////////////////////////////////////////////////////////////////

class VectorNTest : public QObject
{
    Q_OBJECT

public:

    VectorNTest();

private Q_SLOTS:

    void initTestCase();
    void cleanupTestCase();

    void constructor();

    void getLength();
    void getNormalized();

    void getArray();
    void getItem();
    void setArray();
    void setItem();
    void setValue();

    void operator_accessor();
    void operator_assign();
    void operator_add();
    void operator_negation();
    void operator_substract();
    void operator_multiply_by_scalar();
    void operator_divide_by_scalar();
    void operator_unary_add();
    void operator_unary_substract();
    void operator_unary_multiply_by_scalar();
    void operator_unary_divide_by_scalar();
};

////////////////////////////////////////////////////////////////////////////////

VectorNTest::VectorNTest() {}

////////////////////////////////////////////////////////////////////////////////

void VectorNTest::initTestCase() {}

////////////////////////////////////////////////////////////////////////////////

void VectorNTest::cleanupTestCase() {}

////////////////////////////////////////////////////////////////////////////////

void VectorNTest::constructor()
{
    fdm::VectorN v1( 3 );

    QVERIFY2( fabs( v1( 0 ) - 0.0 ) < 1.0e-9, "Failure 0 v1" );
    QVERIFY2( fabs( v1( 1 ) - 0.0 ) < 1.0e-9, "Failure 1 v1" );
    QVERIFY2( fabs( v1( 2 ) - 0.0 ) < 1.0e-9, "Failure 2 v1" );
    QVERIFY2( v1.getSize() == 3, "Failure getSize v1" );

    v1( 0 ) = 1.0;
    v1( 1 ) = 2.0;
    v1( 2 ) = 3.0;

    fdm::VectorN v2( v1 );

    QVERIFY2( fabs( v2( 0 ) - 1.0 ) < 1.0e-9, "Failure 0 v2" );
    QVERIFY2( fabs( v2( 1 ) - 2.0 ) < 1.0e-9, "Failure 1 v2" );
    QVERIFY2( fabs( v2( 2 ) - 3.0 ) < 1.0e-9, "Failure 2 v2" );
    QVERIFY2( v2.getSize() == 3, "Failure getSize v2" );

}

////////////////////////////////////////////////////////////////////////////////

void VectorNTest::getLength()
{
    fdm::VectorN v1( 3 );

    QVERIFY2( fabs( v1.getLength() ) < 1.0e-9, "Failure" );

    v1( 0 ) = 1.0;
    v1( 1 ) = 2.0;
    v1( 2 ) = 3.0;

    // sqrt( 1^2 + 2^2 + 3^2 ) = sqrt( 1 + 4 + 9 ) = sqrt( 14 )

    QVERIFY2( fabs( v1.getLength() - sqrt( 14 ) ) < 1.0e-9, "Failure" );
}

////////////////////////////////////////////////////////////////////////////////

void VectorNTest::getNormalized()
{
    fdm::VectorN v1( 3 );

    v1( 0 ) = 1.0;
    v1( 1 ) = 2.0;
    v1( 2 ) = 3.0;

    v1.normalize();

    QVERIFY2( fabs( v1.getLength() - 1.0 ) < 1.0e-9, "Failure v1" );

    fdm::VectorN v2( 4 );

    v2( 0 ) = 1.0;
    v2( 1 ) = 2.0;
    v2( 2 ) = 3.0;
    v2( 3 ) = 4.0;

    v2.normalize();

    QVERIFY2( fabs( v2.getLength() - 1.0 ) < 1.0e-9, "Failure v2" );
}

////////////////////////////////////////////////////////////////////////////////

void VectorNTest::getArray()
{
    fdm::VectorN v1( 4 );

    v1( 0 ) = 1.0;
    v1( 1 ) = 2.0;
    v1( 2 ) = 3.0;
    v1( 3 ) = 4.0;

    double x[4];

    v1.getArray( x );

    QVERIFY2( fabs( x[ 0 ] - 1.0 ) < 1.0e-9, "Failure 0" );
    QVERIFY2( fabs( x[ 1 ] - 2.0 ) < 1.0e-9, "Failure 1" );
    QVERIFY2( fabs( x[ 2 ] - 3.0 ) < 1.0e-9, "Failure 2" );
    QVERIFY2( fabs( x[ 3 ] - 4.0 ) < 1.0e-9, "Failure 3" );
}

////////////////////////////////////////////////////////////////////////////////

void VectorNTest::getItem()
{
    fdm::VectorN v1( 3 );

    v1( 0 ) = 1.0;
    v1( 1 ) = 2.0;
    v1( 2 ) = 3.0;

    QVERIFY2( fabs( v1.getItem( 0 ) - 1.0 ) < 1.0e-9, "Failure 0" );
    QVERIFY2( fabs( v1.getItem( 1 ) - 2.0 ) < 1.0e-9, "Failure 1" );
    QVERIFY2( fabs( v1.getItem( 2 ) - 3.0 ) < 1.0e-9, "Failure 2" );
}

////////////////////////////////////////////////////////////////////////////////

void VectorNTest::setArray()
{
    fdm::VectorN v1( 3 );

    double x[] = { 1.0, 2.0, 3.0 };

    v1.setArray( x );

    QVERIFY2( fabs( v1( 0 ) - 1.0 ) < 1.0e-9, "Failure 0" );
    QVERIFY2( fabs( v1( 1 ) - 2.0 ) < 1.0e-9, "Failure 1" );
    QVERIFY2( fabs( v1( 2 ) - 3.0 ) < 1.0e-9, "Failure 2" );
}

////////////////////////////////////////////////////////////////////////////////

void VectorNTest::setItem()
{
    fdm::VectorN v1( 3 );

    v1.setItem( 0, 1.0 );
    v1.setItem( 1, 2.0 );
    v1.setItem( 2, 3.0 );

    QVERIFY2( fabs( v1( 0 ) - 1.0 ) < 1.0e-9, "Failure 0" );
    QVERIFY2( fabs( v1( 1 ) - 2.0 ) < 1.0e-9, "Failure 1" );
    QVERIFY2( fabs( v1( 2 ) - 3.0 ) < 1.0e-9, "Failure 2" );
}

////////////////////////////////////////////////////////////////////////////////

void VectorNTest::setValue()
{
    fdm::VectorN v1( 3 );

    v1.setValue( 1.0 );

    QVERIFY2( fabs( v1( 0 ) - 1.0 ) < 1.0e-9, "Failure 0" );
    QVERIFY2( fabs( v1( 1 ) - 1.0 ) < 1.0e-9, "Failure 1" );
    QVERIFY2( fabs( v1( 2 ) - 1.0 ) < 1.0e-9, "Failure 2" );
}

////////////////////////////////////////////////////////////////////////////////

void VectorNTest::operator_accessor()
{
    fdm::VectorN v1( 4 );

    v1( 0 ) = 1.0;
    v1( 1 ) = 2.0;
    v1( 2 ) = 3.0;
    v1( 3 ) = 4.0;

    QVERIFY2( fabs( v1( 0 ) - 1.0 ) < 1.0e-9, "Failure 0" );
    QVERIFY2( fabs( v1( 1 ) - 2.0 ) < 1.0e-9, "Failure 1" );
    QVERIFY2( fabs( v1( 2 ) - 3.0 ) < 1.0e-9, "Failure 2" );
    QVERIFY2( fabs( v1( 3 ) - 4.0 ) < 1.0e-9, "Failure 3" );
}

////////////////////////////////////////////////////////////////////////////////

void VectorNTest::operator_assign()
{
    fdm::VectorN v( 3 );

    double x1[] = { 1.0, 0.0, 0.0 };
    double x2[] = { 0.0, 1.0, 0.0 };
    double x3[] = { 0.0, 0.0, 1.0 };
    double x4[] = { 1.0, 2.0, 3.0 };

    fdm::VectorN v1( 3 );
    fdm::VectorN v2( 3 );
    fdm::VectorN v3( 3 );
    fdm::VectorN v4( 3 );

    v1.setArray( x1 );
    v2.setArray( x2 );
    v3.setArray( x3 );
    v4.setArray( x4 );

    v = v1;
    QVERIFY2( fabs( v( 0 ) - 1.0 ) < 1.0e-9, "Failure 0 v1" );
    QVERIFY2( fabs( v( 1 ) - 0.0 ) < 1.0e-9, "Failure 1 v1" );
    QVERIFY2( fabs( v( 2 ) - 0.0 ) < 1.0e-9, "Failure 2 v1" );

    v = v2;
    QVERIFY2( fabs( v( 0 ) - 0.0 ) < 1.0e-9, "Failure 0 v2" );
    QVERIFY2( fabs( v( 1 ) - 1.0 ) < 1.0e-9, "Failure 1 v2" );
    QVERIFY2( fabs( v( 2 ) - 0.0 ) < 1.0e-9, "Failure 2 v2" );

    v = v3;
    QVERIFY2( fabs( v( 0 ) - 0.0 ) < 1.0e-9, "Failure 0 v3" );
    QVERIFY2( fabs( v( 1 ) - 0.0 ) < 1.0e-9, "Failure 1 v3" );
    QVERIFY2( fabs( v( 2 ) - 1.0 ) < 1.0e-9, "Failure 2 v3" );

    v = v4;
    QVERIFY2( fabs( v( 0 ) - 1.0 ) < 1.0e-9, "Failure 0 v4" );
    QVERIFY2( fabs( v( 1 ) - 2.0 ) < 1.0e-9, "Failure 1 v4" );
    QVERIFY2( fabs( v( 2 ) - 3.0 ) < 1.0e-9, "Failure 2 v4" );
}

////////////////////////////////////////////////////////////////////////////////

void VectorNTest::operator_add()
{
    double x1[] = { 1.0, 2.0, 3.0 };
    double x2[] = { 3.0, 2.0, 1.0 };
    double x3[] = { 0.0, 0.0, 0.0 };
    double x4[] = { 1.0, 2.0, 3.0 };

    fdm::VectorN v1( 3 );
    fdm::VectorN v2( 3 );
    fdm::VectorN v3( 3 );
    fdm::VectorN v4( 3 );

    v1.setArray( x1 );
    v2.setArray( x2 );
    v3.setArray( x3 );
    v4.setArray( x4 );

    fdm::VectorN v12 = v1 + v2;
    fdm::VectorN v34 = v3 + v4;

    QVERIFY2( fabs( v12( 0 ) - 4.0 ) < 1.0e-9, "Failure 0 v12" );
    QVERIFY2( fabs( v12( 1 ) - 4.0 ) < 1.0e-9, "Failure 1 v12" );
    QVERIFY2( fabs( v12( 2 ) - 4.0 ) < 1.0e-9, "Failure 2 v12" );

    QVERIFY2( fabs( v34( 0 ) - 1.0 ) < 1.0e-9, "Failure 0 v34" );
    QVERIFY2( fabs( v34( 1 ) - 2.0 ) < 1.0e-9, "Failure 1 v34" );
    QVERIFY2( fabs( v34( 2 ) - 3.0 ) < 1.0e-9, "Failure 2 v34" );
}

////////////////////////////////////////////////////////////////////////////////

void VectorNTest::operator_negation()
{
    double x[] = { 1.0, 2.0, 3.0 };

    fdm::VectorN v1( 3 );
    v1.setArray( x );

    fdm::VectorN v2 = -v1;

    QVERIFY2( fabs( v2( 0 ) + 1.0 ) < 1.0e-9, "Failure 0" );
    QVERIFY2( fabs( v2( 1 ) + 2.0 ) < 1.0e-9, "Failure 1" );
    QVERIFY2( fabs( v2( 2 ) + 3.0 ) < 1.0e-9, "Failure 2" );
}

////////////////////////////////////////////////////////////////////////////////

void VectorNTest::operator_substract()
{
    double x1[] = { 3.0, 3.0, 3.0 };
    double x2[] = { 1.0, 2.0, 3.0 };

    fdm::VectorN v1( 3 );
    fdm::VectorN v2( 3 );

    v1.setArray( x1 );
    v2.setArray( x2 );

    fdm::VectorN v = v1 - v2;

    QVERIFY2( fabs( v( 0 ) - 2.0 ) < 1.0e-9, "Failure 0" );
    QVERIFY2( fabs( v( 1 ) - 1.0 ) < 1.0e-9, "Failure 1" );
    QVERIFY2( fabs( v( 2 ) - 0.0 ) < 1.0e-9, "Failure 2" );
}

////////////////////////////////////////////////////////////////////////////////

void VectorNTest::operator_multiply_by_scalar()
{
    double x[] = { 1.0, 2.0, 3.0 };

    fdm::VectorN v1( 3 );

    v1.setArray( x );

    fdm::VectorN v2 = v1 * 2.0;

    QVERIFY2( fabs( v2( 0 ) - 2.0 ) < 1.0e-9, "Failure 0" );
    QVERIFY2( fabs( v2( 1 ) - 4.0 ) < 1.0e-9, "Failure 1" );
    QVERIFY2( fabs( v2( 2 ) - 6.0 ) < 1.0e-9, "Failure 2" );
}

////////////////////////////////////////////////////////////////////////////////

void VectorNTest::operator_divide_by_scalar()
{
    double x[] = { 2.0, 4.0, 6.0 };

    fdm::VectorN v1( 3 );

    v1.setArray( x );

    fdm::VectorN v2 = v1 / 2.0;

    QVERIFY2( fabs( v2( 0 ) - 1.0 ) < 1.0e-9, "Failure 0" );
    QVERIFY2( fabs( v2( 1 ) - 2.0 ) < 1.0e-9, "Failure 1" );
    QVERIFY2( fabs( v2( 2 ) - 3.0 ) < 1.0e-9, "Failure 2" );
}

////////////////////////////////////////////////////////////////////////////////

void VectorNTest::operator_unary_add()
{
    double x0[] = { 1.0, 2.0, 3.0 };
    double x1[] = { 3.0, 2.0, 1.0 };

    fdm::VectorN v0( 3 );
    fdm::VectorN v1( 3 );

    v0.setArray( x0 );
    v1.setArray( x1 );

    v0 += v1;

    QVERIFY2( fabs( v0( 0 ) - 4.0 ) < 1.0e-9, "Failure 0" );
    QVERIFY2( fabs( v0( 1 ) - 4.0 ) < 1.0e-9, "Failure 1" );
    QVERIFY2( fabs( v0( 2 ) - 4.0 ) < 1.0e-9, "Failure 2" );
}

////////////////////////////////////////////////////////////////////////////////

void VectorNTest::operator_unary_substract()
{
    double x0[] = { 3.0, 3.0, 3.0 };
    double x1[] = { 1.0, 2.0, 3.0 };

    fdm::VectorN v0( 3 );
    fdm::VectorN v1( 3 );

    v0.setArray( x0 );
    v1.setArray( x1 );

    v0 -= v1;

    QVERIFY2( fabs( v0( 0 ) - 2.0 ) < 1.0e-9, "Failure 0" );
    QVERIFY2( fabs( v0( 1 ) - 1.0 ) < 1.0e-9, "Failure 1" );
    QVERIFY2( fabs( v0( 2 ) - 0.0 ) < 1.0e-9, "Failure 2" );
}

////////////////////////////////////////////////////////////////////////////////

void VectorNTest::operator_unary_multiply_by_scalar()
{
    double x0[] = { 1.0, 2.0, 3.0 };

    fdm::VectorN v0( 3 );

    v0.setArray( x0 );

    v0 *= 2.0;

    QVERIFY2( fabs( v0( 0 ) - 2.0 ) < 1.0e-9, "Failure 0" );
    QVERIFY2( fabs( v0( 1 ) - 4.0 ) < 1.0e-9, "Failure 1" );
    QVERIFY2( fabs( v0( 2 ) - 6.0 ) < 1.0e-9, "Failure 2" );
}

////////////////////////////////////////////////////////////////////////////////

void VectorNTest::operator_unary_divide_by_scalar()
{
    double x0[] = { 2.0, 4.0, 6.0 };

    fdm::VectorN v0( 3 );

    v0.setArray( x0 );

    v0 /= 2.0;

    QVERIFY2( fabs( v0( 0 ) - 1.0 ) < 1.0e-9, "Failure 0" );
    QVERIFY2( fabs( v0( 1 ) - 2.0 ) < 1.0e-9, "Failure 1" );
    QVERIFY2( fabs( v0( 2 ) - 3.0 ) < 1.0e-9, "Failure 2" );
}

////////////////////////////////////////////////////////////////////////////////

QTEST_APPLESS_MAIN(VectorNTest)

////////////////////////////////////////////////////////////////////////////////

#include "test_fdm_vectorn.moc"
