#include <iostream>

#include <QString>
#include <QtTest>

#include <fdm/utils/fdm_Matrix.h>

////////////////////////////////////////////////////////////////////////////////

using namespace std;

////////////////////////////////////////////////////////////////////////////////

class MatrixTest : public QObject
{
    Q_OBJECT

public:

    MatrixTest();

private Q_SLOTS:

    void initTestCase();
    void cleanupTestCase();

    void constructor();

    void isValid();
    void getArray();
    void getItem();
    void setArray();
    void setItem();
    void swapRows();
    void toString();

    void operator_accessor();
    void operator_assign();
    void operator_add();
    void operator_substract();
    void operator_multiply_by_scalar();
    void operator_multiply_by_vector();
    void operator_divide_by_scalar();
    void operator_unary_add();
    void operator_unary_substract();
    void operator_unary_multiply_by_scalar();
    void operator_unary_divide_by_scalar();
    void operator_equality();
    void operator_inequality();
};

////////////////////////////////////////////////////////////////////////////////

MatrixTest::MatrixTest() {}

////////////////////////////////////////////////////////////////////////////////

void MatrixTest::initTestCase() {}

////////////////////////////////////////////////////////////////////////////////

void MatrixTest::cleanupTestCase() {}

////////////////////////////////////////////////////////////////////////////////

void MatrixTest::constructor()
{
    fdm::Matrix< 3,3 > m1;

    QVERIFY2( fabs( m1(0,0) - 0.0 ) < 1.0e-9, "Failure 0,0 m1" );
    QVERIFY2( fabs( m1(0,1) - 0.0 ) < 1.0e-9, "Failure 0,1 m1" );
    QVERIFY2( fabs( m1(0,2) - 0.0 ) < 1.0e-9, "Failure 0,2 m1" );
    QVERIFY2( fabs( m1(1,0) - 0.0 ) < 1.0e-9, "Failure 1,0 m1" );
    QVERIFY2( fabs( m1(1,1) - 0.0 ) < 1.0e-9, "Failure 1,1 m1" );
    QVERIFY2( fabs( m1(1,2) - 0.0 ) < 1.0e-9, "Failure 1,2 m1" );
    QVERIFY2( fabs( m1(2,0) - 0.0 ) < 1.0e-9, "Failure 2,0 m1" );
    QVERIFY2( fabs( m1(2,1) - 0.0 ) < 1.0e-9, "Failure 2,1 m1" );
    QVERIFY2( fabs( m1(2,2) - 0.0 ) < 1.0e-9, "Failure 2,2 m1" );

    m1(0,0) = 1.0;
    m1(0,1) = 2.0;
    m1(0,2) = 3.0;
    m1(1,0) = 4.0;
    m1(1,1) = 5.0;
    m1(1,2) = 6.0;
    m1(2,0) = 7.0;
    m1(2,1) = 8.0;
    m1(2,2) = 9.0;

    fdm::Matrix< 3,3 > m2( m1 );

    QVERIFY2( fabs( m2(0,0) - 1.0 ) < 1.0e-9, "Failure 0,0 m2" );
    QVERIFY2( fabs( m2(0,1) - 2.0 ) < 1.0e-9, "Failure 0,1 m2" );
    QVERIFY2( fabs( m2(0,2) - 3.0 ) < 1.0e-9, "Failure 0,2 m2" );
    QVERIFY2( fabs( m2(1,0) - 4.0 ) < 1.0e-9, "Failure 1,0 m2" );
    QVERIFY2( fabs( m2(1,1) - 5.0 ) < 1.0e-9, "Failure 1,1 m2" );
    QVERIFY2( fabs( m2(1,2) - 6.0 ) < 1.0e-9, "Failure 1,2 m2" );
    QVERIFY2( fabs( m2(2,0) - 7.0 ) < 1.0e-9, "Failure 2,0 m2" );
    QVERIFY2( fabs( m2(2,1) - 8.0 ) < 1.0e-9, "Failure 2,1 m2" );
    QVERIFY2( fabs( m2(2,2) - 9.0 ) < 1.0e-9, "Failure 2,2 m2" );

    double x[] = { 1.0, 2.0, 3.0,
                   4.0, 5.0, 6.0,
                   7.0, 8.0, 9.0 };

    fdm::Matrix< 3,3 > m3( x );

    QVERIFY2( fabs( m3(0,0) - 1.0 ) < 1.0e-9, "Failure 0,0 m3" );
    QVERIFY2( fabs( m3(0,1) - 2.0 ) < 1.0e-9, "Failure 0,1 m3" );
    QVERIFY2( fabs( m3(0,2) - 3.0 ) < 1.0e-9, "Failure 0,2 m3" );
    QVERIFY2( fabs( m3(1,0) - 4.0 ) < 1.0e-9, "Failure 1,0 m3" );
    QVERIFY2( fabs( m3(1,1) - 5.0 ) < 1.0e-9, "Failure 1,1 m3" );
    QVERIFY2( fabs( m3(1,2) - 6.0 ) < 1.0e-9, "Failure 1,2 m3" );
    QVERIFY2( fabs( m3(2,0) - 7.0 ) < 1.0e-9, "Failure 2,0 m3" );
    QVERIFY2( fabs( m3(2,1) - 8.0 ) < 1.0e-9, "Failure 2,1 m3" );
    QVERIFY2( fabs( m3(2,2) - 9.0 ) < 1.0e-9, "Failure 2,2 m3" );
}

