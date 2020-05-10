#include "oglview.hh"
#include <GL/glu.h>
#include <GL/glut.h>

#include <cmath>
#include "oblist.hh"
#include <assert.h>
#ifdef _DEBUG_DUMP_
#include <iostream>
#endif

namespace mvc
{

/////////////////////////////////////////////////////////
// cOGLView implementation

cOGLView::cOGLView ()
    : m_pVP( nullptr )
{
    SetupScene();

}

cOGLView::~cOGLView ()
{
    SetupScene();

}

void
cOGLView::AssociateViewport( ogl::cViewport* pVP )
{
    _ASSERT( pVP );
    m_pVP = pVP;
}

const int nDrawQueues = 4;


void
cOGLView::SetupScene()
{
    glLineWidth( 1.0 );

    // OK, set up the light first, proudly stolen from OGL example code
    GLfloat mat_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
    GLfloat model_ambient[] = { 0.5, 0.5, 0.5, 1.0 };



    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glEnable( GL_COLOR_MATERIAL );


    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_NORMALIZE);

    glDepthFunc(GL_LEQUAL);
    glBlendFunc( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);

    glShadeModel (GL_SMOOTH);

    // allocate different LOD liasts
    m_uLODDisplayLists = glGenLists ( nDrawQueues );

    GLUquadricObj *quadric;

    glNewList(m_uLODDisplayLists + Low, GL_COMPILE);

        quadric = gluNewQuadric();

        gluQuadricDrawStyle(quadric, GLU_LINE ); // this creates a neat little pseudotexture on very small objects
        gluQuadricNormals( quadric, GLU_SMOOTH);
        gluQuadricOrientation(	quadric, GLU_OUTSIDE );

        gluSphere( quadric , 1 , 8, 8 );

        gluDeleteQuadric(quadric);

   glEndList();

   glNewList(m_uLODDisplayLists + Meduim, GL_COMPILE);

       quadric = gluNewQuadric();

       gluQuadricDrawStyle(quadric, GLU_FILL );// set GLU_LINE ) for wireframe
       gluQuadricNormals( quadric, GLU_SMOOTH);
       gluQuadricOrientation(	quadric, GLU_OUTSIDE );

       gluSphere( quadric , 1 , 16, 16 );

       gluDeleteQuadric(quadric);

  glEndList();

  glNewList(m_uLODDisplayLists + High, GL_COMPILE);

      quadric = gluNewQuadric();

      gluQuadricDrawStyle(quadric, GLU_FILL );/// set GLU_LINE ) for wireframe
      gluQuadricNormals( quadric, GLU_SMOOTH);
      gluQuadricOrientation(	quadric, GLU_OUTSIDE );

      gluSphere( quadric , 1 , 24, 24 );

      gluDeleteQuadric(quadric);

  glEndList();

  glNewList(m_uLODDisplayLists + Highest, GL_COMPILE);

      quadric = gluNewQuadric();

      gluQuadricDrawStyle(quadric, GLU_FILL ); // set GLU_LINE ) for wireframe
      gluQuadricNormals( quadric, GLU_SMOOTH);
      gluQuadricOrientation(	quadric, GLU_OUTSIDE );

      gluSphere( quadric , 1 , 32, 32 );

      gluDeleteQuadric(quadric);

  glEndList();
}


void
cOGLView::DrawElement( const cElement* pElem, LevelOfSDetail lodView )
{
    glPushMatrix();

    // because our lists are with sphere size of one, we have to scale the sphere accordingly
    geom::scalar sR = pElem->GetBoundingSphereRadius();
    geom::cMatrix3d matScale;
    geom::decorator::LoadScale( matScale, geom::cVector3d( sR, sR, sR ));

    geom::cMatrix3d matCS = pElem ->GetLocalCS() * matScale ;

    // place the elemt CS in the GL modelvuew matrix
    geom::scalar sMatGL[16];
    geom::decorator::ExportOGLMatrix( sMatGL, matCS );

#ifdef _GEOM_FLOAT_
    glMultMatrixf( sMatGL );
#else
    glMultMatrixd( sMatGL );
#endif

    if( m_pStencil )
        m_pStencil->Apply( pElem );
    else
        glColor4f( 1, 1, 1, 1 );
    // call the compiled sphere loist to draw it
    // in a 'real' application code here be dragons. Well, here be the type switch/virtual mechanism to render specific object type
    // no we just select the compiled sphere list based on LOD
    glCallList( m_uLODDisplayLists + lodView );

    glPopMatrix();
}


