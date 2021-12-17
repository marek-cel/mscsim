#include <iostream>

#include <QString>
#include <QtTest>

#include <fdm/utils/fdm_Vector3.h>
#include <fdm/utils/fdm_Matrix3x3.h>

////////////////////////////////////////////////////////////////////////////////

using namespace std;

////////////////////////////////////////////////////////////////////////////////

class Vector3Test : public QObject
{
    Q_OBJECT

public:

    Vector3Test();

private Q_SLOTS:

    void initTestCase();
    void cleanupTestCase();

    void constructor();
    void getLength2();
    void getLengthXY();
    void getLengthXZ();
    void getLengthYZ();
    void getNormalized();
    void getRotated();
    void set();

    void operator_assign();
    void operator_add();
    void operator_negation();
    void operator_substract();
    void operator_multiply_by_scalar();
    void operator_divide_by_scalar();
    void operator_vector_dot_product();
    void operator_vector_cross_product();
    void operator_unary_add();
    void operator_unary_substract();
    void operator_unary_multiply_by_scalar();
    void operator_unary_divide_by_scalar();
    void operator_unary_vector_cross_product();
};

////////////////////////////////////////////////////////////////////////////////

Vector3Test::Vector3Test() {}

////////////////////////////////////////////////////////////////////////////////

void Vector3Test::initTestCase() {}

////////////////////////////////////////////////////////////////////////////////

void Vector3Test::cleanupTestCase() {}

////////////////////////////////////////////////////////////////////////////////

void Vector3Test::constructor()
{
    fdm::Vector3 v;
    QVERIFY2( fabs( v.x() - 0.0 ) < 1.0e-9, "Failure x v" );
    QVERIFY2( fabs( v.y() - 0.0 ) < 1.0e-9, "Failure y v" );
    QVERIFY2( fabs( v.z() - 0.0 ) < 1.0e-9, "Failure z v" );

    fdm::Vector3 v0( 0.0, 0.0, 0.0 );
    QVERIFY2( fabs( v0.x() - 0.0 ) < 1.0e-9, "Failure x v0" );
    QVERIFY2( fabs( v0.y() - 0.0 ) < 1.0e-9, "Failure y v0" );
    QVERIFY2( fabs( v0.z() - 0.0 ) < 1.0e-9, "Failure z v0" );

    fdm::Vector3 v1( 1.0, 0.0, 0.0 );
    QVERIFY2( fabs( v1.x() - 1.0 ) < 1.0e-9, "Failure x v1" );
    QVERIFY2( fabs( v1.y() - 0.0 ) < 1.0e-9, "Failure y v1" );
    QVERIFY2( fabs( v1.z() - 0.0 ) < 1.0e-9, "Failure z v1" );

    fdm::Vector3 v2( 0.0, 1.0, 0.0 );
    QVERIFY2( fabs( v2.x() - 0.0 ) < 1.0e-9, "Failure x v2" );
    QVERIFY2( fabs( v2.y() - 1.0 ) < 1.0e-9, "Failure y v2" );
    QVERIFY2( fabs( v2.z() - 0.0 ) < 1.0e-9, "Failure z v2" );

    fdm::Vector3 v3( 0.0, 0.0, 1.0 );
    QVERIFY2( fabs( v3.x() - 0.0 ) < 1.0e-9, "Failure x v3" );
    QVERIFY2( fabs( v3.y() - 0.0 ) < 1.0e-9, "Failure y v3" );
    QVERIFY2( fabs( v3.z() - 1.0 ) < 1.0e-9, "Failure z v3" );

    fdm::Vector3 v4( 1.0, 2.0, 3.0 );
    QVERIFY2( fabs( v4.x() - 1.0 ) < 1.0e-9, "Failure x v4" );
    QVERIFY2( fabs( v4.y() - 2.0 ) < 1.0e-9, "Failure y v4" );
    QVERIFY2( fabs( v4.z() - 3.0 ) < 1.0e-9, "Failure z v4" );

    double items[] = { 2.0, 3.0, 4.0 };
    fdm::Vector3 v5( items );
    QVERIFY2( fabs( v5.x() - 2.0 ) < 1.0e-9, "Failure x v5" );
    QVERIFY2( fabs( v5.y() - 3.0 ) < 1.0e-9, "Failure y v5" );
    QVERIFY2( fabs( v5.z() - 4.0 ) < 1.0e-9, "Failure z v5" );

    fdm::Vector3 v6( v0 );
    QVERIFY2( fabs( v6.x() - v0.x() ) < 1.0e-9, "Failure x v6" );
    QVERIFY2( fabs( v6.y() - v0.y() ) < 1.0e-9, "Failure y v6" );
    QVERIFY2( fabs( v6.z() - v0.z() ) < 1.0e-9, "Failure z v6" );

    fdm::Vector3 v7( v4 );
    QVERIFY2( fabs( v7.x() - v4.x() ) < 1.0e-9, "Failure x v7" );
    QVERIFY2( fabs( v7.y() - v4.y() ) < 1.0e-9, "Failure y v7" );
    QVERIFY2( fabs( v7.z() - v4.z() ) < 1.0e-9, "Failure z v7" );
}

