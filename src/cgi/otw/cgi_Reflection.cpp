/****************************************************************************//*
 * Copyright (C) 2021 Marek M. Cel
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 ******************************************************************************/

#include <cgi/otw/cgi_Reflection.h>

#include <osg/ClipNode>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/LOD>
#include <osg/MatrixTransform>
#include <osg/Program>

#include <cgi/cgi_Textures.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

const char Reflection::_frag[] =
    "uniform sampler2D reflection;\n"
    "uniform sampler2D refraction;\n"
    "uniform sampler2D normalTex;\n"
    "varying vec4 projCoords;\n"
    "varying vec3 lightDir, eyeDir;\n"
    "varying vec2 flowCoords, rippleCoords;\n"
    "\n"
    "void main()\n"
    "{\n"
    "   vec2 rippleEffect = 0.02 * texture2D(refraction, rippleCoords * 0.1).xy;\n"
    "   vec4 N = texture2D(reflection, flowCoords + rippleEffect);\n"
    "   N = N * 2.0 - vec4(1.0);\n"
    "   N.a = 1.0; N = normalize(N);\n"
    "\n"
    "   vec3 refVec = normalize(reflect(-lightDir, vec3(N) * 0.6));\n"
    "   float refAngle = clamp(dot(eyeDir, refVec), 0.0, 1.0);\n"
    "   vec4 specular = vec4(pow(refAngle, 40.0));\n"
    "\n"
    "   vec2 dist = texture2D(refraction, flowCoords + rippleEffect).xy;\n"
    "   float dist_inv = 1.0 / sqrt( dist.x*dist.x + dist.y*dist.y );\n"
    "   dist = (dist * 2.0 - vec2(1.0)) * dist_inv * 0.02;\n"
    "   vec2 uv = projCoords.xy / projCoords.w;\n"
    "   uv = clamp((uv + 1.0) * 0.5 + dist, 0.0, 1.0);\n"
    "\n"
    "   vec4 refl = texture2D(reflection, uv);\n"
    "   refl.w = refl.w * 0.4;\n"
    "   gl_FragColor = mix(refl, refl + specular, 0.6);\n"
    "}\n"
    "\n";

const char Reflection::_vert[] =
    "uniform float osg_FrameTime;\n"
    "varying vec4 projCoords;\n"
    "varying vec3 lightDir, eyeDir;\n"
    "varying vec2 flowCoords, rippleCoords;\n"
    "\n"
    "void main()\n"
    "{\n"
    "   vec3 T = vec3(0.0, 1.0, 0.0);\n"
    "   vec3 N = vec3(0.0, 0.0, 1.0);\n"
    "   vec3 B = vec3(1.0, 0.0, 0.0);\n"
    "   T = normalize(gl_NormalMatrix * T);\n"
    "   B = normalize(gl_NormalMatrix * B);\n"
    "   N = normalize(gl_NormalMatrix * N);\n"
    "\n"
    "   mat3 TBNmat;\n"
    "   TBNmat[0][0] = T[0]; TBNmat[1][0] = T[1]; TBNmat[2][0] = T[2];\n"
    "   TBNmat[0][1] = B[0]; TBNmat[1][1] = B[1]; TBNmat[2][1] = B[2];\n"
    "   TBNmat[0][2] = N[0]; TBNmat[1][2] = N[1]; TBNmat[2][2] = N[2];\n"
    "\n"
    "   vec3 vertexInEye = vec3(gl_ModelViewMatrix * gl_Vertex);\n"
    "   lightDir =  gl_LightSource[0].position.xyz - vertexInEye;\n"
    "   lightDir = normalize(TBNmat * lightDir);\n"
    "   eyeDir = normalize(TBNmat * (-vertexInEye));\n"
    "\n"
    "   vec2 t1 = vec2(osg_FrameTime*0.02, osg_FrameTime*0.02);\n"
    "   vec2 t2 = vec2(osg_FrameTime*0.05, osg_FrameTime*0.05);\n"
    "   flowCoords = gl_MultiTexCoord0.xy * 5.0 + t1;\n"
    "   rippleCoords = gl_MultiTexCoord0.xy * 10.0 + t2;\n"
    "\n"
    "   gl_TexCoord[0] = gl_MultiTexCoord0;\n"
    "   gl_Position = ftransform();\n"
    "   projCoords = gl_Position;\n"
    "}\n"
    "\n";

////////////////////////////////////////////////////////////////////////////////

