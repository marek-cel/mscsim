#include <QString>
#include <QtTest>

#include <iostream>

#include <fdm/utils/fdm_String.h>
#include <fdm/utils/fdm_Table.h>
#include <fdm/utils/fdm_Table2D.h>

#include <fdm/xml/fdm_XmlDoc.h>
#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace std;
using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

class TestF16AeroData : public QObject
{
    Q_OBJECT

public:

    TestF16AeroData();


    struct AeroData
    {
        Table2D cx_dh_n25;
        Table2D cx_dh_n10;
        Table2D cx_dh_0;
        Table2D cx_dh_p10;
        Table2D cx_dh_p25;
        Table2D cx_lef;
        Table   delta_cx_sb;
        Table   cx_q;
        Table   delta_cx_q_lef;

        Table2D cy;
        Table2D cy_lef;
        Table2D cy_da_20;
        Table2D cy_da_20_lef;
        Table2D cy_dr_30;
        Table   cy_r;
        Table   delta_cy_r_lef;
        Table   cy_p;
        Table   delta_cy_p_lef;

        Table2D cz_dh_n25;
        Table2D cz_dh_n10;
        Table2D cz_dh_0;
        Table2D cz_dh_p10;
        Table2D cz_dh_p25;
        Table2D cz_lef;
        Table   delta_cz_sb;
        Table   cz_q;
        Table   delta_cz_q_lef;
        Table   eta_delta_h;

        Table2D cl_dh_n25;
        Table2D cl_dh_0;
        Table2D cl_dh_p25;
        Table2D cl_lef;
        Table2D cl_da_20;
        Table2D cl_da_20_lef;
        Table2D cl_dr_30;
        Table   cl_r;
        Table   delta_cl_beta;
        Table   delta_cl_r_lef;
        Table   cl_p;
        Table   delta_cl_p_lef;

        Table2D cm_dh_n25;
        Table2D cm_dh_n10;
        Table2D cm_dh_0;
        Table2D cm_dh_p10;
        Table2D cm_dh_p25;
        Table2D cm_lef;
        Table   delta_cm_sb;
        Table   cm_q;
        Table   delta_cm_q_lef;
        Table   delta_cm;
        Table2D delta_cm_ds;

        Table2D cn_dh_n25;
        Table2D cn_dh_0;
        Table2D cn_dh_p25;
        Table2D cn_lef;
        Table2D cn_da_20;
        Table2D cn_da_20_lef;
        Table2D cn_dr_30;
        Table   cn_r;
        Table   delta_cn_beta;
        Table   delta_cn_delta_a;
        Table   delta_cn_r_lef;
        Table   cn_p;
        Table   delta_cn_p_lef;
    };

    AeroData _data1;
    AeroData _data2;

    void readData( AeroData &data, const QString &path );

    void test( Table2D &table , Table2D &table2, const QString &name );
    void test( Table   &table , Table   &table2, const QString &name );

private Q_SLOTS:

    void initTestCase();

    void cleanupTestCase();

    void testData();
};

////////////////////////////////////////////////////////////////////////////////

TestF16AeroData::TestF16AeroData() {}

////////////////////////////////////////////////////////////////////////////////