////////////////////////////////////////////////////////////////////////////////

void Vector3Test::getLength2()
{
    fdm::Vector3 v1( 1.0, 2.0, 3.0 );
    // 1^2 + 2^2 + 3^2 = 1 + 4 + 9 = 14
    QVERIFY2( fabs( v1.getLength2() - 14.0 ) < 1.0e-9, "Failure v1" );

    fdm::Vector3 v2( 2.0, 3.0, 4.0 );
    // 2^2 + 3^2 + 4^2 = 4 + 9 + 16 = 29
    QVERIFY2( fabs( v2.getLength2() - 29.0 ) < 1.0e-9, "Failure v2" );
}

////////////////////////////////////////////////////////////////////////////////

void Vector3Test::getLengthXY()
{
    fdm::Vector3 v1( 1.0, 2.0, 3.0 );
    // 1^2 + 2^2 = 1 + 4 = 5
    QVERIFY2( fabs( v1.getLengthXY() - sqrt( 5.0 ) ) < 1.0e-9, "Failure v1" );

    fdm::Vector3 v2( 2.0, 3.0, 4.0 );
    // 2^2 + 3^2 = 4 + 9 = 13
    QVERIFY2( fabs( v2.getLengthXY() - sqrt( 13.0 ) ) < 1.0e-9, "Failure v2" );
}

////////////////////////////////////////////////////////////////////////////////

void Vector3Test::getLengthXZ()
{
    fdm::Vector3 v1( 1.0, 2.0, 3.0 );
    // 1^2 + 3^2 = 1 + 9 = 10
    QVERIFY2( fabs( v1.getLengthXZ() - sqrt( 10.0 ) ) < 1.0e-9, "Failure v1" );

    fdm::Vector3 v2( 2.0, 3.0, 4.0 );
    // 2^2 + 4^2 = 4 + 16 = 20
    QVERIFY2( fabs( v2.getLengthXZ() - sqrt( 20.0 ) ) < 1.0e-9, "Failure v2" );
}

////////////////////////////////////////////////////////////////////////////////

void Vector3Test::getLengthYZ()
{
    fdm::Vector3 v1( 1.0, 2.0, 3.0 );
    // 2^2 + 3^2 = 4 + 9 = 13
    QVERIFY2( fabs( v1.getLengthYZ() - sqrt( 13.0 ) ) < 1.0e-9, "Failure v1" );

    fdm::Vector3 v2( 2.0, 3.0, 4.0 );
    // 3^2 + 4^2 = 9 + 16 = 25
    QVERIFY2( fabs( v2.getLengthYZ() - sqrt( 25.0 ) ) < 1.0e-9, "Failure v2" );
}

////////////////////////////////////////////////////////////////////////////////

void Vector3Test::getNormalized()
{
    fdm::Vector3 v1( 1.0, 2.0, 3.0 );
    fdm::Vector3 v1_n = v1.getNormalized();
    QVERIFY2( fabs( v1_n.getLength() - 1.0 ) < 1.0e-9, "Failure v1_n" );

    fdm::Vector3 v2( 2.0, 3.0, 4.0 );
    fdm::Vector3 v2_n = v2.getNormalized();
    QVERIFY2( fabs( v2_n.getLength() - 1.0 ) < 1.0e-9, "Failure v2_n" );
}

////////////////////////////////////////////////////////////////////////////////

