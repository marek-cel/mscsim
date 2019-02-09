#ifndef SCENEROOT_H
#define SCENEROOT_H

////////////////////////////////////////////////////////////////////////////////

#include <osg/Group>

////////////////////////////////////////////////////////////////////////////////

/** */
class SceneRoot
{
public:

    static const osg::Vec3d colorFuselage;
    static const osg::Vec3d colorRotor;

    static osg::Vec3d getNormal( const osg::Vec3d &r0,
                                 const osg::Vec3d &r1,
                                 const osg::Vec3d &r2 );

    /** */
    SceneRoot();

    /** */
    virtual ~SceneRoot();

    void update( const double coning, const double roll, const double pitch );

    /** Returns root node. */
    inline osg::ref_ptr<osg::Group> getRoot() { return m_root; }

private:

    osg::ref_ptr<osg::Group> m_root;
    osg::ref_ptr<osg::PositionAttitudeTransform> m_rotor;

    void createHelicopter();

    void createSceneLight();

    void drawStartSection( const osg::Vec3d &p0, const osg::Vec3d &p1,
                           const double ry, const double rz,
                           const int nodes = 16 );

    void drawCrossSection( const osg::Vec3d &p0, const osg::Vec3d &p1,
                           const double ry_1, const double rz_1,
                           const double ry_2, const double rz_2,
                           const int nodes = 16 );

    void drawRotor( osg::Group *parent, const double coning,
                    const double r, const osg::Vec4d &color,
                    const int nodes = 32 );
};

////////////////////////////////////////////////////////////////////////////////

#endif // SCENEROOT_H