void TestF16AeroData::readData( AeroData &data, const QString &path )
{
    XmlDoc doc( path.toStdString() );

    if ( doc.isOpen() )
    {
        XmlNode rootNode = doc.getRootNode();

        if ( rootNode.isValid() )
        {
            XmlNode dataNode = rootNode.getFirstChildElement( "aerodynamics" );

            int result = FDM_SUCCESS;

            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cx_dh_n25      , "cx_dh_n25"      );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cx_dh_n10      , "cx_dh_n10"      );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cx_dh_0        , "cx_dh_0"        );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cx_dh_p10      , "cx_dh_p10"      );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cx_dh_p25      , "cx_dh_p25"      );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cx_lef         , "cx_lef"         );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.delta_cx_sb    , "delta_cx_sb"    );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cx_q           , "cx_q"           );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.delta_cx_q_lef , "delta_cx_q_lef" );

            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cy             , "cy"             );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cy_lef         , "cy_lef"         );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cy_da_20       , "cy_da_20"       );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cy_da_20_lef   , "cy_da_20_lef"   );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cy_dr_30       , "cy_dr_30"       );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cy_r           , "cy_r"           );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.delta_cy_r_lef , "delta_cy_r_lef" );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cy_p           , "cy_p"           );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.delta_cy_p_lef , "delta_cy_p_lef" );

            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cz_dh_n25      , "cz_dh_n25"      );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cz_dh_n10      , "cz_dh_n10"      );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cz_dh_0        , "cz_dh_0"        );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cz_dh_p10      , "cz_dh_p10"      );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cz_dh_p25      , "cz_dh_p25"      );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cz_lef         , "cz_lef"         );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.delta_cz_sb    , "delta_cz_sb"    );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cz_q           , "cz_q"           );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.delta_cz_q_lef , "delta_cz_q_lef" );

            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cl_dh_n25      , "cl_dh_n25"      );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cl_dh_0        , "cl_dh_0"        );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cl_dh_p25      , "cl_dh_p25"      );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cl_lef         , "cl_lef"         );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cl_da_20       , "cl_da_20"       );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cl_da_20_lef   , "cl_da_20_lef"   );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cl_dr_30       , "cl_dr_30"       );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cl_r           , "cl_r"           );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.delta_cl_beta  , "delta_cl_beta"  );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.delta_cl_r_lef , "delta_cl_r_lef" );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cl_p           , "cl_p"           );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.delta_cl_p_lef , "delta_cl_p_lef" );

            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cm_dh_n25      , "cm_dh_n25"      );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cm_dh_n10      , "cm_dh_n10"      );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cm_dh_0        , "cm_dh_0"        );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cm_dh_p10      , "cm_dh_p10"      );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cm_dh_p25      , "cm_dh_p25"      );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cm_lef         , "cm_lef"         );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.delta_cm_sb    , "delta_cm_sb"    );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cm_q           , "cm_q"           );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.delta_cm_q_lef , "delta_cm_q_lef" );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.delta_cm       , "delta_cm"       );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.delta_cm_ds    , "delta_cm_ds"    );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.eta_delta_h    , "eta_delta_h"    );

            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cn_dh_n25        , "cn_dh_n25"        );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cn_dh_0          , "cn_dh_0"          );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cn_dh_p25        , "cn_dh_p25"        );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cn_lef           , "cn_lef"           );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cn_da_20         , "cn_da_20"         );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cn_da_20_lef     , "cn_da_20_lef"     );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cn_dr_30         , "cn_dr_30"         );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cn_r             , "cn_r"             );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.delta_cn_beta    , "delta_cn_beta"    );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.delta_cn_delta_a , "delta_cn_delta_a" );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.delta_cn_r_lef   , "delta_cn_r_lef"   );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.cn_p             , "cn_p"             );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, data.delta_cn_p_lef   , "delta_cn_p_lef"   );

            cout << "Ok." << endl;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void TestF16AeroData::test( Table2D &table1, Table2D &table2, const QString &name )
{
    cout << "Checking: " << name.toStdString() << endl;

    QVERIFY2( table1.getCols() == table2.getCols(), name.toStdString().c_str() );
    QVERIFY2( table1.getRows() == table2.getRows(), name.toStdString().c_str() );

    for ( unsigned int c = 0; c < table1.getCols(); c++ )
    {
        for ( unsigned int r = 0; r < table1.getRows(); r++ )
        {
            QString info = name + " " + QString::number( r ) + "x" + QString::number( c );
            QVERIFY2( table1.getValueByIndex( r, c ) - table2.getValueByIndex( r, c ) == 0, info.toStdString().c_str() );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void TestF16AeroData::test( Table &table1, Table &table2, const QString &name )
{
    cout << "Checking: " << name.toStdString() << endl;

    QVERIFY2( table1.getSize() == table2.getSize(), name.toStdString().c_str() );

    for ( unsigned int i = 0; i < table1.getSize(); i++ )
    {
        QString info = name + " " + QString::number( i );
        QVERIFY2( table1.getValueByIndex( i ) - table2.getValueByIndex( i ) == 0, info.toStdString().c_str() );
    }
}

////////////////////////////////////////////////////////////////////////////////

void TestF16AeroData::initTestCase()
{
    readData( _data1, "../data/fdm/f16/f16_fdm.xml" );
    readData( _data2, "data/f16_aero_test.xml" );
}

////////////////////////////////////////////////////////////////////////////////

void TestF16AeroData::cleanupTestCase() {}

////////////////////////////////////////////////////////////////////////////////

void TestF16AeroData::testData()
{
    test( _data1.cx_dh_n25        , _data2.cx_dh_n25        , "cx_dh_n25"        );
    test( _data1.cx_dh_n10        , _data2.cx_dh_n10        , "cx_dh_n10"        );
    test( _data1.cx_dh_0          , _data2.cx_dh_0          , "cx_dh_0"          );
    test( _data1.cx_dh_p10        , _data2.cx_dh_p10        , "cx_dh_p10"        );
    test( _data1.cx_dh_p25        , _data2.cx_dh_p25        , "cx_dh_p25"        );
    test( _data1.cx_lef           , _data2.cx_lef           , "cx_lef"           );
    test( _data1.delta_cx_sb      , _data2.delta_cx_sb      , "delta_cx_sb"      );
    test( _data1.cx_q             , _data2.cx_q             , "cx_q"             );
    test( _data1.delta_cx_q_lef   , _data2.delta_cx_q_lef   , "delta_cx_q_lef"   );
    test( _data1.cy               , _data2.cy               , "cy"               );
    test( _data1.cy_lef           , _data2.cy_lef           , "cy_lef"           );
    test( _data1.cy_da_20         , _data2.cy_da_20         , "cy_da_20"         );
    test( _data1.cy_da_20_lef     , _data2.cy_da_20_lef     , "cy_da_20_lef"     );
    test( _data1.cy_dr_30         , _data2.cy_dr_30         , "cy_dr_30"         );
    test( _data1.cy_r             , _data2.cy_r             , "cy_r"             );
    test( _data1.delta_cy_r_lef   , _data2.delta_cy_r_lef   , "delta_cy_r_lef"   );
    test( _data1.cy_p             , _data2.cy_p             , "cy_p"             );
    test( _data1.delta_cy_p_lef   , _data2.delta_cy_p_lef   , "delta_cy_p_lef"   );
    test( _data1.cz_dh_n25        , _data2.cz_dh_n25        , "cz_dh_n25"        );
    test( _data1.cz_dh_n10        , _data2.cz_dh_n10        , "cz_dh_n10"        );
    test( _data1.cz_dh_0          , _data2.cz_dh_0          , "cz_dh_0"          );
    test( _data1.cz_dh_p10        , _data2.cz_dh_p10        , "cz_dh_p10"        );
    test( _data1.cz_dh_p25        , _data2.cz_dh_p25        , "cz_dh_p25"        );
    test( _data1.cz_lef           , _data2.cz_lef           , "cz_lef"           );
    test( _data1.delta_cz_sb      , _data2.delta_cz_sb      , "delta_cz_sb"      );
    test( _data1.cz_q             , _data2.cz_q             , "cz_q"             );
    test( _data1.delta_cz_q_lef   , _data2.delta_cz_q_lef   , "delta_cz_q_lef"   );
    test( _data1.eta_delta_h      , _data2.eta_delta_h      , "eta_delta_h"      );
    test( _data1.cl_dh_n25        , _data2.cl_dh_n25        , "cl_dh_n25"        );
    test( _data1.cl_dh_0          , _data2.cl_dh_0          , "cl_dh_0"          );
    test( _data1.cl_dh_p25        , _data2.cl_dh_p25        , "cl_dh_p25"        );
    test( _data1.cl_lef           , _data2.cl_lef           , "cl_lef"           );
    test( _data1.cl_da_20         , _data2.cl_da_20         , "cl_da_20"         );
    test( _data1.cl_da_20_lef     , _data2.cl_da_20_lef     , "cl_da_20_lef"     );
    test( _data1.cl_dr_30         , _data2.cl_dr_30         , "cl_dr_30"         );
    test( _data1.cl_r             , _data2.cl_r             , "cl_r"             );
    test( _data1.delta_cl_beta    , _data2.delta_cl_beta    , "delta_cl_beta"    );
    test( _data1.delta_cl_r_lef   , _data2.delta_cl_r_lef   , "delta_cl_r_lef"   );
    test( _data1.cl_p             , _data2.cl_p             , "cl_p"             );
    test( _data1.delta_cl_p_lef   , _data2.delta_cl_p_lef   , "delta_cl_p_lef"   );
    test( _data1.cm_dh_n25        , _data2.cm_dh_n25        , "cm_dh_n25"        );
    test( _data1.cm_dh_n10        , _data2.cm_dh_n10        , "cm_dh_n10"        );
    test( _data1.cm_dh_0          , _data2.cm_dh_0          , "cm_dh_0"          );
    test( _data1.cm_dh_p10        , _data2.cm_dh_p10        , "cm_dh_p10"        );
    test( _data1.cm_dh_p25        , _data2.cm_dh_p25        , "cm_dh_p25"        );
    test( _data1.cm_lef           , _data2.cm_lef           , "cm_lef"           );
    test( _data1.delta_cm_sb      , _data2.delta_cm_sb      , "delta_cm_sb"      );
    test( _data1.cm_q             , _data2.cm_q             , "cm_q"             );
    test( _data1.delta_cm_q_lef   , _data2.delta_cm_q_lef   , "delta_cm_q_lef"   );
    test( _data1.delta_cm         , _data2.delta_cm         , "delta_cm"         );
    test( _data1.delta_cm_ds      , _data2.delta_cm_ds      , "delta_cm_ds"      );
    test( _data1.cn_dh_n25        , _data2.cn_dh_n25        , "cn_dh_n25"        );
    test( _data1.cn_dh_0          , _data2.cn_dh_0          , "cn_dh_0"          );
    test( _data1.cn_dh_p25        , _data2.cn_dh_p25        , "cn_dh_p25"        );
    test( _data1.cn_lef           , _data2.cn_lef           , "cn_lef"           );
    test( _data1.cn_da_20         , _data2.cn_da_20         , "cn_da_20"         );
    test( _data1.cn_da_20_lef     , _data2.cn_da_20_lef     , "cn_da_20_lef"     );
    test( _data1.cn_dr_30         , _data2.cn_dr_30         , "cn_dr_30"         );
    test( _data1.cn_r             , _data2.cn_r             , "cn_r"             );
    test( _data1.delta_cn_beta    , _data2.delta_cn_beta    , "delta_cn_beta"    );
    test( _data1.delta_cn_delta_a , _data2.delta_cn_delta_a , "delta_cn_delta_a" );
    test( _data1.delta_cn_r_lef   , _data2.delta_cn_r_lef   , "delta_cn_r_lef"   );
    test( _data1.cn_p             , _data2.cn_p             , "cn_p"             );
    test( _data1.delta_cn_p_lef   , _data2.delta_cn_p_lef   , "delta_cn_p_lef"   );
}

////////////////////////////////////////////////////////////////////////////////

QTEST_APPLESS_MAIN(TestF16AeroData)

////////////////////////////////////////////////////////////////////////////////

#include "test_f16_aero_data.moc"
