#include <iostream>

#include <QString>
#include <QtTest>

#include <fdm/utils/fdm_Vector4.h>

////////////////////////////////////////////////////////////////////////////////

using namespace std;

////////////////////////////////////////////////////////////////////////////////

class Vector4Test : public QObject
{
    Q_OBJECT

public:

    Vector4Test();

private Q_SLOTS:

    void initTestCase();
    void cleanupTestCase();

    void constructor();

    void getNormalized();

    void operator_assign();
    void operator_add();
    void operator_negation();
    void operator_substract();
    void operator_multiply_by_scalar();
    void operator_divide_by_scalar();
    void operator_vector_dot_product();
    void operator_unary_add();
    void operator_unary_substract();
    void operator_unary_multiply_by_scalar();
    void operator_unary_divide_by_scalar();
};

////////////////////////////////////////////////////////////////////////////////

Vector4Test::Vector4Test() {}

////////////////////////////////////////////////////////////////////////////////

void Vector4Test::initTestCase() {}

////////////////////////////////////////////////////////////////////////////////

void Vector4Test::cleanupTestCase() {}

////////////////////////////////////////////////////////////////////////////////

void Vector4Test::constructor()
{
    fdm::Vector4 v;
    QVERIFY2( fabs( v( 0 ) - 0.0 ) < 1.0e-9, "Failure 0 v" );
    QVERIFY2( fabs( v( 1 ) - 0.0 ) < 1.0e-9, "Failure 1 v" );
    QVERIFY2( fabs( v( 2 ) - 0.0 ) < 1.0e-9, "Failure 2 v" );
    QVERIFY2( fabs( v( 3 ) - 0.0 ) < 1.0e-9, "Failure 3 v" );

    double items6[] = { 2.0, 3.0, 4.0, 5.0 };
    fdm::Vector4 v6( items6 );
    QVERIFY2( fabs( v6( 0 ) - 2.0 ) < 1.0e-9, "Failure 0 v6" );
    QVERIFY2( fabs( v6( 1 ) - 3.0 ) < 1.0e-9, "Failure 1 v6" );
    QVERIFY2( fabs( v6( 2 ) - 4.0 ) < 1.0e-9, "Failure 2 v6" );
    QVERIFY2( fabs( v6( 3 ) - 5.0 ) < 1.0e-9, "Failure 3 v6" );

    fdm::Vector4 v7( v );
    QVERIFY2( fabs( v7( 0 ) - v( 0 ) ) < 1.0e-9, "Failure 0 v7" );
    QVERIFY2( fabs( v7( 1 ) - v( 1 ) ) < 1.0e-9, "Failure 1 v7" );
    QVERIFY2( fabs( v7( 2 ) - v( 2 ) ) < 1.0e-9, "Failure 2 v7" );
    QVERIFY2( fabs( v7( 3 ) - v( 3 ) ) < 1.0e-9, "Failure 3 v7" );

    fdm::Vector4 v8( v6 );
    QVERIFY2( fabs( v8( 0 ) - v6( 0 ) ) < 1.0e-9, "Failure 0 v8" );
    QVERIFY2( fabs( v8( 1 ) - v6( 1 ) ) < 1.0e-9, "Failure 1 v8" );
    QVERIFY2( fabs( v8( 2 ) - v6( 2 ) ) < 1.0e-9, "Failure 2 v8" );
    QVERIFY2( fabs( v8( 3 ) - v6( 3 ) ) < 1.0e-9, "Failure 3 v8" );
}

////////////////////////////////////////////////////////////////////////////////

void Vector4Test::getNormalized()
{
    double x1[] = { 1.0, 2.0, 3.0, 4.0 };
    fdm::Vector4 v1( x1 );
    fdm::Vector4 v1_n = v1.getNormalized();
    QVERIFY2( fabs( v1_n.getLength() - 1.0 ) < 1.0e-9, "Failure v1_n" );

    double x2[] = { 2.0, 3.0, 4.0, 5.0 };
    fdm::Vector4 v2( x2 );
    fdm::Vector4 v2_n = v2.getNormalized();
    QVERIFY2( fabs( v2_n.getLength() - 1.0 ) < 1.0e-9, "Failure v2_n" );
}

