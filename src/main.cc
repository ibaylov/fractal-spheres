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
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <iostream>

#include "geom.hh"
#include "geom-decorator.hh"
#include "viewport.hh"
#include "view.hh"
#include "model.hh"
#include "fractal-model.hh"
#include "oglview.hh"
#include <assert.h>

/**
@file  main.cc
@brief  The all things glued together
Here we mage singleton instances for a Model, View and Viewport, some basic setup and user interaction
See init() for the actual implementation code
*/


// globals/singletons

/////////////////////////////////////////////////
/// \brief gpVP - the cViewport singleton pointer
///
ogl::cViewport* gpVP  = nullptr;
/////////////////////////////////////////////////
/// \brief gpView - the cView singleton pointer
///
mvc::cView *    gpView = nullptr;
/////////////////////////////////////////////////
/// \brief gpModel - the cModel singleton pointer
///
mvc::cModel*    gpModel = nullptr;

////////////////////////////////////////////////
/// \brief ReshapeProc - called on window resize
/// \param nW - the new window width
/// \param nH - the new window height
///
void ReshapeProc(int nW, int nH)
{
    assert( gpVP != nullptr );
    gpVP->SetExtents( nW, nH );
}

void DisplayProc(); // forward decl

// make some stencils for the view

////////////////////////////////////////////////////
/// \brief The cGoldStencil class.
/// We implement in-place deruved class for object painting and a singleton that we pass to the view global
class cGoldStencil : public mvc::cView::cElementStencil
{
public:
    ~cGoldStencil(){}
    virtual void Apply( const mvc::cElement* pElem )
    {
        geom::scalar sIntensity = static_cast<geom::scalar>( 1 - pElem ->GetHierarchyDepth() % 6 / 6.0 );
        glColor4d( sIntensity, static_cast<geom::scalar>(0.9) * sIntensity, static_cast<geom::scalar>(0.1) * sIntensity, 1.0);
    }
} gcGoldStencil;

////////////////////////////////////////////////////
/// \brief The cPierotStencil class.
/// We implement in-place derived class for object painting and a singleton that we pass to the view global
class cPierotStencil : public mvc::cView::cElementStencil
{
public:
    ~cPierotStencil(){}
    virtual void Apply( const mvc::cElement* pElem )
    {
        switch( pElem ->GetHierarchyDepth() % 3 )
        {
            case 0:
            glColor4d( 1.0, 0.1, 0.1, 1.0);
            break;
            case 1:
            glColor4d( 0.1, 1.0, 0.1, 1.0);
            break;
            default:
            glColor4d( 0.1, 0.1, 1.0, 1.0);
        }
    }
} gcPierotStencil;

////////////////////////////////////////////////////
/// \brief The cVitroStencil class.
/// We implement in-place derived class for object painting and a singleton that we pass to the view global
class cVitroStencil : public mvc::cView::cElementStencil
{
public:
    ~cVitroStencil(){}
    virtual void Apply( const mvc::cElement* pElem )
    {
        size_t nDepth = pElem ->GetHierarchyDepth();
        switch( nDepth % 2 )
        {
            case 0:
            glColor4d( 0.5, 0.5, 0.5, 1);
            break;
            case 1:
            glColor4d( 0.2, 0.2, 0.2, 0.99);
            break;
        }
    }
} gcVitroStencil;