void Reflection::create( osg::Node *model, osg::Group *parent )
{
    const float z = -0.1f;

    osg::ref_ptr<osg::LOD> lodReflection = new osg::LOD();
    osg::ref_ptr<osg::Group> groupReflection = new osg::Group();

    lodReflection->addChild( groupReflection.get(), 0.0f, 10000.0f );

    osg::ref_ptr<osg::MatrixTransform> reverse = new osg::MatrixTransform;
    reverse->preMult( osg::Matrix::translate(0.0f, 0.0f, -z) *
                      osg::Matrix::scale(1.0f, 1.0f, -1.0f) *
                      osg::Matrix::translate(0.0f, 0.0f, z) );
    reverse->addChild( model );

    osg::ref_ptr<osg::ClipPlane> clipPlane = new osg::ClipPlane;
    clipPlane->setClipPlane( 0.0, 0.0, 1.0, z );
    clipPlane->setClipPlaneNum( 0 );

    osg::ref_ptr<osg::ClipNode> clipNode = new osg::ClipNode;
    clipNode->addClipPlane( clipPlane.get() );
    clipNode->addChild( reverse.get() );

    ////////////////////////////////

    if ( 1 )
    {
        // The RTT camera
        osg::ref_ptr<osg::Texture2D> tex2D = new osg::Texture2D();
        tex2D->setTextureSize( 1024, 1024 );
        tex2D->setInternalFormat( GL_RGBA );
        tex2D->setFilter( osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR );
        tex2D->setFilter( osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR );

        osg::ref_ptr<osg::Camera> cameraRTT = new osg::Camera();

        cameraRTT->setClearColor( osg::Vec4() );
        cameraRTT->setClearMask( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );
        cameraRTT->setRenderTargetImplementation( osg::Camera::FRAME_BUFFER_OBJECT );
        cameraRTT->setRenderOrder( osg::Camera::PRE_RENDER );

        cameraRTT->setViewport( 0, 0, tex2D->getTextureWidth(), tex2D->getTextureHeight() );
        cameraRTT->attach( osg::Camera::COLOR_BUFFER, tex2D.get() );

        cameraRTT->addChild( clipNode.get() );

        // The water plane
        const osg::Vec3& center = model->getBound().center();
        float planeSize = 4.0f * model->getBound().radius();
        //float planeSize = 200.0f;

        osg::Vec3 planeCorner( center.x() - 0.5f*planeSize, center.y() - 0.5f*planeSize, z );

        osg::ref_ptr<osg::Geometry> quad = osg::createTexturedQuadGeometry(
                    planeCorner,
                    osg::Vec3( planeSize, 0.0f, 0.0f ),
                    osg::Vec3( 0.0f, planeSize, 0.0f ) );

        osg::ref_ptr<osg::Geode> geodeQuad = new osg::Geode;
        geodeQuad->addDrawable( quad.get() );

        osg::ref_ptr<osg::Texture2D> texWaterDUDV = Textures::get( "cgi/textures/water_dudv.png" );
        texWaterDUDV->setWrap( osg::Texture::WRAP_S, osg::Texture::REPEAT );
        texWaterDUDV->setWrap( osg::Texture::WRAP_T, osg::Texture::REPEAT );
        texWaterDUDV->setFilter( osg::Texture::MIN_FILTER, osg::Texture::LINEAR );
        texWaterDUDV->setFilter( osg::Texture::MAG_FILTER, osg::Texture::LINEAR );

        osg::ref_ptr<osg::Texture2D> texWaterNM = Textures::get( "cgi/textures/water_nm.png" );
        texWaterNM->setWrap( osg::Texture::WRAP_S, osg::Texture::REPEAT );
        texWaterNM->setWrap( osg::Texture::WRAP_T, osg::Texture::REPEAT );
        texWaterNM->setFilter( osg::Texture::MIN_FILTER, osg::Texture::LINEAR );
        texWaterNM->setFilter( osg::Texture::MAG_FILTER, osg::Texture::LINEAR );

        osg::ref_ptr<osg::StateSet> stateSet = geodeQuad->getOrCreateStateSet();
        stateSet->setTextureAttributeAndModes( 0, tex2D.get()        , osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE );
        stateSet->setTextureAttributeAndModes( 1, texWaterDUDV.get() , osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE );
        stateSet->setTextureAttributeAndModes( 2, texWaterNM.get()   , osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE );

        osg::ref_ptr<osg::Program> program = new osg::Program();
        program->addShader( new osg::Shader( osg::Shader::VERTEX   , _vert ) );
        program->addShader( new osg::Shader( osg::Shader::FRAGMENT , _frag ) );
        stateSet->setAttributeAndModes( program.get() );
        stateSet->addUniform( new osg::Uniform("defaultTex" , 0) );
        stateSet->addUniform( new osg::Uniform("refraction" , 1) );
        stateSet->addUniform( new osg::Uniform("normalTex"  , 2) );

        ////////////////////////////////

        //groupReflection->addChild( model );
        groupReflection->addChild( cameraRTT.get() );
        groupReflection->addChild( geodeQuad.get() );

        parent->addChild( lodReflection.get() );
    }
    else
    {
        parent->addChild( clipNode.get() );
    }
}