////////////////////////////////////////////////////////////////////////////////

void MatrixTest::isValid()
{
    fdm::Matrix< 3,3 > m1;

    QVERIFY2( m1.isValid(), "Failure m1" );

    fdm::Matrix< 3,3 > m2( m1 );

    m2(0,0) = 1.0;
    m2(0,1) = 2.0;
    m2(0,2) = 3.0;
    m2(1,0) = 4.0;
    m2(1,1) = 5.0;
    m2(1,2) = 6.0;
    m2(2,0) = 7.0;
    m2(2,1) = 8.0;
    m2(2,2) = 9.0;

    QVERIFY2( m2.isValid(), "Failure m2" );

    fdm::Matrix< 3,3 > m3( m1 );

    m3(0,0) = std::numeric_limits< double >::quiet_NaN();
    m3(0,1) = 2.0;
    m3(0,2) = 3.0;
    m3(1,0) = 4.0;
    m3(1,1) = 5.0;
    m3(1,2) = 6.0;
    m3(2,0) = 7.0;
    m3(2,1) = 8.0;
    m3(2,2) = 9.0;

    QVERIFY2( !m3.isValid(), "Failure m3" );

    fdm::Matrix< 3,3 > m4( m1 );

    m4(0,0) = 1.0;
    m4(0,1) = 2.0;
    m4(0,2) = 3.0;
    m4(1,0) = 4.0;
    m4(1,1) = std::numeric_limits< double >::quiet_NaN();
    m4(1,2) = 6.0;
    m4(2,0) = 7.0;
    m4(2,1) = 8.0;
    m4(2,2) = 9.0;

    QVERIFY2( !m4.isValid(), "Failure m4" );

    fdm::Matrix< 3,3 > m5( m1 );

    m5(0,0) = std::numeric_limits< double >::quiet_NaN();
    m5(0,1) = std::numeric_limits< double >::quiet_NaN();
    m5(0,2) = std::numeric_limits< double >::quiet_NaN();
    m5(1,0) = std::numeric_limits< double >::quiet_NaN();
    m5(1,1) = std::numeric_limits< double >::quiet_NaN();
    m5(1,2) = std::numeric_limits< double >::quiet_NaN();
    m5(2,0) = std::numeric_limits< double >::quiet_NaN();
    m5(2,1) = std::numeric_limits< double >::quiet_NaN();
    m5(2,2) = std::numeric_limits< double >::quiet_NaN();

    QVERIFY2( !m4.isValid(), "Failure m5" );
}

////////////////////////////////////////////////////////////////////////////////

void MatrixTest::getArray()
{
    fdm::Matrix< 3,3 > m1;

    m1(0,0) = 1.0;
    m1(0,1) = 2.0;
    m1(0,2) = 3.0;
    m1(1,0) = 4.0;
    m1(1,1) = 5.0;
    m1(1,2) = 6.0;
    m1(2,0) = 7.0;
    m1(2,1) = 8.0;
    m1(2,2) = 9.0;

    double x[ 9 ];

    m1.getArray( x );

    QVERIFY2( fabs( x[ 0 ] - 1.0 ) < 1.0e-9, "Failure 0" );
    QVERIFY2( fabs( x[ 1 ] - 2.0 ) < 1.0e-9, "Failure 1" );
    QVERIFY2( fabs( x[ 2 ] - 3.0 ) < 1.0e-9, "Failure 2" );
    QVERIFY2( fabs( x[ 3 ] - 4.0 ) < 1.0e-9, "Failure 3" );
    QVERIFY2( fabs( x[ 4 ] - 5.0 ) < 1.0e-9, "Failure 4" );
    QVERIFY2( fabs( x[ 5 ] - 6.0 ) < 1.0e-9, "Failure 5" );
    QVERIFY2( fabs( x[ 6 ] - 7.0 ) < 1.0e-9, "Failure 6" );
    QVERIFY2( fabs( x[ 7 ] - 8.0 ) < 1.0e-9, "Failure 7" );
    QVERIFY2( fabs( x[ 8 ] - 9.0 ) < 1.0e-9, "Failure 8" );
}

////////////////////////////////////////////////////////////////////////////////

void MatrixTest::getItem()
{
    fdm::Matrix< 3,3 > m1;

    m1(0,0) = 1.0;
    m1(0,1) = 2.0;
    m1(0,2) = 3.0;
    m1(1,0) = 4.0;
    m1(1,1) = 5.0;
    m1(1,2) = 6.0;
    m1(2,0) = 7.0;
    m1(2,1) = 8.0;
    m1(2,2) = 9.0;

    QVERIFY2( fabs( m1.getItem(0,0) - 1.0 ) < 1.0e-9, "Failure 0,0" );
    QVERIFY2( fabs( m1.getItem(0,1) - 2.0 ) < 1.0e-9, "Failure 0,1" );
    QVERIFY2( fabs( m1.getItem(0,2) - 3.0 ) < 1.0e-9, "Failure 0,2" );
    QVERIFY2( fabs( m1.getItem(1,0) - 4.0 ) < 1.0e-9, "Failure 1,0" );
    QVERIFY2( fabs( m1.getItem(1,1) - 5.0 ) < 1.0e-9, "Failure 1,1" );
    QVERIFY2( fabs( m1.getItem(1,2) - 6.0 ) < 1.0e-9, "Failure 1,2" );
    QVERIFY2( fabs( m1.getItem(2,0) - 7.0 ) < 1.0e-9, "Failure 2,0" );
    QVERIFY2( fabs( m1.getItem(2,1) - 8.0 ) < 1.0e-9, "Failure 2,1" );
    QVERIFY2( fabs( m1.getItem(2,2) - 9.0 ) < 1.0e-9, "Failure 2,2" );
}

