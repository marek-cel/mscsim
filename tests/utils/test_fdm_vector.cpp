#include <iostream>

#include <QString>
#include <QtTest>

#include <fdm/utils/fdm_Vector.h>

////////////////////////////////////////////////////////////////////////////////

using namespace std;

////////////////////////////////////////////////////////////////////////////////

class VectorTest : public QObject
{
    Q_OBJECT

public:

    VectorTest();

private Q_SLOTS:

    void initTestCase();
    void cleanupTestCase();

    void constructor();

    void isValid();
    void getLength2();
    void getLength();
    void normalize();
    void getArray();
    void getItem();
    void setArray();
    void setItem();
    void swapRows();
    void toString();
    void zeroize();

    void operator_accessor();
    void operator_assign();
    void operator_add();
    void operator_negation();
    void operator_substract();
    void operator_multiply_by_scalar();
    void operator_vector_dot_product();
    void operator_divide_by_scalar();
    void operator_unary_add();
    void operator_unary_substract();
    void operator_unary_multiply_by_scalar();
    void operator_unary_divide_by_scalar();
    void operator_equality();
    void operator_inequality();
};

////////////////////////////////////////////////////////////////////////////////

VectorTest::VectorTest() {}

////////////////////////////////////////////////////////////////////////////////

void VectorTest::initTestCase() {}

////////////////////////////////////////////////////////////////////////////////

void VectorTest::cleanupTestCase() {}

////////////////////////////////////////////////////////////////////////////////

void VectorTest::constructor()
{
    fdm::Vector< 3 > v1;

    QVERIFY2( fabs( v1( 0 ) ) < 1.0e-9, "Failure 0 v1" );
    QVERIFY2( fabs( v1( 1 ) ) < 1.0e-9, "Failure 1 v1" );
    QVERIFY2( fabs( v1( 2 ) ) < 1.0e-9, "Failure 2 v1" );

    v1( 0 ) = 1.0;
    v1( 1 ) = 2.0;
    v1( 2 ) = 3.0;

    fdm::Vector< 3 > v2( v1 );

    QVERIFY2( fabs( v2( 0 ) - 1.0 ) < 1.0e-9, "Failure 0 v2" );
    QVERIFY2( fabs( v2( 1 ) - 2.0 ) < 1.0e-9, "Failure 1 v2" );
    QVERIFY2( fabs( v2( 2 ) - 3.0 ) < 1.0e-9, "Failure 2 v2" );

    double x[] = { 4.0, 5.0, 6.0 };

    fdm::Vector< 3 > v3( x );

    QVERIFY2( fabs( v3( 0 ) - 4.0 ) < 1.0e-9, "Failure 0 v3" );
    QVERIFY2( fabs( v3( 1 ) - 5.0 ) < 1.0e-9, "Failure 1 v3" );
    QVERIFY2( fabs( v3( 2 ) - 6.0 ) < 1.0e-9, "Failure 2 v3" );
}

////////////////////////////////////////////////////////////////////////////////

void VectorTest::isValid()
{
    fdm::Vector< 3 > v1;

    QVERIFY2( v1.isValid(), "Failure" );

    v1( 0 ) = 1.0;
    v1( 1 ) = 2.0;
    v1( 2 ) = 3.0;

    QVERIFY2( v1.isValid(), "Failure" );

    v1( 0 ) = std::numeric_limits< double >::quiet_NaN();
    v1( 1 ) = 2.0;
    v1( 2 ) = 3.0;

    QVERIFY2( !v1.isValid(), "Failure" );

    v1( 0 ) = 1.0;
    v1( 1 ) = std::numeric_limits< double >::quiet_NaN();
    v1( 2 ) = 3.0;

    QVERIFY2( !v1.isValid(), "Failure" );

    v1( 0 ) = 1.0;
    v1( 1 ) = 2.0;
    v1( 2 ) = std::numeric_limits< double >::quiet_NaN();

    QVERIFY2( !v1.isValid(), "Failure" );

    v1( 0 ) = std::numeric_limits< double >::quiet_NaN();
    v1( 1 ) = std::numeric_limits< double >::quiet_NaN();
    v1( 2 ) = std::numeric_limits< double >::quiet_NaN();

    QVERIFY2( !v1.isValid(), "Failure" );
}

