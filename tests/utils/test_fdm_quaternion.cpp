#include <iostream>

#include <QString>
#include <QtTest>

#include <fdm/utils/fdm_Quaternion.h>
#include <fdm/utils/fdm_Vector3.h>

#include <osg/Quat>

////////////////////////////////////////////////////////////////////////////////

using namespace std;

////////////////////////////////////////////////////////////////////////////////

class QuaternionTest : public QObject
{
    Q_OBJECT

public:

    QuaternionTest();

private Q_SLOTS:

    void initTestCase();
    void cleanupTestCase();

    void createZeroRotationQuaternion();

    void constructor();

    void isValid();
    void conjugate();
    void invert();
    void normalize();
    void getLength2();
    void getLength();
    void getAngles();
    void getConjugated();
    void getDerivative();
    void getInverted();
    void getNormalized();
    void set();

    void operator_assign();
    void operator_add();
    void operator_substract();
    void operator_multiply_by_scalar();
    void operator_multiply_by_quaternion();
    void operator_divide_by_scalar();
    void operator_unary_add();
    void operator_unary_substract();
    void operator_unary_multiply_by_scalar();
    void operator_unary_divide_by_scalar();
    void operator_equality();
    void operator_inequality();
};

////////////////////////////////////////////////////////////////////////////////

QuaternionTest::QuaternionTest() {}

////////////////////////////////////////////////////////////////////////////////

void QuaternionTest::initTestCase() {}

////////////////////////////////////////////////////////////////////////////////

void QuaternionTest::cleanupTestCase() {}

////////////////////////////////////////////////////////////////////////////////

void QuaternionTest::createZeroRotationQuaternion()
{
    fdm::Quaternion q0 = fdm::Quaternion::createZeroRotationQuaternion();

    QVERIFY2( fabs( q0.e0() - 1.0 ) < 1.0e-9, "Failure e0" );
    QVERIFY2( fabs( q0.ex() - 0.0 ) < 1.0e-9, "Failure ex" );
    QVERIFY2( fabs( q0.ey() - 0.0 ) < 1.0e-9, "Failure ey" );
    QVERIFY2( fabs( q0.ez() - 0.0 ) < 1.0e-9, "Failure ez" );
}

////////////////////////////////////////////////////////////////////////////////