////////////////////////////////////////////////////////////////////////////////

void MatrixTest::setArray()
{
    double x[] = { 1.0, 2.0, 3.0,
                   4.0, 5.0, 6.0,
                   7.0, 8.0, 9.0 };

    fdm::Matrix< 3,3 > m1;

    m1.setArray( x );

    QVERIFY2( fabs( m1(0,0) - 1.0 ) < 1.0e-9, "Failure 0,0" );
    QVERIFY2( fabs( m1(0,1) - 2.0 ) < 1.0e-9, "Failure 0,1" );
    QVERIFY2( fabs( m1(0,2) - 3.0 ) < 1.0e-9, "Failure 0,2" );
    QVERIFY2( fabs( m1(1,0) - 4.0 ) < 1.0e-9, "Failure 1,0" );
    QVERIFY2( fabs( m1(1,1) - 5.0 ) < 1.0e-9, "Failure 1,1" );
    QVERIFY2( fabs( m1(1,2) - 6.0 ) < 1.0e-9, "Failure 1,2" );
    QVERIFY2( fabs( m1(2,0) - 7.0 ) < 1.0e-9, "Failure 2,0" );
    QVERIFY2( fabs( m1(2,1) - 8.0 ) < 1.0e-9, "Failure 2,1" );
    QVERIFY2( fabs( m1(2,2) - 9.0 ) < 1.0e-9, "Failure 2,2" );
}

////////////////////////////////////////////////////////////////////////////////

void MatrixTest::setItem()
{
    fdm::Matrix< 3,3 > m1;

    m1.setItem( 0,0, 1.0 );
    m1.setItem( 0,1, 2.0 );
    m1.setItem( 0,2, 3.0 );
    m1.setItem( 1,0, 4.0 );
    m1.setItem( 1,1, 5.0 );
    m1.setItem( 1,2, 6.0 );
    m1.setItem( 2,0, 7.0 );
    m1.setItem( 2,1, 8.0 );
    m1.setItem( 2,2, 9.0 );

    QVERIFY2( fabs( m1(0,0) - 1.0 ) < 1.0e-9, "Failure 0,0" );
    QVERIFY2( fabs( m1(0,1) - 2.0 ) < 1.0e-9, "Failure 0,1" );
    QVERIFY2( fabs( m1(0,2) - 3.0 ) < 1.0e-9, "Failure 0,2" );
    QVERIFY2( fabs( m1(1,0) - 4.0 ) < 1.0e-9, "Failure 1,0" );
    QVERIFY2( fabs( m1(1,1) - 5.0 ) < 1.0e-9, "Failure 1,1" );
    QVERIFY2( fabs( m1(1,2) - 6.0 ) < 1.0e-9, "Failure 1,2" );
    QVERIFY2( fabs( m1(2,0) - 7.0 ) < 1.0e-9, "Failure 2,0" );
    QVERIFY2( fabs( m1(2,1) - 8.0 ) < 1.0e-9, "Failure 2,1" );
    QVERIFY2( fabs( m1(2,2) - 9.0 ) < 1.0e-9, "Failure 2,2" );
}

////////////////////////////////////////////////////////////////////////////////