void Vector3Test::getRotated()
{
    fdm::Angles a1( M_PI_2, 0.0, 0.0 );
    fdm::Angles a2( 0.0, M_PI_2, 0.0 );
    fdm::Angles a3( 0.0, 0.0, M_PI_2 );
    fdm::Angles a4( M_PI_2, M_PI_2, M_PI_2 );

    fdm::Quaternion q1( a1 );
    fdm::Quaternion q2( a2 );
    fdm::Quaternion q3( a3 );
    fdm::Quaternion q4( a4 );

    fdm::Matrix3x3 m1( a1 );
    fdm::Matrix3x3 m2( a2 );
    fdm::Matrix3x3 m3( a3 );
    fdm::Matrix3x3 m4( a4 );

    fdm::Vector3 v1( 1.0, 2.0, 3.0 );
    fdm::Vector3 v2( 2.0, 3.0, 4.0 );

    fdm::Vector3 v1_1r = v1.getRotated( q1 );
    fdm::Vector3 v1_2r = v1.getRotated( q2 );
    fdm::Vector3 v1_3r = v1.getRotated( q3 );
    fdm::Vector3 v1_4r = v1.getRotated( q4 );

    fdm::Vector3 v2_1r = v2.getRotated( q1 );
    fdm::Vector3 v2_2r = v2.getRotated( q2 );
    fdm::Vector3 v2_3r = v2.getRotated( q3 );
    fdm::Vector3 v2_4r = v2.getRotated( q4 );

    fdm::Vector3 v1_1m = m1 * v1;
    fdm::Vector3 v1_2m = m2 * v1;
    fdm::Vector3 v1_3m = m3 * v1;
    fdm::Vector3 v1_4m = m4 * v1;

    fdm::Vector3 v2_1m = m1 * v2;
    fdm::Vector3 v2_2m = m2 * v2;
    fdm::Vector3 v2_3m = m3 * v2;
    fdm::Vector3 v2_4m = m4 * v2;

    QVERIFY2( fabs( v1_1r.x() - v1_1m.x() ) < 1.0e-9, "Failure v1_1 x" );
    QVERIFY2( fabs( v1_1r.y() - v1_1m.y() ) < 1.0e-9, "Failure v1_1 y" );
    QVERIFY2( fabs( v1_1r.z() - v1_1m.z() ) < 1.0e-9, "Failure v1_1 z" );

    QVERIFY2( fabs( v1_2r.x() - v1_2m.x() ) < 1.0e-9, "Failure v1_2 x" );
    QVERIFY2( fabs( v1_2r.y() - v1_2m.y() ) < 1.0e-9, "Failure v1_2 y" );
    QVERIFY2( fabs( v1_2r.z() - v1_2m.z() ) < 1.0e-9, "Failure v1_2 z" );

    QVERIFY2( fabs( v1_3r.x() - v1_3m.x() ) < 1.0e-9, "Failure v1_3 x" );
    QVERIFY2( fabs( v1_3r.y() - v1_3m.y() ) < 1.0e-9, "Failure v1_3 y" );
    QVERIFY2( fabs( v1_3r.z() - v1_3m.z() ) < 1.0e-9, "Failure v1_3 z" );

    QVERIFY2( fabs( v1_4r.x() - v1_4m.x() ) < 1.0e-9, "Failure v1_4 x" );
    QVERIFY2( fabs( v1_4r.y() - v1_4m.y() ) < 1.0e-9, "Failure v1_4 y" );
    QVERIFY2( fabs( v1_4r.z() - v1_4m.z() ) < 1.0e-9, "Failure v1_4 z" );

    QVERIFY2( fabs( v2_1r.x() - v2_1m.x() ) < 1.0e-9, "Failure v2_1 x" );
    QVERIFY2( fabs( v2_1r.y() - v2_1m.y() ) < 1.0e-9, "Failure v2_1 y" );
    QVERIFY2( fabs( v2_1r.z() - v2_1m.z() ) < 1.0e-9, "Failure v2_1 z" );

    QVERIFY2( fabs( v2_2r.x() - v2_2m.x() ) < 1.0e-9, "Failure v2_2 x" );
    QVERIFY2( fabs( v2_2r.y() - v2_2m.y() ) < 1.0e-9, "Failure v2_2 y" );
    QVERIFY2( fabs( v2_2r.z() - v2_2m.z() ) < 1.0e-9, "Failure v2_2 z" );

    QVERIFY2( fabs( v2_3r.x() - v2_3m.x() ) < 1.0e-9, "Failure v2_3 x" );
    QVERIFY2( fabs( v2_3r.y() - v2_3m.y() ) < 1.0e-9, "Failure v2_3 y" );
    QVERIFY2( fabs( v2_3r.z() - v2_3m.z() ) < 1.0e-9, "Failure v2_3 z" );

    QVERIFY2( fabs( v2_4r.x() - v2_4m.x() ) < 1.0e-9, "Failure v2_4 x" );
    QVERIFY2( fabs( v2_4r.y() - v2_4m.y() ) < 1.0e-9, "Failure v2_4 y" );
    QVERIFY2( fabs( v2_4r.z() - v2_4m.z() ) < 1.0e-9, "Failure v2_4 z" );
}

////////////////////////////////////////////////////////////////////////////////

void Vector3Test::set()
{
    fdm::Vector3 v;

    v.set( 1.0, 0.0, 0.0 );
    QVERIFY2( fabs( v.x() - 1.0 ) < 1.0e-9, "Failure x v1" );
    QVERIFY2( fabs( v.y() - 0.0 ) < 1.0e-9, "Failure y v1" );
    QVERIFY2( fabs( v.z() - 0.0 ) < 1.0e-9, "Failure z v1" );

    v.set( 0.0, 1.0, 0.0 );
    QVERIFY2( fabs( v.x() - 0.0 ) < 1.0e-9, "Failure x v2" );
    QVERIFY2( fabs( v.y() - 1.0 ) < 1.0e-9, "Failure y v2" );
    QVERIFY2( fabs( v.z() - 0.0 ) < 1.0e-9, "Failure z v2" );

    v.set( 0.0, 0.0, 1.0 );
    QVERIFY2( fabs( v.x() - 0.0 ) < 1.0e-9, "Failure x v3" );
    QVERIFY2( fabs( v.y() - 0.0 ) < 1.0e-9, "Failure y v3" );
    QVERIFY2( fabs( v.z() - 1.0 ) < 1.0e-9, "Failure z v3" );

    v.set( 1.0, 2.0, 3.0 );
    QVERIFY2( fabs( v.x() - 1.0 ) < 1.0e-9, "Failure x v4" );
    QVERIFY2( fabs( v.y() - 2.0 ) < 1.0e-9, "Failure y v4" );
    QVERIFY2( fabs( v.z() - 3.0 ) < 1.0e-9, "Failure z v4" );
}