////////////////////////////////////////////////////////////////////////////////

void VectorTest::getLength2()
{
    fdm::Vector< 3 > v1;

    QVERIFY2( fabs( v1.getLength2() ) < 1.0e-9, "Failure" );

    v1( 0 ) = 1.0;
    v1( 1 ) = 2.0;
    v1( 2 ) = 3.0;

    // 1^2 + 2^2 + 3^2 = 1 + 4 + 9 = 14

    QVERIFY2( fabs( v1.getLength2() - 14.0 ) < 1.0e-9, "Failure" );
}

////////////////////////////////////////////////////////////////////////////////

void VectorTest::getLength()
{
    fdm::Vector< 3 > v1;

    QVERIFY2( fabs( v1.getLength() ) < 1.0e-9, "Failure" );

    v1( 0 ) = 1.0;
    v1( 1 ) = 2.0;
    v1( 2 ) = 3.0;

    // sqrt( 1^2 + 2^2 + 3^2 ) = sqrt( 1 + 4 + 9 ) = sqrt( 14 )

    QVERIFY2( fabs( v1.getLength() - sqrt( 14 ) ) < 1.0e-9, "Failure" );
}

////////////////////////////////////////////////////////////////////////////////

void VectorTest::normalize()
{
    fdm::Vector< 3 > v1;

    v1( 0 ) = 1.0;
    v1( 1 ) = 2.0;
    v1( 2 ) = 3.0;

    v1.normalize();

    QVERIFY2( fabs( v1.getLength() - 1.0 ) < 1.0e-9, "Failure" );
}

////////////////////////////////////////////////////////////////////////////////

void VectorTest::getArray()
{
    fdm::Vector< 3 > v1;

    v1( 0 ) = 1.0;
    v1( 1 ) = 2.0;
    v1( 2 ) = 3.0;

    double x[3];

    v1.getArray( x );

    QVERIFY2( fabs( x[ 0 ] - 1.0 ) < 1.0e-9, "Failure 0" );
    QVERIFY2( fabs( x[ 1 ] - 2.0 ) < 1.0e-9, "Failure 1" );
    QVERIFY2( fabs( x[ 2 ] - 3.0 ) < 1.0e-9, "Failure 2" );
}

////////////////////////////////////////////////////////////////////////////////

void VectorTest::getItem()
{
    fdm::Vector< 3 > v1;

    v1( 0 ) = 1.0;
    v1( 1 ) = 2.0;
    v1( 2 ) = 3.0;

    QVERIFY2( fabs( v1.getItem( 0 ) - 1.0 ) < 1.0e-9, "Failure 0" );
    QVERIFY2( fabs( v1.getItem( 1 ) - 2.0 ) < 1.0e-9, "Failure 1" );
    QVERIFY2( fabs( v1.getItem( 2 ) - 3.0 ) < 1.0e-9, "Failure 2" );
}

////////////////////////////////////////////////////////////////////////////////

void VectorTest::setArray()
{
    fdm::Vector< 3 > v1;

    double x[] = { 1.0, 2.0, 3.0 };

    v1.setArray( x );

    QVERIFY2( fabs( v1( 0 ) - 1.0 ) < 1.0e-9, "Failure 0" );
    QVERIFY2( fabs( v1( 1 ) - 2.0 ) < 1.0e-9, "Failure 1" );
    QVERIFY2( fabs( v1( 2 ) - 3.0 ) < 1.0e-9, "Failure 2" );
}

////////////////////////////////////////////////////////////////////////////////

void VectorTest::setItem()
{
    fdm::Vector< 3 > v1;

    v1.setItem( 0, 1.0 );
    v1.setItem( 1, 2.0 );
    v1.setItem( 2, 3.0 );

    QVERIFY2( fabs( v1( 0 ) - 1.0 ) < 1.0e-9, "Failure 0" );
    QVERIFY2( fabs( v1( 1 ) - 2.0 ) < 1.0e-9, "Failure 1" );
    QVERIFY2( fabs( v1( 2 ) - 3.0 ) < 1.0e-9, "Failure 2" );
}