void MatrixTest::swapRows()
{
    fdm::Matrix< 3,3 > m0;

    m0(0,0) = 1.0;
    m0(0,1) = 2.0;
    m0(0,2) = 3.0;
    m0(1,0) = 4.0;
    m0(1,1) = 5.0;
    m0(1,2) = 6.0;
    m0(2,0) = 7.0;
    m0(2,1) = 8.0;
    m0(2,2) = 9.0;

    QVERIFY2( fabs( m0.getItem(0,0) - 1.0 ) < 1.0e-9, "Failure 0,0 m0" );
    QVERIFY2( fabs( m0.getItem(0,1) - 2.0 ) < 1.0e-9, "Failure 0,1 m0" );
    QVERIFY2( fabs( m0.getItem(0,2) - 3.0 ) < 1.0e-9, "Failure 0,2 m0" );
    QVERIFY2( fabs( m0.getItem(1,0) - 4.0 ) < 1.0e-9, "Failure 1,0 m0" );
    QVERIFY2( fabs( m0.getItem(1,1) - 5.0 ) < 1.0e-9, "Failure 1,1 m0" );
    QVERIFY2( fabs( m0.getItem(1,2) - 6.0 ) < 1.0e-9, "Failure 1,2 m0" );
    QVERIFY2( fabs( m0.getItem(2,0) - 7.0 ) < 1.0e-9, "Failure 2,0 m0" );
    QVERIFY2( fabs( m0.getItem(2,1) - 8.0 ) < 1.0e-9, "Failure 2,1 m0" );
    QVERIFY2( fabs( m0.getItem(2,2) - 9.0 ) < 1.0e-9, "Failure 2,2 m0" );

    fdm::Matrix< 3,3 > m01( m0 );

    m01.swapRows(0,1);

    QVERIFY2( fabs( m01.getItem(1,0) - 1.0 ) < 1.0e-9, "Failure 0,0 m01" );
    QVERIFY2( fabs( m01.getItem(1,1) - 2.0 ) < 1.0e-9, "Failure 0,1 m01" );
    QVERIFY2( fabs( m01.getItem(1,2) - 3.0 ) < 1.0e-9, "Failure 0,2 m01" );
    QVERIFY2( fabs( m01.getItem(0,0) - 4.0 ) < 1.0e-9, "Failure 1,0 m01" );
    QVERIFY2( fabs( m01.getItem(0,1) - 5.0 ) < 1.0e-9, "Failure 1,1 m01" );
    QVERIFY2( fabs( m01.getItem(0,2) - 6.0 ) < 1.0e-9, "Failure 1,2 m01" );
    QVERIFY2( fabs( m01.getItem(2,0) - 7.0 ) < 1.0e-9, "Failure 2,0 m01" );
    QVERIFY2( fabs( m01.getItem(2,1) - 8.0 ) < 1.0e-9, "Failure 2,1 m01" );
    QVERIFY2( fabs( m01.getItem(2,2) - 9.0 ) < 1.0e-9, "Failure 2,2 m01" );

    fdm::Matrix< 3,3 > m02( m0 );

    m02.swapRows(0,2);

    QVERIFY2( fabs( m02.getItem(2,0) - 1.0 ) < 1.0e-9, "Failure 0,0 m02" );
    QVERIFY2( fabs( m02.getItem(2,1) - 2.0 ) < 1.0e-9, "Failure 0,1 m02" );
    QVERIFY2( fabs( m02.getItem(2,2) - 3.0 ) < 1.0e-9, "Failure 0,2 m02" );
    QVERIFY2( fabs( m02.getItem(1,0) - 4.0 ) < 1.0e-9, "Failure 1,0 m02" );
    QVERIFY2( fabs( m02.getItem(1,1) - 5.0 ) < 1.0e-9, "Failure 1,1 m02" );
    QVERIFY2( fabs( m02.getItem(1,2) - 6.0 ) < 1.0e-9, "Failure 1,2 m02" );
    QVERIFY2( fabs( m02.getItem(0,0) - 7.0 ) < 1.0e-9, "Failure 2,0 m02" );
    QVERIFY2( fabs( m02.getItem(0,1) - 8.0 ) < 1.0e-9, "Failure 2,1 m02" );
    QVERIFY2( fabs( m02.getItem(0,2) - 9.0 ) < 1.0e-9, "Failure 2,2 m02" );

    fdm::Matrix< 3,3 > m12( m0 );

    m12.swapRows(1,2);

    QVERIFY2( fabs( m12.getItem(0,0) - 1.0 ) < 1.0e-9, "Failure 0,0 m12" );
    QVERIFY2( fabs( m12.getItem(0,1) - 2.0 ) < 1.0e-9, "Failure 0,1 m12" );
    QVERIFY2( fabs( m12.getItem(0,2) - 3.0 ) < 1.0e-9, "Failure 0,2 m12" );
    QVERIFY2( fabs( m12.getItem(2,0) - 4.0 ) < 1.0e-9, "Failure 1,0 m12" );
    QVERIFY2( fabs( m12.getItem(2,1) - 5.0 ) < 1.0e-9, "Failure 1,1 m12" );
    QVERIFY2( fabs( m12.getItem(2,2) - 6.0 ) < 1.0e-9, "Failure 1,2 m12" );
    QVERIFY2( fabs( m12.getItem(1,0) - 7.0 ) < 1.0e-9, "Failure 2,0 m12" );
    QVERIFY2( fabs( m12.getItem(1,1) - 8.0 ) < 1.0e-9, "Failure 2,1 m12" );
    QVERIFY2( fabs( m12.getItem(1,2) - 9.0 ) < 1.0e-9, "Failure 2,2 m12" );
}

////////////////////////////////////////////////////////////////////////////////

void MatrixTest::toString()
{
    // TODO
}

////////////////////////////////////////////////////////////////////////////////

