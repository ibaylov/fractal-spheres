/*
* MIT License
* 
* Copyright (c) 2020 ibaylov@gmail.com
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#include "viewport.hh"
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <iostream>

namespace ogl
{
using namespace geom;

cViewport::cViewport()
{

}

cViewport::cViewport( const geom::cPoint3d& ptEye, const geom::cVector3d& vecView, const geom::cVector3d& vecUp, 
                      scalar sFOV,  geom::decorator::AngleUnit unitFOV, int nPortWidth, int nPortHeight )
: m_ptEye( ptEye), m_vecView( vecView ), m_vecUp( vecUp ), m_nW( nPortWidth ), m_nH( nPortHeight ) 
{
    m_sFOV = decorator::ScalarToRadians( sFOV,  unitFOV );
    m_vecView.Normalize();

    SetupOGLViev( );
}

void
cViewport::Reset ( const geom::cPoint3d& ptEye, const geom::cVector3d& vecView, const geom::cVector3d& vecUp,
             scalar sFOV,  geom::decorator::AngleUnit unitFOV )
{
    m_ptEye = ptEye;
    m_vecView  =  vecView ;
    m_vecUp  = vecUp ;
    m_sFOV = decorator::ScalarToRadians( sFOV,  unitFOV );
    m_vecView.Normalize();

    SetupOGLViev( );
}


cViewport::~cViewport()
{

}

void
cViewport::SetExtents( int nPortWidth, int nPortHeight )
{
    m_nW = nPortWidth;
    m_nH = nPortHeight;
    SetupOGLViev( );
}

void 
cViewport::TransformBasis( const cMatrix3d& matTrans )
{
    m_ptEye = matTrans * m_ptEye;
    m_vecView = matTrans * m_vecView;
    m_vecUp = matTrans * m_vecUp;

    m_vecView.Normalize();
    m_vecUp.Normalize();
}

void 
cViewport::OrbitHorz( scalar sAngle, decorator::AngleUnit unitAngle )
{
    cMatrix3d matRot;

    decorator::LoadRotation( matRot, m_vecUp, sAngle, unitAngle );
    TransformBasis(  matRot ); 

    SetupOGLViev( );
}

void 
cViewport::OrbitVert( scalar sAngle, decorator::AngleUnit unitAngle )
{
    cVector3d vecRight = m_vecView ^ m_vecUp;
    cMatrix3d matRot;

    decorator::LoadRotation( matRot, vecRight, sAngle, unitAngle );
    TransformBasis(  matRot ); 

    SetupOGLViev( );
}

void
cViewport::Pitch( scalar sAngle, decorator::AngleUnit unitAngle  )
{
    cVector3d vecRight = m_vecView ^ m_vecUp;
    cMatrix3d matRot, matOrg, matOrgBack;

    decorator::LoadTranslation( matOrg, cPoint3d( 0, 0, 0 ) - m_ptEye  );
    decorator::LoadRotation( matRot, vecRight, sAngle, unitAngle );
    decorator::LoadTranslation( matOrgBack,  m_ptEye  - cPoint3d( 0, 0, 0 ) );
    TransformBasis(  matOrgBack * matRot * matOrg );

    SetupOGLViev( );
}

void
cViewport::Yaw( scalar sAngle, decorator::AngleUnit unitAngle  )
{

    cMatrix3d matRot, matOrg, matOrgBack;

    decorator::LoadTranslation( matOrg, cPoint3d( 0, 0, 0 ) - m_ptEye  );
    decorator::LoadRotation( matRot, m_vecUp, sAngle, unitAngle );
    decorator::LoadTranslation( matOrgBack,  m_ptEye  - cPoint3d( 0, 0, 0 ) );
    TransformBasis(  matOrgBack * matRot * matOrg );

    SetupOGLViev( );
}

void 
cViewport::MoveInViewDir( scalar sMove )
{
    m_ptEye += m_vecView * sMove;
    SetupOGLViev( );
}

void
cViewport::AddFOV( geom::scalar sFOV, geom::decorator::AngleUnit unitFOV )
{
    m_sFOV += decorator::ScalarToRadians( sFOV,  unitFOV );
    if( m_sFOV > M_PI / 2 ) // clamp it
        m_sFOV = M_PI / 2;
    if( m_sFOV < M_PI / 20 )
        m_sFOV = M_PI / 20;

    SetupOGLViev( );
}

geom::scalar
cViewport::GetFOV() const
{
    return  m_sFOV;
}

void 
cViewport::SetupOGLViev( )
{
    scalar sAspect =  static_cast<scalar>(m_nW)/static_cast<scalar>(m_nH);
    // the VP
    glViewport(0, 0, m_nW, m_nH);
    // the projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();     
    gluPerspective( decorator::RadiansToScalar( m_sFOV, decorator::Degrees ), sAspect, 0.1, 30.0);
    // the viewport
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    cPoint3d ptDir = m_ptEye + m_vecView;    
    gluLookAt(	m_ptEye[X], m_ptEye[Y], m_ptEye[Z],
                ptDir[X], ptDir[Y], ptDir[Z],
                m_vecUp[X], m_vecUp[Y], m_vecUp[Z]);

    // extract planes
    scalar arrsProjection[gnDim3d*gnDim3d];
    scalar arrsView[gnDim3d*gnDim3d];
    
    #ifdef _GEOM_FLOAT_
    	glGetFloatv(GL_PROJECTION_MATRIX,arrsProjection);
    	glGetFloatv(GL_MODELVIEW_MATRIX,arrsView);        
    #else
    	glGetDoublev(GL_PROJECTION_MATRIX,arrsProjection);
    	glGetDoublev(GL_MODELVIEW_MATRIX,arrsView);        
    #endif

//#if 0
    cMatrix3d matProjection, matView;

    decorator::ImportOGLMatrix( matProjection, arrsProjection );
    decorator::ImportOGLMatrix( matView, arrsView );

    cMatrix3d matOut =  matProjection * matView   ;

    cTuple3d tplRowSum;
    tplRowSum = decorator::ElementSumMul( matOut[ 0 ], 1, matOut[ 3 ], 1 );
    m_arrPlanesClip[  Left ]  = cPlane3d( tplRowSum[ X ], tplRowSum[ Y ], tplRowSum[ Z ], tplRowSum[ W ]);

    tplRowSum = decorator::ElementSumMul( matOut[ 0 ], -1, matOut[ 3 ], 1 );
    m_arrPlanesClip[  Right ]  = cPlane3d( tplRowSum[ X ], tplRowSum[ Y ], tplRowSum[ Z ], tplRowSum[ W ]);

    tplRowSum = decorator::ElementSumMul( matOut[ 1 ],  1, matOut[ 3 ], 1 );
    m_arrPlanesClip[  Bottom ]  = cPlane3d( tplRowSum[ X ], tplRowSum[ Y ], tplRowSum[ Z ], tplRowSum[ W ]);

    tplRowSum = decorator::ElementSumMul( matOut[ 1 ],  -1, matOut[ 3 ], 1 );
    m_arrPlanesClip[  Top ]  = cPlane3d( tplRowSum[ X ], tplRowSum[ Y ], tplRowSum[ Z ], tplRowSum[ W ]);

    tplRowSum = decorator::ElementSumMul( matOut[ 2 ],  1, matOut[ 3 ], 1 );
    m_arrPlanesClip[  Near ]  = cPlane3d( tplRowSum[ X ], tplRowSum[ Y ], tplRowSum[ Z ], tplRowSum[ W ]);

    tplRowSum = decorator::ElementSumMul( matOut[ 2 ], -1, matOut[ 3 ], 1 );
    m_arrPlanesClip[  Far ]  = cPlane3d( tplRowSum[ X ], tplRowSum[ Y ], tplRowSum[ Z ], tplRowSum[ W ]);
}

bool     
cViewport::PointInFrustum( const cPoint3d&  ptTest )
{
    int cPlane;
    for( cPlane = 0; cPlane < gnClipPlanes ; cPlane ++ )
        if( m_arrPlanesClip[ cPlane ].PointDistance( ptTest ) < static_cast<scalar>(0.0))
            return false;
    return true;        
}

bool 
cViewport::SphereInFrustum( const cPoint3d& ptTest, scalar sR )
{
    int cPlane;
    for( cPlane = 0; cPlane < gnClipPlanes ; cPlane ++ )
        if( m_arrPlanesClip[ cPlane ].PointDistance( ptTest ) < - sR )
            return false;
    return true;        
} 

scalar
cViewport::PointMinimalFrustumDistance( const cPoint3d& ptTest)
{
    geom::scalar sDist = static_cast<scalar>(1.0);
    int cPlane;
    for( cPlane = 0; cPlane < gnClipPlanes ; cPlane ++ )
    {
        scalar sR = m_arrPlanesClip[ cPlane ].PointDistance( ptTest );
        if( sR < sDist )
            sDist =sR;
    }
    return sDist;
}


const geom::cPoint3d 
cViewport::GetEyePoint()
{
    return m_ptEye;
}

///////////////////////////////////////////////////////////////////////////////////////

scalar 
cViewport::SegmentVisibleAngle( const cPoint3d& ptOrg, scalar sLen )
{

    return fabs(acos( SegmentVisibleCosine( ptOrg, sLen ) ));
} 

scalar
cViewport::SegmentVisibleCosine( const geom::cPoint3d& ptOrg, geom::scalar sLen )
{
    cVector3d vecPt = ptOrg - m_ptEye;
    vecPt.Normalize();

    cVector3d vecLocalUp = m_vecView ^ vecPt;
    cVector3d vecLocalRadius;
    if( (m_vecView - vecPt)*(m_vecView - vecPt) < cTuple3d::gsEps ) // they are colinear, so we just use the up vector
        vecLocalRadius = m_vecUp;
    else
        vecLocalRadius =  vecPt ^ vecLocalUp;
    vecLocalRadius.Normalize();

    cPoint3d ptSegment = ptOrg + vecLocalRadius * sLen;
    cVector3d vecOut = ptSegment - m_ptEye;
    vecOut. Normalize();
    //ptOrg.Dump();
    //ptSegment.Dump();
    return vecPt * vecOut;
}


} //NS end