////////////////////////////////////////////////////////////////////////////////

void Vector4Test::operator_assign()
{
    fdm::Vector4 v;

    double x1[] = { 1.0, 0.0, 0.0, 0.0 };
    double x2[] = { 0.0, 1.0, 0.0, 0.0 };
    double x3[] = { 0.0, 0.0, 1.0, 0.0 };
    double x4[] = { 0.0, 0.0, 0.0, 1.0 };
    double x5[] = { 1.0, 2.0, 3.0, 4.0 };

    fdm::Vector4 v1( x1 );
    fdm::Vector4 v2( x2 );
    fdm::Vector4 v3( x3 );
    fdm::Vector4 v4( x4 );
    fdm::Vector4 v5( x5 );

    v = v1;
    QVERIFY2( fabs( v( 0 ) - 1.0 ) < 1.0e-9, "Failure 0 v1" );
    QVERIFY2( fabs( v( 1 ) - 0.0 ) < 1.0e-9, "Failure 1 v1" );
    QVERIFY2( fabs( v( 2 ) - 0.0 ) < 1.0e-9, "Failure 2 v1" );
    QVERIFY2( fabs( v( 3 ) - 0.0 ) < 1.0e-9, "Failure 3 v1" );

    v = v2;
    QVERIFY2( fabs( v( 0 ) - 0.0 ) < 1.0e-9, "Failure 0 v2" );
    QVERIFY2( fabs( v( 1 ) - 1.0 ) < 1.0e-9, "Failure 1 v2" );
    QVERIFY2( fabs( v( 2 ) - 0.0 ) < 1.0e-9, "Failure 2 v2" );
    QVERIFY2( fabs( v( 3 ) - 0.0 ) < 1.0e-9, "Failure 3 v2" );

    v = v3;
    QVERIFY2( fabs( v( 0 ) - 0.0 ) < 1.0e-9, "Failure 0 v3" );
    QVERIFY2( fabs( v( 1 ) - 0.0 ) < 1.0e-9, "Failure 1 v3" );
    QVERIFY2( fabs( v( 2 ) - 1.0 ) < 1.0e-9, "Failure 2 v3" );
    QVERIFY2( fabs( v( 3 ) - 0.0 ) < 1.0e-9, "Failure 3 v3" );

    v = v4;
    QVERIFY2( fabs( v( 0 ) - 0.0 ) < 1.0e-9, "Failure 0 v4" );
    QVERIFY2( fabs( v( 1 ) - 0.0 ) < 1.0e-9, "Failure 1 v4" );
    QVERIFY2( fabs( v( 2 ) - 0.0 ) < 1.0e-9, "Failure 2 v4" );
    QVERIFY2( fabs( v( 3 ) - 1.0 ) < 1.0e-9, "Failure 3 v4" );

    v = v5;
    QVERIFY2( fabs( v( 0 ) - 1.0 ) < 1.0e-9, "Failure 0 v4" );
    QVERIFY2( fabs( v( 1 ) - 2.0 ) < 1.0e-9, "Failure 1 v4" );
    QVERIFY2( fabs( v( 2 ) - 3.0 ) < 1.0e-9, "Failure 2 v4" );
    QVERIFY2( fabs( v( 3 ) - 4.0 ) < 1.0e-9, "Failure 3 v4" );
}

////////////////////////////////////////////////////////////////////////////////