void MatrixTest::operator_accessor()
{
    fdm::Matrix< 3,3 > m1;

    m1(0,0) = 1.0;
    m1(0,1) = 2.0;
    m1(0,2) = 3.0;
    m1(1,0) = 4.0;
    m1(1,1) = 5.0;
    m1(1,2) = 6.0;
    m1(2,0) = 7.0;
    m1(2,1) = 8.0;
    m1(2,2) = 9.0;

    QVERIFY2( fabs( m1(0,0) - 1.0 ) < 1.0e-9, "Failure 0,0" );
    QVERIFY2( fabs( m1(0,1) - 2.0 ) < 1.0e-9, "Failure 0,1" );
    QVERIFY2( fabs( m1(0,2) - 3.0 ) < 1.0e-9, "Failure 0,2" );
    QVERIFY2( fabs( m1(1,0) - 4.0 ) < 1.0e-9, "Failure 1,0" );
    QVERIFY2( fabs( m1(1,1) - 5.0 ) < 1.0e-9, "Failure 1,1" );
    QVERIFY2( fabs( m1(1,2) - 6.0 ) < 1.0e-9, "Failure 1,2" );
    QVERIFY2( fabs( m1(2,0) - 7.0 ) < 1.0e-9, "Failure 2,0" );
    QVERIFY2( fabs( m1(2,1) - 8.0 ) < 1.0e-9, "Failure 2,1" );
    QVERIFY2( fabs( m1(2,2) - 9.0 ) < 1.0e-9, "Failure 2,2" );
}

////////////////////////////////////////////////////////////////////////////////

void MatrixTest::operator_assign()
{
    fdm::Matrix< 3,3 > m1;

    m1(0,0) = 1.0;
    m1(0,1) = 2.0;
    m1(0,2) = 3.0;
    m1(1,0) = 4.0;
    m1(1,1) = 5.0;
    m1(1,2) = 6.0;
    m1(2,0) = 7.0;
    m1(2,1) = 8.0;
    m1(2,2) = 9.0;

    fdm::Matrix< 3,3 > m2 = m1;

    QVERIFY2( fabs( m2(0,0) - 1.0 ) < 1.0e-9, "Failure 0,0" );
    QVERIFY2( fabs( m2(0,1) - 2.0 ) < 1.0e-9, "Failure 0,1" );
    QVERIFY2( fabs( m2(0,2) - 3.0 ) < 1.0e-9, "Failure 0,2" );
    QVERIFY2( fabs( m2(1,0) - 4.0 ) < 1.0e-9, "Failure 1,0" );
    QVERIFY2( fabs( m2(1,1) - 5.0 ) < 1.0e-9, "Failure 1,1" );
    QVERIFY2( fabs( m2(1,2) - 6.0 ) < 1.0e-9, "Failure 1,2" );
    QVERIFY2( fabs( m2(2,0) - 7.0 ) < 1.0e-9, "Failure 2,0" );
    QVERIFY2( fabs( m2(2,1) - 8.0 ) < 1.0e-9, "Failure 2,1" );
    QVERIFY2( fabs( m2(2,2) - 9.0 ) < 1.0e-9, "Failure 2,2" );
}

////////////////////////////////////////////////////////////////////////////////

void MatrixTest::operator_add()
{
    fdm::Matrix< 3,3 > m1;
    fdm::Matrix< 3,3 > m2;

    m2(0,0) = 1.0;
    m2(0,1) = 2.0;
    m2(0,2) = 3.0;
    m2(1,0) = 4.0;
    m2(1,1) = 5.0;
    m2(1,2) = 6.0;
    m2(2,0) = 7.0;
    m2(2,1) = 8.0;
    m2(2,2) = 9.0;

    fdm::Matrix< 3,3 > m3 = m1 + m2;

    QVERIFY2( fabs( m3(0,0) - 1.0 ) < 1.0e-9, "Failure 0,0" );
    QVERIFY2( fabs( m3(0,1) - 2.0 ) < 1.0e-9, "Failure 0,1" );
    QVERIFY2( fabs( m3(0,2) - 3.0 ) < 1.0e-9, "Failure 0,2" );
    QVERIFY2( fabs( m3(1,0) - 4.0 ) < 1.0e-9, "Failure 1,0" );
    QVERIFY2( fabs( m3(1,1) - 5.0 ) < 1.0e-9, "Failure 1,1" );
    QVERIFY2( fabs( m3(1,2) - 6.0 ) < 1.0e-9, "Failure 1,2" );
    QVERIFY2( fabs( m3(2,0) - 7.0 ) < 1.0e-9, "Failure 2,0" );
    QVERIFY2( fabs( m3(2,1) - 8.0 ) < 1.0e-9, "Failure 2,1" );
    QVERIFY2( fabs( m3(2,2) - 9.0 ) < 1.0e-9, "Failure 2,2" );
}

////////////////////////////////////////////////////////////////////////////////

void MatrixTest::operator_substract()
{
    fdm::Matrix< 3,3 > m1;
    fdm::Matrix< 3,3 > m2;

    m1(0,0) = 1.0;
    m1(0,1) = 1.0;
    m1(0,2) = 1.0;
    m1(1,0) = 1.0;
    m1(1,1) = 1.0;
    m1(1,2) = 1.0;
    m1(2,0) = 1.0;
    m1(2,1) = 1.0;
    m1(2,2) = 1.0;

    m2(0,0) = 1.0;
    m2(0,1) = 2.0;
    m2(0,2) = 3.0;
    m2(1,0) = 4.0;
    m2(1,1) = 5.0;
    m2(1,2) = 6.0;
    m2(2,0) = 7.0;
    m2(2,1) = 8.0;
    m2(2,2) = 9.0;

    fdm::Matrix< 3,3 > m3 = m2 - m1;

    QVERIFY2( fabs( m3(0,0) - 0.0 ) < 1.0e-9, "Failure 0,0" );
    QVERIFY2( fabs( m3(0,1) - 1.0 ) < 1.0e-9, "Failure 0,1" );
    QVERIFY2( fabs( m3(0,2) - 2.0 ) < 1.0e-9, "Failure 0,2" );
    QVERIFY2( fabs( m3(1,0) - 3.0 ) < 1.0e-9, "Failure 1,0" );
    QVERIFY2( fabs( m3(1,1) - 4.0 ) < 1.0e-9, "Failure 1,1" );
    QVERIFY2( fabs( m3(1,2) - 5.0 ) < 1.0e-9, "Failure 1,2" );
    QVERIFY2( fabs( m3(2,0) - 6.0 ) < 1.0e-9, "Failure 2,0" );
    QVERIFY2( fabs( m3(2,1) - 7.0 ) < 1.0e-9, "Failure 2,1" );
    QVERIFY2( fabs( m3(2,2) - 8.0 ) < 1.0e-9, "Failure 2,2" );
}