void QuaternionTest::constructor()
{
    fdm::Quaternion q0;
    QVERIFY2( fabs( q0.e0() - 0.0 ) < 1.0e-9, "Failure e0 q0" );
    QVERIFY2( fabs( q0.ex() - 0.0 ) < 1.0e-9, "Failure ex q0" );
    QVERIFY2( fabs( q0.ey() - 0.0 ) < 1.0e-9, "Failure ey q0" );
    QVERIFY2( fabs( q0.ez() - 0.0 ) < 1.0e-9, "Failure ez q0" );

    q0.e0() = 0.1;
    q0.ex() = 0.2;
    q0.ey() = 0.3;
    q0.ez() = 0.4;

    fdm::Quaternion q1( q0 );
    QVERIFY2( fabs( q1.e0() - 0.1 ) < 1.0e-9, "Failure e0 q1" );
    QVERIFY2( fabs( q1.ex() - 0.2 ) < 1.0e-9, "Failure ex q1" );
    QVERIFY2( fabs( q1.ey() - 0.3 ) < 1.0e-9, "Failure ey q1" );
    QVERIFY2( fabs( q1.ez() - 0.4 ) < 1.0e-9, "Failure ez q1" );

    fdm::Quaternion q2( 0.4, 0.3, 0.2, 0.1 );
    QVERIFY2( fabs( q2.e0() - 0.4 ) < 1.0e-9, "Failure e0 q2" );
    QVERIFY2( fabs( q2.ex() - 0.3 ) < 1.0e-9, "Failure ex q2" );
    QVERIFY2( fabs( q2.ey() - 0.2 ) < 1.0e-9, "Failure ey q2" );
    QVERIFY2( fabs( q2.ez() - 0.1 ) < 1.0e-9, "Failure ez q2" );

    fdm::Quaternion q3( fdm::Angles( M_PI_4, M_PI_4, M_PI_4 ) );
    osg::Quat q3t( M_PI_4, osg::X_AXIS,
                   M_PI_4, osg::Y_AXIS,
                   M_PI_4, osg::Z_AXIS );
    QVERIFY2( fabs( q3.e0() - q3t.w() ) < 1.0e-9, "Failure e0 q3" );
    QVERIFY2( fabs( q3.ex() - q3t.x() ) < 1.0e-9, "Failure ex q3" );
    QVERIFY2( fabs( q3.ey() - q3t.y() ) < 1.0e-9, "Failure ey q3" );
    QVERIFY2( fabs( q3.ez() - q3t.z() ) < 1.0e-9, "Failure ez q3" );

    fdm::Quaternion q4( M_PI_4, fdm::Vector3( 1.0, 0.0, 0.0 ) );
    osg::Quat q4t( M_PI_4, osg::Vec3d( 1.0, 0.0, 0.0 ) );
    QVERIFY2( fabs( q4.e0() - q4t.w() ) < 1.0e-9, "Failure e0 q4" );
    QVERIFY2( fabs( q4.ex() - q4t.x() ) < 1.0e-9, "Failure ex q4" );
    QVERIFY2( fabs( q4.ey() - q4t.y() ) < 1.0e-9, "Failure ey q4" );
    QVERIFY2( fabs( q4.ez() - q4t.z() ) < 1.0e-9, "Failure ez q4" );

    fdm::Quaternion q5( M_PI_4, fdm::Vector3( 0.0, 1.0, 0.0 ) );
    osg::Quat q5t( M_PI_4, osg::Vec3d( 0.0, 1.0, 0.0 ) );
    QVERIFY2( fabs( q5.e0() - q5t.w() ) < 1.0e-9, "Failure e0 q5" );
    QVERIFY2( fabs( q5.ex() - q5t.x() ) < 1.0e-9, "Failure ex q5" );
    QVERIFY2( fabs( q5.ey() - q5t.y() ) < 1.0e-9, "Failure ey q5" );
    QVERIFY2( fabs( q5.ez() - q5t.z() ) < 1.0e-9, "Failure ez q5" );

    fdm::Quaternion q6( M_PI_4, fdm::Vector3( 0.0, 0.0, 1.0 ) );
    osg::Quat q6t( M_PI_4, osg::Vec3d( 0.0, 0.0, 1.0 ) );
    QVERIFY2( fabs( q6.e0() - q6t.w() ) < 1.0e-9, "Failure e0 q6" );
    QVERIFY2( fabs( q6.ex() - q6t.x() ) < 1.0e-9, "Failure ex q6" );
    QVERIFY2( fabs( q6.ey() - q6t.y() ) < 1.0e-9, "Failure ey q6" );
    QVERIFY2( fabs( q6.ez() - q6t.z() ) < 1.0e-9, "Failure ez q6" );


}

////////////////////////////////////////////////////////////////////////////////