void
cOGLView::ClassifyElement( const cElement* pElem, ObjectClassifier& ocElem )
{
    geom::cPoint3d ptLocalCenter = pElem->GetLocalCS() * geom::cPoint3d( 0, 0, 0 );
    geom::scalar sViewCosine =   m_pVP->SegmentVisibleCosine( ptLocalCenter, pElem->GetBoundingSphereRadius());
    geom::scalar sMinDistance = m_pVP->PointMinimalFrustumDistance( ptLocalCenter );
    geom::scalar sFOVCoef = m_pVP->GetFOV() / ( M_PI / 4 );  // ve take the viewport FOV / ( pi / 4 ) as a reference (neutral) view angle
    if( sViewCosine > cos( sFOVCoef * 0.15 * M_PI / 180.0) ) // if the object viewing angle corrected for zoom is below 0.15 rad, it's invisible
    {
        ocElem.m_LOD = Invisible;
        ocElem.m_bVisible = false;
        ocElem.m_bTreeVisible = false;
        return;
    }
    // set the LOD according to view angle
    //   we are using the following heuristic here based on the Object viewing angle corrected for vieport zoom
    if( sViewCosine > cos( sFOVCoef * 0.5 * M_PI / 180.0))
        ocElem.m_LOD = Low;
    else
    if( sViewCosine > cos( sFOVCoef * 1 * M_PI / 180.0))
        ocElem.m_LOD = Meduim;
    else
    if( sViewCosine > cos( sFOVCoef * 0.25 * M_PI / 180.0))
        ocElem.m_LOD = High;
    else
        ocElem.m_LOD = Highest;
    // set the visibility indicators
    ocElem.m_bVisible =  ( sMinDistance >= - pElem->GetBoundingSphereRadius());
    ocElem.m_bTreeVisible =(sMinDistance >=  - pElem->GetDescendantSphereRadius());

    _ASSERT( ocElem.m_bVisible == m_pVP->SphereInFrustum(ptLocalCenter, pElem->GetBoundingSphereRadius()) );
    _ASSERT( ocElem.m_bTreeVisible == m_pVP->SphereInFrustum(ptLocalCenter, pElem->GetDescendantSphereRadius()) );

}

bool
cOGLView::OccludesCompletely( const cElement* pOuter, const cElement* pInner)
{
    // OK, we are doing a major cheat here
    // The proper implementation should be using cone/sphere intersection test
    // To speed up the process we simply place a plane behind the possible occluder and see if the inner object is behind it
    // This wil not work properly on object hierarchies where the children descendant bounding spheres don't touch the occluders sphere

    geom::cPoint3d ptOuterCenter = pOuter->GetLocalCS() * geom::cPoint3d( 0, 0, 0 );
    geom::cPoint3d ptInnerCenter = pInner->GetLocalCS() * geom::cPoint3d( 0, 0, 0 );
    geom::cVector3d vecOuter = (ptOuterCenter - m_pVP->GetEyePoint());
    geom::scalar sOuter = vecOuter.Normalize();

    geom::cPoint3d ptPlane = m_pVP->GetEyePoint() + vecOuter * ( sOuter + /*0.8*/ 0.6 * pOuter->GetBoundingSphereRadius());
    geom::cPlane3d planeCull( vecOuter, ptPlane );

    if( planeCull.PointDistance(  ptInnerCenter ) >= pInner->GetDescendantSphereRadius())
        return true;
    return false;
}

void
cOGLView::DisplayImpl()
{
    glMatrixMode(GL_MODELVIEW);


    // draw model
    utl::cObList<cElement*> lstOpen;
    utl::cObList<const cElement*> alstDraw[nDrawQueues];
    cElement* pElemRoot = m_pModel->GetRootElement();
    lstOpen.Add( pElemRoot );

    // some stats
    size_t nProcessed = 0;
    size_t nCulled = 0;
    size_t nOcluded = 0;
    // not, the recursive part
    ObjectClassifier ocElem;
    while( lstOpen.HasData()  )
    {
        cElement* pElem = lstOpen.PullHead();
        nProcessed ++;
        // classify element
        ClassifyElement( pElem, ocElem );
        // if the element and its descendands are ocluded, terminate the recursion
        nCulled ++;
        if(  ! ocElem.m_bTreeVisible )
            continue;
        nCulled --;
        // if elemt is not occluded, draw it, placiong on draw queue according to LOD
        if( ocElem.m_bVisible )
            alstDraw[ ocElem.m_LOD ].Add( pElem );
        // get the descendands and push them to the open list
        utl::cObList<cElement*> lstDesc = m_pModel->GetDescendantElements(pElem);
        while( lstDesc.HasData() )
        {
            cElement* pelemChild = lstDesc.PullHead();
            if( ! OccludesCompletely( pElem, pelemChild ) )
                lstOpen.Add(  pelemChild );
            else
                nOcluded ++;
        }
        ///
    }
#ifdef _DEBUG_DUMP_
    std::cerr << "Processed: " << nProcessed << ", Culled:" << nCulled << ", Ocluded: " << nOcluded;
#endif
    int cQueue;
    for( cQueue = 0; cQueue < nDrawQueues; cQueue ++ )
    {
        size_t nEnq = 0;
        while( alstDraw [ cQueue ].HasData())
        {
            DrawElement( alstDraw [ cQueue ].PullHead(), (LevelOfSDetail) cQueue);
            nEnq ++;
        }
#ifdef _DEBUG_DUMP_
        std::cerr << ", Queued for drawing in " << cQueue << ":" << nEnq;
#endif
    }
#ifdef _DEBUG_DUMP_
    std::cerr << std::endl;
#endif
}

} // NS end