////////////////////////////////////////////////////////////////////////////////

void MatrixTest::operator_multiply_by_scalar()
{
    fdm::Matrix< 3,3 > m1;

    m1(0,0) = 1.0;
    m1(0,1) = 2.0;
    m1(0,2) = 3.0;
    m1(1,0) = 4.0;
    m1(1,1) = 5.0;
    m1(1,2) = 6.0;
    m1(2,0) = 7.0;
    m1(2,1) = 8.0;
    m1(2,2) = 9.0;

    fdm::Matrix< 3,3 > m3 = m1 * 2.0;

    QVERIFY2( fabs( m3(0,0) -  2.0 ) < 1.0e-9, "Failure 0,0" );
    QVERIFY2( fabs( m3(0,1) -  4.0 ) < 1.0e-9, "Failure 0,1" );
    QVERIFY2( fabs( m3(0,2) -  6.0 ) < 1.0e-9, "Failure 0,2" );
    QVERIFY2( fabs( m3(1,0) -  8.0 ) < 1.0e-9, "Failure 1,0" );
    QVERIFY2( fabs( m3(1,1) - 10.0 ) < 1.0e-9, "Failure 1,1" );
    QVERIFY2( fabs( m3(1,2) - 12.0 ) < 1.0e-9, "Failure 1,2" );
    QVERIFY2( fabs( m3(2,0) - 14.0 ) < 1.0e-9, "Failure 2,0" );
    QVERIFY2( fabs( m3(2,1) - 16.0 ) < 1.0e-9, "Failure 2,1" );
    QVERIFY2( fabs( m3(2,2) - 18.0 ) < 1.0e-9, "Failure 2,2" );
}

////////////////////////////////////////////////////////////////////////////////

void MatrixTest::operator_multiply_by_vector()
{
    double x1[] = {  1.0,  0.0,  0.0,
                     0.0,  1.0,  0.0,
                     0.0,  0.0,  1.0 };

    double x2[] = { -1.0,  0.0,  0.0,
                     0.0, -1.0,  0.0,
                     0.0,  0.0, -1.0 };

    double x3[] = {  0.0,  1.0,  0.0,
                     1.0,  0.0,  0.0,
                     0.0,  0.0,  1.0 };

    double x4[] = {  1.0,  0.0,  0.0,
                     0.0,  0.0, -1.0,
                     0.0, -1.0,  0.0 };

    double va[] = { 1.0, 2.0, 3.0 };

    fdm::Matrix< 3,3 > m1( x1 );
    fdm::Matrix< 3,3 > m2( x2 );
    fdm::Matrix< 3,3 > m3( x3 );
    fdm::Matrix< 3,3 > m4( x4 );

    fdm::Vector< 3 > v( va );

    fdm::Vector< 3 > v1 = m1 * v;
    QVERIFY2( fabs( v1( 0 ) - 1.0 ) < 1.0e-9, "Failure 0 v1" );
    QVERIFY2( fabs( v1( 1 ) - 2.0 ) < 1.0e-9, "Failure 1 v1" );
    QVERIFY2( fabs( v1( 2 ) - 3.0 ) < 1.0e-9, "Failure 2 v1" );

    fdm::Vector< 3 > v2 = m2 * v;
    QVERIFY2( fabs( v2( 0 ) + 1.0 ) < 1.0e-9, "Failure 0 v2" );
    QVERIFY2( fabs( v2( 1 ) + 2.0 ) < 1.0e-9, "Failure 1 v2" );
    QVERIFY2( fabs( v2( 2 ) + 3.0 ) < 1.0e-9, "Failure 2 v2" );

    fdm::Vector< 3 > v3 = m3 * v;
    QVERIFY2( fabs( v3( 0 ) - 2.0 ) < 1.0e-9, "Failure 0 v3" );
    QVERIFY2( fabs( v3( 1 ) - 1.0 ) < 1.0e-9, "Failure 1 v3" );
    QVERIFY2( fabs( v3( 2 ) - 3.0 ) < 1.0e-9, "Failure 2 v3" );

    fdm::Vector< 3 > v4 = m4 * v;
    QVERIFY2( fabs( v4( 0 ) - 1.0 ) < 1.0e-9, "Failure 0 v4" );
    QVERIFY2( fabs( v4( 1 ) + 3.0 ) < 1.0e-9, "Failure 1 v4" );
    QVERIFY2( fabs( v4( 2 ) + 2.0 ) < 1.0e-9, "Failure 2 v4" );
}