void QuaternionTest::isValid()
{
    fdm::Quaternion q0;

    QVERIFY2( q0.isValid(), "Failure" );

    q0.e0() = 0.5;
    q0.ex() = 0.5;
    q0.ey() = 0.5;
    q0.ez() = 0.5;

    QVERIFY2( q0.isValid(), "Failure" );

    q0.e0() = std::numeric_limits< double >::quiet_NaN();
    q0.ex() = 0.5;
    q0.ey() = 0.5;
    q0.ez() = 0.5;

    QVERIFY2( !q0.isValid(), "Failure" );

    q0.e0() = 0.5;
    q0.ex() = std::numeric_limits< double >::quiet_NaN();
    q0.ey() = 0.5;
    q0.ez() = 0.5;

    QVERIFY2( !q0.isValid(), "Failure" );

    q0.e0() = 0.5;
    q0.ex() = 0.5;
    q0.ey() = std::numeric_limits< double >::quiet_NaN();
    q0.ez() = 0.5;

    QVERIFY2( !q0.isValid(), "Failure" );

    q0.e0() = 0.5;
    q0.ex() = 0.5;
    q0.ey() = 0.5;
    q0.ez() = std::numeric_limits< double >::quiet_NaN();

    QVERIFY2( !q0.isValid(), "Failure" );

    q0.e0() = std::numeric_limits< double >::quiet_NaN();
    q0.ex() = std::numeric_limits< double >::quiet_NaN();
    q0.ey() = std::numeric_limits< double >::quiet_NaN();
    q0.ez() = std::numeric_limits< double >::quiet_NaN();

    QVERIFY2( !q0.isValid(), "Failure" );
}

////////////////////////////////////////////////////////////////////////////////

void QuaternionTest::conjugate()
{
    fdm::Quaternion q0( 1.0, 2.0, 3.0, 4.0 );

    q0.conjugate();

    QVERIFY2( fabs( q0.e0() - 1.0 ) < 1.0e-9, "Failure e0" );
    QVERIFY2( fabs( q0.ex() + 2.0 ) < 1.0e-9, "Failure ex" );
    QVERIFY2( fabs( q0.ey() + 3.0 ) < 1.0e-9, "Failure ey" );
    QVERIFY2( fabs( q0.ez() + 4.0 ) < 1.0e-9, "Failure ez" );
}

////////////////////////////////////////////////////////////////////////////////

void QuaternionTest::invert()
{
    fdm::Quaternion q0(  M_PI, fdm::Vector3( 1.0, 0.0, 0.0 ) );
    fdm::Quaternion q1( -M_PI, fdm::Vector3( 1.0, 0.0, 0.0 ) );

    q0.invert();

    QVERIFY2( fabs( q0.e0() - q1.e0() ) < 1.0e-9, "Failure e0" );
    QVERIFY2( fabs( q0.ex() - q1.ex() ) < 1.0e-9, "Failure ex" );
    QVERIFY2( fabs( q0.ey() - q1.ey() ) < 1.0e-9, "Failure ey" );
    QVERIFY2( fabs( q0.ez() - q1.ez() ) < 1.0e-9, "Failure ez" );

    fdm::Quaternion q2(  M_PI_4, fdm::Vector3( 0.0, 1.0, 0.0 ) );
    fdm::Quaternion q3( -M_PI_4, fdm::Vector3( 0.0, 1.0, 0.0 ) );

    q2.invert();

    QVERIFY2( fabs( q2.e0() - q3.e0() ) < 1.0e-9, "Failure e0" );
    QVERIFY2( fabs( q2.ex() - q3.ex() ) < 1.0e-9, "Failure ex" );
    QVERIFY2( fabs( q2.ey() - q3.ey() ) < 1.0e-9, "Failure ey" );
    QVERIFY2( fabs( q2.ez() - q3.ez() ) < 1.0e-9, "Failure ez" );
}

////////////////////////////////////////////////////////////////////////////////

