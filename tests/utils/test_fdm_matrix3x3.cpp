#include <iostream>

#include <QString>
#include <QtTest>

#include <fdm/utils/fdm_Matrix3x3.h>

////////////////////////////////////////////////////////////////////////////////

using namespace std;

////////////////////////////////////////////////////////////////////////////////

class Matrix3x3Test : public QObject
{
    Q_OBJECT

public:

    Matrix3x3Test();

private Q_SLOTS:

    void initTestCase();
    void cleanupTestCase();

    void constructor();

    void xx();
    void xy();
    void xz();
    void yx();
    void yy();
    void yz();
    void zx();
    void zy();
    void zz();
    void transpose();
    void getAngles();
    void getQuaternion();
    void getTransposed();

    void operator_assign();
    void operator_add();
    void operator_substract();
    void operator_multiply_by_scalar();
    void operator_multiply_by_matrix();
    void operator_multiply_by_vector();
    void operator_divide_by_scalar();
    void operator_unary_add();
    void operator_unary_substract();
    void operator_unary_multiply_by_scalar();
    void operator_unary_divide_by_scalar();
};

////////////////////////////////////////////////////////////////////////////////

Matrix3x3Test::Matrix3x3Test() {}

////////////////////////////////////////////////////////////////////////////////

void Matrix3x3Test::initTestCase() {}

////////////////////////////////////////////////////////////////////////////////

void Matrix3x3Test::cleanupTestCase() {}

////////////////////////////////////////////////////////////////////////////////

