#include <iostream>

#include <QString>
#include <QtTest>

#include <fdm/utils/fdm_Matrix4x4.h>

////////////////////////////////////////////////////////////////////////////////

using namespace std;

////////////////////////////////////////////////////////////////////////////////

class Matrix4x4Test : public QObject
{
    Q_OBJECT

public:

    Matrix4x4Test();

private Q_SLOTS:

    void initTestCase();
    void cleanupTestCase();

    void constructor();

    void transpose();
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

Matrix4x4Test::Matrix4x4Test() {}

////////////////////////////////////////////////////////////////////////////////

void Matrix4x4Test::initTestCase() {}

////////////////////////////////////////////////////////////////////////////////

void Matrix4x4Test::cleanupTestCase() {}

////////////////////////////////////////////////////////////////////////////////

void Matrix4x4Test::constructor()
{
    fdm::Matrix4x4 m;
    QVERIFY2( fabs( m(0,0) - 0.0 ) < 1.0e-9, "Failure 0,0 m" );
    QVERIFY2( fabs( m(0,1) - 0.0 ) < 1.0e-9, "Failure 0,1 m" );
    QVERIFY2( fabs( m(0,2) - 0.0 ) < 1.0e-9, "Failure 0,2 m" );
    QVERIFY2( fabs( m(0,3) - 0.0 ) < 1.0e-9, "Failure 0,3 m" );
    QVERIFY2( fabs( m(1,0) - 0.0 ) < 1.0e-9, "Failure 1,0 m" );
    QVERIFY2( fabs( m(1,1) - 0.0 ) < 1.0e-9, "Failure 1,1 m" );
    QVERIFY2( fabs( m(1,2) - 0.0 ) < 1.0e-9, "Failure 1,2 m" );
    QVERIFY2( fabs( m(1,3) - 0.0 ) < 1.0e-9, "Failure 1,3 m" );
    QVERIFY2( fabs( m(2,0) - 0.0 ) < 1.0e-9, "Failure 2,0 m" );
    QVERIFY2( fabs( m(2,1) - 0.0 ) < 1.0e-9, "Failure 2,1 m" );
    QVERIFY2( fabs( m(2,2) - 0.0 ) < 1.0e-9, "Failure 2,2 m" );
    QVERIFY2( fabs( m(2,3) - 0.0 ) < 1.0e-9, "Failure 2,3 m" );
    QVERIFY2( fabs( m(3,0) - 0.0 ) < 1.0e-9, "Failure 3,0 m" );
    QVERIFY2( fabs( m(3,1) - 0.0 ) < 1.0e-9, "Failure 3,1 m" );
    QVERIFY2( fabs( m(3,2) - 0.0 ) < 1.0e-9, "Failure 3,2 m" );
    QVERIFY2( fabs( m(3,3) - 0.0 ) < 1.0e-9, "Failure 3,3 m" );

    m(0,0) =  1.0;
    m(0,1) =  2.0;
    m(0,2) =  3.0;
    m(0,3) =  4.0;
    m(1,0) =  5.0;
    m(1,1) =  6.0;
    m(1,2) =  7.0;
    m(1,3) =  8.0;
    m(2,0) =  9.0;
    m(2,1) = 10.0;
    m(2,2) = 11.0;
    m(2,3) = 12.0;
    m(3,0) = 13.0;
    m(3,1) = 14.0;
    m(3,2) = 15.0;
    m(3,3) = 16.0;

    fdm::Matrix4x4 m1( m );
    QVERIFY2( fabs( m1(0,0) -  1.0 ) < 1.0e-9, "Failure 0,0 m1" );
    QVERIFY2( fabs( m1(0,1) -  2.0 ) < 1.0e-9, "Failure 0,1 m1" );
    QVERIFY2( fabs( m1(0,2) -  3.0 ) < 1.0e-9, "Failure 0,2 m1" );
    QVERIFY2( fabs( m1(0,3) -  4.0 ) < 1.0e-9, "Failure 0,3 m1" );
    QVERIFY2( fabs( m1(1,0) -  5.0 ) < 1.0e-9, "Failure 1,0 m1" );
    QVERIFY2( fabs( m1(1,1) -  6.0 ) < 1.0e-9, "Failure 1,1 m1" );
    QVERIFY2( fabs( m1(1,2) -  7.0 ) < 1.0e-9, "Failure 1,2 m1" );
    QVERIFY2( fabs( m1(1,3) -  8.0 ) < 1.0e-9, "Failure 1,3 m1" );
    QVERIFY2( fabs( m1(2,0) -  9.0 ) < 1.0e-9, "Failure 2,0 m1" );
    QVERIFY2( fabs( m1(2,1) - 10.0 ) < 1.0e-9, "Failure 2,1 m1" );
    QVERIFY2( fabs( m1(2,2) - 11.0 ) < 1.0e-9, "Failure 2,2 m1" );
    QVERIFY2( fabs( m1(2,3) - 12.0 ) < 1.0e-9, "Failure 2,3 m1" );
    QVERIFY2( fabs( m1(3,0) - 13.0 ) < 1.0e-9, "Failure 3,0 m1" );
    QVERIFY2( fabs( m1(3,1) - 14.0 ) < 1.0e-9, "Failure 3,1 m1" );
    QVERIFY2( fabs( m1(3,2) - 15.0 ) < 1.0e-9, "Failure 3,2 m1" );
    QVERIFY2( fabs( m1(3,3) - 16.0 ) < 1.0e-9, "Failure 3,3 m1" );

    double x[] = {
         1.0,  2.0,  3.0,  4.0,
         5.0,  6.0,  7.0,  8.0,
         9.0, 10.0, 11.0, 12.0,
        13.0, 14.0, 15.0, 16.0
    };

    fdm::Matrix4x4 m2( x );
    QVERIFY2( fabs( m2(0,0) -  1.0 ) < 1.0e-9, "Failure 0,0 m2" );
    QVERIFY2( fabs( m2(0,1) -  2.0 ) < 1.0e-9, "Failure 0,1 m2" );
    QVERIFY2( fabs( m2(0,2) -  3.0 ) < 1.0e-9, "Failure 0,2 m2" );
    QVERIFY2( fabs( m2(0,3) -  4.0 ) < 1.0e-9, "Failure 0,3 m2" );
    QVERIFY2( fabs( m2(1,0) -  5.0 ) < 1.0e-9, "Failure 1,0 m2" );
    QVERIFY2( fabs( m2(1,1) -  6.0 ) < 1.0e-9, "Failure 1,1 m2" );
    QVERIFY2( fabs( m2(1,2) -  7.0 ) < 1.0e-9, "Failure 1,2 m2" );
    QVERIFY2( fabs( m2(1,3) -  8.0 ) < 1.0e-9, "Failure 1,3 m2" );
    QVERIFY2( fabs( m2(2,0) -  9.0 ) < 1.0e-9, "Failure 2,0 m2" );
    QVERIFY2( fabs( m2(2,1) - 10.0 ) < 1.0e-9, "Failure 2,1 m2" );
    QVERIFY2( fabs( m2(2,2) - 11.0 ) < 1.0e-9, "Failure 2,2 m2" );
    QVERIFY2( fabs( m2(2,3) - 12.0 ) < 1.0e-9, "Failure 2,3 m2" );
    QVERIFY2( fabs( m2(3,0) - 13.0 ) < 1.0e-9, "Failure 3,0 m2" );
    QVERIFY2( fabs( m2(3,1) - 14.0 ) < 1.0e-9, "Failure 3,1 m2" );
    QVERIFY2( fabs( m2(3,2) - 15.0 ) < 1.0e-9, "Failure 3,2 m2" );
    QVERIFY2( fabs( m2(3,3) - 16.0 ) < 1.0e-9, "Failure 3,3 m2" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix4x4Test::transpose()
{
    double x[] = {
         1.0,  2.0,  3.0,  4.0,
         5.0,  6.0,  7.0,  8.0,
         9.0, 10.0, 11.0, 12.0,
        13.0, 14.0, 15.0, 16.0
    };

    fdm::Matrix4x4 m( x );

    m.transpose();

    QVERIFY2( fabs( m(0,0) -  1.0 ) < 1.0e-9, "Failure 0,0" );
    QVERIFY2( fabs( m(0,1) -  5.0 ) < 1.0e-9, "Failure 0,1" );
    QVERIFY2( fabs( m(0,2) -  9.0 ) < 1.0e-9, "Failure 0,2" );
    QVERIFY2( fabs( m(0,3) - 13.0 ) < 1.0e-9, "Failure 0,3" );
    QVERIFY2( fabs( m(1,0) -  2.0 ) < 1.0e-9, "Failure 1,0" );
    QVERIFY2( fabs( m(1,1) -  6.0 ) < 1.0e-9, "Failure 1,1" );
    QVERIFY2( fabs( m(1,2) - 10.0 ) < 1.0e-9, "Failure 1,2" );
    QVERIFY2( fabs( m(1,3) - 14.0 ) < 1.0e-9, "Failure 1,3" );
    QVERIFY2( fabs( m(2,0) -  3.0 ) < 1.0e-9, "Failure 2,0" );
    QVERIFY2( fabs( m(2,1) -  7.0 ) < 1.0e-9, "Failure 2,1" );
    QVERIFY2( fabs( m(2,2) - 11.0 ) < 1.0e-9, "Failure 2,2" );
    QVERIFY2( fabs( m(2,3) - 15.0 ) < 1.0e-9, "Failure 2,3" );
    QVERIFY2( fabs( m(3,0) -  4.0 ) < 1.0e-9, "Failure 3,0" );
    QVERIFY2( fabs( m(3,1) -  8.0 ) < 1.0e-9, "Failure 3,1" );
    QVERIFY2( fabs( m(3,2) - 12.0 ) < 1.0e-9, "Failure 3,2" );
    QVERIFY2( fabs( m(3,3) - 16.0 ) < 1.0e-9, "Failure 3,3" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix4x4Test::getTransposed()
{
    double x[] = {
         1.0,  2.0,  3.0,  4.0,
         5.0,  6.0,  7.0,  8.0,
         9.0, 10.0, 11.0, 12.0,
        13.0, 14.0, 15.0, 16.0
    };

    fdm::Matrix4x4 m0( x );

    fdm::Matrix4x4 m = m0.getTransposed();

    QVERIFY2( fabs( m(0,0) -  1.0 ) < 1.0e-9, "Failure 0,0" );
    QVERIFY2( fabs( m(0,1) -  5.0 ) < 1.0e-9, "Failure 0,1" );
    QVERIFY2( fabs( m(0,2) -  9.0 ) < 1.0e-9, "Failure 0,2" );
    QVERIFY2( fabs( m(0,3) - 13.0 ) < 1.0e-9, "Failure 0,3" );
    QVERIFY2( fabs( m(1,0) -  2.0 ) < 1.0e-9, "Failure 1,0" );
    QVERIFY2( fabs( m(1,1) -  6.0 ) < 1.0e-9, "Failure 1,1" );
    QVERIFY2( fabs( m(1,2) - 10.0 ) < 1.0e-9, "Failure 1,2" );
    QVERIFY2( fabs( m(1,3) - 14.0 ) < 1.0e-9, "Failure 1,3" );
    QVERIFY2( fabs( m(2,0) -  3.0 ) < 1.0e-9, "Failure 2,0" );
    QVERIFY2( fabs( m(2,1) -  7.0 ) < 1.0e-9, "Failure 2,1" );
    QVERIFY2( fabs( m(2,2) - 11.0 ) < 1.0e-9, "Failure 2,2" );
    QVERIFY2( fabs( m(2,3) - 15.0 ) < 1.0e-9, "Failure 2,3" );
    QVERIFY2( fabs( m(3,0) -  4.0 ) < 1.0e-9, "Failure 3,0" );
    QVERIFY2( fabs( m(3,1) -  8.0 ) < 1.0e-9, "Failure 3,1" );
    QVERIFY2( fabs( m(3,2) - 12.0 ) < 1.0e-9, "Failure 3,2" );
    QVERIFY2( fabs( m(3,3) - 16.0 ) < 1.0e-9, "Failure 3,3" );
}


////////////////////////////////////////////////////////////////////////////////

void Matrix4x4Test::operator_assign()
{
    double x[] = {
         1.0,  2.0,  3.0,  4.0,
         5.0,  6.0,  7.0,  8.0,
         9.0, 10.0, 11.0, 12.0,
        13.0, 14.0, 15.0, 16.0
    };

    fdm::Matrix4x4 m0( x );

    fdm::Matrix4x4 m = m0;

    QVERIFY2( fabs( m(0,0) -  1.0 ) < 1.0e-9, "Failure 0,0" );
    QVERIFY2( fabs( m(0,1) -  2.0 ) < 1.0e-9, "Failure 0,1" );
    QVERIFY2( fabs( m(0,2) -  3.0 ) < 1.0e-9, "Failure 0,2" );
    QVERIFY2( fabs( m(0,3) -  4.0 ) < 1.0e-9, "Failure 0,3" );
    QVERIFY2( fabs( m(1,0) -  5.0 ) < 1.0e-9, "Failure 1,0" );
    QVERIFY2( fabs( m(1,1) -  6.0 ) < 1.0e-9, "Failure 1,1" );
    QVERIFY2( fabs( m(1,2) -  7.0 ) < 1.0e-9, "Failure 1,2" );
    QVERIFY2( fabs( m(1,3) -  8.0 ) < 1.0e-9, "Failure 1,3" );
    QVERIFY2( fabs( m(2,0) -  9.0 ) < 1.0e-9, "Failure 2,0" );
    QVERIFY2( fabs( m(2,1) - 10.0 ) < 1.0e-9, "Failure 2,1" );
    QVERIFY2( fabs( m(2,2) - 11.0 ) < 1.0e-9, "Failure 2,2" );
    QVERIFY2( fabs( m(2,3) - 12.0 ) < 1.0e-9, "Failure 2,3" );
    QVERIFY2( fabs( m(3,0) - 13.0 ) < 1.0e-9, "Failure 3,0" );
    QVERIFY2( fabs( m(3,1) - 14.0 ) < 1.0e-9, "Failure 3,1" );
    QVERIFY2( fabs( m(3,2) - 15.0 ) < 1.0e-9, "Failure 3,2" );
    QVERIFY2( fabs( m(3,3) - 16.0 ) < 1.0e-9, "Failure 3,3" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix4x4Test::operator_add()
{
    double x[] = {
         1.0,  2.0,  3.0,  4.0,
         5.0,  6.0,  7.0,  8.0,
         9.0, 10.0, 11.0, 12.0,
        13.0, 14.0, 15.0, 16.0
    };

    fdm::Matrix4x4 m0;
    fdm::Matrix4x4 m1( x );

    fdm::Matrix4x4 m = m0 + m1;

    QVERIFY2( fabs( m(0,0) -  1.0 ) < 1.0e-9, "Failure 0,0" );
    QVERIFY2( fabs( m(0,1) -  2.0 ) < 1.0e-9, "Failure 0,1" );
    QVERIFY2( fabs( m(0,2) -  3.0 ) < 1.0e-9, "Failure 0,2" );
    QVERIFY2( fabs( m(0,3) -  4.0 ) < 1.0e-9, "Failure 0,3" );
    QVERIFY2( fabs( m(1,0) -  5.0 ) < 1.0e-9, "Failure 1,0" );
    QVERIFY2( fabs( m(1,1) -  6.0 ) < 1.0e-9, "Failure 1,1" );
    QVERIFY2( fabs( m(1,2) -  7.0 ) < 1.0e-9, "Failure 1,2" );
    QVERIFY2( fabs( m(1,3) -  8.0 ) < 1.0e-9, "Failure 1,3" );
    QVERIFY2( fabs( m(2,0) -  9.0 ) < 1.0e-9, "Failure 2,0" );
    QVERIFY2( fabs( m(2,1) - 10.0 ) < 1.0e-9, "Failure 2,1" );
    QVERIFY2( fabs( m(2,2) - 11.0 ) < 1.0e-9, "Failure 2,2" );
    QVERIFY2( fabs( m(2,3) - 12.0 ) < 1.0e-9, "Failure 2,3" );
    QVERIFY2( fabs( m(3,0) - 13.0 ) < 1.0e-9, "Failure 3,0" );
    QVERIFY2( fabs( m(3,1) - 14.0 ) < 1.0e-9, "Failure 3,1" );
    QVERIFY2( fabs( m(3,2) - 15.0 ) < 1.0e-9, "Failure 3,2" );
    QVERIFY2( fabs( m(3,3) - 16.0 ) < 1.0e-9, "Failure 3,3" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix4x4Test::operator_substract()
{
    double x[] = {
         1.0,  2.0,  3.0,  4.0,
         5.0,  6.0,  7.0,  8.0,
         9.0, 10.0, 11.0, 12.0,
        13.0, 14.0, 15.0, 16.0
    };

    fdm::Matrix4x4 m0;
    fdm::Matrix4x4 m1( x );

    fdm::Matrix4x4 m = m0 - m1;

    QVERIFY2( fabs( m(0,0) +  1.0 ) < 1.0e-9, "Failure 0,0" );
    QVERIFY2( fabs( m(0,1) +  2.0 ) < 1.0e-9, "Failure 0,1" );
    QVERIFY2( fabs( m(0,2) +  3.0 ) < 1.0e-9, "Failure 0,2" );
    QVERIFY2( fabs( m(0,3) +  4.0 ) < 1.0e-9, "Failure 0,3" );
    QVERIFY2( fabs( m(1,0) +  5.0 ) < 1.0e-9, "Failure 1,0" );
    QVERIFY2( fabs( m(1,1) +  6.0 ) < 1.0e-9, "Failure 1,1" );
    QVERIFY2( fabs( m(1,2) +  7.0 ) < 1.0e-9, "Failure 1,2" );
    QVERIFY2( fabs( m(1,3) +  8.0 ) < 1.0e-9, "Failure 1,3" );
    QVERIFY2( fabs( m(2,0) +  9.0 ) < 1.0e-9, "Failure 2,0" );
    QVERIFY2( fabs( m(2,1) + 10.0 ) < 1.0e-9, "Failure 2,1" );
    QVERIFY2( fabs( m(2,2) + 11.0 ) < 1.0e-9, "Failure 2,2" );
    QVERIFY2( fabs( m(2,3) + 12.0 ) < 1.0e-9, "Failure 2,3" );
    QVERIFY2( fabs( m(3,0) + 13.0 ) < 1.0e-9, "Failure 3,0" );
    QVERIFY2( fabs( m(3,1) + 14.0 ) < 1.0e-9, "Failure 3,1" );
    QVERIFY2( fabs( m(3,2) + 15.0 ) < 1.0e-9, "Failure 3,2" );
    QVERIFY2( fabs( m(3,3) + 16.0 ) < 1.0e-9, "Failure 3,3" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix4x4Test::operator_multiply_by_scalar()
{
    double x[] = {
         1.0,  2.0,  3.0,  4.0,
         5.0,  6.0,  7.0,  8.0,
         9.0, 10.0, 11.0, 12.0,
        13.0, 14.0, 15.0, 16.0
    };

    fdm::Matrix4x4 m0( x );

    fdm::Matrix4x4 m = m0 * 2.0;

    QVERIFY2( fabs( m(0,0) -  2.0 ) < 1.0e-9, "Failure 0,0" );
    QVERIFY2( fabs( m(0,1) -  4.0 ) < 1.0e-9, "Failure 0,1" );
    QVERIFY2( fabs( m(0,2) -  6.0 ) < 1.0e-9, "Failure 0,2" );
    QVERIFY2( fabs( m(0,3) -  8.0 ) < 1.0e-9, "Failure 0,3" );
    QVERIFY2( fabs( m(1,0) - 10.0 ) < 1.0e-9, "Failure 1,0" );
    QVERIFY2( fabs( m(1,1) - 12.0 ) < 1.0e-9, "Failure 1,1" );
    QVERIFY2( fabs( m(1,2) - 14.0 ) < 1.0e-9, "Failure 1,2" );
    QVERIFY2( fabs( m(1,3) - 16.0 ) < 1.0e-9, "Failure 1,3" );
    QVERIFY2( fabs( m(2,0) - 18.0 ) < 1.0e-9, "Failure 2,0" );
    QVERIFY2( fabs( m(2,1) - 20.0 ) < 1.0e-9, "Failure 2,1" );
    QVERIFY2( fabs( m(2,2) - 22.0 ) < 1.0e-9, "Failure 2,2" );
    QVERIFY2( fabs( m(2,3) - 24.0 ) < 1.0e-9, "Failure 2,3" );
    QVERIFY2( fabs( m(3,0) - 26.0 ) < 1.0e-9, "Failure 3,0" );
    QVERIFY2( fabs( m(3,1) - 28.0 ) < 1.0e-9, "Failure 3,1" );
    QVERIFY2( fabs( m(3,2) - 30.0 ) < 1.0e-9, "Failure 3,2" );
    QVERIFY2( fabs( m(3,3) - 32.0 ) < 1.0e-9, "Failure 3,3" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix4x4Test::operator_multiply_by_matrix()
{
    double x1[] = {
         1.0,  2.0,  3.0,  4.0,
         5.0,  6.0,  7.0,  8.0,
         9.0, 10.0, 11.0, 12.0,
        13.0, 14.0, 15.0, 16.0
    };

    double x2[] = {
        2.0,  3.0,  4.0,  5.0,
        6.0,  7.0,  8.0,  9.0,
       10.0, 11.0, 12.0, 13.0,
       14.0, 15.0, 16.0, 17.0
    };

    fdm::Matrix4x4 m1( x1 );
    fdm::Matrix4x4 m2( x2 );

    fdm::Matrix4x4 m = m1 * m2;

    //std::cout << m.toString() << std::endl;

    QVERIFY2( fabs( m(0,0) - 100.0 ) < 1.0e-9, "Failure 0,0" );
    QVERIFY2( fabs( m(0,1) - 110.0 ) < 1.0e-9, "Failure 0,1" );
    QVERIFY2( fabs( m(0,2) - 120.0 ) < 1.0e-9, "Failure 0,2" );
    QVERIFY2( fabs( m(0,3) - 130.0 ) < 1.0e-9, "Failure 0,3" );
    QVERIFY2( fabs( m(1,0) - 228.0 ) < 1.0e-9, "Failure 1,0" );
    QVERIFY2( fabs( m(1,1) - 254.0 ) < 1.0e-9, "Failure 1,1" );
    QVERIFY2( fabs( m(1,2) - 280.0 ) < 1.0e-9, "Failure 1,2" );
    QVERIFY2( fabs( m(1,3) - 306.0 ) < 1.0e-9, "Failure 1,3" );
    QVERIFY2( fabs( m(2,0) - 356.0 ) < 1.0e-9, "Failure 2,0" );
    QVERIFY2( fabs( m(2,1) - 398.0 ) < 1.0e-9, "Failure 2,1" );
    QVERIFY2( fabs( m(2,2) - 440.0 ) < 1.0e-9, "Failure 2,2" );
    QVERIFY2( fabs( m(2,3) - 482.0 ) < 1.0e-9, "Failure 2,3" );
    QVERIFY2( fabs( m(3,0) - 484.0 ) < 1.0e-9, "Failure 3,0" );
    QVERIFY2( fabs( m(3,1) - 542.0 ) < 1.0e-9, "Failure 3,1" );
    QVERIFY2( fabs( m(3,2) - 600.0 ) < 1.0e-9, "Failure 3,2" );
    QVERIFY2( fabs( m(3,3) - 658.0 ) < 1.0e-9, "Failure 3,3" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix4x4Test::operator_multiply_by_vector()
{
    double x1[] = {
         1.0,  2.0,  3.0,  4.0,
         5.0,  6.0,  7.0,  8.0,
         9.0, 10.0, 11.0, 12.0,
        13.0, 14.0, 15.0, 16.0
    };

    double x2[] = {
         1.0,  2.0,  3.0,  4.0
    };

    fdm::Matrix4x4 m1( x1 );
    fdm::Vector4 v2( x2 );

    fdm::Vector4 v = m1 * v2;

    QVERIFY2( fabs( v(0) -  30.0 ) < 1.0e-9, "Failure 0" );
    QVERIFY2( fabs( v(1) -  70.0 ) < 1.0e-9, "Failure 1" );
    QVERIFY2( fabs( v(2) - 110.0 ) < 1.0e-9, "Failure 2" );
    QVERIFY2( fabs( v(3) - 150.0 ) < 1.0e-9, "Failure 3" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix4x4Test::operator_divide_by_scalar()
{
    double x[] = {
         2.0,  4.0,  6.0,  8.0,
        10.0, 12.0, 14.0, 16.0,
        18.0, 20.0, 22.0, 24.0,
        26.0, 28.0, 30.0, 32.0
    };

    fdm::Matrix4x4 m0( x );

    fdm::Matrix4x4 m = m0 / 2.0;

    QVERIFY2( fabs( m(0,0) -  1.0 ) < 1.0e-9, "Failure 0,0" );
    QVERIFY2( fabs( m(0,1) -  2.0 ) < 1.0e-9, "Failure 0,1" );
    QVERIFY2( fabs( m(0,2) -  3.0 ) < 1.0e-9, "Failure 0,2" );
    QVERIFY2( fabs( m(0,3) -  4.0 ) < 1.0e-9, "Failure 0,3" );
    QVERIFY2( fabs( m(1,0) -  5.0 ) < 1.0e-9, "Failure 1,0" );
    QVERIFY2( fabs( m(1,1) -  6.0 ) < 1.0e-9, "Failure 1,1" );
    QVERIFY2( fabs( m(1,2) -  7.0 ) < 1.0e-9, "Failure 1,2" );
    QVERIFY2( fabs( m(1,3) -  8.0 ) < 1.0e-9, "Failure 1,3" );
    QVERIFY2( fabs( m(2,0) -  9.0 ) < 1.0e-9, "Failure 2,0" );
    QVERIFY2( fabs( m(2,1) - 10.0 ) < 1.0e-9, "Failure 2,1" );
    QVERIFY2( fabs( m(2,2) - 11.0 ) < 1.0e-9, "Failure 2,2" );
    QVERIFY2( fabs( m(2,3) - 12.0 ) < 1.0e-9, "Failure 2,3" );
    QVERIFY2( fabs( m(3,0) - 13.0 ) < 1.0e-9, "Failure 3,0" );
    QVERIFY2( fabs( m(3,1) - 14.0 ) < 1.0e-9, "Failure 3,1" );
    QVERIFY2( fabs( m(3,2) - 15.0 ) < 1.0e-9, "Failure 3,2" );
    QVERIFY2( fabs( m(3,3) - 16.0 ) < 1.0e-9, "Failure 3,3" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix4x4Test::operator_unary_add()
{
    double x[] = {
         1.0,  2.0,  3.0,  4.0,
         5.0,  6.0,  7.0,  8.0,
         9.0, 10.0, 11.0, 12.0,
        13.0, 14.0, 15.0, 16.0
    };

    fdm::Matrix4x4 m0;
    fdm::Matrix4x4 m1( x );

    m0 += m1;

    QVERIFY2( fabs( m0(0,0) -  1.0 ) < 1.0e-9, "Failure 0,0" );
    QVERIFY2( fabs( m0(0,1) -  2.0 ) < 1.0e-9, "Failure 0,1" );
    QVERIFY2( fabs( m0(0,2) -  3.0 ) < 1.0e-9, "Failure 0,2" );
    QVERIFY2( fabs( m0(0,3) -  4.0 ) < 1.0e-9, "Failure 0,3" );
    QVERIFY2( fabs( m0(1,0) -  5.0 ) < 1.0e-9, "Failure 1,0" );
    QVERIFY2( fabs( m0(1,1) -  6.0 ) < 1.0e-9, "Failure 1,1" );
    QVERIFY2( fabs( m0(1,2) -  7.0 ) < 1.0e-9, "Failure 1,2" );
    QVERIFY2( fabs( m0(1,3) -  8.0 ) < 1.0e-9, "Failure 1,3" );
    QVERIFY2( fabs( m0(2,0) -  9.0 ) < 1.0e-9, "Failure 2,0" );
    QVERIFY2( fabs( m0(2,1) - 10.0 ) < 1.0e-9, "Failure 2,1" );
    QVERIFY2( fabs( m0(2,2) - 11.0 ) < 1.0e-9, "Failure 2,2" );
    QVERIFY2( fabs( m0(2,3) - 12.0 ) < 1.0e-9, "Failure 2,3" );
    QVERIFY2( fabs( m0(3,0) - 13.0 ) < 1.0e-9, "Failure 3,0" );
    QVERIFY2( fabs( m0(3,1) - 14.0 ) < 1.0e-9, "Failure 3,1" );
    QVERIFY2( fabs( m0(3,2) - 15.0 ) < 1.0e-9, "Failure 3,2" );
    QVERIFY2( fabs( m0(3,3) - 16.0 ) < 1.0e-9, "Failure 3,3" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix4x4Test::operator_unary_substract()
{
    double x[] = {
         1.0,  2.0,  3.0,  4.0,
         5.0,  6.0,  7.0,  8.0,
         9.0, 10.0, 11.0, 12.0,
        13.0, 14.0, 15.0, 16.0
    };

    fdm::Matrix4x4 m0;
    fdm::Matrix4x4 m1( x );

    m0 -= m1;

    QVERIFY2( fabs( m0(0,0) +  1.0 ) < 1.0e-9, "Failure 0,0" );
    QVERIFY2( fabs( m0(0,1) +  2.0 ) < 1.0e-9, "Failure 0,1" );
    QVERIFY2( fabs( m0(0,2) +  3.0 ) < 1.0e-9, "Failure 0,2" );
    QVERIFY2( fabs( m0(0,3) +  4.0 ) < 1.0e-9, "Failure 0,3" );
    QVERIFY2( fabs( m0(1,0) +  5.0 ) < 1.0e-9, "Failure 1,0" );
    QVERIFY2( fabs( m0(1,1) +  6.0 ) < 1.0e-9, "Failure 1,1" );
    QVERIFY2( fabs( m0(1,2) +  7.0 ) < 1.0e-9, "Failure 1,2" );
    QVERIFY2( fabs( m0(1,3) +  8.0 ) < 1.0e-9, "Failure 1,3" );
    QVERIFY2( fabs( m0(2,0) +  9.0 ) < 1.0e-9, "Failure 2,0" );
    QVERIFY2( fabs( m0(2,1) + 10.0 ) < 1.0e-9, "Failure 2,1" );
    QVERIFY2( fabs( m0(2,2) + 11.0 ) < 1.0e-9, "Failure 2,2" );
    QVERIFY2( fabs( m0(2,3) + 12.0 ) < 1.0e-9, "Failure 2,3" );
    QVERIFY2( fabs( m0(3,0) + 13.0 ) < 1.0e-9, "Failure 3,0" );
    QVERIFY2( fabs( m0(3,1) + 14.0 ) < 1.0e-9, "Failure 3,1" );
    QVERIFY2( fabs( m0(3,2) + 15.0 ) < 1.0e-9, "Failure 3,2" );
    QVERIFY2( fabs( m0(3,3) + 16.0 ) < 1.0e-9, "Failure 3,3" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix4x4Test::operator_unary_multiply_by_scalar()
{
    double x[] = {
         1.0,  2.0,  3.0,  4.0,
         5.0,  6.0,  7.0,  8.0,
         9.0, 10.0, 11.0, 12.0,
        13.0, 14.0, 15.0, 16.0
    };

    fdm::Matrix4x4 m0( x );

    m0 *= 2.0;

    QVERIFY2( fabs( m0(0,0) -  2.0 ) < 1.0e-9, "Failure 0,0" );
    QVERIFY2( fabs( m0(0,1) -  4.0 ) < 1.0e-9, "Failure 0,1" );
    QVERIFY2( fabs( m0(0,2) -  6.0 ) < 1.0e-9, "Failure 0,2" );
    QVERIFY2( fabs( m0(0,3) -  8.0 ) < 1.0e-9, "Failure 0,3" );
    QVERIFY2( fabs( m0(1,0) - 10.0 ) < 1.0e-9, "Failure 1,0" );
    QVERIFY2( fabs( m0(1,1) - 12.0 ) < 1.0e-9, "Failure 1,1" );
    QVERIFY2( fabs( m0(1,2) - 14.0 ) < 1.0e-9, "Failure 1,2" );
    QVERIFY2( fabs( m0(1,3) - 16.0 ) < 1.0e-9, "Failure 1,3" );
    QVERIFY2( fabs( m0(2,0) - 18.0 ) < 1.0e-9, "Failure 2,0" );
    QVERIFY2( fabs( m0(2,1) - 20.0 ) < 1.0e-9, "Failure 2,1" );
    QVERIFY2( fabs( m0(2,2) - 22.0 ) < 1.0e-9, "Failure 2,2" );
    QVERIFY2( fabs( m0(2,3) - 24.0 ) < 1.0e-9, "Failure 2,3" );
    QVERIFY2( fabs( m0(3,0) - 26.0 ) < 1.0e-9, "Failure 3,0" );
    QVERIFY2( fabs( m0(3,1) - 28.0 ) < 1.0e-9, "Failure 3,1" );
    QVERIFY2( fabs( m0(3,2) - 30.0 ) < 1.0e-9, "Failure 3,2" );
    QVERIFY2( fabs( m0(3,3) - 32.0 ) < 1.0e-9, "Failure 3,3" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix4x4Test::operator_unary_divide_by_scalar()
{
    double x[] = {
         2.0,  4.0,  6.0,  8.0,
        10.0, 12.0, 14.0, 16.0,
        18.0, 20.0, 22.0, 24.0,
        26.0, 28.0, 30.0, 32.0
    };

    fdm::Matrix4x4 m0( x );

    m0 /= 2.0;

    QVERIFY2( fabs( m0(0,0) -  1.0 ) < 1.0e-9, "Failure 0,0" );
    QVERIFY2( fabs( m0(0,1) -  2.0 ) < 1.0e-9, "Failure 0,1" );
    QVERIFY2( fabs( m0(0,2) -  3.0 ) < 1.0e-9, "Failure 0,2" );
    QVERIFY2( fabs( m0(0,3) -  4.0 ) < 1.0e-9, "Failure 0,3" );
    QVERIFY2( fabs( m0(1,0) -  5.0 ) < 1.0e-9, "Failure 1,0" );
    QVERIFY2( fabs( m0(1,1) -  6.0 ) < 1.0e-9, "Failure 1,1" );
    QVERIFY2( fabs( m0(1,2) -  7.0 ) < 1.0e-9, "Failure 1,2" );
    QVERIFY2( fabs( m0(1,3) -  8.0 ) < 1.0e-9, "Failure 1,3" );
    QVERIFY2( fabs( m0(2,0) -  9.0 ) < 1.0e-9, "Failure 2,0" );
    QVERIFY2( fabs( m0(2,1) - 10.0 ) < 1.0e-9, "Failure 2,1" );
    QVERIFY2( fabs( m0(2,2) - 11.0 ) < 1.0e-9, "Failure 2,2" );
    QVERIFY2( fabs( m0(2,3) - 12.0 ) < 1.0e-9, "Failure 2,3" );
    QVERIFY2( fabs( m0(3,0) - 13.0 ) < 1.0e-9, "Failure 3,0" );
    QVERIFY2( fabs( m0(3,1) - 14.0 ) < 1.0e-9, "Failure 3,1" );
    QVERIFY2( fabs( m0(3,2) - 15.0 ) < 1.0e-9, "Failure 3,2" );
    QVERIFY2( fabs( m0(3,3) - 16.0 ) < 1.0e-9, "Failure 3,3" );
}

////////////////////////////////////////////////////////////////////////////////

QTEST_APPLESS_MAIN(Matrix4x4Test)

////////////////////////////////////////////////////////////////////////////////

#include "test_fdm_matrix4x4.moc"