void QuaternionTest::normalize()
{
    fdm::Quaternion q0( 1.0, 1.0, 1.0, 1.0 );

    q0.normalize();

    QVERIFY2( fabs( q0.getLength() - 1.0 ) < 1.0e-9, "Failure len q0" );

    QVERIFY2( fabs( q0.e0() - 0.5 ) < 1.0e-9, "Failure e0 q0" );
    QVERIFY2( fabs( q0.ex() - 0.5 ) < 1.0e-9, "Failure ex q0" );
    QVERIFY2( fabs( q0.ey() - 0.5 ) < 1.0e-9, "Failure ey q0" );
    QVERIFY2( fabs( q0.ez() - 0.5 ) < 1.0e-9, "Failure ez q0" );

    fdm::Quaternion q1( 0.5, 0.5, 0.5, 0.5 );

    q1.normalize();

    QVERIFY2( fabs( q1.getLength() - 1.0 ) < 1.0e-9, "Failure len q1" );

    QVERIFY2( fabs( q1.e0() - 0.5 ) < 1.0e-9, "Failure e0 q1" );
    QVERIFY2( fabs( q1.ex() - 0.5 ) < 1.0e-9, "Failure ex q1" );
    QVERIFY2( fabs( q1.ey() - 0.5 ) < 1.0e-9, "Failure ey q1" );
    QVERIFY2( fabs( q1.ez() - 0.5 ) < 1.0e-9, "Failure ez q1" );
}

////////////////////////////////////////////////////////////////////////////////

void QuaternionTest::getLength2()
{
    fdm::Quaternion q0( 1.0, 1.0, 1.0, 1.0 );
    QVERIFY2( fabs( q0.getLength2() - 4.0 ) < 1.0e-9, "Failure q0" );

    fdm::Quaternion q1( 0.5, 0.5, 0.5, 0.5 );
    QVERIFY2( fabs( q1.getLength2() - 1.0 ) < 1.0e-9, "Failure q0" );
}

////////////////////////////////////////////////////////////////////////////////

void QuaternionTest::getLength()
{
    fdm::Quaternion q0( 1.0, 1.0, 1.0, 1.0 );
    QVERIFY2( fabs( q0.getLength() - 2.0 ) < 1.0e-9, "Failure q0" );

    fdm::Quaternion q1( 0.5, 0.5, 0.5, 0.5 );
    QVERIFY2( fabs( q1.getLength() - 1.0 ) < 1.0e-9, "Failure q0" );
}

////////////////////////////////////////////////////////////////////////////////

void QuaternionTest::getAngles()
{
    fdm::Angles a1( M_PI_4, 0.0, 0.0 );
    fdm::Angles a2( 0.0, M_PI_4, 0.0 );
    fdm::Angles a3( 0.0, 0.0, M_PI_4 );
    fdm::Angles a4( M_PI_4, M_PI_4, M_PI_4 );

    fdm::Quaternion q1( a1 );
    fdm::Quaternion q2( a2 );
    fdm::Quaternion q3( a3 );
    fdm::Quaternion q4( a4 );

    fdm::Angles ar1 = q1.getAngles();
    fdm::Angles ar2 = q2.getAngles();
    fdm::Angles ar3 = q3.getAngles();
    fdm::Angles ar4 = q4.getAngles();

    QVERIFY2( fabs( ar1.phi() - a1.phi() ) < 1.0e-9, "Failure phi a1" );
    QVERIFY2( fabs( ar1.tht() - a1.tht() ) < 1.0e-9, "Failure tht a1" );
    QVERIFY2( fabs( ar1.psi() - a1.psi() ) < 1.0e-9, "Failure psi a1" );

    QVERIFY2( fabs( ar2.phi() - a2.phi() ) < 1.0e-9, "Failure phi a2" );
    QVERIFY2( fabs( ar2.tht() - a2.tht() ) < 1.0e-9, "Failure tht a2" );
    QVERIFY2( fabs( ar2.psi() - a2.psi() ) < 1.0e-9, "Failure psi a2" );

    QVERIFY2( fabs( ar3.phi() - a3.phi() ) < 1.0e-9, "Failure phi a3" );
    QVERIFY2( fabs( ar3.tht() - a3.tht() ) < 1.0e-9, "Failure tht a3" );
    QVERIFY2( fabs( ar3.psi() - a3.psi() ) < 1.0e-9, "Failure psi a3" );

    QVERIFY2( fabs( ar4.phi() - a4.phi() ) < 1.0e-9, "Failure phi a4" );
    QVERIFY2( fabs( ar4.tht() - a4.tht() ) < 1.0e-9, "Failure tht a4" );
    QVERIFY2( fabs( ar4.psi() - a4.psi() ) < 1.0e-9, "Failure psi a4" );
}