void Vector4Test::operator_add()
{
    double x1[] = { 0.0, 0.0, 0.0, 0.0 };
    double x2[] = { 1.0, 2.0, 3.0, 4.0 };
    double x3[] = { 1.0, 2.0, 3.0, 4.0 };
    double x4[] = { 4.0, 3.0, 2.0, 1.0 };

    fdm::Vector4 v1( x1 );
    fdm::Vector4 v2( x2 );
    fdm::Vector4 v3( x3 );
    fdm::Vector4 v4( x4 );

    fdm::Vector4 v12 = v1 + v2;
    fdm::Vector4 v34 = v3 + v4;

    QVERIFY2( fabs( v12( 0 ) - 1.0 ) < 1.0e-9, "Failure 0 v12" );
    QVERIFY2( fabs( v12( 1 ) - 2.0 ) < 1.0e-9, "Failure 1 v12" );
    QVERIFY2( fabs( v12( 2 ) - 3.0 ) < 1.0e-9, "Failure 2 v12" );
    QVERIFY2( fabs( v12( 3 ) - 4.0 ) < 1.0e-9, "Failure 3 v12" );

    QVERIFY2( fabs( v34( 0 ) - 5.0 ) < 1.0e-9, "Failure 0 v34" );
    QVERIFY2( fabs( v34( 1 ) - 5.0 ) < 1.0e-9, "Failure 1 v34" );
    QVERIFY2( fabs( v34( 2 ) - 5.0 ) < 1.0e-9, "Failure 2 v34" );
    QVERIFY2( fabs( v34( 3 ) - 5.0 ) < 1.0e-9, "Failure 3 v34" );
}

////////////////////////////////////////////////////////////////////////////////

void Vector4Test::operator_negation()
{
    double x1[] = { 1.0, 2.0, 3.0, 4.0 };
    double x2[] = { 4.0, 3.0, 2.0, 1.0 };

    fdm::Vector4 v1( x1 );
    fdm::Vector4 v2( x2 );

    fdm::Vector4 v1n = -v1;
    fdm::Vector4 v2n = -v2;

    QVERIFY2( fabs( v1n( 0 ) + 1.0 ) < 1.0e-9, "Failure 0 v1n" );
    QVERIFY2( fabs( v1n( 1 ) + 2.0 ) < 1.0e-9, "Failure 1 v1n" );
    QVERIFY2( fabs( v1n( 2 ) + 3.0 ) < 1.0e-9, "Failure 2 v1n" );
    QVERIFY2( fabs( v1n( 3 ) + 4.0 ) < 1.0e-9, "Failure 3 v1n" );

    QVERIFY2( fabs( v2n( 0 ) + 4.0 ) < 1.0e-9, "Failure 0 v2n" );
    QVERIFY2( fabs( v2n( 1 ) + 3.0 ) < 1.0e-9, "Failure 1 v2n" );
    QVERIFY2( fabs( v2n( 2 ) + 2.0 ) < 1.0e-9, "Failure 2 v2n" );
    QVERIFY2( fabs( v2n( 3 ) + 1.0 ) < 1.0e-9, "Failure 3 v2n" );
}

////////////////////////////////////////////////////////////////////////////////

void Vector4Test::operator_substract()
{
    double x1[] = { 4.0, 4.0, 4.0, 4.0 };
    double x2[] = { 1.0, 2.0, 3.0, 4.0 };

    fdm::Vector4 v1( x1 );
    fdm::Vector4 v2( x2 );

    fdm::Vector4 v = v1 - v2;

    QVERIFY2( fabs( v( 0 ) - 3.0 ) < 1.0e-9, "Failure 0" );
    QVERIFY2( fabs( v( 1 ) - 2.0 ) < 1.0e-9, "Failure 1" );
    QVERIFY2( fabs( v( 2 ) - 1.0 ) < 1.0e-9, "Failure 2" );
    QVERIFY2( fabs( v( 3 ) - 0.0 ) < 1.0e-9, "Failure 3" );
}

////////////////////////////////////////////////////////////////////////////////

void Vector4Test::operator_multiply_by_scalar()
{
    double x1[] = { 1.0, 2.0, 3.0, 4.0 };

    fdm::Vector4 v1( x1 );

    fdm::Vector4 v = v1 * 2.0;

    QVERIFY2( fabs( v( 0 ) - 2.0 ) < 1.0e-9, "Failure 0" );
    QVERIFY2( fabs( v( 1 ) - 4.0 ) < 1.0e-9, "Failure 1" );
    QVERIFY2( fabs( v( 2 ) - 6.0 ) < 1.0e-9, "Failure 2" );
    QVERIFY2( fabs( v( 3 ) - 8.0 ) < 1.0e-9, "Failure 3" );
}

////////////////////////////////////////////////////////////////////////////////