void Matrix3x3Test::constructor()
{
    fdm::Matrix3x3 m;
    QVERIFY2( fabs( m.xx() - 0.0 ) < 1.0e-9, "Failure xx m" );
    QVERIFY2( fabs( m.xy() - 0.0 ) < 1.0e-9, "Failure xy m" );
    QVERIFY2( fabs( m.xz() - 0.0 ) < 1.0e-9, "Failure xz m" );
    QVERIFY2( fabs( m.yx() - 0.0 ) < 1.0e-9, "Failure yx m" );
    QVERIFY2( fabs( m.yy() - 0.0 ) < 1.0e-9, "Failure yy m" );
    QVERIFY2( fabs( m.yz() - 0.0 ) < 1.0e-9, "Failure yz m" );
    QVERIFY2( fabs( m.zx() - 0.0 ) < 1.0e-9, "Failure zx m" );
    QVERIFY2( fabs( m.zy() - 0.0 ) < 1.0e-9, "Failure zy m" );
    QVERIFY2( fabs( m.zz() - 0.0 ) < 1.0e-9, "Failure zz m" );

    fdm::Matrix3x3 m0( 0.0, 0.0, 0.0,
                       0.0, 0.0, 0.0,
                       0.0, 0.0, 0.0 );
    QVERIFY2( fabs( m0.xx() - 0.0 ) < 1.0e-9, "Failure xx m0" );
    QVERIFY2( fabs( m0.xy() - 0.0 ) < 1.0e-9, "Failure xy m0" );
    QVERIFY2( fabs( m0.xz() - 0.0 ) < 1.0e-9, "Failure xz m0" );
    QVERIFY2( fabs( m0.yx() - 0.0 ) < 1.0e-9, "Failure yx m0" );
    QVERIFY2( fabs( m0.yy() - 0.0 ) < 1.0e-9, "Failure yy m0" );
    QVERIFY2( fabs( m0.yz() - 0.0 ) < 1.0e-9, "Failure yz m0" );
    QVERIFY2( fabs( m0.zx() - 0.0 ) < 1.0e-9, "Failure zx m0" );
    QVERIFY2( fabs( m0.zy() - 0.0 ) < 1.0e-9, "Failure zy m0" );
    QVERIFY2( fabs( m0.zz() - 0.0 ) < 1.0e-9, "Failure zz m0" );

    fdm::Matrix3x3 m1( 1.0, 2.0, 3.0,
                       4.0, 5.0, 6.0,
                       7.0, 8.0, 9.0 );
    QVERIFY2( fabs( m1.xx() - 1.0 ) < 1.0e-9, "Failure xx m1" );
    QVERIFY2( fabs( m1.xy() - 2.0 ) < 1.0e-9, "Failure xy m1" );
    QVERIFY2( fabs( m1.xz() - 3.0 ) < 1.0e-9, "Failure xz m1" );
    QVERIFY2( fabs( m1.yx() - 4.0 ) < 1.0e-9, "Failure yx m1" );
    QVERIFY2( fabs( m1.yy() - 5.0 ) < 1.0e-9, "Failure yy m1" );
    QVERIFY2( fabs( m1.yz() - 6.0 ) < 1.0e-9, "Failure yz m1" );
    QVERIFY2( fabs( m1.zx() - 7.0 ) < 1.0e-9, "Failure zx m1" );
    QVERIFY2( fabs( m1.zy() - 8.0 ) < 1.0e-9, "Failure zy m1" );
    QVERIFY2( fabs( m1.zz() - 9.0 ) < 1.0e-9, "Failure zz m1" );

    double items[] = { 9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0 };

    fdm::Matrix3x3 m2( items );
    QVERIFY2( fabs( m2.xx() - 9.0 ) < 1.0e-9, "Failure xx m2" );
    QVERIFY2( fabs( m2.xy() - 8.0 ) < 1.0e-9, "Failure xy m2" );
    QVERIFY2( fabs( m2.xz() - 7.0 ) < 1.0e-9, "Failure xz m2" );
    QVERIFY2( fabs( m2.yx() - 6.0 ) < 1.0e-9, "Failure yx m2" );
    QVERIFY2( fabs( m2.yy() - 5.0 ) < 1.0e-9, "Failure yy m2" );
    QVERIFY2( fabs( m2.yz() - 4.0 ) < 1.0e-9, "Failure yz m2" );
    QVERIFY2( fabs( m2.zx() - 3.0 ) < 1.0e-9, "Failure zx m2" );
    QVERIFY2( fabs( m2.zy() - 2.0 ) < 1.0e-9, "Failure zy m2" );
    QVERIFY2( fabs( m2.zz() - 1.0 ) < 1.0e-9, "Failure zz m2" );

    fdm::Matrix3x3 m3( m2 );
    QVERIFY2( fabs( m3.xx() - 9.0 ) < 1.0e-9, "Failure xx m3" );
    QVERIFY2( fabs( m3.xy() - 8.0 ) < 1.0e-9, "Failure xy m3" );
    QVERIFY2( fabs( m3.xz() - 7.0 ) < 1.0e-9, "Failure xz m3" );
    QVERIFY2( fabs( m3.yx() - 6.0 ) < 1.0e-9, "Failure yx m3" );
    QVERIFY2( fabs( m3.yy() - 5.0 ) < 1.0e-9, "Failure yy m3" );
    QVERIFY2( fabs( m3.yz() - 4.0 ) < 1.0e-9, "Failure yz m3" );
    QVERIFY2( fabs( m3.zx() - 3.0 ) < 1.0e-9, "Failure zx m3" );
    QVERIFY2( fabs( m3.zy() - 2.0 ) < 1.0e-9, "Failure zy m3" );
    QVERIFY2( fabs( m3.zz() - 1.0 ) < 1.0e-9, "Failure zz m3" );

    fdm::Angles a0( 0.0, 0.0, 0.0 );
    fdm::Angles a1( M_PI, 0.0, 0.0 );
    fdm::Angles a2( 0.0, M_PI, 0.0 );
    fdm::Angles a3( 0.0, 0.0, M_PI );
    fdm::Angles a4( M_PI_2, M_PI_2, M_PI_2 );

    fdm::Matrix3x3 ma0( a0 );
    QVERIFY2( fabs( ma0.xx() - 1.0 ) < 1.0e-9, "Failure xx ma0" );
    QVERIFY2( fabs( ma0.xy() - 0.0 ) < 1.0e-9, "Failure xy ma0" );
    QVERIFY2( fabs( ma0.xz() - 0.0 ) < 1.0e-9, "Failure xz ma0" );
    QVERIFY2( fabs( ma0.yx() - 0.0 ) < 1.0e-9, "Failure yx ma0" );
    QVERIFY2( fabs( ma0.yy() - 1.0 ) < 1.0e-9, "Failure yy ma0" );
    QVERIFY2( fabs( ma0.yz() - 0.0 ) < 1.0e-9, "Failure yz ma0" );
    QVERIFY2( fabs( ma0.zx() - 0.0 ) < 1.0e-9, "Failure zx ma0" );
    QVERIFY2( fabs( ma0.zy() - 0.0 ) < 1.0e-9, "Failure zy ma0" );
    QVERIFY2( fabs( ma0.zz() - 1.0 ) < 1.0e-9, "Failure zz ma0" );

    fdm::Matrix3x3 ma1( a1 );
    QVERIFY2( fabs( ma1.xx() - 1.0 ) < 1.0e-9, "Failure xx ma1" );
    QVERIFY2( fabs( ma1.xy() - 0.0 ) < 1.0e-9, "Failure xy ma1" );
    QVERIFY2( fabs( ma1.xz() - 0.0 ) < 1.0e-9, "Failure xz ma1" );
    QVERIFY2( fabs( ma1.yx() - 0.0 ) < 1.0e-9, "Failure yx ma1" );
    QVERIFY2( fabs( ma1.yy() + 1.0 ) < 1.0e-9, "Failure yy ma1" );
    QVERIFY2( fabs( ma1.yz() - 0.0 ) < 1.0e-9, "Failure yz ma1" );
    QVERIFY2( fabs( ma1.zx() - 0.0 ) < 1.0e-9, "Failure zx ma1" );
    QVERIFY2( fabs( ma1.zy() - 0.0 ) < 1.0e-9, "Failure zy ma1" );
    QVERIFY2( fabs( ma1.zz() + 1.0 ) < 1.0e-9, "Failure zz ma1" );

    fdm::Matrix3x3 ma2( a2 );
    QVERIFY2( fabs( ma2.xx() + 1.0 ) < 1.0e-9, "Failure xx ma2" );
    QVERIFY2( fabs( ma2.xy() - 0.0 ) < 1.0e-9, "Failure xy ma2" );
    QVERIFY2( fabs( ma2.xz() - 0.0 ) < 1.0e-9, "Failure xz ma2" );
    QVERIFY2( fabs( ma2.yx() - 0.0 ) < 1.0e-9, "Failure yx ma2" );
    QVERIFY2( fabs( ma2.yy() - 1.0 ) < 1.0e-9, "Failure yy ma2" );
    QVERIFY2( fabs( ma2.yz() - 0.0 ) < 1.0e-9, "Failure yz ma2" );
    QVERIFY2( fabs( ma2.zx() - 0.0 ) < 1.0e-9, "Failure zx ma2" );
    QVERIFY2( fabs( ma2.zy() - 0.0 ) < 1.0e-9, "Failure zy ma2" );
    QVERIFY2( fabs( ma2.zz() + 1.0 ) < 1.0e-9, "Failure zz ma2" );

    fdm::Matrix3x3 ma3( a3 );
    QVERIFY2( fabs( ma3.xx() + 1.0 ) < 1.0e-9, "Failure xx ma3" );
    QVERIFY2( fabs( ma3.xy() - 0.0 ) < 1.0e-9, "Failure xy ma3" );
    QVERIFY2( fabs( ma3.xz() - 0.0 ) < 1.0e-9, "Failure xz ma3" );
    QVERIFY2( fabs( ma3.yx() - 0.0 ) < 1.0e-9, "Failure yx ma3" );
    QVERIFY2( fabs( ma3.yy() + 1.0 ) < 1.0e-9, "Failure yy ma3" );
    QVERIFY2( fabs( ma3.yz() - 0.0 ) < 1.0e-9, "Failure yz ma3" );
    QVERIFY2( fabs( ma3.zx() - 0.0 ) < 1.0e-9, "Failure zx ma3" );
    QVERIFY2( fabs( ma3.zy() - 0.0 ) < 1.0e-9, "Failure zy ma3" );
    QVERIFY2( fabs( ma3.zz() - 1.0 ) < 1.0e-9, "Failure zz ma3" );

    fdm::Matrix3x3 ma4( a4 );
    QVERIFY2( fabs( ma4.xx() - 0.0 ) < 1.0e-9, "Failure xx ma4" );
    QVERIFY2( fabs( ma4.xy() - 0.0 ) < 1.0e-9, "Failure xy ma4" );
    QVERIFY2( fabs( ma4.xz() + 1.0 ) < 1.0e-9, "Failure xz ma4" );
    QVERIFY2( fabs( ma4.yx() - 0.0 ) < 1.0e-9, "Failure yx ma4" );
    QVERIFY2( fabs( ma4.yy() - 1.0 ) < 1.0e-9, "Failure yy ma4" );
    QVERIFY2( fabs( ma4.yz() - 0.0 ) < 1.0e-9, "Failure yz ma4" );
    QVERIFY2( fabs( ma4.zx() - 1.0 ) < 1.0e-9, "Failure zx ma4" );
    QVERIFY2( fabs( ma4.zy() - 0.0 ) < 1.0e-9, "Failure zy ma4" );
    QVERIFY2( fabs( ma4.zz() - 0.0 ) < 1.0e-9, "Failure zz ma4" );

    fdm::Quaternion q0( a0 );
    fdm::Quaternion q1( a1 );
    fdm::Quaternion q2( a2 );
    fdm::Quaternion q3( a3 );
    fdm::Quaternion q4( a4 );

    fdm::Matrix3x3 mq0( q0 );
    QVERIFY2( fabs( mq0.xx() - 1.0 ) < 1.0e-9, "Failure xx mq0" );
    QVERIFY2( fabs( mq0.xy() - 0.0 ) < 1.0e-9, "Failure xy mq0" );
    QVERIFY2( fabs( mq0.xz() - 0.0 ) < 1.0e-9, "Failure xz mq0" );
    QVERIFY2( fabs( mq0.yx() - 0.0 ) < 1.0e-9, "Failure yx mq0" );
    QVERIFY2( fabs( mq0.yy() - 1.0 ) < 1.0e-9, "Failure yy mq0" );
    QVERIFY2( fabs( mq0.yz() - 0.0 ) < 1.0e-9, "Failure yz mq0" );
    QVERIFY2( fabs( mq0.zx() - 0.0 ) < 1.0e-9, "Failure zx mq0" );
    QVERIFY2( fabs( mq0.zy() - 0.0 ) < 1.0e-9, "Failure zy mq0" );
    QVERIFY2( fabs( mq0.zz() - 1.0 ) < 1.0e-9, "Failure zz mq0" );

    fdm::Matrix3x3 mq1( q1 );
    QVERIFY2( fabs( mq1.xx() - 1.0 ) < 1.0e-9, "Failure xx mq1" );
    QVERIFY2( fabs( mq1.xy() - 0.0 ) < 1.0e-9, "Failure xy mq1" );
    QVERIFY2( fabs( mq1.xz() - 0.0 ) < 1.0e-9, "Failure xz mq1" );
    QVERIFY2( fabs( mq1.yx() - 0.0 ) < 1.0e-9, "Failure yx mq1" );
    QVERIFY2( fabs( mq1.yy() + 1.0 ) < 1.0e-9, "Failure yy mq1" );
    QVERIFY2( fabs( mq1.yz() - 0.0 ) < 1.0e-9, "Failure yz mq1" );
    QVERIFY2( fabs( mq1.zx() - 0.0 ) < 1.0e-9, "Failure zx mq1" );
    QVERIFY2( fabs( mq1.zy() - 0.0 ) < 1.0e-9, "Failure zy mq1" );
    QVERIFY2( fabs( mq1.zz() + 1.0 ) < 1.0e-9, "Failure zz mq1" );

    fdm::Matrix3x3 mq2( q2 );
    QVERIFY2( fabs( mq2.xx() + 1.0 ) < 1.0e-9, "Failure xx mq2" );
    QVERIFY2( fabs( mq2.xy() - 0.0 ) < 1.0e-9, "Failure xy mq2" );
    QVERIFY2( fabs( mq2.xz() - 0.0 ) < 1.0e-9, "Failure xz mq2" );
    QVERIFY2( fabs( mq2.yx() - 0.0 ) < 1.0e-9, "Failure yx mq2" );
    QVERIFY2( fabs( mq2.yy() - 1.0 ) < 1.0e-9, "Failure yy mq2" );
    QVERIFY2( fabs( mq2.yz() - 0.0 ) < 1.0e-9, "Failure yz mq2" );
    QVERIFY2( fabs( mq2.zx() - 0.0 ) < 1.0e-9, "Failure zx mq2" );
    QVERIFY2( fabs( mq2.zy() - 0.0 ) < 1.0e-9, "Failure zy mq2" );
    QVERIFY2( fabs( mq2.zz() + 1.0 ) < 1.0e-9, "Failure zz mq2" );

    fdm::Matrix3x3 mq3( q3 );
    QVERIFY2( fabs( mq3.xx() + 1.0 ) < 1.0e-9, "Failure xx mq3" );
    QVERIFY2( fabs( mq3.xy() - 0.0 ) < 1.0e-9, "Failure xy mq3" );
    QVERIFY2( fabs( mq3.xz() - 0.0 ) < 1.0e-9, "Failure xz mq3" );
    QVERIFY2( fabs( mq3.yx() - 0.0 ) < 1.0e-9, "Failure yx mq3" );
    QVERIFY2( fabs( mq3.yy() + 1.0 ) < 1.0e-9, "Failure yy mq3" );
    QVERIFY2( fabs( mq3.yz() - 0.0 ) < 1.0e-9, "Failure yz mq3" );
    QVERIFY2( fabs( mq3.zx() - 0.0 ) < 1.0e-9, "Failure zx mq3" );
    QVERIFY2( fabs( mq3.zy() - 0.0 ) < 1.0e-9, "Failure zy mq3" );
    QVERIFY2( fabs( mq3.zz() - 1.0 ) < 1.0e-9, "Failure zz mq3" );

    fdm::Matrix3x3 mq4( q4 );
    QVERIFY2( fabs( mq4.xx() - 0.0 ) < 1.0e-9, "Failure xx mq4" );
    QVERIFY2( fabs( mq4.xy() - 0.0 ) < 1.0e-9, "Failure xy mq4" );
    QVERIFY2( fabs( mq4.xz() + 1.0 ) < 1.0e-9, "Failure xz mq4" );
    QVERIFY2( fabs( mq4.yx() - 0.0 ) < 1.0e-9, "Failure yx mq4" );
    QVERIFY2( fabs( mq4.yy() - 1.0 ) < 1.0e-9, "Failure yy mq4" );
    QVERIFY2( fabs( mq4.yz() - 0.0 ) < 1.0e-9, "Failure yz mq4" );
    QVERIFY2( fabs( mq4.zx() - 1.0 ) < 1.0e-9, "Failure zx mq4" );
    QVERIFY2( fabs( mq4.zy() - 0.0 ) < 1.0e-9, "Failure zy mq4" );
    QVERIFY2( fabs( mq4.zz() - 0.0 ) < 1.0e-9, "Failure zz mq4" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix3x3Test::xx()
{
    fdm::Matrix3x3 m0;

    m0.xx() = 1.0;

    QVERIFY2( fabs( m0.xx() - 1.0 ) < 1.0e-9, "Failure xx m0" );
    QVERIFY2( fabs( m0.xy() - 0.0 ) < 1.0e-9, "Failure xy m0" );
    QVERIFY2( fabs( m0.xz() - 0.0 ) < 1.0e-9, "Failure xz m0" );
    QVERIFY2( fabs( m0.yx() - 0.0 ) < 1.0e-9, "Failure yx m0" );
    QVERIFY2( fabs( m0.yy() - 0.0 ) < 1.0e-9, "Failure yy m0" );
    QVERIFY2( fabs( m0.yz() - 0.0 ) < 1.0e-9, "Failure yz m0" );
    QVERIFY2( fabs( m0.zx() - 0.0 ) < 1.0e-9, "Failure zx m0" );
    QVERIFY2( fabs( m0.zy() - 0.0 ) < 1.0e-9, "Failure zy m0" );
    QVERIFY2( fabs( m0.zz() - 0.0 ) < 1.0e-9, "Failure zz m0" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix3x3Test::xy()
{
    fdm::Matrix3x3 m0;

    m0.xy() = 1.0;

    QVERIFY2( fabs( m0.xx() - 0.0 ) < 1.0e-9, "Failure xx m0" );
    QVERIFY2( fabs( m0.xy() - 1.0 ) < 1.0e-9, "Failure xy m0" );
    QVERIFY2( fabs( m0.xz() - 0.0 ) < 1.0e-9, "Failure xz m0" );
    QVERIFY2( fabs( m0.yx() - 0.0 ) < 1.0e-9, "Failure yx m0" );
    QVERIFY2( fabs( m0.yy() - 0.0 ) < 1.0e-9, "Failure yy m0" );
    QVERIFY2( fabs( m0.yz() - 0.0 ) < 1.0e-9, "Failure yz m0" );
    QVERIFY2( fabs( m0.zx() - 0.0 ) < 1.0e-9, "Failure zx m0" );
    QVERIFY2( fabs( m0.zy() - 0.0 ) < 1.0e-9, "Failure zy m0" );
    QVERIFY2( fabs( m0.zz() - 0.0 ) < 1.0e-9, "Failure zz m0" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix3x3Test::xz()
{
    fdm::Matrix3x3 m0;

    m0.xz() = 1.0;

    QVERIFY2( fabs( m0.xx() - 0.0 ) < 1.0e-9, "Failure xx m0" );
    QVERIFY2( fabs( m0.xy() - 0.0 ) < 1.0e-9, "Failure xy m0" );
    QVERIFY2( fabs( m0.xz() - 1.0 ) < 1.0e-9, "Failure xz m0" );
    QVERIFY2( fabs( m0.yx() - 0.0 ) < 1.0e-9, "Failure yx m0" );
    QVERIFY2( fabs( m0.yy() - 0.0 ) < 1.0e-9, "Failure yy m0" );
    QVERIFY2( fabs( m0.yz() - 0.0 ) < 1.0e-9, "Failure yz m0" );
    QVERIFY2( fabs( m0.zx() - 0.0 ) < 1.0e-9, "Failure zx m0" );
    QVERIFY2( fabs( m0.zy() - 0.0 ) < 1.0e-9, "Failure zy m0" );
    QVERIFY2( fabs( m0.zz() - 0.0 ) < 1.0e-9, "Failure zz m0" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix3x3Test::yx()
{
    fdm::Matrix3x3 m0;

    m0.yx() = 1.0;

    QVERIFY2( fabs( m0.xx() - 0.0 ) < 1.0e-9, "Failure xx m0" );
    QVERIFY2( fabs( m0.xy() - 0.0 ) < 1.0e-9, "Failure xy m0" );
    QVERIFY2( fabs( m0.xz() - 0.0 ) < 1.0e-9, "Failure xz m0" );
    QVERIFY2( fabs( m0.yx() - 1.0 ) < 1.0e-9, "Failure yx m0" );
    QVERIFY2( fabs( m0.yy() - 0.0 ) < 1.0e-9, "Failure yy m0" );
    QVERIFY2( fabs( m0.yz() - 0.0 ) < 1.0e-9, "Failure yz m0" );
    QVERIFY2( fabs( m0.zx() - 0.0 ) < 1.0e-9, "Failure zx m0" );
    QVERIFY2( fabs( m0.zy() - 0.0 ) < 1.0e-9, "Failure zy m0" );
    QVERIFY2( fabs( m0.zz() - 0.0 ) < 1.0e-9, "Failure zz m0" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix3x3Test::yy()
{
    fdm::Matrix3x3 m0;

    m0.yy() = 1.0;

    QVERIFY2( fabs( m0.xx() - 0.0 ) < 1.0e-9, "Failure xx m0" );
    QVERIFY2( fabs( m0.xy() - 0.0 ) < 1.0e-9, "Failure xy m0" );
    QVERIFY2( fabs( m0.xz() - 0.0 ) < 1.0e-9, "Failure xz m0" );
    QVERIFY2( fabs( m0.yx() - 0.0 ) < 1.0e-9, "Failure yx m0" );
    QVERIFY2( fabs( m0.yy() - 1.0 ) < 1.0e-9, "Failure yy m0" );
    QVERIFY2( fabs( m0.yz() - 0.0 ) < 1.0e-9, "Failure yz m0" );
    QVERIFY2( fabs( m0.zx() - 0.0 ) < 1.0e-9, "Failure zx m0" );
    QVERIFY2( fabs( m0.zy() - 0.0 ) < 1.0e-9, "Failure zy m0" );
    QVERIFY2( fabs( m0.zz() - 0.0 ) < 1.0e-9, "Failure zz m0" );
}

////////////////////////////////////////////////////////////////////////////////


void Matrix3x3Test::yz()
{
    fdm::Matrix3x3 m0;

    m0.yz() = 1.0;

    QVERIFY2( fabs( m0.xx() - 0.0 ) < 1.0e-9, "Failure xx m0" );
    QVERIFY2( fabs( m0.xy() - 0.0 ) < 1.0e-9, "Failure xy m0" );
    QVERIFY2( fabs( m0.xz() - 0.0 ) < 1.0e-9, "Failure xz m0" );
    QVERIFY2( fabs( m0.yx() - 0.0 ) < 1.0e-9, "Failure yx m0" );
    QVERIFY2( fabs( m0.yy() - 0.0 ) < 1.0e-9, "Failure yy m0" );
    QVERIFY2( fabs( m0.yz() - 1.0 ) < 1.0e-9, "Failure yz m0" );
    QVERIFY2( fabs( m0.zx() - 0.0 ) < 1.0e-9, "Failure zx m0" );
    QVERIFY2( fabs( m0.zy() - 0.0 ) < 1.0e-9, "Failure zy m0" );
    QVERIFY2( fabs( m0.zz() - 0.0 ) < 1.0e-9, "Failure zz m0" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix3x3Test::zx()
{
    fdm::Matrix3x3 m0;

    m0.zx() = 1.0;

    QVERIFY2( fabs( m0.xx() - 0.0 ) < 1.0e-9, "Failure xx m0" );
    QVERIFY2( fabs( m0.xy() - 0.0 ) < 1.0e-9, "Failure xy m0" );
    QVERIFY2( fabs( m0.xz() - 0.0 ) < 1.0e-9, "Failure xz m0" );
    QVERIFY2( fabs( m0.yx() - 0.0 ) < 1.0e-9, "Failure yx m0" );
    QVERIFY2( fabs( m0.yy() - 0.0 ) < 1.0e-9, "Failure yy m0" );
    QVERIFY2( fabs( m0.yz() - 0.0 ) < 1.0e-9, "Failure yz m0" );
    QVERIFY2( fabs( m0.zx() - 1.0 ) < 1.0e-9, "Failure zx m0" );
    QVERIFY2( fabs( m0.zy() - 0.0 ) < 1.0e-9, "Failure zy m0" );
    QVERIFY2( fabs( m0.zz() - 0.0 ) < 1.0e-9, "Failure zz m0" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix3x3Test::zy()
{
    fdm::Matrix3x3 m0;

    m0.zy() = 1.0;

    QVERIFY2( fabs( m0.xx() - 0.0 ) < 1.0e-9, "Failure xx m0" );
    QVERIFY2( fabs( m0.xy() - 0.0 ) < 1.0e-9, "Failure xy m0" );
    QVERIFY2( fabs( m0.xz() - 0.0 ) < 1.0e-9, "Failure xz m0" );
    QVERIFY2( fabs( m0.yx() - 0.0 ) < 1.0e-9, "Failure yx m0" );
    QVERIFY2( fabs( m0.yy() - 0.0 ) < 1.0e-9, "Failure yy m0" );
    QVERIFY2( fabs( m0.yz() - 0.0 ) < 1.0e-9, "Failure yz m0" );
    QVERIFY2( fabs( m0.zx() - 0.0 ) < 1.0e-9, "Failure zx m0" );
    QVERIFY2( fabs( m0.zy() - 1.0 ) < 1.0e-9, "Failure zy m0" );
    QVERIFY2( fabs( m0.zz() - 0.0 ) < 1.0e-9, "Failure zz m0" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix3x3Test::zz()
{
    fdm::Matrix3x3 m0;

    m0.zz() = 1.0;

    QVERIFY2( fabs( m0.xx() - 0.0 ) < 1.0e-9, "Failure xx m0" );
    QVERIFY2( fabs( m0.xy() - 0.0 ) < 1.0e-9, "Failure xy m0" );
    QVERIFY2( fabs( m0.xz() - 0.0 ) < 1.0e-9, "Failure xz m0" );
    QVERIFY2( fabs( m0.yx() - 0.0 ) < 1.0e-9, "Failure yx m0" );
    QVERIFY2( fabs( m0.yy() - 0.0 ) < 1.0e-9, "Failure yy m0" );
    QVERIFY2( fabs( m0.yz() - 0.0 ) < 1.0e-9, "Failure yz m0" );
    QVERIFY2( fabs( m0.zx() - 0.0 ) < 1.0e-9, "Failure zx m0" );
    QVERIFY2( fabs( m0.zy() - 0.0 ) < 1.0e-9, "Failure zy m0" );
    QVERIFY2( fabs( m0.zz() - 1.0 ) < 1.0e-9, "Failure zz m0" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix3x3Test::transpose()
{
    fdm::Matrix3x3 m1( 1.0, 2.0, 3.0,
                       4.0, 5.0, 6.0,
                       7.0, 8.0, 9.0 );
    m1.transpose();

    QVERIFY2( fabs( m1.xx() - 1.0 ) < 1.0e-9, "Failure xx m1" );
    QVERIFY2( fabs( m1.xy() - 4.0 ) < 1.0e-9, "Failure xy m1" );
    QVERIFY2( fabs( m1.xz() - 7.0 ) < 1.0e-9, "Failure xz m1" );
    QVERIFY2( fabs( m1.yx() - 2.0 ) < 1.0e-9, "Failure yx m1" );
    QVERIFY2( fabs( m1.yy() - 5.0 ) < 1.0e-9, "Failure yy m1" );
    QVERIFY2( fabs( m1.yz() - 8.0 ) < 1.0e-9, "Failure yz m1" );
    QVERIFY2( fabs( m1.zx() - 3.0 ) < 1.0e-9, "Failure zx m1" );
    QVERIFY2( fabs( m1.zy() - 6.0 ) < 1.0e-9, "Failure zy m1" );
    QVERIFY2( fabs( m1.zz() - 9.0 ) < 1.0e-9, "Failure zz m1" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix3x3Test::getAngles()
{
    fdm::Matrix3x3 ma0( fdm::Angles( 0.0, 0.0, 0.0 ) );
    fdm::Matrix3x3 ma1( fdm::Angles( M_PI, 0.0, 0.0 ) );
    fdm::Matrix3x3 ma3( fdm::Angles( 0.0, 0.0, M_PI ) );

    fdm::Angles a0 =  ma0.getAngles();
    fdm::Angles a1 =  ma1.getAngles();
    fdm::Angles a3 =  ma3.getAngles();

    QVERIFY2( fabs( a0.phi() - 0.0 ) < 1.0e-9, "Failure phi a0" );
    QVERIFY2( fabs( a0.tht() - 0.0 ) < 1.0e-9, "Failure tht a0" );
    QVERIFY2( fabs( a0.psi() - 0.0 ) < 1.0e-9, "Failure psi a0" );

    QVERIFY2( fabs( a1.phi() - M_PI ) < 1.0e-9, "Failure phi a1" );
    QVERIFY2( fabs( a1.tht() - 0.0  ) < 1.0e-9, "Failure tht a1" );
    QVERIFY2( fabs( a1.psi() - 0.0  ) < 1.0e-9, "Failure psi a1" );

    QVERIFY2( fabs( a3.phi() - 0.0  ) < 1.0e-9, "Failure phi a3" );
    QVERIFY2( fabs( a3.tht() - 0.0  ) < 1.0e-9, "Failure tht a3" );
    QVERIFY2( fabs( a3.psi() - M_PI ) < 1.0e-9, "Failure psi a3" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix3x3Test::getQuaternion()
{
    fdm::Matrix3x3 mq0( fdm::Quaternion::createZeroRotationQuaternion() );
    fdm::Matrix3x3 mq1( fdm::Quaternion( 0.6, 0.0, 0.0, 0.8 ) );

    fdm::Quaternion q0 =  mq0.getQuaternion();
    fdm::Quaternion q1 =  mq1.getQuaternion();

    QVERIFY2( fabs( q0.e0() - 1.0 ) < 1.0e-9, "Failure e0 q0" );
    QVERIFY2( fabs( q0.ex() - 0.0 ) < 1.0e-9, "Failure ex q0" );
    QVERIFY2( fabs( q0.ey() - 0.0 ) < 1.0e-9, "Failure ey q0" );
    QVERIFY2( fabs( q0.ez() - 0.0 ) < 1.0e-9, "Failure ez q0" );

    QVERIFY2( fabs( q1.e0() - 0.6 ) < 1.0e-9, "Failure e0 q1" );
    QVERIFY2( fabs( q1.ex() - 0.0 ) < 1.0e-9, "Failure ex q1" );
    QVERIFY2( fabs( q1.ey() - 0.0 ) < 1.0e-9, "Failure ey q1" );
    QVERIFY2( fabs( q1.ez() - 0.8 ) < 1.0e-9, "Failure ez q1" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix3x3Test::getTransposed()
{
    fdm::Matrix3x3 m1( 1.0, 2.0, 3.0,
                       4.0, 5.0, 6.0,
                       7.0, 8.0, 9.0 );
    fdm::Matrix3x3 mt = m1.getTransposed();

    QVERIFY2( fabs( mt.xx() - 1.0 ) < 1.0e-9, "Failure xx mt" );
    QVERIFY2( fabs( mt.xy() - 4.0 ) < 1.0e-9, "Failure xy mt" );
    QVERIFY2( fabs( mt.xz() - 7.0 ) < 1.0e-9, "Failure xz mt" );
    QVERIFY2( fabs( mt.yx() - 2.0 ) < 1.0e-9, "Failure yx mt" );
    QVERIFY2( fabs( mt.yy() - 5.0 ) < 1.0e-9, "Failure yy mt" );
    QVERIFY2( fabs( mt.yz() - 8.0 ) < 1.0e-9, "Failure yz mt" );
    QVERIFY2( fabs( mt.zx() - 3.0 ) < 1.0e-9, "Failure zx mt" );
    QVERIFY2( fabs( mt.zy() - 6.0 ) < 1.0e-9, "Failure zy mt" );
    QVERIFY2( fabs( mt.zz() - 9.0 ) < 1.0e-9, "Failure zz mt" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix3x3Test::operator_assign()
{
    fdm::Matrix3x3 m1( 1.0, 2.0, 3.0,
                       4.0, 5.0, 6.0,
                       7.0, 8.0, 9.0 );
    fdm::Matrix3x3 m0 = m1;

    QVERIFY2( fabs( m0.xx() - 1.0 ) < 1.0e-9, "Failure xx m0" );
    QVERIFY2( fabs( m0.xy() - 2.0 ) < 1.0e-9, "Failure xy m0" );
    QVERIFY2( fabs( m0.xz() - 3.0 ) < 1.0e-9, "Failure xz m0" );
    QVERIFY2( fabs( m0.yx() - 4.0 ) < 1.0e-9, "Failure yx m0" );
    QVERIFY2( fabs( m0.yy() - 5.0 ) < 1.0e-9, "Failure yy m0" );
    QVERIFY2( fabs( m0.yz() - 6.0 ) < 1.0e-9, "Failure yz m0" );
    QVERIFY2( fabs( m0.zx() - 7.0 ) < 1.0e-9, "Failure zx m0" );
    QVERIFY2( fabs( m0.zy() - 8.0 ) < 1.0e-9, "Failure zy m0" );
    QVERIFY2( fabs( m0.zz() - 9.0 ) < 1.0e-9, "Failure zz m0" );
}


////////////////////////////////////////////////////////////////////////////////

void Matrix3x3Test::operator_add()
{
    fdm::Matrix3x3 m1( 1.0, 2.0, 3.0,
                       4.0, 5.0, 6.0,
                       7.0, 8.0, 9.0 );

    fdm::Matrix3x3 m2( 1.0, 1.0, 1.0,
                       1.0, 1.0, 1.0,
                       1.0, 1.0, 1.0 );

    fdm::Matrix3x3 m0 = m1 + m2;

    QVERIFY2( fabs( m0.xx() -  2.0 ) < 1.0e-9, "Failure xx m0" );
    QVERIFY2( fabs( m0.xy() -  3.0 ) < 1.0e-9, "Failure xy m0" );
    QVERIFY2( fabs( m0.xz() -  4.0 ) < 1.0e-9, "Failure xz m0" );
    QVERIFY2( fabs( m0.yx() -  5.0 ) < 1.0e-9, "Failure yx m0" );
    QVERIFY2( fabs( m0.yy() -  6.0 ) < 1.0e-9, "Failure yy m0" );
    QVERIFY2( fabs( m0.yz() -  7.0 ) < 1.0e-9, "Failure yz m0" );
    QVERIFY2( fabs( m0.zx() -  8.0 ) < 1.0e-9, "Failure zx m0" );
    QVERIFY2( fabs( m0.zy() -  9.0 ) < 1.0e-9, "Failure zy m0" );
    QVERIFY2( fabs( m0.zz() - 10.0 ) < 1.0e-9, "Failure zz m0" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix3x3Test::operator_substract()
{
    fdm::Matrix3x3 m1( 1.0, 2.0, 3.0,
                       4.0, 5.0, 6.0,
                       7.0, 8.0, 9.0 );

    fdm::Matrix3x3 m2( 1.0, 1.0, 1.0,
                       1.0, 1.0, 1.0,
                       1.0, 1.0, 1.0 );

    fdm::Matrix3x3 m0 = m1 - m2;

    QVERIFY2( fabs( m0.xx() - 0.0 ) < 1.0e-9, "Failure xx m0" );
    QVERIFY2( fabs( m0.xy() - 1.0 ) < 1.0e-9, "Failure xy m0" );
    QVERIFY2( fabs( m0.xz() - 2.0 ) < 1.0e-9, "Failure xz m0" );
    QVERIFY2( fabs( m0.yx() - 3.0 ) < 1.0e-9, "Failure yx m0" );
    QVERIFY2( fabs( m0.yy() - 4.0 ) < 1.0e-9, "Failure yy m0" );
    QVERIFY2( fabs( m0.yz() - 5.0 ) < 1.0e-9, "Failure yz m0" );
    QVERIFY2( fabs( m0.zx() - 6.0 ) < 1.0e-9, "Failure zx m0" );
    QVERIFY2( fabs( m0.zy() - 7.0 ) < 1.0e-9, "Failure zy m0" );
    QVERIFY2( fabs( m0.zz() - 8.0 ) < 1.0e-9, "Failure zz m0" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix3x3Test::operator_multiply_by_scalar()
{
    fdm::Matrix3x3 m1( 1.0, 2.0, 3.0,
                       4.0, 5.0, 6.0,
                       7.0, 8.0, 9.0 );

    fdm::Matrix3x3 m0 = m1 * 0.5;

    QVERIFY2( fabs( m0.xx() - 0.5 ) < 1.0e-9, "Failure xx m0" );
    QVERIFY2( fabs( m0.xy() - 1.0 ) < 1.0e-9, "Failure xy m0" );
    QVERIFY2( fabs( m0.xz() - 1.5 ) < 1.0e-9, "Failure xz m0" );
    QVERIFY2( fabs( m0.yx() - 2.0 ) < 1.0e-9, "Failure yx m0" );
    QVERIFY2( fabs( m0.yy() - 2.5 ) < 1.0e-9, "Failure yy m0" );
    QVERIFY2( fabs( m0.yz() - 3.0 ) < 1.0e-9, "Failure yz m0" );
    QVERIFY2( fabs( m0.zx() - 3.5 ) < 1.0e-9, "Failure zx m0" );
    QVERIFY2( fabs( m0.zy() - 4.0 ) < 1.0e-9, "Failure zy m0" );
    QVERIFY2( fabs( m0.zz() - 4.5 ) < 1.0e-9, "Failure zz m0" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix3x3Test::operator_multiply_by_matrix()
{
    fdm::Matrix3x3 m1( 1.0, 2.0, 3.0,
                       4.0, 5.0, 6.0,
                       7.0, 8.0, 9.0 );

    fdm::Matrix3x3 m2( 1.1, 2.2, 3.3,
                       4.4, 5.5, 6.6,
                       7.7, 8.8, 9.9 );

    fdm::Matrix3x3 m0 = m1 * m2;

    QVERIFY2( fabs( m0.xx() -  33.0 ) < 1.0e-9, "Failure xx m0" );
    QVERIFY2( fabs( m0.xy() -  39.6 ) < 1.0e-9, "Failure xy m0" );
    QVERIFY2( fabs( m0.xz() -  46.2 ) < 1.0e-9, "Failure xz m0" );
    QVERIFY2( fabs( m0.yx() -  72.6 ) < 1.0e-9, "Failure yx m0" );
    QVERIFY2( fabs( m0.yy() -  89.1 ) < 1.0e-9, "Failure yy m0" );
    QVERIFY2( fabs( m0.yz() - 105.6 ) < 1.0e-9, "Failure yz m0" );
    QVERIFY2( fabs( m0.zx() - 112.2 ) < 1.0e-9, "Failure zx m0" );
    QVERIFY2( fabs( m0.zy() - 138.6 ) < 1.0e-9, "Failure zy m0" );
    QVERIFY2( fabs( m0.zz() - 165.0 ) < 1.0e-9, "Failure zz m0" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix3x3Test::operator_multiply_by_vector()
{
    fdm::Matrix3x3 m1( 1.0, 0.0, 0.0,
                       0.0, 1.0, 0.0,
                       0.0, 0.0, 1.0 );

    fdm::Matrix3x3 m2( -1.0, 0.0, 0.0,
                       0.0, -1.0, 0.0,
                       0.0, 0.0, -1.0 );

    fdm::Matrix3x3 m3( 0.0, 1.0, 0.0,
                       1.0, 0.0, 0.0,
                       0.0, 0.0, 1.0 );

    fdm::Matrix3x3 m4( 1.0, 0.0, 0.0,
                       0.0, 0.0, -1.0,
                       0.0, -1.0, 0.0 );

    fdm::Vector3 v( 1.0, 2.0, 3.0 );

    fdm::Vector3 v1 = m1 * v;
    QVERIFY2( fabs( v1.x() - 1.0 ) < 1.0e-9, "Failure x v1" );
    QVERIFY2( fabs( v1.y() - 2.0 ) < 1.0e-9, "Failure y v1" );
    QVERIFY2( fabs( v1.z() - 3.0 ) < 1.0e-9, "Failure z v1" );

    fdm::Vector3 v2 = m2 * v;
    QVERIFY2( fabs( v2.x() + 1.0 ) < 1.0e-9, "Failure x v2" );
    QVERIFY2( fabs( v2.y() + 2.0 ) < 1.0e-9, "Failure y v2" );
    QVERIFY2( fabs( v2.z() + 3.0 ) < 1.0e-9, "Failure z v2" );

    fdm::Vector3 v3 = m3 * v;
    QVERIFY2( fabs( v3.x() - 2.0 ) < 1.0e-9, "Failure x v3" );
    QVERIFY2( fabs( v3.y() - 1.0 ) < 1.0e-9, "Failure y v3" );
    QVERIFY2( fabs( v3.z() - 3.0 ) < 1.0e-9, "Failure z v3" );

    fdm::Vector3 v4 = m4 * v;
    QVERIFY2( fabs( v4.x() - 1.0 ) < 1.0e-9, "Failure x v4" );
    QVERIFY2( fabs( v4.y() + 3.0 ) < 1.0e-9, "Failure y v4" );
    QVERIFY2( fabs( v4.z() + 2.0 ) < 1.0e-9, "Failure z v4" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix3x3Test::operator_divide_by_scalar()
{
    fdm::Matrix3x3 m1( 1.0, 2.0, 3.0,
                       4.0, 5.0, 6.0,
                       7.0, 8.0, 9.0 );

    fdm::Matrix3x3 m0 = m1 / 2.0;

    QVERIFY2( fabs( m0.xx() - 0.5 ) < 1.0e-9, "Failure xx m0" );
    QVERIFY2( fabs( m0.xy() - 1.0 ) < 1.0e-9, "Failure xy m0" );
    QVERIFY2( fabs( m0.xz() - 1.5 ) < 1.0e-9, "Failure xz m0" );
    QVERIFY2( fabs( m0.yx() - 2.0 ) < 1.0e-9, "Failure yx m0" );
    QVERIFY2( fabs( m0.yy() - 2.5 ) < 1.0e-9, "Failure yy m0" );
    QVERIFY2( fabs( m0.yz() - 3.0 ) < 1.0e-9, "Failure yz m0" );
    QVERIFY2( fabs( m0.zx() - 3.5 ) < 1.0e-9, "Failure zx m0" );
    QVERIFY2( fabs( m0.zy() - 4.0 ) < 1.0e-9, "Failure zy m0" );
    QVERIFY2( fabs( m0.zz() - 4.5 ) < 1.0e-9, "Failure zz m0" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix3x3Test::operator_unary_add()
{
    fdm::Matrix3x3 m1( 1.0, 2.0, 3.0,
                       4.0, 5.0, 6.0,
                       7.0, 8.0, 9.0 );

    fdm::Matrix3x3 m2( 1.0, 1.0, 1.0,
                       1.0, 1.0, 1.0,
                       1.0, 1.0, 1.0 );

    m1 += m2;

    QVERIFY2( fabs( m1.xx() -  2.0 ) < 1.0e-9, "Failure xx m1" );
    QVERIFY2( fabs( m1.xy() -  3.0 ) < 1.0e-9, "Failure xy m1" );
    QVERIFY2( fabs( m1.xz() -  4.0 ) < 1.0e-9, "Failure xz m1" );
    QVERIFY2( fabs( m1.yx() -  5.0 ) < 1.0e-9, "Failure yx m1" );
    QVERIFY2( fabs( m1.yy() -  6.0 ) < 1.0e-9, "Failure yy m1" );
    QVERIFY2( fabs( m1.yz() -  7.0 ) < 1.0e-9, "Failure yz m1" );
    QVERIFY2( fabs( m1.zx() -  8.0 ) < 1.0e-9, "Failure zx m1" );
    QVERIFY2( fabs( m1.zy() -  9.0 ) < 1.0e-9, "Failure zy m1" );
    QVERIFY2( fabs( m1.zz() - 10.0 ) < 1.0e-9, "Failure zz m1" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix3x3Test::operator_unary_substract()
{
    fdm::Matrix3x3 m1( 1.0, 2.0, 3.0,
                       4.0, 5.0, 6.0,
                       7.0, 8.0, 9.0 );

    fdm::Matrix3x3 m2( 1.0, 1.0, 1.0,
                       1.0, 1.0, 1.0,
                       1.0, 1.0, 1.0 );

    m1 -= m2;

    QVERIFY2( fabs( m1.xx() - 0.0 ) < 1.0e-9, "Failure xx m1" );
    QVERIFY2( fabs( m1.xy() - 1.0 ) < 1.0e-9, "Failure xy m1" );
    QVERIFY2( fabs( m1.xz() - 2.0 ) < 1.0e-9, "Failure xz m1" );
    QVERIFY2( fabs( m1.yx() - 3.0 ) < 1.0e-9, "Failure yx m1" );
    QVERIFY2( fabs( m1.yy() - 4.0 ) < 1.0e-9, "Failure yy m1" );
    QVERIFY2( fabs( m1.yz() - 5.0 ) < 1.0e-9, "Failure yz m1" );
    QVERIFY2( fabs( m1.zx() - 6.0 ) < 1.0e-9, "Failure zx m1" );
    QVERIFY2( fabs( m1.zy() - 7.0 ) < 1.0e-9, "Failure zy m1" );
    QVERIFY2( fabs( m1.zz() - 8.0 ) < 1.0e-9, "Failure zz m1" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix3x3Test::operator_unary_multiply_by_scalar()
{
    fdm::Matrix3x3 m1( 1.0, 2.0, 3.0,
                       4.0, 5.0, 6.0,
                       7.0, 8.0, 9.0 );

    m1 *= 0.5;

    QVERIFY2( fabs( m1.xx() - 0.5 ) < 1.0e-9, "Failure xx m1" );
    QVERIFY2( fabs( m1.xy() - 1.0 ) < 1.0e-9, "Failure xy m1" );
    QVERIFY2( fabs( m1.xz() - 1.5 ) < 1.0e-9, "Failure xz m1" );
    QVERIFY2( fabs( m1.yx() - 2.0 ) < 1.0e-9, "Failure yx m1" );
    QVERIFY2( fabs( m1.yy() - 2.5 ) < 1.0e-9, "Failure yy m1" );
    QVERIFY2( fabs( m1.yz() - 3.0 ) < 1.0e-9, "Failure yz m1" );
    QVERIFY2( fabs( m1.zx() - 3.5 ) < 1.0e-9, "Failure zx m1" );
    QVERIFY2( fabs( m1.zy() - 4.0 ) < 1.0e-9, "Failure zy m1" );
    QVERIFY2( fabs( m1.zz() - 4.5 ) < 1.0e-9, "Failure zz m1" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix3x3Test::operator_unary_divide_by_scalar()
{
    fdm::Matrix3x3 m1( 1.0, 2.0, 3.0,
                       4.0, 5.0, 6.0,
                       7.0, 8.0, 9.0 );

    m1 /= 2.0;

    QVERIFY2( fabs( m1.xx() - 0.5 ) < 1.0e-9, "Failure xx m1" );
    QVERIFY2( fabs( m1.xy() - 1.0 ) < 1.0e-9, "Failure xy m1" );
    QVERIFY2( fabs( m1.xz() - 1.5 ) < 1.0e-9, "Failure xz m1" );
    QVERIFY2( fabs( m1.yx() - 2.0 ) < 1.0e-9, "Failure yx m1" );
    QVERIFY2( fabs( m1.yy() - 2.5 ) < 1.0e-9, "Failure yy m1" );
    QVERIFY2( fabs( m1.yz() - 3.0 ) < 1.0e-9, "Failure yz m1" );
    QVERIFY2( fabs( m1.zx() - 3.5 ) < 1.0e-9, "Failure zx m1" );
    QVERIFY2( fabs( m1.zy() - 4.0 ) < 1.0e-9, "Failure zy m1" );
    QVERIFY2( fabs( m1.zz() - 4.5 ) < 1.0e-9, "Failure zz m1" );
}

////////////////////////////////////////////////////////////////////////////////

QTEST_APPLESS_MAIN(Matrix3x3Test)

////////////////////////////////////////////////////////////////////////////////

#include "test_fdm_matrix3x3.moc"