////////////////////////////////////////////////////////////////////////////////

void QuaternionTest::getConjugated()
{
    fdm::Quaternion q( 1.0, 2.0, 3.0, 4.0 );

    fdm::Quaternion q0 = q.getConjugated();

    QVERIFY2( fabs( q0.e0() - 1.0 ) < 1.0e-9, "Failure e0" );
    QVERIFY2( fabs( q0.ex() + 2.0 ) < 1.0e-9, "Failure ex" );
    QVERIFY2( fabs( q0.ey() + 3.0 ) < 1.0e-9, "Failure ey" );
    QVERIFY2( fabs( q0.ez() + 4.0 ) < 1.0e-9, "Failure ez" );
}

////////////////////////////////////////////////////////////////////////////////

void QuaternionTest::getDerivative()
{
    fdm::Quaternion q( M_PI_2, fdm::Vector3( 1.0, 1.0, 1.0 ) );
    fdm::Vector3 omega( 1.0, 2.0, 3.0 );

    fdm::Quaternion q0 = q.getDerivative( omega );

    QVERIFY2( fabs( q0.e0() + 1.2247  ) < 1.0e-3, "Failure e0" );
    QVERIFY2( fabs( q0.ex() - 0.55768 ) < 1.0e-3, "Failure ex" );
    QVERIFY2( fabs( q0.ey() - 0.29886 ) < 1.0e-3, "Failure ey" );
    QVERIFY2( fabs( q0.ez() - 1.2648  ) < 1.0e-3, "Failure ez" );
}

////////////////////////////////////////////////////////////////////////////////

void QuaternionTest::getInverted()
{
    fdm::Quaternion q  (  M_PI, fdm::Vector3( 1.0, 0.0, 0.0 ) );
    fdm::Quaternion q1 ( -M_PI, fdm::Vector3( 1.0, 0.0, 0.0 ) );

    fdm::Quaternion q0 = q.getInverted();

    QVERIFY2( fabs( q0.e0() - q1.e0() ) < 1.0e-9, "Failure e0" );
    QVERIFY2( fabs( q0.ex() - q1.ex() ) < 1.0e-9, "Failure ex" );
    QVERIFY2( fabs( q0.ey() - q1.ey() ) < 1.0e-9, "Failure ey" );
    QVERIFY2( fabs( q0.ez() - q1.ez() ) < 1.0e-9, "Failure ez" );
}

////////////////////////////////////////////////////////////////////////////////

void QuaternionTest::getNormalized()
{
    fdm::Quaternion q( 1.0, 1.0, 1.0, 1.0 );

    fdm::Quaternion q0 = q.getNormalized();

    QVERIFY2( fabs( q0.getLength() - 1.0 ) < 1.0e-9, "Failure len q0" );

    QVERIFY2( fabs( q0.e0() - 0.5 ) < 1.0e-9, "Failure e0 q0" );
    QVERIFY2( fabs( q0.ex() - 0.5 ) < 1.0e-9, "Failure ex q0" );
    QVERIFY2( fabs( q0.ey() - 0.5 ) < 1.0e-9, "Failure ey q0" );
    QVERIFY2( fabs( q0.ez() - 0.5 ) < 1.0e-9, "Failure ez q0" );

    fdm::Quaternion q1( 0.5, 0.5, 0.5, 0.5 );

    q0 = q1.getNormalized();

    QVERIFY2( fabs( q1.getLength() - 1.0 ) < 1.0e-9, "Failure len q1" );

    QVERIFY2( fabs( q1.e0() - 0.5 ) < 1.0e-9, "Failure e0 q1" );
    QVERIFY2( fabs( q1.ex() - 0.5 ) < 1.0e-9, "Failure ex q1" );
    QVERIFY2( fabs( q1.ey() - 0.5 ) < 1.0e-9, "Failure ey q1" );
    QVERIFY2( fabs( q1.ez() - 0.5 ) < 1.0e-9, "Failure ez q1" );
}