////////////////////////////////////////////////////////////////////////////////

void Vector3Test::operator_assign()
{
    fdm::Vector3 v;

    fdm::Vector3 v1( 1.0, 0.0, 0.0 );
    fdm::Vector3 v2( 0.0, 1.0, 0.0 );
    fdm::Vector3 v3( 0.0, 0.0, 1.0 );
    fdm::Vector3 v4( 1.0, 2.0, 3.0 );

    v = v1;
    QVERIFY2( fabs( v.x() - 1.0 ) < 1.0e-9, "Failure x v1" );
    QVERIFY2( fabs( v.y() - 0.0 ) < 1.0e-9, "Failure y v1" );
    QVERIFY2( fabs( v.z() - 0.0 ) < 1.0e-9, "Failure z v1" );

    v = v2;
    QVERIFY2( fabs( v.x() - 0.0 ) < 1.0e-9, "Failure x v2" );
    QVERIFY2( fabs( v.y() - 1.0 ) < 1.0e-9, "Failure y v2" );
    QVERIFY2( fabs( v.z() - 0.0 ) < 1.0e-9, "Failure z v2" );

    v = v3;
    QVERIFY2( fabs( v.x() - 0.0 ) < 1.0e-9, "Failure x v3" );
    QVERIFY2( fabs( v.y() - 0.0 ) < 1.0e-9, "Failure y v3" );
    QVERIFY2( fabs( v.z() - 1.0 ) < 1.0e-9, "Failure z v3" );

    v = v4;
    QVERIFY2( fabs( v.x() - 1.0 ) < 1.0e-9, "Failure x v4" );
    QVERIFY2( fabs( v.y() - 2.0 ) < 1.0e-9, "Failure y v4" );
    QVERIFY2( fabs( v.z() - 3.0 ) < 1.0e-9, "Failure z v4" );
}

////////////////////////////////////////////////////////////////////////////////

void Vector3Test::operator_add()
{
    fdm::Vector3 v1( 1.0, 0.0, 0.0 );
    fdm::Vector3 v2( 0.0, 1.0, 0.0 );
    fdm::Vector3 v3( 0.0, 0.0, 1.0 );
    fdm::Vector3 v4( 1.0, 2.0, 3.0 );

    fdm::Vector3 v12 = v1 + v2;
    fdm::Vector3 v13 = v1 + v3;
    fdm::Vector3 v23 = v2 + v3;

    fdm::Vector3 v123 = v1 + v2 + v3;

    fdm::Vector3 v41 = v4 + v1;
    fdm::Vector3 v42 = v4 + v2;
    fdm::Vector3 v43 = v4 + v3;

    fdm::Vector3 v4123 = v4 + v1 + v2 + v3;

    QVERIFY2( fabs( v12.x() - 1.0 ) < 1.0e-9, "Failure x v12" );
    QVERIFY2( fabs( v12.y() - 1.0 ) < 1.0e-9, "Failure y v12" );
    QVERIFY2( fabs( v12.z() - 0.0 ) < 1.0e-9, "Failure z v12" );

    QVERIFY2( fabs( v13.x() - 1.0 ) < 1.0e-9, "Failure x v13" );
    QVERIFY2( fabs( v13.y() - 0.0 ) < 1.0e-9, "Failure y v13" );
    QVERIFY2( fabs( v13.z() - 1.0 ) < 1.0e-9, "Failure z v13" );

    QVERIFY2( fabs( v23.x() - 0.0 ) < 1.0e-9, "Failure x v23" );
    QVERIFY2( fabs( v23.y() - 1.0 ) < 1.0e-9, "Failure y v23" );
    QVERIFY2( fabs( v23.z() - 1.0 ) < 1.0e-9, "Failure z v23" );

    QVERIFY2( fabs( v123.x() - 1.0 ) < 1.0e-9, "Failure x v123" );
    QVERIFY2( fabs( v123.y() - 1.0 ) < 1.0e-9, "Failure y v123" );
    QVERIFY2( fabs( v123.z() - 1.0 ) < 1.0e-9, "Failure z v123" );

    QVERIFY2( fabs( v41.x() - 2.0 ) < 1.0e-9, "Failure x v41" );
    QVERIFY2( fabs( v41.y() - 2.0 ) < 1.0e-9, "Failure y v41" );
    QVERIFY2( fabs( v41.z() - 3.0 ) < 1.0e-9, "Failure z v41" );

    QVERIFY2( fabs( v42.x() - 1.0 ) < 1.0e-9, "Failure x v42" );
    QVERIFY2( fabs( v42.y() - 3.0 ) < 1.0e-9, "Failure y v42" );
    QVERIFY2( fabs( v42.z() - 3.0 ) < 1.0e-9, "Failure z v42" );

    QVERIFY2( fabs( v43.x() - 1.0 ) < 1.0e-9, "Failure x v43" );
    QVERIFY2( fabs( v43.y() - 2.0 ) < 1.0e-9, "Failure y v43" );
    QVERIFY2( fabs( v43.z() - 4.0 ) < 1.0e-9, "Failure z v43" );

    QVERIFY2( fabs( v4123.x() - 2.0 ) < 1.0e-9, "Failure x v4123" );
    QVERIFY2( fabs( v4123.y() - 3.0 ) < 1.0e-9, "Failure y v4123" );
    QVERIFY2( fabs( v4123.z() - 4.0 ) < 1.0e-9, "Failure z v4123" );
}

