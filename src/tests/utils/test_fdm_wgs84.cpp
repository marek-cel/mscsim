#include <iomanip>
#include <iostream>

#include <QString>
#include <QtTest>

#include <fdm/utils/fdm_WGS84.h>

////////////////////////////////////////////////////////////////////////////////

using namespace std;

////////////////////////////////////////////////////////////////////////////////

class WGS84Test : public QObject
{
    Q_OBJECT

public:

    WGS84Test();

private Q_SLOTS:

    void initTestCase();
    void cleanupTestCase();

    void testWGS2GEO();
    void testGEO2WGS();
};

////////////////////////////////////////////////////////////////////////////////

WGS84Test::WGS84Test() {}

////////////////////////////////////////////////////////////////////////////////

void WGS84Test::initTestCase() {}

////////////////////////////////////////////////////////////////////////////////

void WGS84Test::cleanupTestCase() {}

////////////////////////////////////////////////////////////////////////////////

void WGS84Test::testWGS2GEO()
{
    fdm::WGS84::Geo geo;
    fdm::Vector3 pos_wgs;

    // 0N 0E
    pos_wgs.x() = fdm::WGS84::_a;
    pos_wgs.y() = 0.0;
    pos_wgs.z() = 0.0;

    geo = fdm::WGS84::wgs2geo( pos_wgs );

//    std::cout << __FILE__ << "(" << __LINE__ << ") " << geo.lat << std::endl;
//    std::cout << __FILE__ << "(" << __LINE__ << ") " << geo.lon << std::endl;
//    std::cout << __FILE__ << "(" << __LINE__ << ") " << geo.alt << std::endl;

    QVERIFY2( fabs( geo.lat - 0.0 ) < 1.0e-5, "Failure 0N 0E" );
    QVERIFY2( fabs( geo.lon - 0.0 ) < 1.0e-5, "Failure 0N 0E" );
    QVERIFY2( fabs( geo.alt - 0.0 ) < 1.0e-4, "Failure 0N 0E" );

    // 0N 90E
    pos_wgs.x() = 0.0;
    pos_wgs.y() = fdm::WGS84::_a;
    pos_wgs.z() = 0.0;

    geo = fdm::WGS84::wgs2geo( pos_wgs );

    QVERIFY2( fabs( geo.lat - 0.0    ) < 1.0e-5, "Failure 0N 90E" );
    QVERIFY2( fabs( geo.lon - M_PI_2 ) < 1.0e-5, "Failure 0N 90E" );
    QVERIFY2( fabs( geo.alt - 0.0    ) < 1.0e-4, "Failure 0N 90E" );

    // 0N -90E
    pos_wgs.x() = 0.0;
    pos_wgs.y() = -fdm::WGS84::_a;
    pos_wgs.z() = 0.0;

    geo = fdm::WGS84::wgs2geo( pos_wgs );

    QVERIFY2( fabs( geo.lat - 0.0    ) < 1.0e-5, "Failure 0N -90E" );
    QVERIFY2( fabs( geo.lon + M_PI_2 ) < 1.0e-5, "Failure 0N -90E" );
    QVERIFY2( fabs( geo.alt - 0.0    ) < 1.0e-4, "Failure 0N -90E" );

    // 0N 180E
    pos_wgs.x() = -fdm::WGS84::_a;
    pos_wgs.y() = 0.0;
    pos_wgs.z() = 0.0;

    geo = fdm::WGS84::wgs2geo( pos_wgs );

    QVERIFY2( fabs( geo.lat - 0.0  ) < 1.0e-5, "Failure 0N 180E" );
    QVERIFY2( fabs( geo.lon - M_PI ) < 1.0e-5, "Failure 0N 180E" );
    QVERIFY2( fabs( geo.alt - 0.0  ) < 1.0e-4, "Failure 0N 180E" );

    // 0N 90W
    pos_wgs.x() = 0.0;
    pos_wgs.y() = -fdm::WGS84::_a;
    pos_wgs.z() = 0.0;

    geo = fdm::WGS84::wgs2geo( pos_wgs );

    QVERIFY2( fabs( geo.lat - 0.0    ) < 1.0e-5, "Failure 0N 90W" );
    QVERIFY2( fabs( geo.lon + M_PI_2 ) < 1.0e-5, "Failure 0N 90W" );
    QVERIFY2( fabs( geo.alt - 0.0    ) < 1.0e-4, "Failure 0N 90W" );

    // 90N 0E
    pos_wgs.x() = 0.0;
    pos_wgs.y() = 0.0;
    pos_wgs.z() = fdm::WGS84::_b;

    geo = fdm::WGS84::wgs2geo( pos_wgs );

    QVERIFY2( fabs( geo.lat - M_PI_2 ) < 1.0e-5, "Failure 90N 0E" );
    QVERIFY2( fabs( geo.lon - 0.0    ) < 1.0e-5, "Failure 90N 0E" );
    QVERIFY2( fabs( geo.alt - 0.0    ) < 1.0e-4, "Failure 90N 0E" );

    // 90S 0E
    pos_wgs.x() = 0.0;
    pos_wgs.y() = 0.0;
    pos_wgs.z() = -fdm::WGS84::_b;

    geo = fdm::WGS84::wgs2geo( pos_wgs );

    QVERIFY2( fabs( geo.lat + M_PI_2 ) < 1.0e-5, "Failure 90S 0E" );
    QVERIFY2( fabs( geo.lon - 0.0    ) < 1.0e-5, "Failure 90S 0E" );
    QVERIFY2( fabs( geo.alt - 0.0    ) < 1.0e-4, "Failure 90S 0E" );

    // 45N 45E
    pos_wgs.x() = 3194419.145061;
    pos_wgs.y() = 3194419.145061;
    pos_wgs.z() = 4487348.408802;

    geo = fdm::WGS84::wgs2geo( pos_wgs );

    QVERIFY2( fabs( geo.lat - M_PI_4 ) < 1.0e-5, "Failure 45N 45E" );
    QVERIFY2( fabs( geo.lon - M_PI_4 ) < 1.0e-5, "Failure 45N 45E" );
    QVERIFY2( fabs( geo.alt - 0.0    ) < 1.0e-4, "Failure 45N 45E" );
}

