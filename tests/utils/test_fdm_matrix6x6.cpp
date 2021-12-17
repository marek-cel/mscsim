#include <iostream>

#include <QString>
#include <QtTest>

#include <fdm/utils/fdm_Matrix6x6.h>

////////////////////////////////////////////////////////////////////////////////

using namespace std;

////////////////////////////////////////////////////////////////////////////////

class Matrix6x6Test : public QObject
{
    Q_OBJECT

public:

    Matrix6x6Test();

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

Matrix6x6Test::Matrix6x6Test() {}

////////////////////////////////////////////////////////////////////////////////

void Matrix6x6Test::initTestCase() {}

////////////////////////////////////////////////////////////////////////////////

void Matrix6x6Test::cleanupTestCase() {}

////////////////////////////////////////////////////////////////////////////////

void Matrix6x6Test::constructor()
{
    fdm::Matrix6x6 m;
    QVERIFY2( fabs( m(0,0) - 0.0 ) < 1.0e-9, "Failure 0,0 m" );
    QVERIFY2( fabs( m(0,1) - 0.0 ) < 1.0e-9, "Failure 0,1 m" );
    QVERIFY2( fabs( m(0,2) - 0.0 ) < 1.0e-9, "Failure 0,2 m" );
    QVERIFY2( fabs( m(0,3) - 0.0 ) < 1.0e-9, "Failure 0,3 m" );
    QVERIFY2( fabs( m(0,4) - 0.0 ) < 1.0e-9, "Failure 0,4 m" );
    QVERIFY2( fabs( m(0,5) - 0.0 ) < 1.0e-9, "Failure 0,5 m" );
    QVERIFY2( fabs( m(1,0) - 0.0 ) < 1.0e-9, "Failure 1,0 m" );
    QVERIFY2( fabs( m(1,1) - 0.0 ) < 1.0e-9, "Failure 1,1 m" );
    QVERIFY2( fabs( m(1,2) - 0.0 ) < 1.0e-9, "Failure 1,2 m" );
    QVERIFY2( fabs( m(1,3) - 0.0 ) < 1.0e-9, "Failure 1,3 m" );
    QVERIFY2( fabs( m(1,4) - 0.0 ) < 1.0e-9, "Failure 1,4 m" );
    QVERIFY2( fabs( m(1,5) - 0.0 ) < 1.0e-9, "Failure 1,5 m" );
    QVERIFY2( fabs( m(2,0) - 0.0 ) < 1.0e-9, "Failure 2,0 m" );
    QVERIFY2( fabs( m(2,1) - 0.0 ) < 1.0e-9, "Failure 2,1 m" );
    QVERIFY2( fabs( m(2,2) - 0.0 ) < 1.0e-9, "Failure 2,2 m" );
    QVERIFY2( fabs( m(2,3) - 0.0 ) < 1.0e-9, "Failure 2,3 m" );
    QVERIFY2( fabs( m(2,4) - 0.0 ) < 1.0e-9, "Failure 2,4 m" );
    QVERIFY2( fabs( m(2,5) - 0.0 ) < 1.0e-9, "Failure 2,5 m" );
    QVERIFY2( fabs( m(3,0) - 0.0 ) < 1.0e-9, "Failure 3,0 m" );
    QVERIFY2( fabs( m(3,1) - 0.0 ) < 1.0e-9, "Failure 3,1 m" );
    QVERIFY2( fabs( m(3,2) - 0.0 ) < 1.0e-9, "Failure 3,2 m" );
    QVERIFY2( fabs( m(3,3) - 0.0 ) < 1.0e-9, "Failure 3,3 m" );
    QVERIFY2( fabs( m(3,4) - 0.0 ) < 1.0e-9, "Failure 3,4 m" );
    QVERIFY2( fabs( m(3,5) - 0.0 ) < 1.0e-9, "Failure 3,5 m" );
    QVERIFY2( fabs( m(4,0) - 0.0 ) < 1.0e-9, "Failure 4,0 m" );
    QVERIFY2( fabs( m(4,1) - 0.0 ) < 1.0e-9, "Failure 4,1 m" );
    QVERIFY2( fabs( m(4,2) - 0.0 ) < 1.0e-9, "Failure 4,2 m" );
    QVERIFY2( fabs( m(4,3) - 0.0 ) < 1.0e-9, "Failure 4,3 m" );
    QVERIFY2( fabs( m(4,4) - 0.0 ) < 1.0e-9, "Failure 4,4 m" );
    QVERIFY2( fabs( m(4,5) - 0.0 ) < 1.0e-9, "Failure 4,5 m" );
    QVERIFY2( fabs( m(5,0) - 0.0 ) < 1.0e-9, "Failure 5,0 m" );
    QVERIFY2( fabs( m(5,1) - 0.0 ) < 1.0e-9, "Failure 5,1 m" );
    QVERIFY2( fabs( m(5,2) - 0.0 ) < 1.0e-9, "Failure 5,2 m" );
    QVERIFY2( fabs( m(5,3) - 0.0 ) < 1.0e-9, "Failure 5,3 m" );
    QVERIFY2( fabs( m(5,4) - 0.0 ) < 1.0e-9, "Failure 5,4 m" );
    QVERIFY2( fabs( m(5,5) - 0.0 ) < 1.0e-9, "Failure 5,5 m" );

    m(0,0) =   1.0;
    m(0,1) =   2.0;
    m(0,2) =   3.0;
    m(0,3) =   4.0;
    m(0,4) =   5.0;
    m(0,5) =   6.0;
    m(1,0) =   7.0;
    m(1,1) =   8.0;
    m(1,2) =   9.0;
    m(1,3) =  10.0;
    m(1,4) =  11.0;
    m(1,5) =  12.0;
    m(2,0) =  13.0;
    m(2,1) =  14.0;
    m(2,2) =  15.0;
    m(2,3) =  16.0;
    m(2,4) =  17.0;
    m(2,5) =  18.0;
    m(3,0) =  19.0;
    m(3,1) =  20.0;
    m(3,2) =  21.0;
    m(3,3) =  22.0;
    m(3,4) =  23.0;
    m(3,5) =  24.0;
    m(4,0) =  25.0;
    m(4,1) =  26.0;
    m(4,2) =  27.0;
    m(4,3) =  28.0;
    m(4,4) =  29.0;
    m(4,5) =  30.0;
    m(5,0) =  31.0;
    m(5,1) =  32.0;
    m(5,2) =  33.0;
    m(5,3) =  34.0;
    m(5,4) =  35.0;
    m(5,5) =  36.0;

    fdm::Matrix6x6 m1( m );
    QVERIFY2( fabs( m1(0,0) -  1.0 ) < 1.0e-9, "Failure 0,0 m1" );
    QVERIFY2( fabs( m1(0,1) -  2.0 ) < 1.0e-9, "Failure 0,1 m1" );
    QVERIFY2( fabs( m1(0,2) -  3.0 ) < 1.0e-9, "Failure 0,2 m1" );
    QVERIFY2( fabs( m1(0,3) -  4.0 ) < 1.0e-9, "Failure 0,3 m1" );
    QVERIFY2( fabs( m1(0,4) -  5.0 ) < 1.0e-9, "Failure 0,4 m1" );
    QVERIFY2( fabs( m1(0,5) -  6.0 ) < 1.0e-9, "Failure 0,5 m1" );
    QVERIFY2( fabs( m1(1,0) -  7.0 ) < 1.0e-9, "Failure 1,0 m1" );
    QVERIFY2( fabs( m1(1,1) -  8.0 ) < 1.0e-9, "Failure 1,1 m1" );
    QVERIFY2( fabs( m1(1,2) -  9.0 ) < 1.0e-9, "Failure 1,2 m1" );
    QVERIFY2( fabs( m1(1,3) - 10.0 ) < 1.0e-9, "Failure 1,3 m1" );
    QVERIFY2( fabs( m1(1,4) - 11.0 ) < 1.0e-9, "Failure 1,4 m1" );
    QVERIFY2( fabs( m1(1,5) - 12.0 ) < 1.0e-9, "Failure 1,5 m1" );
    QVERIFY2( fabs( m1(2,0) - 13.0 ) < 1.0e-9, "Failure 2,0 m1" );
    QVERIFY2( fabs( m1(2,1) - 14.0 ) < 1.0e-9, "Failure 2,1 m1" );
    QVERIFY2( fabs( m1(2,2) - 15.0 ) < 1.0e-9, "Failure 2,2 m1" );
    QVERIFY2( fabs( m1(2,3) - 16.0 ) < 1.0e-9, "Failure 2,3 m1" );
    QVERIFY2( fabs( m1(2,4) - 17.0 ) < 1.0e-9, "Failure 2,4 m1" );
    QVERIFY2( fabs( m1(2,5) - 18.0 ) < 1.0e-9, "Failure 2,5 m1" );
    QVERIFY2( fabs( m1(3,0) - 19.0 ) < 1.0e-9, "Failure 3,0 m1" );
    QVERIFY2( fabs( m1(3,1) - 20.0 ) < 1.0e-9, "Failure 3,1 m1" );
    QVERIFY2( fabs( m1(3,2) - 21.0 ) < 1.0e-9, "Failure 3,2 m1" );
    QVERIFY2( fabs( m1(3,3) - 22.0 ) < 1.0e-9, "Failure 3,3 m1" );
    QVERIFY2( fabs( m1(3,4) - 23.0 ) < 1.0e-9, "Failure 3,4 m1" );
    QVERIFY2( fabs( m1(3,5) - 24.0 ) < 1.0e-9, "Failure 3,5 m1" );
    QVERIFY2( fabs( m1(4,0) - 25.0 ) < 1.0e-9, "Failure 4,0 m1" );
    QVERIFY2( fabs( m1(4,1) - 26.0 ) < 1.0e-9, "Failure 4,1 m1" );
    QVERIFY2( fabs( m1(4,2) - 27.0 ) < 1.0e-9, "Failure 4,2 m1" );
    QVERIFY2( fabs( m1(4,3) - 28.0 ) < 1.0e-9, "Failure 4,3 m1" );
    QVERIFY2( fabs( m1(4,4) - 29.0 ) < 1.0e-9, "Failure 4,4 m1" );
    QVERIFY2( fabs( m1(4,5) - 30.0 ) < 1.0e-9, "Failure 4,5 m1" );
    QVERIFY2( fabs( m1(5,0) - 31.0 ) < 1.0e-9, "Failure 5,0 m1" );
    QVERIFY2( fabs( m1(5,1) - 32.0 ) < 1.0e-9, "Failure 5,1 m1" );
    QVERIFY2( fabs( m1(5,2) - 33.0 ) < 1.0e-9, "Failure 5,2 m1" );
    QVERIFY2( fabs( m1(5,3) - 34.0 ) < 1.0e-9, "Failure 5,3 m1" );
    QVERIFY2( fabs( m1(5,4) - 35.0 ) < 1.0e-9, "Failure 5,4 m1" );
    QVERIFY2( fabs( m1(5,5) - 36.0 ) < 1.0e-9, "Failure 5,5 m1" );

    double x[] = {
         1.0,  2.0,  3.0,  4.0,  5.0,  6.0,
         7.0,  8.0,  9.0, 10.0, 11.0, 12.0,
        13.0, 14.0, 15.0, 16.0, 17.0, 18.0,
        19.0, 20.0, 21.0, 22.0, 23.0, 24.0,
        25.0, 26.0, 27.0, 28.0, 29.0, 30.0,
        31.0, 32.0, 33.0, 34.0, 35.0, 36.0
    };

    fdm::Matrix6x6 m2( x );
    QVERIFY2( fabs( m2(0,0) -  1.0 ) < 1.0e-9, "Failure 0,0 m2" );
    QVERIFY2( fabs( m2(0,1) -  2.0 ) < 1.0e-9, "Failure 0,1 m2" );
    QVERIFY2( fabs( m2(0,2) -  3.0 ) < 1.0e-9, "Failure 0,2 m2" );
    QVERIFY2( fabs( m2(0,3) -  4.0 ) < 1.0e-9, "Failure 0,3 m2" );
    QVERIFY2( fabs( m2(0,4) -  5.0 ) < 1.0e-9, "Failure 0,4 m2" );
    QVERIFY2( fabs( m2(0,5) -  6.0 ) < 1.0e-9, "Failure 0,5 m2" );
    QVERIFY2( fabs( m2(1,0) -  7.0 ) < 1.0e-9, "Failure 1,0 m2" );
    QVERIFY2( fabs( m2(1,1) -  8.0 ) < 1.0e-9, "Failure 1,1 m2" );
    QVERIFY2( fabs( m2(1,2) -  9.0 ) < 1.0e-9, "Failure 1,2 m2" );
    QVERIFY2( fabs( m2(1,3) - 10.0 ) < 1.0e-9, "Failure 1,3 m2" );
    QVERIFY2( fabs( m2(1,4) - 11.0 ) < 1.0e-9, "Failure 1,4 m2" );
    QVERIFY2( fabs( m2(1,5) - 12.0 ) < 1.0e-9, "Failure 1,5 m2" );
    QVERIFY2( fabs( m2(2,0) - 13.0 ) < 1.0e-9, "Failure 2,0 m2" );
    QVERIFY2( fabs( m2(2,1) - 14.0 ) < 1.0e-9, "Failure 2,1 m2" );
    QVERIFY2( fabs( m2(2,2) - 15.0 ) < 1.0e-9, "Failure 2,2 m2" );
    QVERIFY2( fabs( m2(2,3) - 16.0 ) < 1.0e-9, "Failure 2,3 m2" );
    QVERIFY2( fabs( m2(2,4) - 17.0 ) < 1.0e-9, "Failure 2,4 m2" );
    QVERIFY2( fabs( m2(2,5) - 18.0 ) < 1.0e-9, "Failure 2,5 m2" );
    QVERIFY2( fabs( m2(3,0) - 19.0 ) < 1.0e-9, "Failure 3,0 m2" );
    QVERIFY2( fabs( m2(3,1) - 20.0 ) < 1.0e-9, "Failure 3,1 m2" );
    QVERIFY2( fabs( m2(3,2) - 21.0 ) < 1.0e-9, "Failure 3,2 m2" );
    QVERIFY2( fabs( m2(3,3) - 22.0 ) < 1.0e-9, "Failure 3,3 m2" );
    QVERIFY2( fabs( m2(3,4) - 23.0 ) < 1.0e-9, "Failure 3,4 m2" );
    QVERIFY2( fabs( m2(3,5) - 24.0 ) < 1.0e-9, "Failure 3,5 m2" );
    QVERIFY2( fabs( m2(4,0) - 25.0 ) < 1.0e-9, "Failure 4,0 m2" );
    QVERIFY2( fabs( m2(4,1) - 26.0 ) < 1.0e-9, "Failure 4,1 m2" );
    QVERIFY2( fabs( m2(4,2) - 27.0 ) < 1.0e-9, "Failure 4,2 m2" );
    QVERIFY2( fabs( m2(4,3) - 28.0 ) < 1.0e-9, "Failure 4,3 m2" );
    QVERIFY2( fabs( m2(4,4) - 29.0 ) < 1.0e-9, "Failure 4,4 m2" );
    QVERIFY2( fabs( m2(4,5) - 30.0 ) < 1.0e-9, "Failure 4,5 m2" );
    QVERIFY2( fabs( m2(5,0) - 31.0 ) < 1.0e-9, "Failure 5,0 m2" );
    QVERIFY2( fabs( m2(5,1) - 32.0 ) < 1.0e-9, "Failure 5,1 m2" );
    QVERIFY2( fabs( m2(5,2) - 33.0 ) < 1.0e-9, "Failure 5,2 m2" );
    QVERIFY2( fabs( m2(5,3) - 34.0 ) < 1.0e-9, "Failure 5,3 m2" );
    QVERIFY2( fabs( m2(5,4) - 35.0 ) < 1.0e-9, "Failure 5,4 m2" );
    QVERIFY2( fabs( m2(5,5) - 36.0 ) < 1.0e-9, "Failure 5,5 m2" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix6x6Test::transpose()
{
    double x[] = {
         1.0,  2.0,  3.0,  4.0,  5.0,  6.0,
         7.0,  8.0,  9.0, 10.0, 11.0, 12.0,
        13.0, 14.0, 15.0, 16.0, 17.0, 18.0,
        19.0, 20.0, 21.0, 22.0, 23.0, 24.0,
        25.0, 26.0, 27.0, 28.0, 29.0, 30.0,
        31.0, 32.0, 33.0, 34.0, 35.0, 36.0
    };

    fdm::Matrix6x6 m( x );

    m.transpose();

    QVERIFY2( fabs( m(0,0) -  1.0 ) < 1.0e-9, "Failure 0,0" );
    QVERIFY2( fabs( m(0,1) -  7.0 ) < 1.0e-9, "Failure 0,1" );
    QVERIFY2( fabs( m(0,2) - 13.0 ) < 1.0e-9, "Failure 0,2" );
    QVERIFY2( fabs( m(0,3) - 19.0 ) < 1.0e-9, "Failure 0,3" );
    QVERIFY2( fabs( m(0,4) - 25.0 ) < 1.0e-9, "Failure 0,4" );
    QVERIFY2( fabs( m(0,5) - 31.0 ) < 1.0e-9, "Failure 0,5" );
    QVERIFY2( fabs( m(1,0) -  2.0 ) < 1.0e-9, "Failure 1,0" );
    QVERIFY2( fabs( m(1,1) -  8.0 ) < 1.0e-9, "Failure 1,1" );
    QVERIFY2( fabs( m(1,2) - 14.0 ) < 1.0e-9, "Failure 1,2" );
    QVERIFY2( fabs( m(1,3) - 20.0 ) < 1.0e-9, "Failure 1,3" );
    QVERIFY2( fabs( m(1,4) - 26.0 ) < 1.0e-9, "Failure 1,4" );
    QVERIFY2( fabs( m(1,5) - 32.0 ) < 1.0e-9, "Failure 1,5" );
    QVERIFY2( fabs( m(2,0) -  3.0 ) < 1.0e-9, "Failure 2,0" );
    QVERIFY2( fabs( m(2,1) -  9.0 ) < 1.0e-9, "Failure 2,1" );
    QVERIFY2( fabs( m(2,2) - 15.0 ) < 1.0e-9, "Failure 2,2" );
    QVERIFY2( fabs( m(2,3) - 21.0 ) < 1.0e-9, "Failure 2,3" );
    QVERIFY2( fabs( m(2,4) - 27.0 ) < 1.0e-9, "Failure 2,4" );
    QVERIFY2( fabs( m(2,5) - 33.0 ) < 1.0e-9, "Failure 2,5" );
    QVERIFY2( fabs( m(3,0) -  4.0 ) < 1.0e-9, "Failure 3,0" );
    QVERIFY2( fabs( m(3,1) - 10.0 ) < 1.0e-9, "Failure 3,1" );
    QVERIFY2( fabs( m(3,2) - 16.0 ) < 1.0e-9, "Failure 3,2" );
    QVERIFY2( fabs( m(3,3) - 22.0 ) < 1.0e-9, "Failure 3,3" );
    QVERIFY2( fabs( m(3,4) - 28.0 ) < 1.0e-9, "Failure 3,4" );
    QVERIFY2( fabs( m(3,5) - 34.0 ) < 1.0e-9, "Failure 3,5" );
    QVERIFY2( fabs( m(4,0) -  5.0 ) < 1.0e-9, "Failure 4,0" );
    QVERIFY2( fabs( m(4,1) - 11.0 ) < 1.0e-9, "Failure 4,1" );
    QVERIFY2( fabs( m(4,2) - 17.0 ) < 1.0e-9, "Failure 4,2" );
    QVERIFY2( fabs( m(4,3) - 23.0 ) < 1.0e-9, "Failure 4,3" );
    QVERIFY2( fabs( m(4,4) - 29.0 ) < 1.0e-9, "Failure 4,4" );
    QVERIFY2( fabs( m(4,5) - 35.0 ) < 1.0e-9, "Failure 4,5" );
    QVERIFY2( fabs( m(5,0) -  6.0 ) < 1.0e-9, "Failure 5,0" );
    QVERIFY2( fabs( m(5,1) - 12.0 ) < 1.0e-9, "Failure 5,1" );
    QVERIFY2( fabs( m(5,2) - 18.0 ) < 1.0e-9, "Failure 5,2" );
    QVERIFY2( fabs( m(5,3) - 24.0 ) < 1.0e-9, "Failure 5,3" );
    QVERIFY2( fabs( m(5,4) - 30.0 ) < 1.0e-9, "Failure 5,4" );
    QVERIFY2( fabs( m(5,5) - 36.0 ) < 1.0e-9, "Failure 5,5" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix6x6Test::getTransposed()
{
    double x[] = {
         1.0,  2.0,  3.0,  4.0,  5.0,  6.0,
         7.0,  8.0,  9.0, 10.0, 11.0, 12.0,
        13.0, 14.0, 15.0, 16.0, 17.0, 18.0,
        19.0, 20.0, 21.0, 22.0, 23.0, 24.0,
        25.0, 26.0, 27.0, 28.0, 29.0, 30.0,
        31.0, 32.0, 33.0, 34.0, 35.0, 36.0
    };

    fdm::Matrix6x6 m0( x );

    fdm::Matrix6x6 m = m0.getTransposed();

    QVERIFY2( fabs( m(0,0) -  1.0 ) < 1.0e-9, "Failure 0,0" );
    QVERIFY2( fabs( m(0,1) -  7.0 ) < 1.0e-9, "Failure 0,1" );
    QVERIFY2( fabs( m(0,2) - 13.0 ) < 1.0e-9, "Failure 0,2" );
    QVERIFY2( fabs( m(0,3) - 19.0 ) < 1.0e-9, "Failure 0,3" );
    QVERIFY2( fabs( m(0,4) - 25.0 ) < 1.0e-9, "Failure 0,4" );
    QVERIFY2( fabs( m(0,5) - 31.0 ) < 1.0e-9, "Failure 0,5" );
    QVERIFY2( fabs( m(1,0) -  2.0 ) < 1.0e-9, "Failure 1,0" );
    QVERIFY2( fabs( m(1,1) -  8.0 ) < 1.0e-9, "Failure 1,1" );
    QVERIFY2( fabs( m(1,2) - 14.0 ) < 1.0e-9, "Failure 1,2" );
    QVERIFY2( fabs( m(1,3) - 20.0 ) < 1.0e-9, "Failure 1,3" );
    QVERIFY2( fabs( m(1,4) - 26.0 ) < 1.0e-9, "Failure 1,4" );
    QVERIFY2( fabs( m(1,5) - 32.0 ) < 1.0e-9, "Failure 1,5" );
    QVERIFY2( fabs( m(2,0) -  3.0 ) < 1.0e-9, "Failure 2,0" );
    QVERIFY2( fabs( m(2,1) -  9.0 ) < 1.0e-9, "Failure 2,1" );
    QVERIFY2( fabs( m(2,2) - 15.0 ) < 1.0e-9, "Failure 2,2" );
    QVERIFY2( fabs( m(2,3) - 21.0 ) < 1.0e-9, "Failure 2,3" );
    QVERIFY2( fabs( m(2,4) - 27.0 ) < 1.0e-9, "Failure 2,4" );
    QVERIFY2( fabs( m(2,5) - 33.0 ) < 1.0e-9, "Failure 2,5" );
    QVERIFY2( fabs( m(3,0) -  4.0 ) < 1.0e-9, "Failure 3,0" );
    QVERIFY2( fabs( m(3,1) - 10.0 ) < 1.0e-9, "Failure 3,1" );
    QVERIFY2( fabs( m(3,2) - 16.0 ) < 1.0e-9, "Failure 3,2" );
    QVERIFY2( fabs( m(3,3) - 22.0 ) < 1.0e-9, "Failure 3,3" );
    QVERIFY2( fabs( m(3,4) - 28.0 ) < 1.0e-9, "Failure 3,4" );
    QVERIFY2( fabs( m(3,5) - 34.0 ) < 1.0e-9, "Failure 3,5" );
    QVERIFY2( fabs( m(4,0) -  5.0 ) < 1.0e-9, "Failure 4,0" );
    QVERIFY2( fabs( m(4,1) - 11.0 ) < 1.0e-9, "Failure 4,1" );
    QVERIFY2( fabs( m(4,2) - 17.0 ) < 1.0e-9, "Failure 4,2" );
    QVERIFY2( fabs( m(4,3) - 23.0 ) < 1.0e-9, "Failure 4,3" );
    QVERIFY2( fabs( m(4,4) - 29.0 ) < 1.0e-9, "Failure 4,4" );
    QVERIFY2( fabs( m(4,5) - 35.0 ) < 1.0e-9, "Failure 4,5" );
    QVERIFY2( fabs( m(5,0) -  6.0 ) < 1.0e-9, "Failure 5,0" );
    QVERIFY2( fabs( m(5,1) - 12.0 ) < 1.0e-9, "Failure 5,1" );
    QVERIFY2( fabs( m(5,2) - 18.0 ) < 1.0e-9, "Failure 5,2" );
    QVERIFY2( fabs( m(5,3) - 24.0 ) < 1.0e-9, "Failure 5,3" );
    QVERIFY2( fabs( m(5,4) - 30.0 ) < 1.0e-9, "Failure 5,4" );
    QVERIFY2( fabs( m(5,5) - 36.0 ) < 1.0e-9, "Failure 5,5" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix6x6Test::operator_assign()
{
    double x[] = {
         1.0,  2.0,  3.0,  4.0,  5.0,  6.0,
         7.0,  8.0,  9.0, 10.0, 11.0, 12.0,
        13.0, 14.0, 15.0, 16.0, 17.0, 18.0,
        19.0, 20.0, 21.0, 22.0, 23.0, 24.0,
        25.0, 26.0, 27.0, 28.0, 29.0, 30.0,
        31.0, 32.0, 33.0, 34.0, 35.0, 36.0
    };

    fdm::Matrix6x6 m0( x );

    fdm::Matrix6x6 m = m0;

    QVERIFY2( fabs( m(0,0) -  1.0 ) < 1.0e-9, "Failure 0,0" );
    QVERIFY2( fabs( m(0,1) -  2.0 ) < 1.0e-9, "Failure 0,1" );
    QVERIFY2( fabs( m(0,2) -  3.0 ) < 1.0e-9, "Failure 0,2" );
    QVERIFY2( fabs( m(0,3) -  4.0 ) < 1.0e-9, "Failure 0,3" );
    QVERIFY2( fabs( m(0,4) -  5.0 ) < 1.0e-9, "Failure 0,4" );
    QVERIFY2( fabs( m(0,5) -  6.0 ) < 1.0e-9, "Failure 0,5" );
    QVERIFY2( fabs( m(1,0) -  7.0 ) < 1.0e-9, "Failure 1,0" );
    QVERIFY2( fabs( m(1,1) -  8.0 ) < 1.0e-9, "Failure 1,1" );
    QVERIFY2( fabs( m(1,2) -  9.0 ) < 1.0e-9, "Failure 1,2" );
    QVERIFY2( fabs( m(1,3) - 10.0 ) < 1.0e-9, "Failure 1,3" );
    QVERIFY2( fabs( m(1,4) - 11.0 ) < 1.0e-9, "Failure 1,4" );
    QVERIFY2( fabs( m(1,5) - 12.0 ) < 1.0e-9, "Failure 1,5" );
    QVERIFY2( fabs( m(2,0) - 13.0 ) < 1.0e-9, "Failure 2,0" );
    QVERIFY2( fabs( m(2,1) - 14.0 ) < 1.0e-9, "Failure 2,1" );
    QVERIFY2( fabs( m(2,2) - 15.0 ) < 1.0e-9, "Failure 2,2" );
    QVERIFY2( fabs( m(2,3) - 16.0 ) < 1.0e-9, "Failure 2,3" );
    QVERIFY2( fabs( m(2,4) - 17.0 ) < 1.0e-9, "Failure 2,4" );
    QVERIFY2( fabs( m(2,5) - 18.0 ) < 1.0e-9, "Failure 2,5" );
    QVERIFY2( fabs( m(3,0) - 19.0 ) < 1.0e-9, "Failure 3,0" );
    QVERIFY2( fabs( m(3,1) - 20.0 ) < 1.0e-9, "Failure 3,1" );
    QVERIFY2( fabs( m(3,2) - 21.0 ) < 1.0e-9, "Failure 3,2" );
    QVERIFY2( fabs( m(3,3) - 22.0 ) < 1.0e-9, "Failure 3,3" );
    QVERIFY2( fabs( m(3,4) - 23.0 ) < 1.0e-9, "Failure 3,4" );
    QVERIFY2( fabs( m(3,5) - 24.0 ) < 1.0e-9, "Failure 3,5" );
    QVERIFY2( fabs( m(4,0) - 25.0 ) < 1.0e-9, "Failure 4,0" );
    QVERIFY2( fabs( m(4,1) - 26.0 ) < 1.0e-9, "Failure 4,1" );
    QVERIFY2( fabs( m(4,2) - 27.0 ) < 1.0e-9, "Failure 4,2" );
    QVERIFY2( fabs( m(4,3) - 28.0 ) < 1.0e-9, "Failure 4,3" );
    QVERIFY2( fabs( m(4,4) - 29.0 ) < 1.0e-9, "Failure 4,4" );
    QVERIFY2( fabs( m(4,5) - 30.0 ) < 1.0e-9, "Failure 4,5" );
    QVERIFY2( fabs( m(5,0) - 31.0 ) < 1.0e-9, "Failure 5,0" );
    QVERIFY2( fabs( m(5,1) - 32.0 ) < 1.0e-9, "Failure 5,1" );
    QVERIFY2( fabs( m(5,2) - 33.0 ) < 1.0e-9, "Failure 5,2" );
    QVERIFY2( fabs( m(5,3) - 34.0 ) < 1.0e-9, "Failure 5,3" );
    QVERIFY2( fabs( m(5,4) - 35.0 ) < 1.0e-9, "Failure 5,4" );
    QVERIFY2( fabs( m(5,5) - 36.0 ) < 1.0e-9, "Failure 5,5" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix6x6Test::operator_add()
{
    double x[] = {
         1.0,  2.0,  3.0,  4.0,  5.0,  6.0,
         7.0,  8.0,  9.0, 10.0, 11.0, 12.0,
        13.0, 14.0, 15.0, 16.0, 17.0, 18.0,
        19.0, 20.0, 21.0, 22.0, 23.0, 24.0,
        25.0, 26.0, 27.0, 28.0, 29.0, 30.0,
        31.0, 32.0, 33.0, 34.0, 35.0, 36.0
    };

    fdm::Matrix6x6 m0;
    fdm::Matrix6x6 m1( x );

    fdm::Matrix6x6 m = m0 + m1;

    QVERIFY2( fabs( m(0,0) -  1.0 ) < 1.0e-9, "Failure 0,0" );
    QVERIFY2( fabs( m(0,1) -  2.0 ) < 1.0e-9, "Failure 0,1" );
    QVERIFY2( fabs( m(0,2) -  3.0 ) < 1.0e-9, "Failure 0,2" );
    QVERIFY2( fabs( m(0,3) -  4.0 ) < 1.0e-9, "Failure 0,3" );
    QVERIFY2( fabs( m(0,4) -  5.0 ) < 1.0e-9, "Failure 0,4" );
    QVERIFY2( fabs( m(0,5) -  6.0 ) < 1.0e-9, "Failure 0,5" );
    QVERIFY2( fabs( m(1,0) -  7.0 ) < 1.0e-9, "Failure 1,0" );
    QVERIFY2( fabs( m(1,1) -  8.0 ) < 1.0e-9, "Failure 1,1" );
    QVERIFY2( fabs( m(1,2) -  9.0 ) < 1.0e-9, "Failure 1,2" );
    QVERIFY2( fabs( m(1,3) - 10.0 ) < 1.0e-9, "Failure 1,3" );
    QVERIFY2( fabs( m(1,4) - 11.0 ) < 1.0e-9, "Failure 1,4" );
    QVERIFY2( fabs( m(1,5) - 12.0 ) < 1.0e-9, "Failure 1,5" );
    QVERIFY2( fabs( m(2,0) - 13.0 ) < 1.0e-9, "Failure 2,0" );
    QVERIFY2( fabs( m(2,1) - 14.0 ) < 1.0e-9, "Failure 2,1" );
    QVERIFY2( fabs( m(2,2) - 15.0 ) < 1.0e-9, "Failure 2,2" );
    QVERIFY2( fabs( m(2,3) - 16.0 ) < 1.0e-9, "Failure 2,3" );
    QVERIFY2( fabs( m(2,4) - 17.0 ) < 1.0e-9, "Failure 2,4" );
    QVERIFY2( fabs( m(2,5) - 18.0 ) < 1.0e-9, "Failure 2,5" );
    QVERIFY2( fabs( m(3,0) - 19.0 ) < 1.0e-9, "Failure 3,0" );
    QVERIFY2( fabs( m(3,1) - 20.0 ) < 1.0e-9, "Failure 3,1" );
    QVERIFY2( fabs( m(3,2) - 21.0 ) < 1.0e-9, "Failure 3,2" );
    QVERIFY2( fabs( m(3,3) - 22.0 ) < 1.0e-9, "Failure 3,3" );
    QVERIFY2( fabs( m(3,4) - 23.0 ) < 1.0e-9, "Failure 3,4" );
    QVERIFY2( fabs( m(3,5) - 24.0 ) < 1.0e-9, "Failure 3,5" );
    QVERIFY2( fabs( m(4,0) - 25.0 ) < 1.0e-9, "Failure 4,0" );
    QVERIFY2( fabs( m(4,1) - 26.0 ) < 1.0e-9, "Failure 4,1" );
    QVERIFY2( fabs( m(4,2) - 27.0 ) < 1.0e-9, "Failure 4,2" );
    QVERIFY2( fabs( m(4,3) - 28.0 ) < 1.0e-9, "Failure 4,3" );
    QVERIFY2( fabs( m(4,4) - 29.0 ) < 1.0e-9, "Failure 4,4" );
    QVERIFY2( fabs( m(4,5) - 30.0 ) < 1.0e-9, "Failure 4,5" );
    QVERIFY2( fabs( m(5,0) - 31.0 ) < 1.0e-9, "Failure 5,0" );
    QVERIFY2( fabs( m(5,1) - 32.0 ) < 1.0e-9, "Failure 5,1" );
    QVERIFY2( fabs( m(5,2) - 33.0 ) < 1.0e-9, "Failure 5,2" );
    QVERIFY2( fabs( m(5,3) - 34.0 ) < 1.0e-9, "Failure 5,3" );
    QVERIFY2( fabs( m(5,4) - 35.0 ) < 1.0e-9, "Failure 5,4" );
    QVERIFY2( fabs( m(5,5) - 36.0 ) < 1.0e-9, "Failure 5,5" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix6x6Test::operator_substract()
{
    double x[] = {
         1.0,  2.0,  3.0,  4.0,  5.0,  6.0,
         7.0,  8.0,  9.0, 10.0, 11.0, 12.0,
        13.0, 14.0, 15.0, 16.0, 17.0, 18.0,
        19.0, 20.0, 21.0, 22.0, 23.0, 24.0,
        25.0, 26.0, 27.0, 28.0, 29.0, 30.0,
        31.0, 32.0, 33.0, 34.0, 35.0, 36.0
    };

    fdm::Matrix6x6 m0;
    fdm::Matrix6x6 m1( x );

    fdm::Matrix6x6 m = m0 - m1;

    QVERIFY2( fabs( m(0,0) +  1.0 ) < 1.0e-9, "Failure 0,0" );
    QVERIFY2( fabs( m(0,1) +  2.0 ) < 1.0e-9, "Failure 0,1" );
    QVERIFY2( fabs( m(0,2) +  3.0 ) < 1.0e-9, "Failure 0,2" );
    QVERIFY2( fabs( m(0,3) +  4.0 ) < 1.0e-9, "Failure 0,3" );
    QVERIFY2( fabs( m(0,4) +  5.0 ) < 1.0e-9, "Failure 0,4" );
    QVERIFY2( fabs( m(0,5) +  6.0 ) < 1.0e-9, "Failure 0,5" );
    QVERIFY2( fabs( m(1,0) +  7.0 ) < 1.0e-9, "Failure 1,0" );
    QVERIFY2( fabs( m(1,1) +  8.0 ) < 1.0e-9, "Failure 1,1" );
    QVERIFY2( fabs( m(1,2) +  9.0 ) < 1.0e-9, "Failure 1,2" );
    QVERIFY2( fabs( m(1,3) + 10.0 ) < 1.0e-9, "Failure 1,3" );
    QVERIFY2( fabs( m(1,4) + 11.0 ) < 1.0e-9, "Failure 1,4" );
    QVERIFY2( fabs( m(1,5) + 12.0 ) < 1.0e-9, "Failure 1,5" );
    QVERIFY2( fabs( m(2,0) + 13.0 ) < 1.0e-9, "Failure 2,0" );
    QVERIFY2( fabs( m(2,1) + 14.0 ) < 1.0e-9, "Failure 2,1" );
    QVERIFY2( fabs( m(2,2) + 15.0 ) < 1.0e-9, "Failure 2,2" );
    QVERIFY2( fabs( m(2,3) + 16.0 ) < 1.0e-9, "Failure 2,3" );
    QVERIFY2( fabs( m(2,4) + 17.0 ) < 1.0e-9, "Failure 2,4" );
    QVERIFY2( fabs( m(2,5) + 18.0 ) < 1.0e-9, "Failure 2,5" );
    QVERIFY2( fabs( m(3,0) + 19.0 ) < 1.0e-9, "Failure 3,0" );
    QVERIFY2( fabs( m(3,1) + 20.0 ) < 1.0e-9, "Failure 3,1" );
    QVERIFY2( fabs( m(3,2) + 21.0 ) < 1.0e-9, "Failure 3,2" );
    QVERIFY2( fabs( m(3,3) + 22.0 ) < 1.0e-9, "Failure 3,3" );
    QVERIFY2( fabs( m(3,4) + 23.0 ) < 1.0e-9, "Failure 3,4" );
    QVERIFY2( fabs( m(3,5) + 24.0 ) < 1.0e-9, "Failure 3,5" );
    QVERIFY2( fabs( m(4,0) + 25.0 ) < 1.0e-9, "Failure 4,0" );
    QVERIFY2( fabs( m(4,1) + 26.0 ) < 1.0e-9, "Failure 4,1" );
    QVERIFY2( fabs( m(4,2) + 27.0 ) < 1.0e-9, "Failure 4,2" );
    QVERIFY2( fabs( m(4,3) + 28.0 ) < 1.0e-9, "Failure 4,3" );
    QVERIFY2( fabs( m(4,4) + 29.0 ) < 1.0e-9, "Failure 4,4" );
    QVERIFY2( fabs( m(4,5) + 30.0 ) < 1.0e-9, "Failure 4,5" );
    QVERIFY2( fabs( m(5,0) + 31.0 ) < 1.0e-9, "Failure 5,0" );
    QVERIFY2( fabs( m(5,1) + 32.0 ) < 1.0e-9, "Failure 5,1" );
    QVERIFY2( fabs( m(5,2) + 33.0 ) < 1.0e-9, "Failure 5,2" );
    QVERIFY2( fabs( m(5,3) + 34.0 ) < 1.0e-9, "Failure 5,3" );
    QVERIFY2( fabs( m(5,4) + 35.0 ) < 1.0e-9, "Failure 5,4" );
    QVERIFY2( fabs( m(5,5) + 36.0 ) < 1.0e-9, "Failure 5,5" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix6x6Test::operator_multiply_by_scalar()
{
    double x[] = {
         1.0,  2.0,  3.0,  4.0,  5.0,  6.0,
         7.0,  8.0,  9.0, 10.0, 11.0, 12.0,
        13.0, 14.0, 15.0, 16.0, 17.0, 18.0,
        19.0, 20.0, 21.0, 22.0, 23.0, 24.0,
        25.0, 26.0, 27.0, 28.0, 29.0, 30.0,
        31.0, 32.0, 33.0, 34.0, 35.0, 36.0
    };

    fdm::Matrix6x6 m0( x );

    fdm::Matrix6x6 m = m0 * 2.0;

    QVERIFY2( fabs( m(0,0) -  2.0 ) < 1.0e-9, "Failure 0,0" );
    QVERIFY2( fabs( m(0,1) -  4.0 ) < 1.0e-9, "Failure 0,1" );
    QVERIFY2( fabs( m(0,2) -  6.0 ) < 1.0e-9, "Failure 0,2" );
    QVERIFY2( fabs( m(0,3) -  8.0 ) < 1.0e-9, "Failure 0,3" );
    QVERIFY2( fabs( m(0,4) - 10.0 ) < 1.0e-9, "Failure 0,4" );
    QVERIFY2( fabs( m(0,5) - 12.0 ) < 1.0e-9, "Failure 0,5" );
    QVERIFY2( fabs( m(1,0) - 14.0 ) < 1.0e-9, "Failure 1,0" );
    QVERIFY2( fabs( m(1,1) - 16.0 ) < 1.0e-9, "Failure 1,1" );
    QVERIFY2( fabs( m(1,2) - 18.0 ) < 1.0e-9, "Failure 1,2" );
    QVERIFY2( fabs( m(1,3) - 20.0 ) < 1.0e-9, "Failure 1,3" );
    QVERIFY2( fabs( m(1,4) - 22.0 ) < 1.0e-9, "Failure 1,4" );
    QVERIFY2( fabs( m(1,5) - 24.0 ) < 1.0e-9, "Failure 1,5" );
    QVERIFY2( fabs( m(2,0) - 26.0 ) < 1.0e-9, "Failure 2,0" );
    QVERIFY2( fabs( m(2,1) - 28.0 ) < 1.0e-9, "Failure 2,1" );
    QVERIFY2( fabs( m(2,2) - 30.0 ) < 1.0e-9, "Failure 2,2" );
    QVERIFY2( fabs( m(2,3) - 32.0 ) < 1.0e-9, "Failure 2,3" );
    QVERIFY2( fabs( m(2,4) - 34.0 ) < 1.0e-9, "Failure 2,4" );
    QVERIFY2( fabs( m(2,5) - 36.0 ) < 1.0e-9, "Failure 2,5" );
    QVERIFY2( fabs( m(3,0) - 38.0 ) < 1.0e-9, "Failure 3,0" );
    QVERIFY2( fabs( m(3,1) - 40.0 ) < 1.0e-9, "Failure 3,1" );
    QVERIFY2( fabs( m(3,2) - 42.0 ) < 1.0e-9, "Failure 3,2" );
    QVERIFY2( fabs( m(3,3) - 44.0 ) < 1.0e-9, "Failure 3,3" );
    QVERIFY2( fabs( m(3,4) - 46.0 ) < 1.0e-9, "Failure 3,4" );
    QVERIFY2( fabs( m(3,5) - 48.0 ) < 1.0e-9, "Failure 3,5" );
    QVERIFY2( fabs( m(4,0) - 50.0 ) < 1.0e-9, "Failure 4,0" );
    QVERIFY2( fabs( m(4,1) - 52.0 ) < 1.0e-9, "Failure 4,1" );
    QVERIFY2( fabs( m(4,2) - 54.0 ) < 1.0e-9, "Failure 4,2" );
    QVERIFY2( fabs( m(4,3) - 56.0 ) < 1.0e-9, "Failure 4,3" );
    QVERIFY2( fabs( m(4,4) - 58.0 ) < 1.0e-9, "Failure 4,4" );
    QVERIFY2( fabs( m(4,5) - 60.0 ) < 1.0e-9, "Failure 4,5" );
    QVERIFY2( fabs( m(5,0) - 62.0 ) < 1.0e-9, "Failure 5,0" );
    QVERIFY2( fabs( m(5,1) - 64.0 ) < 1.0e-9, "Failure 5,1" );
    QVERIFY2( fabs( m(5,2) - 66.0 ) < 1.0e-9, "Failure 5,2" );
    QVERIFY2( fabs( m(5,3) - 68.0 ) < 1.0e-9, "Failure 5,3" );
    QVERIFY2( fabs( m(5,4) - 70.0 ) < 1.0e-9, "Failure 5,4" );
    QVERIFY2( fabs( m(5,5) - 72.0 ) < 1.0e-9, "Failure 5,5" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix6x6Test::operator_multiply_by_matrix()
{
    double x0[] = {
         1.0,  2.0,  3.0,  4.0,  5.0,  6.0,
         7.0,  8.0,  9.0, 10.0, 11.0, 12.0,
        13.0, 14.0, 15.0, 16.0, 17.0, 18.0,
        19.0, 20.0, 21.0, 22.0, 23.0, 24.0,
        25.0, 26.0, 27.0, 28.0, 29.0, 30.0,
        31.0, 32.0, 33.0, 34.0, 35.0, 36.0
    };

    double x1[] = {
        31.0, 32.0, 33.0, 34.0, 35.0, 36.0,
        25.0, 26.0, 27.0, 28.0, 29.0, 30.0,
        19.0, 20.0, 21.0, 22.0, 23.0, 24.0,
        13.0, 14.0, 15.0, 16.0, 17.0, 18.0,
         7.0,  8.0,  9.0, 10.0, 11.0, 12.0,
         1.0,  2.0,  3.0,  4.0,  5.0,  6.0
    };

    fdm::Matrix6x6 m0( x0 );
    fdm::Matrix6x6 m1( x1 );

    fdm::Matrix6x6 m = m0 * m1;

    QVERIFY2( fabs( m(0,0) -  231.0 ) < 1.0e-9, "Failure 0,0" );
    QVERIFY2( fabs( m(0,1) -  252.0 ) < 1.0e-9, "Failure 0,1" );
    QVERIFY2( fabs( m(0,2) -  273.0 ) < 1.0e-9, "Failure 0,2" );
    QVERIFY2( fabs( m(0,3) -  294.0 ) < 1.0e-9, "Failure 0,3" );
    QVERIFY2( fabs( m(0,4) -  315.0 ) < 1.0e-9, "Failure 0,4" );
    QVERIFY2( fabs( m(0,5) -  336.0 ) < 1.0e-9, "Failure 0,5" );

    QVERIFY2( fabs( m(1,0) -  807.0 ) < 1.0e-9, "Failure 1,0" );
    QVERIFY2( fabs( m(1,1) -  864.0 ) < 1.0e-9, "Failure 1,1" );
    QVERIFY2( fabs( m(1,2) -  921.0 ) < 1.0e-9, "Failure 1,2" );
    QVERIFY2( fabs( m(1,3) -  978.0 ) < 1.0e-9, "Failure 1,3" );
    QVERIFY2( fabs( m(1,4) - 1035.0 ) < 1.0e-9, "Failure 1,4" );
    QVERIFY2( fabs( m(1,5) - 1092.0 ) < 1.0e-9, "Failure 1,5" );

    QVERIFY2( fabs( m(2,0) - 1383.0 ) < 1.0e-9, "Failure 2,0" );
    QVERIFY2( fabs( m(2,1) - 1476.0 ) < 1.0e-9, "Failure 2,1" );
    QVERIFY2( fabs( m(2,2) - 1569.0 ) < 1.0e-9, "Failure 2,2" );
    QVERIFY2( fabs( m(2,3) - 1662.0 ) < 1.0e-9, "Failure 2,3" );
    QVERIFY2( fabs( m(2,4) - 1755.0 ) < 1.0e-9, "Failure 2,4" );
    QVERIFY2( fabs( m(2,5) - 1848.0 ) < 1.0e-9, "Failure 2,5" );

    QVERIFY2( fabs( m(3,0) - 1959.0 ) < 1.0e-9, "Failure 3,0" );
    QVERIFY2( fabs( m(3,1) - 2088.0 ) < 1.0e-9, "Failure 3,1" );
    QVERIFY2( fabs( m(3,2) - 2217.0 ) < 1.0e-9, "Failure 3,2" );
    QVERIFY2( fabs( m(3,3) - 2346.0 ) < 1.0e-9, "Failure 3,3" );
    QVERIFY2( fabs( m(3,4) - 2475.0 ) < 1.0e-9, "Failure 3,4" );
    QVERIFY2( fabs( m(3,5) - 2604.0 ) < 1.0e-9, "Failure 3,5" );

    QVERIFY2( fabs( m(4,0) - 2535.0 ) < 1.0e-9, "Failure 4,0" );
    QVERIFY2( fabs( m(4,1) - 2700.0 ) < 1.0e-9, "Failure 4,1" );
    QVERIFY2( fabs( m(4,2) - 2865.0 ) < 1.0e-9, "Failure 4,2" );
    QVERIFY2( fabs( m(4,3) - 3030.0 ) < 1.0e-9, "Failure 4,3" );
    QVERIFY2( fabs( m(4,4) - 3195.0 ) < 1.0e-9, "Failure 4,4" );
    QVERIFY2( fabs( m(4,5) - 3360.0 ) < 1.0e-9, "Failure 4,5" );

    QVERIFY2( fabs( m(5,0) - 3111.0 ) < 1.0e-9, "Failure 5,0" );
    QVERIFY2( fabs( m(5,1) - 3312.0 ) < 1.0e-9, "Failure 5,1" );
    QVERIFY2( fabs( m(5,2) - 3513.0 ) < 1.0e-9, "Failure 5,2" );
    QVERIFY2( fabs( m(5,3) - 3714.0 ) < 1.0e-9, "Failure 5,3" );
    QVERIFY2( fabs( m(5,4) - 3915.0 ) < 1.0e-9, "Failure 5,4" );
    QVERIFY2( fabs( m(5,5) - 4116.0 ) < 1.0e-9, "Failure 5,5" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix6x6Test::operator_multiply_by_vector()
{
    double x0[] = {
         1.0,  2.0,  3.0,  4.0,  5.0,  6.0,
         7.0,  8.0,  9.0, 10.0, 11.0, 12.0,
        13.0, 14.0, 15.0, 16.0, 17.0, 18.0,
        19.0, 20.0, 21.0, 22.0, 23.0, 24.0,
        25.0, 26.0, 27.0, 28.0, 29.0, 30.0,
        31.0, 32.0, 33.0, 34.0, 35.0, 36.0
    };

    double x1[] = { 1.0,  2.0,  3.0,  4.0,  5.0,  6.0 };

    fdm::Matrix6x6 m0( x0 );

    fdm::Vector6 v1( x1 );

    fdm::Vector6 v = m0 * v1;

    QVERIFY2( fabs( v(0) -  91.0 ) < 1.0e-9, "Failure 0,0" );
    QVERIFY2( fabs( v(1) - 217.0 ) < 1.0e-9, "Failure 0,1" );
    QVERIFY2( fabs( v(2) - 343.0 ) < 1.0e-9, "Failure 0,2" );
    QVERIFY2( fabs( v(3) - 469.0 ) < 1.0e-9, "Failure 0,3" );
    QVERIFY2( fabs( v(4) - 595.0 ) < 1.0e-9, "Failure 0,4" );
    QVERIFY2( fabs( v(5) - 721.0 ) < 1.0e-9, "Failure 0,5" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix6x6Test::operator_divide_by_scalar()
{
    double x[] = {
         2.0,  4.0,  6.0,  8.0, 10.0, 12.0,
        14.0, 16.0, 18.0, 20.0, 22.0, 24.0,
        26.0, 28.0, 30.0, 32.0, 34.0, 36.0,
        38.0, 40.0, 42.0, 44.0, 46.0, 48.0,
        50.0, 52.0, 54.0, 56.0, 58.0, 60.0,
        62.0, 64.0, 66.0, 68.0, 70.0, 72.0
    };

    fdm::Matrix6x6 m0( x );

    fdm::Matrix6x6 m = m0 / 2.0;

    QVERIFY2( fabs( m(0,0) -  1.0 ) < 1.0e-9, "Failure 0,0" );
    QVERIFY2( fabs( m(0,1) -  2.0 ) < 1.0e-9, "Failure 0,1" );
    QVERIFY2( fabs( m(0,2) -  3.0 ) < 1.0e-9, "Failure 0,2" );
    QVERIFY2( fabs( m(0,3) -  4.0 ) < 1.0e-9, "Failure 0,3" );
    QVERIFY2( fabs( m(0,4) -  5.0 ) < 1.0e-9, "Failure 0,4" );
    QVERIFY2( fabs( m(0,5) -  6.0 ) < 1.0e-9, "Failure 0,5" );
    QVERIFY2( fabs( m(1,0) -  7.0 ) < 1.0e-9, "Failure 1,0" );
    QVERIFY2( fabs( m(1,1) -  8.0 ) < 1.0e-9, "Failure 1,1" );
    QVERIFY2( fabs( m(1,2) -  9.0 ) < 1.0e-9, "Failure 1,2" );
    QVERIFY2( fabs( m(1,3) - 10.0 ) < 1.0e-9, "Failure 1,3" );
    QVERIFY2( fabs( m(1,4) - 11.0 ) < 1.0e-9, "Failure 1,4" );
    QVERIFY2( fabs( m(1,5) - 12.0 ) < 1.0e-9, "Failure 1,5" );
    QVERIFY2( fabs( m(2,0) - 13.0 ) < 1.0e-9, "Failure 2,0" );
    QVERIFY2( fabs( m(2,1) - 14.0 ) < 1.0e-9, "Failure 2,1" );
    QVERIFY2( fabs( m(2,2) - 15.0 ) < 1.0e-9, "Failure 2,2" );
    QVERIFY2( fabs( m(2,3) - 16.0 ) < 1.0e-9, "Failure 2,3" );
    QVERIFY2( fabs( m(2,4) - 17.0 ) < 1.0e-9, "Failure 2,4" );
    QVERIFY2( fabs( m(2,5) - 18.0 ) < 1.0e-9, "Failure 2,5" );
    QVERIFY2( fabs( m(3,0) - 19.0 ) < 1.0e-9, "Failure 3,0" );
    QVERIFY2( fabs( m(3,1) - 20.0 ) < 1.0e-9, "Failure 3,1" );
    QVERIFY2( fabs( m(3,2) - 21.0 ) < 1.0e-9, "Failure 3,2" );
    QVERIFY2( fabs( m(3,3) - 22.0 ) < 1.0e-9, "Failure 3,3" );
    QVERIFY2( fabs( m(3,4) - 23.0 ) < 1.0e-9, "Failure 3,4" );
    QVERIFY2( fabs( m(3,5) - 24.0 ) < 1.0e-9, "Failure 3,5" );
    QVERIFY2( fabs( m(4,0) - 25.0 ) < 1.0e-9, "Failure 4,0" );
    QVERIFY2( fabs( m(4,1) - 26.0 ) < 1.0e-9, "Failure 4,1" );
    QVERIFY2( fabs( m(4,2) - 27.0 ) < 1.0e-9, "Failure 4,2" );
    QVERIFY2( fabs( m(4,3) - 28.0 ) < 1.0e-9, "Failure 4,3" );
    QVERIFY2( fabs( m(4,4) - 29.0 ) < 1.0e-9, "Failure 4,4" );
    QVERIFY2( fabs( m(4,5) - 30.0 ) < 1.0e-9, "Failure 4,5" );
    QVERIFY2( fabs( m(5,0) - 31.0 ) < 1.0e-9, "Failure 5,0" );
    QVERIFY2( fabs( m(5,1) - 32.0 ) < 1.0e-9, "Failure 5,1" );
    QVERIFY2( fabs( m(5,2) - 33.0 ) < 1.0e-9, "Failure 5,2" );
    QVERIFY2( fabs( m(5,3) - 34.0 ) < 1.0e-9, "Failure 5,3" );
    QVERIFY2( fabs( m(5,4) - 35.0 ) < 1.0e-9, "Failure 5,4" );
    QVERIFY2( fabs( m(5,5) - 36.0 ) < 1.0e-9, "Failure 5,5" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix6x6Test::operator_unary_add()
{
    double x[] = {
         1.0,  2.0,  3.0,  4.0,  5.0,  6.0,
         7.0,  8.0,  9.0, 10.0, 11.0, 12.0,
        13.0, 14.0, 15.0, 16.0, 17.0, 18.0,
        19.0, 20.0, 21.0, 22.0, 23.0, 24.0,
        25.0, 26.0, 27.0, 28.0, 29.0, 30.0,
        31.0, 32.0, 33.0, 34.0, 35.0, 36.0
    };

    fdm::Matrix6x6 m;
    fdm::Matrix6x6 m1( x );

    m += m1;

    QVERIFY2( fabs( m(0,0) -  1.0 ) < 1.0e-9, "Failure 0,0" );
    QVERIFY2( fabs( m(0,1) -  2.0 ) < 1.0e-9, "Failure 0,1" );
    QVERIFY2( fabs( m(0,2) -  3.0 ) < 1.0e-9, "Failure 0,2" );
    QVERIFY2( fabs( m(0,3) -  4.0 ) < 1.0e-9, "Failure 0,3" );
    QVERIFY2( fabs( m(0,4) -  5.0 ) < 1.0e-9, "Failure 0,4" );
    QVERIFY2( fabs( m(0,5) -  6.0 ) < 1.0e-9, "Failure 0,5" );
    QVERIFY2( fabs( m(1,0) -  7.0 ) < 1.0e-9, "Failure 1,0" );
    QVERIFY2( fabs( m(1,1) -  8.0 ) < 1.0e-9, "Failure 1,1" );
    QVERIFY2( fabs( m(1,2) -  9.0 ) < 1.0e-9, "Failure 1,2" );
    QVERIFY2( fabs( m(1,3) - 10.0 ) < 1.0e-9, "Failure 1,3" );
    QVERIFY2( fabs( m(1,4) - 11.0 ) < 1.0e-9, "Failure 1,4" );
    QVERIFY2( fabs( m(1,5) - 12.0 ) < 1.0e-9, "Failure 1,5" );
    QVERIFY2( fabs( m(2,0) - 13.0 ) < 1.0e-9, "Failure 2,0" );
    QVERIFY2( fabs( m(2,1) - 14.0 ) < 1.0e-9, "Failure 2,1" );
    QVERIFY2( fabs( m(2,2) - 15.0 ) < 1.0e-9, "Failure 2,2" );
    QVERIFY2( fabs( m(2,3) - 16.0 ) < 1.0e-9, "Failure 2,3" );
    QVERIFY2( fabs( m(2,4) - 17.0 ) < 1.0e-9, "Failure 2,4" );
    QVERIFY2( fabs( m(2,5) - 18.0 ) < 1.0e-9, "Failure 2,5" );
    QVERIFY2( fabs( m(3,0) - 19.0 ) < 1.0e-9, "Failure 3,0" );
    QVERIFY2( fabs( m(3,1) - 20.0 ) < 1.0e-9, "Failure 3,1" );
    QVERIFY2( fabs( m(3,2) - 21.0 ) < 1.0e-9, "Failure 3,2" );
    QVERIFY2( fabs( m(3,3) - 22.0 ) < 1.0e-9, "Failure 3,3" );
    QVERIFY2( fabs( m(3,4) - 23.0 ) < 1.0e-9, "Failure 3,4" );
    QVERIFY2( fabs( m(3,5) - 24.0 ) < 1.0e-9, "Failure 3,5" );
    QVERIFY2( fabs( m(4,0) - 25.0 ) < 1.0e-9, "Failure 4,0" );
    QVERIFY2( fabs( m(4,1) - 26.0 ) < 1.0e-9, "Failure 4,1" );
    QVERIFY2( fabs( m(4,2) - 27.0 ) < 1.0e-9, "Failure 4,2" );
    QVERIFY2( fabs( m(4,3) - 28.0 ) < 1.0e-9, "Failure 4,3" );
    QVERIFY2( fabs( m(4,4) - 29.0 ) < 1.0e-9, "Failure 4,4" );
    QVERIFY2( fabs( m(4,5) - 30.0 ) < 1.0e-9, "Failure 4,5" );
    QVERIFY2( fabs( m(5,0) - 31.0 ) < 1.0e-9, "Failure 5,0" );
    QVERIFY2( fabs( m(5,1) - 32.0 ) < 1.0e-9, "Failure 5,1" );
    QVERIFY2( fabs( m(5,2) - 33.0 ) < 1.0e-9, "Failure 5,2" );
    QVERIFY2( fabs( m(5,3) - 34.0 ) < 1.0e-9, "Failure 5,3" );
    QVERIFY2( fabs( m(5,4) - 35.0 ) < 1.0e-9, "Failure 5,4" );
    QVERIFY2( fabs( m(5,5) - 36.0 ) < 1.0e-9, "Failure 5,5" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix6x6Test::operator_unary_substract()
{
    double x[] = {
         1.0,  2.0,  3.0,  4.0,  5.0,  6.0,
         7.0,  8.0,  9.0, 10.0, 11.0, 12.0,
        13.0, 14.0, 15.0, 16.0, 17.0, 18.0,
        19.0, 20.0, 21.0, 22.0, 23.0, 24.0,
        25.0, 26.0, 27.0, 28.0, 29.0, 30.0,
        31.0, 32.0, 33.0, 34.0, 35.0, 36.0
    };

    fdm::Matrix6x6 m;
    fdm::Matrix6x6 m1( x );

    m -= m1;

    QVERIFY2( fabs( m(0,0) +  1.0 ) < 1.0e-9, "Failure 0,0" );
    QVERIFY2( fabs( m(0,1) +  2.0 ) < 1.0e-9, "Failure 0,1" );
    QVERIFY2( fabs( m(0,2) +  3.0 ) < 1.0e-9, "Failure 0,2" );
    QVERIFY2( fabs( m(0,3) +  4.0 ) < 1.0e-9, "Failure 0,3" );
    QVERIFY2( fabs( m(0,4) +  5.0 ) < 1.0e-9, "Failure 0,4" );
    QVERIFY2( fabs( m(0,5) +  6.0 ) < 1.0e-9, "Failure 0,5" );
    QVERIFY2( fabs( m(1,0) +  7.0 ) < 1.0e-9, "Failure 1,0" );
    QVERIFY2( fabs( m(1,1) +  8.0 ) < 1.0e-9, "Failure 1,1" );
    QVERIFY2( fabs( m(1,2) +  9.0 ) < 1.0e-9, "Failure 1,2" );
    QVERIFY2( fabs( m(1,3) + 10.0 ) < 1.0e-9, "Failure 1,3" );
    QVERIFY2( fabs( m(1,4) + 11.0 ) < 1.0e-9, "Failure 1,4" );
    QVERIFY2( fabs( m(1,5) + 12.0 ) < 1.0e-9, "Failure 1,5" );
    QVERIFY2( fabs( m(2,0) + 13.0 ) < 1.0e-9, "Failure 2,0" );
    QVERIFY2( fabs( m(2,1) + 14.0 ) < 1.0e-9, "Failure 2,1" );
    QVERIFY2( fabs( m(2,2) + 15.0 ) < 1.0e-9, "Failure 2,2" );
    QVERIFY2( fabs( m(2,3) + 16.0 ) < 1.0e-9, "Failure 2,3" );
    QVERIFY2( fabs( m(2,4) + 17.0 ) < 1.0e-9, "Failure 2,4" );
    QVERIFY2( fabs( m(2,5) + 18.0 ) < 1.0e-9, "Failure 2,5" );
    QVERIFY2( fabs( m(3,0) + 19.0 ) < 1.0e-9, "Failure 3,0" );
    QVERIFY2( fabs( m(3,1) + 20.0 ) < 1.0e-9, "Failure 3,1" );
    QVERIFY2( fabs( m(3,2) + 21.0 ) < 1.0e-9, "Failure 3,2" );
    QVERIFY2( fabs( m(3,3) + 22.0 ) < 1.0e-9, "Failure 3,3" );
    QVERIFY2( fabs( m(3,4) + 23.0 ) < 1.0e-9, "Failure 3,4" );
    QVERIFY2( fabs( m(3,5) + 24.0 ) < 1.0e-9, "Failure 3,5" );
    QVERIFY2( fabs( m(4,0) + 25.0 ) < 1.0e-9, "Failure 4,0" );
    QVERIFY2( fabs( m(4,1) + 26.0 ) < 1.0e-9, "Failure 4,1" );
    QVERIFY2( fabs( m(4,2) + 27.0 ) < 1.0e-9, "Failure 4,2" );
    QVERIFY2( fabs( m(4,3) + 28.0 ) < 1.0e-9, "Failure 4,3" );
    QVERIFY2( fabs( m(4,4) + 29.0 ) < 1.0e-9, "Failure 4,4" );
    QVERIFY2( fabs( m(4,5) + 30.0 ) < 1.0e-9, "Failure 4,5" );
    QVERIFY2( fabs( m(5,0) + 31.0 ) < 1.0e-9, "Failure 5,0" );
    QVERIFY2( fabs( m(5,1) + 32.0 ) < 1.0e-9, "Failure 5,1" );
    QVERIFY2( fabs( m(5,2) + 33.0 ) < 1.0e-9, "Failure 5,2" );
    QVERIFY2( fabs( m(5,3) + 34.0 ) < 1.0e-9, "Failure 5,3" );
    QVERIFY2( fabs( m(5,4) + 35.0 ) < 1.0e-9, "Failure 5,4" );
    QVERIFY2( fabs( m(5,5) + 36.0 ) < 1.0e-9, "Failure 5,5" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix6x6Test::operator_unary_multiply_by_scalar()
{
    double x[] = {
         1.0,  2.0,  3.0,  4.0,  5.0,  6.0,
         7.0,  8.0,  9.0, 10.0, 11.0, 12.0,
        13.0, 14.0, 15.0, 16.0, 17.0, 18.0,
        19.0, 20.0, 21.0, 22.0, 23.0, 24.0,
        25.0, 26.0, 27.0, 28.0, 29.0, 30.0,
        31.0, 32.0, 33.0, 34.0, 35.0, 36.0
    };

    fdm::Matrix6x6 m( x );

    m *= 2.0;

    QVERIFY2( fabs( m(0,0) -  2.0 ) < 1.0e-9, "Failure 0,0" );
    QVERIFY2( fabs( m(0,1) -  4.0 ) < 1.0e-9, "Failure 0,1" );
    QVERIFY2( fabs( m(0,2) -  6.0 ) < 1.0e-9, "Failure 0,2" );
    QVERIFY2( fabs( m(0,3) -  8.0 ) < 1.0e-9, "Failure 0,3" );
    QVERIFY2( fabs( m(0,4) - 10.0 ) < 1.0e-9, "Failure 0,4" );
    QVERIFY2( fabs( m(0,5) - 12.0 ) < 1.0e-9, "Failure 0,5" );
    QVERIFY2( fabs( m(1,0) - 14.0 ) < 1.0e-9, "Failure 1,0" );
    QVERIFY2( fabs( m(1,1) - 16.0 ) < 1.0e-9, "Failure 1,1" );
    QVERIFY2( fabs( m(1,2) - 18.0 ) < 1.0e-9, "Failure 1,2" );
    QVERIFY2( fabs( m(1,3) - 20.0 ) < 1.0e-9, "Failure 1,3" );
    QVERIFY2( fabs( m(1,4) - 22.0 ) < 1.0e-9, "Failure 1,4" );
    QVERIFY2( fabs( m(1,5) - 24.0 ) < 1.0e-9, "Failure 1,5" );
    QVERIFY2( fabs( m(2,0) - 26.0 ) < 1.0e-9, "Failure 2,0" );
    QVERIFY2( fabs( m(2,1) - 28.0 ) < 1.0e-9, "Failure 2,1" );
    QVERIFY2( fabs( m(2,2) - 30.0 ) < 1.0e-9, "Failure 2,2" );
    QVERIFY2( fabs( m(2,3) - 32.0 ) < 1.0e-9, "Failure 2,3" );
    QVERIFY2( fabs( m(2,4) - 34.0 ) < 1.0e-9, "Failure 2,4" );
    QVERIFY2( fabs( m(2,5) - 36.0 ) < 1.0e-9, "Failure 2,5" );
    QVERIFY2( fabs( m(3,0) - 38.0 ) < 1.0e-9, "Failure 3,0" );
    QVERIFY2( fabs( m(3,1) - 40.0 ) < 1.0e-9, "Failure 3,1" );
    QVERIFY2( fabs( m(3,2) - 42.0 ) < 1.0e-9, "Failure 3,2" );
    QVERIFY2( fabs( m(3,3) - 44.0 ) < 1.0e-9, "Failure 3,3" );
    QVERIFY2( fabs( m(3,4) - 46.0 ) < 1.0e-9, "Failure 3,4" );
    QVERIFY2( fabs( m(3,5) - 48.0 ) < 1.0e-9, "Failure 3,5" );
    QVERIFY2( fabs( m(4,0) - 50.0 ) < 1.0e-9, "Failure 4,0" );
    QVERIFY2( fabs( m(4,1) - 52.0 ) < 1.0e-9, "Failure 4,1" );
    QVERIFY2( fabs( m(4,2) - 54.0 ) < 1.0e-9, "Failure 4,2" );
    QVERIFY2( fabs( m(4,3) - 56.0 ) < 1.0e-9, "Failure 4,3" );
    QVERIFY2( fabs( m(4,4) - 58.0 ) < 1.0e-9, "Failure 4,4" );
    QVERIFY2( fabs( m(4,5) - 60.0 ) < 1.0e-9, "Failure 4,5" );
    QVERIFY2( fabs( m(5,0) - 62.0 ) < 1.0e-9, "Failure 5,0" );
    QVERIFY2( fabs( m(5,1) - 64.0 ) < 1.0e-9, "Failure 5,1" );
    QVERIFY2( fabs( m(5,2) - 66.0 ) < 1.0e-9, "Failure 5,2" );
    QVERIFY2( fabs( m(5,3) - 68.0 ) < 1.0e-9, "Failure 5,3" );
    QVERIFY2( fabs( m(5,4) - 70.0 ) < 1.0e-9, "Failure 5,4" );
    QVERIFY2( fabs( m(5,5) - 72.0 ) < 1.0e-9, "Failure 5,5" );
}

////////////////////////////////////////////////////////////////////////////////

void Matrix6x6Test::operator_unary_divide_by_scalar()
{
    double x[] = {
         2.0,  4.0,  6.0,  8.0, 10.0, 12.0,
        14.0, 16.0, 18.0, 20.0, 22.0, 24.0,
        26.0, 28.0, 30.0, 32.0, 34.0, 36.0,
        38.0, 40.0, 42.0, 44.0, 46.0, 48.0,
        50.0, 52.0, 54.0, 56.0, 58.0, 60.0,
        62.0, 64.0, 66.0, 68.0, 70.0, 72.0
    };

    fdm::Matrix6x6 m( x );

    m /= 2.0;

    QVERIFY2( fabs( m(0,0) -  1.0 ) < 1.0e-9, "Failure 0,0" );
    QVERIFY2( fabs( m(0,1) -  2.0 ) < 1.0e-9, "Failure 0,1" );
    QVERIFY2( fabs( m(0,2) -  3.0 ) < 1.0e-9, "Failure 0,2" );
    QVERIFY2( fabs( m(0,3) -  4.0 ) < 1.0e-9, "Failure 0,3" );
    QVERIFY2( fabs( m(0,4) -  5.0 ) < 1.0e-9, "Failure 0,4" );
    QVERIFY2( fabs( m(0,5) -  6.0 ) < 1.0e-9, "Failure 0,5" );
    QVERIFY2( fabs( m(1,0) -  7.0 ) < 1.0e-9, "Failure 1,0" );
    QVERIFY2( fabs( m(1,1) -  8.0 ) < 1.0e-9, "Failure 1,1" );
    QVERIFY2( fabs( m(1,2) -  9.0 ) < 1.0e-9, "Failure 1,2" );
    QVERIFY2( fabs( m(1,3) - 10.0 ) < 1.0e-9, "Failure 1,3" );
    QVERIFY2( fabs( m(1,4) - 11.0 ) < 1.0e-9, "Failure 1,4" );
    QVERIFY2( fabs( m(1,5) - 12.0 ) < 1.0e-9, "Failure 1,5" );
    QVERIFY2( fabs( m(2,0) - 13.0 ) < 1.0e-9, "Failure 2,0" );
    QVERIFY2( fabs( m(2,1) - 14.0 ) < 1.0e-9, "Failure 2,1" );
    QVERIFY2( fabs( m(2,2) - 15.0 ) < 1.0e-9, "Failure 2,2" );
    QVERIFY2( fabs( m(2,3) - 16.0 ) < 1.0e-9, "Failure 2,3" );
    QVERIFY2( fabs( m(2,4) - 17.0 ) < 1.0e-9, "Failure 2,4" );
    QVERIFY2( fabs( m(2,5) - 18.0 ) < 1.0e-9, "Failure 2,5" );
    QVERIFY2( fabs( m(3,0) - 19.0 ) < 1.0e-9, "Failure 3,0" );
    QVERIFY2( fabs( m(3,1) - 20.0 ) < 1.0e-9, "Failure 3,1" );
    QVERIFY2( fabs( m(3,2) - 21.0 ) < 1.0e-9, "Failure 3,2" );
    QVERIFY2( fabs( m(3,3) - 22.0 ) < 1.0e-9, "Failure 3,3" );
    QVERIFY2( fabs( m(3,4) - 23.0 ) < 1.0e-9, "Failure 3,4" );
    QVERIFY2( fabs( m(3,5) - 24.0 ) < 1.0e-9, "Failure 3,5" );
    QVERIFY2( fabs( m(4,0) - 25.0 ) < 1.0e-9, "Failure 4,0" );
    QVERIFY2( fabs( m(4,1) - 26.0 ) < 1.0e-9, "Failure 4,1" );
    QVERIFY2( fabs( m(4,2) - 27.0 ) < 1.0e-9, "Failure 4,2" );
    QVERIFY2( fabs( m(4,3) - 28.0 ) < 1.0e-9, "Failure 4,3" );
    QVERIFY2( fabs( m(4,4) - 29.0 ) < 1.0e-9, "Failure 4,4" );
    QVERIFY2( fabs( m(4,5) - 30.0 ) < 1.0e-9, "Failure 4,5" );
    QVERIFY2( fabs( m(5,0) - 31.0 ) < 1.0e-9, "Failure 5,0" );
    QVERIFY2( fabs( m(5,1) - 32.0 ) < 1.0e-9, "Failure 5,1" );
    QVERIFY2( fabs( m(5,2) - 33.0 ) < 1.0e-9, "Failure 5,2" );
    QVERIFY2( fabs( m(5,3) - 34.0 ) < 1.0e-9, "Failure 5,3" );
    QVERIFY2( fabs( m(5,4) - 35.0 ) < 1.0e-9, "Failure 5,4" );
    QVERIFY2( fabs( m(5,5) - 36.0 ) < 1.0e-9, "Failure 5,5" );
}

////////////////////////////////////////////////////////////////////////////////

QTEST_APPLESS_MAIN(Matrix6x6Test)

////////////////////////////////////////////////////////////////////////////////

#include "test_fdm_matrix6x6.moc"