////////////////////////////////////////////////////
/// \brief KbdProc - the keyboard processing GLUT callback
/// \param key     - the arhument holds the ASCII value
/// \param x       - the X coord of mouse pointer, not used
/// \param y       - the Y coord of mouse pointer, not used
///
void KbdProc(unsigned char key, int x, int y)
{
    geom::scalar sDist = (geom::cPoint3d( 0, 0, 0 ) - gpVP->GetEyePoint()).Normalize();
    geom::scalar sC = 0.1 * sDist /static_cast<geom::scalar>(3.00);
    switch( key )
    {
        case '1':
            gpView->SetStencil( &gcGoldStencil );
        break;
        case '2':
            gpView->SetStencil( &gcPierotStencil );
        break;
        case '3':
            gpView->SetStencil( &gcVitroStencil);
        break;
        case 'a':
            gpVP->OrbitHorz( sC , geom::decorator::Degrees );
        break;
        case 'd':
            gpVP->OrbitHorz( -sC , geom::decorator::Degrees );
        break;
        case 'w':
            gpVP->OrbitVert( sC, geom::decorator::Degrees );
        break;
        case 's':
            gpVP->OrbitVert( -sC, geom::decorator::Degrees );
        break;
        case '[':
            gpVP->MoveInViewDir( sC / 10.0);
        break;
        case ']':
            gpVP->MoveInViewDir( -sC / 10.0);
        break;
        case '{':
            gpVP->AddFOV( -1, geom::decorator::Degrees );
        break;
        case '}':
            gpVP->AddFOV( 1, geom::decorator::Degrees );
        break;
        case 'A':
            gpVP->Yaw( sC , geom::decorator::Degrees );
        break;
        case 'D':
            gpVP->Yaw( -sC , geom::decorator::Degrees );
        break;
        case 'W':
            gpVP->Pitch( sC, geom::decorator::Degrees );
        break;
        case 'S':
            gpVP->Pitch( -sC, geom::decorator::Degrees );
        break;
        case ' ':
            gpVP ->Reset(geom::cPoint3d( 12, 0, 0 ), geom::cVector3d( -1, 0, 0), geom::cVector3d( 0,  0, 1 ),45, geom::decorator::Degrees );
        break;
        case '\033':
            // clear the singletons and quit
            // there should be better way to do this, though
            delete gpView;
            delete gpModel;
            delete gpVP;
            exit( 0);
        default:
            return;
    }

    DisplayProc();
}

/////////////////////////////////////////////////////////
/// \brief init - the initialization, called by us inmain
/// \param nW   - the window wdth
/// \param nH   - the window height
///
void init(int nW, int nH)
{
    glClearColor(0, 0 ,0, 0);
    glPointSize(1.0f);

    // ve create the viewport with Z-axis pointing up, since quads poles are on Z by default
    gpVP =  new ogl::cViewport( geom::cPoint3d( 12, 0, 0 ),
                                    geom::cVector3d( -1, 0, 0),
                                    geom::cVector3d( 0,  0, 1 ),
                                    45, geom::decorator::Degrees,
                                    nW, nH  );
    // this should be implemented in factory method/proc
    gpModel = new mvc::cFractalcModel();
    mvc::cOGLView* pvOGL  = new mvc::cOGLView();
    pvOGL->AssociateViewport( gpVP );
    pvOGL->AssociateModel( gpModel );
    gpView = pvOGL;
    gpView->SetStencil( &gcGoldStencil ); // we associate the first stencil instance with the view
}

///////////////////////////////////////////////////
/// \brief DisplayProc - the GLUT dispaly callback
///
void DisplayProc()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // clear buffers before render

    gpView->Display();  // call view actual render implementation

    glFlush();          // flush the OpenGL state machine buffers
    glutSwapBuffers();  // and swap them

}

int main( int argc, char** argv )
{
    glutInit(&argc, argv);
    // we need double-buffering, alpha channel and depth buffering
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
    glutInitWindowPosition(200, 200);
    // let's choose 800x600 res for initial window; we can change this later by resizing it
    glutInitWindowSize(800, 600);
    glutCreateWindow("SphereFlake");
    // associate our callbacks
    glutDisplayFunc(DisplayProc);
    glutReshapeFunc(ReshapeProc);
    glutKeyboardFunc( KbdProc );
    // and do our initialization
    init(800, 600);
    // begin event processing loop
    glutMainLoop();

    return 0;
}