////////////////////////////////////////////////////////////////////////////////

void WGS84Test::testGEO2WGS()
{
    fdm::WGS84::Geo geo;
    fdm::Vector3 pos_wgs;

    // 0N 0E
    geo.lat = 0.0;
    geo.lon = 0.0;
    geo.alt = 0.0;

    pos_wgs = fdm::WGS84::geo2wgs( geo );

//    std::cout << __FILE__ << "(" << __LINE__ << ") " << std::fixed <<  pos_wgs.x() << std::endl;
//    std::cout << __FILE__ << "(" << __LINE__ << ") " << std::fixed <<  pos_wgs.y() << std::endl;
//    std::cout << __FILE__ << "(" << __LINE__ << ") " << std::fixed <<  pos_wgs.z() << std::endl;

    QVERIFY2( fabs( pos_wgs.x() - fdm::WGS84::_a ) < 1.0e-4, "Failure 0N 0E" );
    QVERIFY2( fabs( pos_wgs.y() - 0.0            ) < 1.0e-4, "Failure 0N 0E" );
    QVERIFY2( fabs( pos_wgs.z() - 0.0            ) < 1.0e-4, "Failure 0N 0E" );

    // 0N 90E
    geo.lat = 0.0;
    geo.lon = M_PI_2;
    geo.alt = 0.0;

    pos_wgs = fdm::WGS84::geo2wgs( geo );

    QVERIFY2( fabs( pos_wgs.x() - 0.0            ) < 1.0e-4, "Failure 0N 90E" );
    QVERIFY2( fabs( pos_wgs.y() - fdm::WGS84::_a ) < 1.0e-4, "Failure 0N 90E" );
    QVERIFY2( fabs( pos_wgs.z() - 0.0            ) < 1.0e-4, "Failure 0N 90E" );

    // 0N 90W
    geo.lat = 0.0;
    geo.lon = -M_PI_2;
    geo.alt = 0.0;

    pos_wgs = fdm::WGS84::geo2wgs( geo );

    QVERIFY2( fabs( pos_wgs.x() - 0.0            ) < 1.0e-4, "Failure 0N 90W" );
    QVERIFY2( fabs( pos_wgs.y() + fdm::WGS84::_a ) < 1.0e-4, "Failure 0N 90W" );
    QVERIFY2( fabs( pos_wgs.z() - 0.0            ) < 1.0e-4, "Failure 0N 90W" );

    // 0N 180E
    geo.lat = 0.0;
    geo.lon = M_PI;
    geo.alt = 0.0;

    pos_wgs = fdm::WGS84::geo2wgs( geo );

    QVERIFY2( fabs( pos_wgs.x() + fdm::WGS84::_a ) < 1.0e-4, "Failure 0N 180E" );
    QVERIFY2( fabs( pos_wgs.y() - 0.0            ) < 1.0e-4, "Failure 0N 180E" );
    QVERIFY2( fabs( pos_wgs.z() - 0.0            ) < 1.0e-4, "Failure 0N 180E" );

    // 90N 0E
    geo.lat = M_PI_2;
    geo.lon = 0.0;
    geo.alt = 0.0;

    pos_wgs = fdm::WGS84::geo2wgs( geo );

    QVERIFY2( fabs( pos_wgs.x() - 0.0            ) < 1.0e-4, "Failure 90N 0E" );
    QVERIFY2( fabs( pos_wgs.y() - 0.0            ) < 1.0e-4, "Failure 90N 0E" );
    QVERIFY2( fabs( pos_wgs.z() - fdm::WGS84::_b ) < 1.0e-4, "Failure 90N 0E" );

    // 90S 0E
    geo.lat = -M_PI_2;
    geo.lon = 0.0;
    geo.alt = 0.0;

    pos_wgs = fdm::WGS84::geo2wgs( geo );

    QVERIFY2( fabs( pos_wgs.x() - 0.0            ) < 1.0e-4, "Failure 90S 0E" );
    QVERIFY2( fabs( pos_wgs.y() - 0.0            ) < 1.0e-4, "Failure 90S 0E" );
    QVERIFY2( fabs( pos_wgs.z() + fdm::WGS84::_b ) < 1.0e-4, "Failure 90S 0E" );

    // 45N 45E
    geo.lat = M_PI_4;
    geo.lon = M_PI_4;
    geo.alt = 0.0;

    pos_wgs = fdm::WGS84::geo2wgs( geo );

    QVERIFY2( fabs( pos_wgs.x() - 3194419.145061 ) < 1.0e-3, "Failure 45N 45E" );
    QVERIFY2( fabs( pos_wgs.y() - 3194419.145061 ) < 1.0e-3, "Failure 45N 45E" );
    QVERIFY2( fabs( pos_wgs.z() - 4487348.408802 ) < 1.0e-3, "Failure 45N 45E" );
}

////////////////////////////////////////////////////////////////////////////////

QTEST_APPLESS_MAIN(WGS84Test)

////////////////////////////////////////////////////////////////////////////////

#include "test_fdm_wgs84.moc"