////////////////////////////////////////////////////////////////////////////////

void VectorTest::swapRows()
{
    double x[] = { 1.0, 2.0, 3.0 };

    fdm::Vector< 3 > v1( x );
    v1.swapRows( 0, 1 );

    QVERIFY2( fabs( v1( 0 ) - 2.0 ) < 1.0e-9, "Failure 0 v1" );
    QVERIFY2( fabs( v1( 1 ) - 1.0 ) < 1.0e-9, "Failure 1 v1" );
    QVERIFY2( fabs( v1( 2 ) - 3.0 ) < 1.0e-9, "Failure 2 v1" );

    fdm::Vector< 3 > v2( x );
    v2.swapRows( 1, 2 );

    QVERIFY2( fabs( v2( 0 ) - 1.0 ) < 1.0e-9, "Failure 0 v2" );
    QVERIFY2( fabs( v2( 1 ) - 3.0 ) < 1.0e-9, "Failure 1 v2" );
    QVERIFY2( fabs( v2( 2 ) - 2.0 ) < 1.0e-9, "Failure 2 v2" );

    fdm::Vector< 3 > v3( x );
    v3.swapRows( 0, 2 );

    QVERIFY2( fabs( v3( 0 ) - 3.0 ) < 1.0e-9, "Failure 0 v3" );
    QVERIFY2( fabs( v3( 1 ) - 2.0 ) < 1.0e-9, "Failure 1 v3" );
    QVERIFY2( fabs( v3( 2 ) - 1.0 ) < 1.0e-9, "Failure 2 v3" );
}

////////////////////////////////////////////////////////////////////////////////

void VectorTest::toString()
{
    // TODO
}

////////////////////////////////////////////////////////////////////////////////

void VectorTest::zeroize()
{
    double x[] = { 1.0, 2.0, 3.0 };

    fdm::Vector< 3 > v1( x );

    v1.zeroize();

    QVERIFY2( fabs( v1( 0 ) ) < 1.0e-9, "Failure 0" );
    QVERIFY2( fabs( v1( 1 ) ) < 1.0e-9, "Failure 1" );
    QVERIFY2( fabs( v1( 2 ) ) < 1.0e-9, "Failure 2" );
}

////////////////////////////////////////////////////////////////////////////////

void VectorTest::operator_accessor()
{
    fdm::Vector< 3 > v1;

    v1( 0 ) = 1.0;
    v1( 1 ) = 2.0;
    v1( 2 ) = 3.0;

    QVERIFY2( fabs( v1( 0 ) - 1.0 ) < 1.0e-9, "Failure 0" );
    QVERIFY2( fabs( v1( 1 ) - 2.0 ) < 1.0e-9, "Failure 1" );
    QVERIFY2( fabs( v1( 2 ) - 3.0 ) < 1.0e-9, "Failure 2" );
}

////////////////////////////////////////////////////////////////////////////////