////////////////////////////////////////////////////////////////////////////////

void Vector3Test::operator_negation()
{
    fdm::Vector3 v1( 1.0, 0.0, 0.0 );
    fdm::Vector3 v2( 0.0, 1.0, 0.0 );
    fdm::Vector3 v3( 0.0, 0.0, 1.0 );
    fdm::Vector3 v4( 1.0, 2.0, 3.0 );

    fdm::Vector3 v1_n = -v1;
    fdm::Vector3 v2_n = -v2;
    fdm::Vector3 v3_n = -v3;
    fdm::Vector3 v4_n = -v4;

    QVERIFY2( fabs( v1_n.x() + 1.0 ) < 1.0e-9, "Failure x v1_n" );
    QVERIFY2( fabs( v1_n.y() - 0.0 ) < 1.0e-9, "Failure y v1_n" );
    QVERIFY2( fabs( v1_n.z() - 0.0 ) < 1.0e-9, "Failure z v1_n" );

    QVERIFY2( fabs( v2_n.x() - 0.0 ) < 1.0e-9, "Failure x v2_n" );
    QVERIFY2( fabs( v2_n.y() + 1.0 ) < 1.0e-9, "Failure y v2_n" );
    QVERIFY2( fabs( v2_n.z() - 0.0 ) < 1.0e-9, "Failure z v2_n" );

    QVERIFY2( fabs( v3_n.x() - 0.0 ) < 1.0e-9, "Failure x v3_n" );
    QVERIFY2( fabs( v3_n.y() - 0.0 ) < 1.0e-9, "Failure y v3_n" );
    QVERIFY2( fabs( v3_n.z() + 1.0 ) < 1.0e-9, "Failure z v3_n" );

    QVERIFY2( fabs( v4_n.x() + 1.0 ) < 1.0e-9, "Failure x v4_n" );
    QVERIFY2( fabs( v4_n.y() + 2.0 ) < 1.0e-9, "Failure y v4_n" );
    QVERIFY2( fabs( v4_n.z() + 3.0 ) < 1.0e-9, "Failure z v4_n" );
}

////////////////////////////////////////////////////////////////////////////////

void Vector3Test::operator_substract()
{
    fdm::Vector3 v1( 1.0, 0.0, 0.0 );
    fdm::Vector3 v2( 0.0, 1.0, 0.0 );
    fdm::Vector3 v3( 0.0, 0.0, 1.0 );
    fdm::Vector3 v4( 1.0, 2.0, 3.0 );

    fdm::Vector3 v41 = v4 - v1;
    fdm::Vector3 v42 = v4 - v2;
    fdm::Vector3 v43 = v4 - v3;

    fdm::Vector3 v4123 = v4 - v1 - v2 - v3;

    QVERIFY2( fabs( v41.x() - 0.0 ) < 1.0e-9, "Failure x v41" );
    QVERIFY2( fabs( v41.y() - 2.0 ) < 1.0e-9, "Failure y v41" );
    QVERIFY2( fabs( v41.z() - 3.0 ) < 1.0e-9, "Failure z v41" );

    QVERIFY2( fabs( v42.x() - 1.0 ) < 1.0e-9, "Failure x v42" );
    QVERIFY2( fabs( v42.y() - 1.0 ) < 1.0e-9, "Failure y v42" );
    QVERIFY2( fabs( v42.z() - 3.0 ) < 1.0e-9, "Failure z v42" );

    QVERIFY2( fabs( v43.x() - 1.0 ) < 1.0e-9, "Failure x v43" );
    QVERIFY2( fabs( v43.y() - 2.0 ) < 1.0e-9, "Failure y v43" );
    QVERIFY2( fabs( v43.z() - 2.0 ) < 1.0e-9, "Failure z v43" );

    QVERIFY2( fabs( v4123.x() - 0.0 ) < 1.0e-9, "Failure x v4123" );
    QVERIFY2( fabs( v4123.y() - 1.0 ) < 1.0e-9, "Failure y v4123" );
    QVERIFY2( fabs( v4123.z() - 2.0 ) < 1.0e-9, "Failure z v4123" );
}

////////////////////////////////////////////////////////////////////////////////