////////////////////////////////////////////////////////////////////////////////

void QuaternionTest::set()
{
    fdm::Quaternion q0;

    q0.set( 1.0, 2.0, 3.0, 4.0 );

    QVERIFY2( fabs( q0.e0() - 1.0 ) < 1.0e-9, "Failure e0 q0" );
    QVERIFY2( fabs( q0.ex() - 2.0 ) < 1.0e-9, "Failure ex q0" );
    QVERIFY2( fabs( q0.ey() - 3.0 ) < 1.0e-9, "Failure ey q0" );
    QVERIFY2( fabs( q0.ez() - 4.0 ) < 1.0e-9, "Failure ez q0" );
}

////////////////////////////////////////////////////////////////////////////////

void QuaternionTest::operator_assign()
{
    fdm::Quaternion q0( 1.0, 2.0, 3.0, 4.0 );

    fdm::Quaternion q;

    q = q0;

    QVERIFY2( fabs( q.e0() - 1.0 ) < 1.0e-9, "Failure e0" );
    QVERIFY2( fabs( q.ex() - 2.0 ) < 1.0e-9, "Failure ex" );
    QVERIFY2( fabs( q.ey() - 3.0 ) < 1.0e-9, "Failure ey" );
    QVERIFY2( fabs( q.ez() - 4.0 ) < 1.0e-9, "Failure ez" );
}

////////////////////////////////////////////////////////////////////////////////

void QuaternionTest::operator_add()
{
    fdm::Quaternion q0;
    fdm::Quaternion q1( 1.0, 2.0, 3.0, 4.0 );

    fdm::Quaternion q = q0 + q1;

    QVERIFY2( fabs( q.e0() - 1.0 ) < 1.0e-9, "Failure e0" );
    QVERIFY2( fabs( q.ex() - 2.0 ) < 1.0e-9, "Failure ex" );
    QVERIFY2( fabs( q.ey() - 3.0 ) < 1.0e-9, "Failure ey" );
    QVERIFY2( fabs( q.ez() - 4.0 ) < 1.0e-9, "Failure ez" );
}

////////////////////////////////////////////////////////////////////////////////

void QuaternionTest::operator_substract()
{
    fdm::Quaternion q0;
    fdm::Quaternion q1( 1.0, 2.0, 3.0, 4.0 );

    fdm::Quaternion q = q0 - q1;

    QVERIFY2( fabs( q.e0() + 1.0 ) < 1.0e-9, "Failure e0" );
    QVERIFY2( fabs( q.ex() + 2.0 ) < 1.0e-9, "Failure ex" );
    QVERIFY2( fabs( q.ey() + 3.0 ) < 1.0e-9, "Failure ey" );
    QVERIFY2( fabs( q.ez() + 4.0 ) < 1.0e-9, "Failure ez" );
}

////////////////////////////////////////////////////////////////////////////////

void QuaternionTest::operator_multiply_by_scalar()
{
    fdm::Quaternion q0( 1.0, 2.0, 3.0, 4.0 );

    fdm::Quaternion q = q0 * 2.0;

    QVERIFY2( fabs( q.e0() - 2.0 ) < 1.0e-9, "Failure e0" );
    QVERIFY2( fabs( q.ex() - 4.0 ) < 1.0e-9, "Failure ex" );
    QVERIFY2( fabs( q.ey() - 6.0 ) < 1.0e-9, "Failure ey" );
    QVERIFY2( fabs( q.ez() - 8.0 ) < 1.0e-9, "Failure ez" );
}

////////////////////////////////////////////////////////////////////////////////