////////////////////////////////////////////////////////////////////////////////

void MatrixTest::operator_divide_by_scalar()
{
    fdm::Matrix< 3,3 > m1;

    m1(0,0) =  2.0;
    m1(0,1) =  4.0;
    m1(0,2) =  6.0;
    m1(1,0) =  8.0;
    m1(1,1) = 10.0;
    m1(1,2) = 12.0;
    m1(2,0) = 14.0;
    m1(2,1) = 16.0;
    m1(2,2) = 18.0;

    fdm::Matrix< 3,3 > m3 = m1 / 2.0;

    QVERIFY2( fabs( m3(0,0) - 1.0 ) < 1.0e-9, "Failure 0,0" );
    QVERIFY2( fabs( m3(0,1) - 2.0 ) < 1.0e-9, "Failure 0,1" );
    QVERIFY2( fabs( m3(0,2) - 3.0 ) < 1.0e-9, "Failure 0,2" );
    QVERIFY2( fabs( m3(1,0) - 4.0 ) < 1.0e-9, "Failure 1,0" );
    QVERIFY2( fabs( m3(1,1) - 5.0 ) < 1.0e-9, "Failure 1,1" );
    QVERIFY2( fabs( m3(1,2) - 6.0 ) < 1.0e-9, "Failure 1,2" );
    QVERIFY2( fabs( m3(2,0) - 7.0 ) < 1.0e-9, "Failure 2,0" );
    QVERIFY2( fabs( m3(2,1) - 8.0 ) < 1.0e-9, "Failure 2,1" );
    QVERIFY2( fabs( m3(2,2) - 9.0 ) < 1.0e-9, "Failure 2,2" );
}

////////////////////////////////////////////////////////////////////////////////

void MatrixTest::operator_unary_add()
{
    fdm::Matrix< 3,3 > m1;
    fdm::Matrix< 3,3 > m2;

    m2(0,0) = 1.0;
    m2(0,1) = 2.0;
    m2(0,2) = 3.0;
    m2(1,0) = 4.0;
    m2(1,1) = 5.0;
    m2(1,2) = 6.0;
    m2(2,0) = 7.0;
    m2(2,1) = 8.0;
    m2(2,2) = 9.0;

    m1 += m2;

    QVERIFY2( fabs( m1(0,0) - 1.0 ) < 1.0e-9, "Failure 0,0" );
    QVERIFY2( fabs( m1(0,1) - 2.0 ) < 1.0e-9, "Failure 0,1" );
    QVERIFY2( fabs( m1(0,2) - 3.0 ) < 1.0e-9, "Failure 0,2" );
    QVERIFY2( fabs( m1(1,0) - 4.0 ) < 1.0e-9, "Failure 1,0" );
    QVERIFY2( fabs( m1(1,1) - 5.0 ) < 1.0e-9, "Failure 1,1" );
    QVERIFY2( fabs( m1(1,2) - 6.0 ) < 1.0e-9, "Failure 1,2" );
    QVERIFY2( fabs( m1(2,0) - 7.0 ) < 1.0e-9, "Failure 2,0" );
    QVERIFY2( fabs( m1(2,1) - 8.0 ) < 1.0e-9, "Failure 2,1" );
    QVERIFY2( fabs( m1(2,2) - 9.0 ) < 1.0e-9, "Failure 2,2" );
}

////////////////////////////////////////////////////////////////////////////////

void MatrixTest::operator_unary_substract()
{
    fdm::Matrix< 3,3 > m1;
    fdm::Matrix< 3,3 > m2;

    m1(0,0) = 1.0;
    m1(0,1) = 1.0;
    m1(0,2) = 1.0;
    m1(1,0) = 1.0;
    m1(1,1) = 1.0;
    m1(1,2) = 1.0;
    m1(2,0) = 1.0;
    m1(2,1) = 1.0;
    m1(2,2) = 1.0;

    m2(0,0) = 1.0;
    m2(0,1) = 2.0;
    m2(0,2) = 3.0;
    m2(1,0) = 4.0;
    m2(1,1) = 5.0;
    m2(1,2) = 6.0;
    m2(2,0) = 7.0;
    m2(2,1) = 8.0;
    m2(2,2) = 9.0;

    m2 -= m1;

    QVERIFY2( fabs( m2(0,0) - 0.0 ) < 1.0e-9, "Failure 0,0" );
    QVERIFY2( fabs( m2(0,1) - 1.0 ) < 1.0e-9, "Failure 0,1" );
    QVERIFY2( fabs( m2(0,2) - 2.0 ) < 1.0e-9, "Failure 0,2" );
    QVERIFY2( fabs( m2(1,0) - 3.0 ) < 1.0e-9, "Failure 1,0" );
    QVERIFY2( fabs( m2(1,1) - 4.0 ) < 1.0e-9, "Failure 1,1" );
    QVERIFY2( fabs( m2(1,2) - 5.0 ) < 1.0e-9, "Failure 1,2" );
    QVERIFY2( fabs( m2(2,0) - 6.0 ) < 1.0e-9, "Failure 2,0" );
    QVERIFY2( fabs( m2(2,1) - 7.0 ) < 1.0e-9, "Failure 2,1" );
    QVERIFY2( fabs( m2(2,2) - 8.0 ) < 1.0e-9, "Failure 2,2" );
}