void Vector4Test::operator_divide_by_scalar()
{
    double x1[] = { 2.0, 4.0, 6.0, 8.0 };

    fdm::Vector4 v1( x1 );

    fdm::Vector4 v = v1 / 2.0;

    QVERIFY2( fabs( v( 0 ) - 1.0 ) < 1.0e-9, "Failure 0" );
    QVERIFY2( fabs( v( 1 ) - 2.0 ) < 1.0e-9, "Failure 1" );
    QVERIFY2( fabs( v( 2 ) - 3.0 ) < 1.0e-9, "Failure 2" );
    QVERIFY2( fabs( v( 3 ) - 4.0 ) < 1.0e-9, "Failure 3" );
}

////////////////////////////////////////////////////////////////////////////////

void Vector4Test::operator_vector_dot_product()
{
    // TODO
}

////////////////////////////////////////////////////////////////////////////////

void Vector4Test::operator_unary_add()
{
    double x1[] = { 0.0, 0.0, 0.0, 0.0 };
    double x2[] = { 1.0, 2.0, 3.0, 4.0 };

    fdm::Vector4 v1( x1 );
    fdm::Vector4 v2( x2 );

    v1 += v2;

    QVERIFY2( fabs( v1( 0 ) - 1.0 ) < 1.0e-9, "Failure 0" );
    QVERIFY2( fabs( v1( 1 ) - 2.0 ) < 1.0e-9, "Failure 1" );
    QVERIFY2( fabs( v1( 2 ) - 3.0 ) < 1.0e-9, "Failure 2" );
    QVERIFY2( fabs( v1( 3 ) - 4.0 ) < 1.0e-9, "Failure 3" );
}

////////////////////////////////////////////////////////////////////////////////

void Vector4Test::operator_unary_substract()
{
    double x1[] = { 4.0, 4.0, 4.0, 4.0 };
    double x2[] = { 1.0, 2.0, 3.0, 4.0 };

    fdm::Vector4 v1( x1 );
    fdm::Vector4 v2( x2 );

    v1 -= v2;

    QVERIFY2( fabs( v1( 0 ) - 3.0 ) < 1.0e-9, "Failure 0" );
    QVERIFY2( fabs( v1( 1 ) - 2.0 ) < 1.0e-9, "Failure 1" );
    QVERIFY2( fabs( v1( 2 ) - 1.0 ) < 1.0e-9, "Failure 2" );
    QVERIFY2( fabs( v1( 3 ) - 0.0 ) < 1.0e-9, "Failure 3" );
}

////////////////////////////////////////////////////////////////////////////////

void Vector4Test::operator_unary_multiply_by_scalar()
{
    double x1[] = { 1.0, 2.0, 3.0, 4.0 };

    fdm::Vector4 v1( x1 );

    v1 *= 2.0;

    QVERIFY2( fabs( v1( 0 ) - 2.0 ) < 1.0e-9, "Failure 0" );
    QVERIFY2( fabs( v1( 1 ) - 4.0 ) < 1.0e-9, "Failure 1" );
    QVERIFY2( fabs( v1( 2 ) - 6.0 ) < 1.0e-9, "Failure 2" );
    QVERIFY2( fabs( v1( 3 ) - 8.0 ) < 1.0e-9, "Failure 3" );
}

////////////////////////////////////////////////////////////////////////////////

void Vector4Test::operator_unary_divide_by_scalar()
{
    double x1[] = { 2.0, 4.0, 6.0, 8.0 };

    fdm::Vector4 v1( x1 );

    v1 /= 2.0;

    QVERIFY2( fabs( v1( 0 ) - 1.0 ) < 1.0e-9, "Failure 0" );
    QVERIFY2( fabs( v1( 1 ) - 2.0 ) < 1.0e-9, "Failure 1" );
    QVERIFY2( fabs( v1( 2 ) - 3.0 ) < 1.0e-9, "Failure 2" );
    QVERIFY2( fabs( v1( 3 ) - 4.0 ) < 1.0e-9, "Failure 3" );
}

////////////////////////////////////////////////////////////////////////////////

QTEST_APPLESS_MAIN(Vector4Test)

////////////////////////////////////////////////////////////////////////////////

#include "test_fdm_vector4.moc"