void QuaternionTest::operator_multiply_by_quaternion()
{
    osg::Quat qt1( M_PI_4, osg::X_AXIS );
    osg::Quat qt2( M_PI_4, osg::Y_AXIS );
    osg::Quat qt = qt1 * qt2;

    //std::cout << qt.w() << "," << qt.x() << "," << qt.y() << "," << qt.z() << std::endl;

    fdm::Quaternion q1( M_PI_4, fdm::Vector3( 1.0, 0.0, 0.0 ) );
    fdm::Quaternion q2( M_PI_4, fdm::Vector3( 0.0, 1.0, 0.0 ) );\
    fdm::Quaternion q = q2 * q1;    // ???

    //std::cout << q.toString() << std::endl;

    QVERIFY2( fabs( q.e0() - qt.w() ) < 1.0e-9, "Failure e0" );
    QVERIFY2( fabs( q.ex() - qt.x() ) < 1.0e-9, "Failure ex" );
    QVERIFY2( fabs( q.ey() - qt.y() ) < 1.0e-9, "Failure ey" );
    QVERIFY2( fabs( q.ez() - qt.z() ) < 1.0e-9, "Failure ez" );
}

////////////////////////////////////////////////////////////////////////////////

void QuaternionTest::operator_divide_by_scalar()
{
    fdm::Quaternion q0( 2.0, 4.0, 6.0, 8.0 );

    fdm::Quaternion q = q0 / 2.0;

    QVERIFY2( fabs( q.e0() - 1.0 ) < 1.0e-9, "Failure e0" );
    QVERIFY2( fabs( q.ex() - 2.0 ) < 1.0e-9, "Failure ex" );
    QVERIFY2( fabs( q.ey() - 3.0 ) < 1.0e-9, "Failure ey" );
    QVERIFY2( fabs( q.ez() - 4.0 ) < 1.0e-9, "Failure ez" );
}

////////////////////////////////////////////////////////////////////////////////

void QuaternionTest::operator_unary_add()
{
    fdm::Quaternion q;
    fdm::Quaternion q1( 1.0, 2.0, 3.0, 4.0 );

    q += q1;

    QVERIFY2( fabs( q.e0() - 1.0 ) < 1.0e-9, "Failure e0" );
    QVERIFY2( fabs( q.ex() - 2.0 ) < 1.0e-9, "Failure ex" );
    QVERIFY2( fabs( q.ey() - 3.0 ) < 1.0e-9, "Failure ey" );
    QVERIFY2( fabs( q.ez() - 4.0 ) < 1.0e-9, "Failure ez" );
}

////////////////////////////////////////////////////////////////////////////////

void QuaternionTest::operator_unary_substract()
{
    fdm::Quaternion q;
    fdm::Quaternion q1( 1.0, 2.0, 3.0, 4.0 );

    q -= q1;

    QVERIFY2( fabs( q.e0() + 1.0 ) < 1.0e-9, "Failure e0" );
    QVERIFY2( fabs( q.ex() + 2.0 ) < 1.0e-9, "Failure ex" );
    QVERIFY2( fabs( q.ey() + 3.0 ) < 1.0e-9, "Failure ey" );
    QVERIFY2( fabs( q.ez() + 4.0 ) < 1.0e-9, "Failure ez" );
}

////////////////////////////////////////////////////////////////////////////////

void QuaternionTest::operator_unary_multiply_by_scalar()
{
    fdm::Quaternion q( 1.0, 2.0, 3.0, 4.0 );

    q *= 2.0;

    QVERIFY2( fabs( q.e0() - 2.0 ) < 1.0e-9, "Failure e0" );
    QVERIFY2( fabs( q.ex() - 4.0 ) < 1.0e-9, "Failure ex" );
    QVERIFY2( fabs( q.ey() - 6.0 ) < 1.0e-9, "Failure ey" );
    QVERIFY2( fabs( q.ez() - 8.0 ) < 1.0e-9, "Failure ez" );
}

////////////////////////////////////////////////////////////////////////////////