////////////////////////////////////////////////////////////////////////////////

void MatrixTest::operator_unary_multiply_by_scalar()
{
    fdm::Matrix< 3,3 > m1;

    m1(0,0) = 1.0;
    m1(0,1) = 2.0;
    m1(0,2) = 3.0;
    m1(1,0) = 4.0;
    m1(1,1) = 5.0;
    m1(1,2) = 6.0;
    m1(2,0) = 7.0;
    m1(2,1) = 8.0;
    m1(2,2) = 9.0;

    m1 *= 2.0;

    QVERIFY2( fabs( m1(0,0) -  2.0 ) < 1.0e-9, "Failure 0,0" );
    QVERIFY2( fabs( m1(0,1) -  4.0 ) < 1.0e-9, "Failure 0,1" );
    QVERIFY2( fabs( m1(0,2) -  6.0 ) < 1.0e-9, "Failure 0,2" );
    QVERIFY2( fabs( m1(1,0) -  8.0 ) < 1.0e-9, "Failure 1,0" );
    QVERIFY2( fabs( m1(1,1) - 10.0 ) < 1.0e-9, "Failure 1,1" );
    QVERIFY2( fabs( m1(1,2) - 12.0 ) < 1.0e-9, "Failure 1,2" );
    QVERIFY2( fabs( m1(2,0) - 14.0 ) < 1.0e-9, "Failure 2,0" );
    QVERIFY2( fabs( m1(2,1) - 16.0 ) < 1.0e-9, "Failure 2,1" );
    QVERIFY2( fabs( m1(2,2) - 18.0 ) < 1.0e-9, "Failure 2,2" );
}

////////////////////////////////////////////////////////////////////////////////

void MatrixTest::operator_unary_divide_by_scalar()
{
    fdm::Matrix< 3,3 > m1;

    m1(0,0) =  2.0;
    m1(0,1) =  4.0;
    m1(0,2) =  6.0;
    m1(1,0) =  8.0;
    m1(1,1) = 10.0;
    m1(1,2) = 12.0;
    m1(2,0) = 14.0;
    m1(2,1) = 16.0;
    m1(2,2) = 18.0;

    m1 /= 2.0;

    QVERIFY2( fabs( m1(0,0) - 1.0 ) < 1.0e-9, "Failure 0,0" );
    QVERIFY2( fabs( m1(0,1) - 2.0 ) < 1.0e-9, "Failure 0,1" );
    QVERIFY2( fabs( m1(0,2) - 3.0 ) < 1.0e-9, "Failure 0,2" );
    QVERIFY2( fabs( m1(1,0) - 4.0 ) < 1.0e-9, "Failure 1,0" );
    QVERIFY2( fabs( m1(1,1) - 5.0 ) < 1.0e-9, "Failure 1,1" );
    QVERIFY2( fabs( m1(1,2) - 6.0 ) < 1.0e-9, "Failure 1,2" );
    QVERIFY2( fabs( m1(2,0) - 7.0 ) < 1.0e-9, "Failure 2,0" );
    QVERIFY2( fabs( m1(2,1) - 8.0 ) < 1.0e-9, "Failure 2,1" );
    QVERIFY2( fabs( m1(2,2) - 9.0 ) < 1.0e-9, "Failure 2,2" );
}

////////////////////////////////////////////////////////////////////////////////

void MatrixTest::operator_equality()
{
    fdm::Matrix< 3,3 > m1;
    fdm::Matrix< 3,3 > m2;

    QVERIFY2( ( m1 == m2 ), "Failure 0,0" );

    m1(0,0) = 1.0;
    m1(0,1) = 2.0;
    m1(0,2) = 3.0;
    m1(1,0) = 4.0;
    m1(1,1) = 5.0;
    m1(1,2) = 6.0;
    m1(2,0) = 7.0;
    m1(2,1) = 8.0;
    m1(2,2) = 9.0;

    QVERIFY2( !( m1 == m2 ), "Failure 0,0" );

    m2 = m1;

    QVERIFY2( ( m1 == m2 ), "Failure 0,0" );
}

////////////////////////////////////////////////////////////////////////////////

void MatrixTest::operator_inequality()
{
    fdm::Matrix< 3,3 > m1;
    fdm::Matrix< 3,3 > m2;

    QVERIFY2( !( m1 != m2 ), "Failure 0,0" );

    m1(0,0) = 1.0;
    m1(0,1) = 2.0;
    m1(0,2) = 3.0;
    m1(1,0) = 4.0;
    m1(1,1) = 5.0;
    m1(1,2) = 6.0;
    m1(2,0) = 7.0;
    m1(2,1) = 8.0;
    m1(2,2) = 9.0;

    QVERIFY2( ( m1 != m2 ), "Failure 0,0" );

    m2 = m1;

    QVERIFY2( !( m1 != m2 ), "Failure 0,0" );
}

////////////////////////////////////////////////////////////////////////////////

QTEST_APPLESS_MAIN(MatrixTest)

////////////////////////////////////////////////////////////////////////////////

#include "test_fdm_matrix.moc"