void Vector3Test::operator_multiply_by_scalar()
{
    fdm::Vector3 v1( 1.0, 0.0, 0.0 );
    fdm::Vector3 v2( 0.0, 1.0, 0.0 );
    fdm::Vector3 v3( 0.0, 0.0, 1.0 );
    fdm::Vector3 v4( 1.0, 2.0, 3.0 );

    fdm::Vector3 v1_2 = v1 * 2.0;
    fdm::Vector3 v2_2 = v2 * 2.0;
    fdm::Vector3 v3_2 = v3 * 2.0;
    fdm::Vector3 v4_2 = v4 * 2.0;

    fdm::Vector3 v1_3 = v1 * 3.0;
    fdm::Vector3 v2_3 = v2 * 3.0;
    fdm::Vector3 v3_3 = v3 * 3.0;
    fdm::Vector3 v4_3 = v4 * 3.0;

    QVERIFY2( fabs( v1_2.x() - 2.0 ) < 1.0e-9, "Failure x v1_2" );
    QVERIFY2( fabs( v1_2.y() - 0.0 ) < 1.0e-9, "Failure y v1_2" );
    QVERIFY2( fabs( v1_2.z() - 0.0 ) < 1.0e-9, "Failure z v1_2" );

    QVERIFY2( fabs( v2_2.x() - 0.0 ) < 1.0e-9, "Failure x v2_2" );
    QVERIFY2( fabs( v2_2.y() - 2.0 ) < 1.0e-9, "Failure y v2_2" );
    QVERIFY2( fabs( v2_2.z() - 0.0 ) < 1.0e-9, "Failure z v2_2" );

    QVERIFY2( fabs( v3_2.x() - 0.0 ) < 1.0e-9, "Failure x v3_2" );
    QVERIFY2( fabs( v3_2.y() - 0.0 ) < 1.0e-9, "Failure y v3_2" );
    QVERIFY2( fabs( v3_2.z() - 2.0 ) < 1.0e-9, "Failure z v3_2" );

    QVERIFY2( fabs( v4_2.x() - 2.0 ) < 1.0e-9, "Failure x v4_2" );
    QVERIFY2( fabs( v4_2.y() - 4.0 ) < 1.0e-9, "Failure y v4_2" );
    QVERIFY2( fabs( v4_2.z() - 6.0 ) < 1.0e-9, "Failure z v4_2" );

    QVERIFY2( fabs( v1_3.x() - 3.0 ) < 1.0e-9, "Failure x v1_3" );
    QVERIFY2( fabs( v1_3.y() - 0.0 ) < 1.0e-9, "Failure y v1_3" );
    QVERIFY2( fabs( v1_3.z() - 0.0 ) < 1.0e-9, "Failure z v1_3" );

    QVERIFY2( fabs( v2_3.x() - 0.0 ) < 1.0e-9, "Failure x v2_3" );
    QVERIFY2( fabs( v2_3.y() - 3.0 ) < 1.0e-9, "Failure y v2_3" );
    QVERIFY2( fabs( v2_3.z() - 0.0 ) < 1.0e-9, "Failure z v2_3" );

    QVERIFY2( fabs( v3_3.x() - 0.0 ) < 1.0e-9, "Failure x v3_3" );
    QVERIFY2( fabs( v3_3.y() - 0.0 ) < 1.0e-9, "Failure y v3_3" );
    QVERIFY2( fabs( v3_3.z() - 3.0 ) < 1.0e-9, "Failure z v3_3" );

    QVERIFY2( fabs( v4_3.x() - 3.0 ) < 1.0e-9, "Failure x v4_3" );
    QVERIFY2( fabs( v4_3.y() - 6.0 ) < 1.0e-9, "Failure y v4_3" );
    QVERIFY2( fabs( v4_3.z() - 9.0 ) < 1.0e-9, "Failure z v4_3" );
}

////////////////////////////////////////////////////////////////////////////////