void QuaternionTest::operator_unary_divide_by_scalar()
{
    fdm::Quaternion q( 2.0, 4.0, 6.0, 8.0 );

    q /= 2.0;

    QVERIFY2( fabs( q.e0() - 1.0 ) < 1.0e-9, "Failure e0" );
    QVERIFY2( fabs( q.ex() - 2.0 ) < 1.0e-9, "Failure ex" );
    QVERIFY2( fabs( q.ey() - 3.0 ) < 1.0e-9, "Failure ey" );
    QVERIFY2( fabs( q.ez() - 4.0 ) < 1.0e-9, "Failure ez" );
}

////////////////////////////////////////////////////////////////////////////////

void QuaternionTest::operator_equality()
{
    fdm::Quaternion q0;
    fdm::Quaternion q1( q0 );

    QVERIFY2( ( q0 == q1 ), "Failure e0" );

    q0.set( 0.5, 0.5, 0.5, 0.5 );
    q1.set( 0.5, 0.5, 0.5, 0.5 );

    QVERIFY2( ( q0 == q1 ), "Failure e0" );

    q0.set( 0.0, 0.0, 0.0, 0.0 );
    q1.set( 0.0, 0.0, 0.0, 0.0 );

    QVERIFY2( ( q0 == q1 ), "Failure e0" );

    q0.set( 1.0, 0.0, 0.0, 0.0 );
    q1.set( 0.0, 0.0, 0.0, 0.0 );

    QVERIFY2( !( q0 == q1 ), "Failure e0" );

    q0.set( 0.0, 1.0, 0.0, 0.0 );
    q1.set( 0.0, 0.0, 0.0, 0.0 );

    QVERIFY2( !( q0 == q1 ), "Failure e0" );

    q0.set( 0.0, 0.0, 1.0, 0.0 );
    q1.set( 0.0, 0.0, 0.0, 0.0 );

    QVERIFY2( !( q0 == q1 ), "Failure e0" );

    q0.set( 0.0, 0.0, 0.0, 1.0 );
    q1.set( 0.0, 0.0, 0.0, 0.0 );

    QVERIFY2( !( q0 == q1 ), "Failure e0" );
}

////////////////////////////////////////////////////////////////////////////////

void QuaternionTest::operator_inequality()
{
    fdm::Quaternion q0;
    fdm::Quaternion q1( q0 );

    QVERIFY2( !( q0 != q1 ), "Failure e0" );

    q0.set( 0.5, 0.5, 0.5, 0.5 );
    q1.set( 0.5, 0.5, 0.5, 0.5 );

    QVERIFY2( !( q0 != q1 ), "Failure e0" );

    q0.set( 0.0, 0.0, 0.0, 0.0 );
    q1.set( 0.0, 0.0, 0.0, 0.0 );

    QVERIFY2( !( q0 != q1 ), "Failure e0" );

    q0.set( 1.0, 0.0, 0.0, 0.0 );
    q1.set( 0.0, 0.0, 0.0, 0.0 );

    QVERIFY2( ( q0 != q1 ), "Failure e0" );

    q0.set( 0.0, 1.0, 0.0, 0.0 );
    q1.set( 0.0, 0.0, 0.0, 0.0 );

    QVERIFY2( ( q0 != q1 ), "Failure e0" );

    q0.set( 0.0, 0.0, 1.0, 0.0 );
    q1.set( 0.0, 0.0, 0.0, 0.0 );

    QVERIFY2( ( q0 != q1 ), "Failure e0" );

    q0.set( 0.0, 0.0, 0.0, 1.0 );
    q1.set( 0.0, 0.0, 0.0, 0.0 );

    QVERIFY2( ( q0 != q1 ), "Failure e0" );
}

////////////////////////////////////////////////////////////////////////////////

QTEST_APPLESS_MAIN(QuaternionTest)

////////////////////////////////////////////////////////////////////////////////

#include "test_fdm_quaternion.moc"