void VectorTest::operator_assign()
{
    fdm::Vector< 3 > v;

    double x1[] = { 1.0, 0.0, 0.0 };
    double x2[] = { 0.0, 1.0, 0.0 };
    double x3[] = { 0.0, 0.0, 1.0 };
    double x4[] = { 1.0, 2.0, 3.0 };

    fdm::Vector< 3 > v1( x1 );
    fdm::Vector< 3 > v2( x2 );
    fdm::Vector< 3 > v3( x3 );
    fdm::Vector< 3 > v4( x4 );

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

void VectorTest::operator_add()
{
    double x1[] = { 1.0, 2.0, 3.0 };
    double x2[] = { 3.0, 2.0, 1.0 };
    double x3[] = { 0.0, 0.0, 0.0 };
    double x4[] = { 1.0, 2.0, 3.0 };

    fdm::Vector< 3 > v1( x1 );
    fdm::Vector< 3 > v2( x2 );
    fdm::Vector< 3 > v3( x3 );
    fdm::Vector< 3 > v4( x4 );

    fdm::Vector< 3 > v12 = v1 + v2;
    fdm::Vector< 3 > v34 = v3 + v4;

    QVERIFY2( fabs( v12( 0 ) - 4.0 ) < 1.0e-9, "Failure 0 v12" );
    QVERIFY2( fabs( v12( 1 ) - 4.0 ) < 1.0e-9, "Failure 1 v12" );
    QVERIFY2( fabs( v12( 2 ) - 4.0 ) < 1.0e-9, "Failure 2 v12" );

    QVERIFY2( fabs( v34( 0 ) - 1.0 ) < 1.0e-9, "Failure 0 v34" );
    QVERIFY2( fabs( v34( 1 ) - 2.0 ) < 1.0e-9, "Failure 1 v34" );
    QVERIFY2( fabs( v34( 2 ) - 3.0 ) < 1.0e-9, "Failure 2 v34" );
}

////////////////////////////////////////////////////////////////////////////////

void VectorTest::operator_negation()
{
    double x[] = { 1.0, 2.0, 3.0 };

    fdm::Vector< 3 > v1( x );

    fdm::Vector< 3 > v2 = -v1;

    QVERIFY2( fabs( v2( 0 ) + 1.0 ) < 1.0e-9, "Failure 0" );
    QVERIFY2( fabs( v2( 1 ) + 2.0 ) < 1.0e-9, "Failure 1" );
    QVERIFY2( fabs( v2( 2 ) + 3.0 ) < 1.0e-9, "Failure 2" );
}

////////////////////////////////////////////////////////////////////////////////

void VectorTest::operator_substract()
{
    double x1[] = { 3.0, 3.0, 3.0 };
    double x2[] = { 1.0, 2.0, 3.0 };

    fdm::Vector< 3 > v1( x1 );
    fdm::Vector< 3 > v2( x2 );

    fdm::Vector< 3 > v = v1 - v2;

    QVERIFY2( fabs( v( 0 ) - 2.0 ) < 1.0e-9, "Failure 0" );
    QVERIFY2( fabs( v( 1 ) - 1.0 ) < 1.0e-9, "Failure 1" );
    QVERIFY2( fabs( v( 2 ) - 0.0 ) < 1.0e-9, "Failure 2" );
}

////////////////////////////////////////////////////////////////////////////////

void VectorTest::operator_multiply_by_scalar()
{
    double x[] = { 1.0, 2.0, 3.0 };

    fdm::Vector< 3 > v1( x );

    fdm::Vector< 3 > v2 = v1 * 2.0;

    QVERIFY2( fabs( v2( 0 ) - 2.0 ) < 1.0e-9, "Failure 0" );
    QVERIFY2( fabs( v2( 1 ) - 4.0 ) < 1.0e-9, "Failure 1" );
    QVERIFY2( fabs( v2( 2 ) - 6.0 ) < 1.0e-9, "Failure 2" );
}

////////////////////////////////////////////////////////////////////////////////

void VectorTest::operator_vector_dot_product()
{
    double x1[] = { 1.0, 0.0, 0.0 };
    double x2[] = { 0.0, 1.0, 0.0 };
    double x3[] = { 0.0, 0.0, 1.0 };
    double x4[] = { 1.0, 2.0, 3.0 };

    fdm::Vector< 3 > v1( x1 );
    fdm::Vector< 3 > v2( x2 );
    fdm::Vector< 3 > v3( x3 );
    fdm::Vector< 3 > v4( x4 );

    double s41 = v4 * v1;
    double s42 = v4 * v2;
    double s43 = v4 * v3;
    double s44 = v4 * v4;

    QVERIFY2( fabs( s41 -  1.0 ) < 1.0e-9, "Failure s41" );
    QVERIFY2( fabs( s42 -  2.0 ) < 1.0e-9, "Failure s42" );
    QVERIFY2( fabs( s43 -  3.0 ) < 1.0e-9, "Failure s43" );
    QVERIFY2( fabs( s44 - 14.0 ) < 1.0e-9, "Failure s44" );
}

////////////////////////////////////////////////////////////////////////////////

void VectorTest::operator_divide_by_scalar()
{
    double x[] = { 2.0, 4.0, 6.0 };

    fdm::Vector< 3 > v1( x );

    fdm::Vector< 3 > v2 = v1 / 2.0;

    QVERIFY2( fabs( v2( 0 ) - 1.0 ) < 1.0e-9, "Failure 0" );
    QVERIFY2( fabs( v2( 1 ) - 2.0 ) < 1.0e-9, "Failure 1" );
    QVERIFY2( fabs( v2( 2 ) - 3.0 ) < 1.0e-9, "Failure 2" );
}

////////////////////////////////////////////////////////////////////////////////

void VectorTest::operator_unary_add()
{
    double x0[] = { 1.0, 2.0, 3.0 };
    double x1[] = { 2.0, 3.0, 4.0 };

    fdm::Vector< 3 > v0( x0 );
    fdm::Vector< 3 > v1( x1 );

    v0 += v1;

    QVERIFY2( fabs( v0( 0 ) - 3.0 ) < 1.0e-9, "Failure 0" );
    QVERIFY2( fabs( v0( 1 ) - 5.0 ) < 1.0e-9, "Failure 1" );
    QVERIFY2( fabs( v0( 2 ) - 7.0 ) < 1.0e-9, "Failure 2" );
}

////////////////////////////////////////////////////////////////////////////////

void VectorTest::operator_unary_substract()
{
    double x0[] = { 3.0, 3.0, 3.0 };
    double x1[] = { 1.0, 2.0, 3.0 };

    fdm::Vector< 3 > v0( x0 );
    fdm::Vector< 3 > v1( x1 );

    v0 -= v1;

    QVERIFY2( fabs( v0( 0 ) - 2.0 ) < 1.0e-9, "Failure 0" );
    QVERIFY2( fabs( v0( 1 ) - 1.0 ) < 1.0e-9, "Failure 1" );
    QVERIFY2( fabs( v0( 2 ) - 0.0 ) < 1.0e-9, "Failure 2" );
}

////////////////////////////////////////////////////////////////////////////////

void VectorTest::operator_unary_multiply_by_scalar()
{
    double x0[] = { 1.0, 2.0, 3.0 };

    fdm::Vector< 3 > v0( x0 );

    v0 *= 2.0;

    QVERIFY2( fabs( v0( 0 ) - 2.0 ) < 1.0e-9, "Failure 0" );
    QVERIFY2( fabs( v0( 1 ) - 4.0 ) < 1.0e-9, "Failure 1" );
    QVERIFY2( fabs( v0( 2 ) - 6.0 ) < 1.0e-9, "Failure 2" );
}

////////////////////////////////////////////////////////////////////////////////

void VectorTest::operator_unary_divide_by_scalar()
{
    double x0[] = { 2.0, 4.0, 6.0 };

    fdm::Vector< 3 > v0( x0 );

    v0 /= 2.0;

    QVERIFY2( fabs( v0( 0 ) - 1.0 ) < 1.0e-9, "Failure 0" );
    QVERIFY2( fabs( v0( 1 ) - 2.0 ) < 1.0e-9, "Failure 1" );
    QVERIFY2( fabs( v0( 2 ) - 3.0 ) < 1.0e-9, "Failure 2" );
}

////////////////////////////////////////////////////////////////////////////////

void VectorTest::operator_equality()
{
    double x0[] = { 2.0, 4.0, 6.0 };
    double x1[] = { 1.0, 2.0, 3.0 };

    fdm::Vector< 3 > v0( x0 );
    fdm::Vector< 3 > v1( x1 );
    fdm::Vector< 3 > v2( x0 );

    QVERIFY2( !( v0 == v1 ), "Failure 01" );
    QVERIFY2(  ( v0 == v2 ), "Failure 02" );
}

////////////////////////////////////////////////////////////////////////////////

void VectorTest::operator_inequality()
{
    double x0[] = { 2.0, 4.0, 6.0 };
    double x1[] = { 1.0, 2.0, 3.0 };

    fdm::Vector< 3 > v0( x0 );
    fdm::Vector< 3 > v1( x1 );
    fdm::Vector< 3 > v2( x0 );

    QVERIFY2(  ( v0 != v1 ), "Failure 01" );
    QVERIFY2( !( v0 != v2 ), "Failure 02" );
}

////////////////////////////////////////////////////////////////////////////////

QTEST_APPLESS_MAIN(VectorTest)

////////////////////////////////////////////////////////////////////////////////

#include "test_fdm_vector.moc"