void Vector3Test::operator_divide_by_scalar()
{
    fdm::Vector3 v1( 1.0, 0.0, 0.0 );
    fdm::Vector3 v2( 0.0, 1.0, 0.0 );
    fdm::Vector3 v3( 0.0, 0.0, 1.0 );
    fdm::Vector3 v4( 1.0, 2.0, 3.0 );

    fdm::Vector3 v1_2 = v1 / 2.0;
    fdm::Vector3 v2_2 = v2 / 2.0;
    fdm::Vector3 v3_2 = v3 / 2.0;
    fdm::Vector3 v4_2 = v4 / 2.0;

    fdm::Vector3 v1_4 = v1 / 4.0;
    fdm::Vector3 v2_4 = v2 / 4.0;
    fdm::Vector3 v3_4 = v3 / 4.0;
    fdm::Vector3 v4_4 = v4 / 4.0;

    QVERIFY2( fabs( v1_2.x() - 0.5 ) < 1.0e-9, "Failure x v1_2" );
    QVERIFY2( fabs( v1_2.y() - 0.0 ) < 1.0e-9, "Failure y v1_2" );
    QVERIFY2( fabs( v1_2.z() - 0.0 ) < 1.0e-9, "Failure z v1_2" );

    QVERIFY2( fabs( v2_2.x() - 0.0 ) < 1.0e-9, "Failure x v2_2" );
    QVERIFY2( fabs( v2_2.y() - 0.5 ) < 1.0e-9, "Failure y v2_2" );
    QVERIFY2( fabs( v2_2.z() - 0.0 ) < 1.0e-9, "Failure z v2_2" );

    QVERIFY2( fabs( v3_2.x() - 0.0 ) < 1.0e-9, "Failure x v3_2" );
    QVERIFY2( fabs( v3_2.y() - 0.0 ) < 1.0e-9, "Failure y v3_2" );
    QVERIFY2( fabs( v3_2.z() - 0.5 ) < 1.0e-9, "Failure z v3_2" );

    QVERIFY2( fabs( v4_2.x() - 0.5 ) < 1.0e-9, "Failure x v4_2" );
    QVERIFY2( fabs( v4_2.y() - 1.0 ) < 1.0e-9, "Failure y v4_2" );
    QVERIFY2( fabs( v4_2.z() - 1.5 ) < 1.0e-9, "Failure z v4_2" );

    QVERIFY2( fabs( v1_4.x() - 0.25 ) < 1.0e-9, "Failure x v1_4" );
    QVERIFY2( fabs( v1_4.y() - 0.0  ) < 1.0e-9, "Failure y v1_4" );
    QVERIFY2( fabs( v1_4.z() - 0.0  ) < 1.0e-9, "Failure z v1_4" );

    QVERIFY2( fabs( v2_4.x() - 0.0  ) < 1.0e-9, "Failure x v2_4" );
    QVERIFY2( fabs( v2_4.y() - 0.25 ) < 1.0e-9, "Failure y v2_4" );
    QVERIFY2( fabs( v2_4.z() - 0.0  ) < 1.0e-9, "Failure z v2_4" );

    QVERIFY2( fabs( v3_4.x() - 0.0  ) < 1.0e-9, "Failure x v3_4" );
    QVERIFY2( fabs( v3_4.y() - 0.0  ) < 1.0e-9, "Failure y v3_4" );
    QVERIFY2( fabs( v3_4.z() - 0.25 ) < 1.0e-9, "Failure z v3_4" );

    QVERIFY2( fabs( v4_4.x() - 0.25 ) < 1.0e-9, "Failure x v4_4" );
    QVERIFY2( fabs( v4_4.y() - 0.5  ) < 1.0e-9, "Failure y v4_4" );
    QVERIFY2( fabs( v4_4.z() - 0.75 ) < 1.0e-9, "Failure z v4_4" );
}

////////////////////////////////////////////////////////////////////////////////

void Vector3Test::operator_vector_dot_product()
{
    fdm::Vector3 v1( 1.0, 0.0, 0.0 );
    fdm::Vector3 v2( 0.0, 1.0, 0.0 );
    fdm::Vector3 v3( 0.0, 0.0, 1.0 );
    fdm::Vector3 v4( 1.0, 2.0, 3.0 );

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

void Vector3Test::operator_vector_cross_product()
{
    fdm::Vector3 v1( 1.0, 0.0, 0.0 );
    fdm::Vector3 v2( 0.0, 1.0, 0.0 );
    fdm::Vector3 v3( 0.0, 0.0, 1.0 );
    fdm::Vector3 v4( 1.0, 2.0, 3.0 );

    fdm::Vector3 v41 = v4 % v1;
    fdm::Vector3 v42 = v4 % v2;
    fdm::Vector3 v43 = v4 % v3;

    fdm::Vector3 v14 = v1 % v4;
    fdm::Vector3 v24 = v2 % v4;
    fdm::Vector3 v34 = v3 % v4;

    QVERIFY2( fabs( v41.x() - 0.0 ) < 1.0e-9, "Failure x v41" );
    QVERIFY2( fabs( v41.y() - 3.0 ) < 1.0e-9, "Failure y v41" );
    QVERIFY2( fabs( v41.z() + 2.0 ) < 1.0e-9, "Failure z v41" );

    QVERIFY2( fabs( v42.x() + 3.0 ) < 1.0e-9, "Failure x v42" );
    QVERIFY2( fabs( v42.y() - 0.0 ) < 1.0e-9, "Failure y v42" );
    QVERIFY2( fabs( v42.z() - 1.0 ) < 1.0e-9, "Failure z v42" );

    QVERIFY2( fabs( v43.x() - 2.0 ) < 1.0e-9, "Failure x v43" );
    QVERIFY2( fabs( v43.y() + 1.0 ) < 1.0e-9, "Failure y v43" );
    QVERIFY2( fabs( v43.z() - 0.0 ) < 1.0e-9, "Failure z v43" );

    QVERIFY2( fabs( v14.x() - 0.0 ) < 1.0e-9, "Failure x v14" );
    QVERIFY2( fabs( v14.y() + 3.0 ) < 1.0e-9, "Failure y v14" );
    QVERIFY2( fabs( v14.z() - 2.0 ) < 1.0e-9, "Failure z v14" );

    QVERIFY2( fabs( v24.x() - 3.0 ) < 1.0e-9, "Failure x v24" );
    QVERIFY2( fabs( v24.y() - 0.0 ) < 1.0e-9, "Failure y v24" );
    QVERIFY2( fabs( v24.z() + 1.0 ) < 1.0e-9, "Failure z v24" );

    QVERIFY2( fabs( v34.x() + 2.0 ) < 1.0e-9, "Failure x v34" );
    QVERIFY2( fabs( v34.y() - 1.0 ) < 1.0e-9, "Failure y v34" );
    QVERIFY2( fabs( v34.z() - 0.0 ) < 1.0e-9, "Failure z v34" );
}

////////////////////////////////////////////////////////////////////////////////

void Vector3Test::operator_unary_add()
{
    fdm::Vector3 v0( 1.0, 2.0, 3.0 );
    fdm::Vector3 v1( 2.0, 3.0, 4.0 );

    v0 += v1;

    QVERIFY2( fabs( v0.x() - 3.0 ) < 1.0e-9, "Failure x v0" );
    QVERIFY2( fabs( v0.y() - 5.0 ) < 1.0e-9, "Failure y v0" );
    QVERIFY2( fabs( v0.z() - 7.0 ) < 1.0e-9, "Failure z v0" );
}

////////////////////////////////////////////////////////////////////////////////

void Vector3Test::operator_unary_substract()
{
    fdm::Vector3 v0( 3.0, 5.0, 7.0 );
    fdm::Vector3 v1( 2.0, 3.0, 4.0 );

    v0 -= v1;

    QVERIFY2( fabs( v0.x() - 1.0 ) < 1.0e-9, "Failure x v0" );
    QVERIFY2( fabs( v0.y() - 2.0 ) < 1.0e-9, "Failure y v0" );
    QVERIFY2( fabs( v0.z() - 3.0 ) < 1.0e-9, "Failure z v0" );
}

////////////////////////////////////////////////////////////////////////////////

void Vector3Test::operator_unary_multiply_by_scalar()
{
    fdm::Vector3 v0( 2.0, 4.0, 6.0 );

    v0 *= 0.5;

    QVERIFY2( fabs( v0.x() - 1.0 ) < 1.0e-9, "Failure x v0" );
    QVERIFY2( fabs( v0.y() - 2.0 ) < 1.0e-9, "Failure y v0" );
    QVERIFY2( fabs( v0.z() - 3.0 ) < 1.0e-9, "Failure z v0" );
}

////////////////////////////////////////////////////////////////////////////////

void Vector3Test::operator_unary_divide_by_scalar()
{
    fdm::Vector3 v0( 2.0, 4.0, 6.0 );

    v0 /= 2.0;

    QVERIFY2( fabs( v0.x() - 1.0 ) < 1.0e-9, "Failure x v0" );
    QVERIFY2( fabs( v0.y() - 2.0 ) < 1.0e-9, "Failure y v0" );
    QVERIFY2( fabs( v0.z() - 3.0 ) < 1.0e-9, "Failure z v0" );
}

////////////////////////////////////////////////////////////////////////////////

void Vector3Test::operator_unary_vector_cross_product()
{
    fdm::Vector3 v0( 1.0, 2.0, 3.0 );

    fdm::Vector3 v1( 1.0, 0.0, 0.0 );
    fdm::Vector3 v2( 0.0, 1.0, 0.0 );
    fdm::Vector3 v3( 0.0, 0.0, 1.0 );

    fdm::Vector3 vt;

    vt = v0;
    vt %= v1;

    QVERIFY2( fabs( vt.x() - 0.0 ) < 1.0e-9, "Failure x vt %= v1" );
    QVERIFY2( fabs( vt.y() - 3.0 ) < 1.0e-9, "Failure y vt %= v1" );
    QVERIFY2( fabs( vt.z() + 2.0 ) < 1.0e-9, "Failure z vt %= v1" );

    vt = v0;
    vt %= v2;

    QVERIFY2( fabs( vt.x() + 3.0 ) < 1.0e-9, "Failure x vt %= v2" );
    QVERIFY2( fabs( vt.y() - 0.0 ) < 1.0e-9, "Failure y vt %= v2" );
    QVERIFY2( fabs( vt.z() - 1.0 ) < 1.0e-9, "Failure z vt %= v2" );

    vt = v0;
    vt %= v3;

    QVERIFY2( fabs( vt.x() - 2.0 ) < 1.0e-9, "Failure x vt %= v3" );
    QVERIFY2( fabs( vt.y() + 1.0 ) < 1.0e-9, "Failure y vt %= v3" );
    QVERIFY2( fabs( vt.z() - 0.0 ) < 1.0e-9, "Failure z vt %= v3" );
}

////////////////////////////////////////////////////////////////////////////////

QTEST_APPLESS_MAIN(Vector3Test)

////////////////////////////////////////////////////////////////////////////////

#include "test_fdm_vector3.moc"
