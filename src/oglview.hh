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
#ifndef _MVC_OGLVIEW_
#define _MVC_OGLVIEW_
#include "view.hh"
#include "viewport.hh"

/**
@file  oglview.hh
@brief  The cOGLView class implements the cView for OpenGL
The cOGLView is as much cElement's implementation agnostic as possible
*/


#ifdef _NO_CXX_11_
#define override
#endif

namespace mvc
{
    ////////////////////////////////////////////////////////////////////////////
    /// \brief The cOGLView class - view implementation for OpenGL
    ///
    class cOGLView : public cView
    {
        protected:
            /// \brief m_pVP - the viewport that we are using for visibility tests
            ///
            ogl::cViewport* m_pVP;
        public:
            cOGLView ();
            #ifndef _NO_CXX_11_
            cOGLView ( const cOGLView & ) = delete; //!<< Prevent model direct copy
            #endif
            virtual ~cOGLView () override;
            void AssociateViewport( ogl::cViewport* ); //!< Associates a viewport with the view

        protected:
            virtual void DisplayImpl() override; //!< override this to do specific drawind
            // visibility check operations
            unsigned m_uLODDisplayLists;

            enum LevelOfSDetail
            {
                Invisible = -1,
                Low       = 0,
                Meduim    = 1,
                High      = 2 ,
                Highest   = 3
            };

            void SetupScene(); //!< Set up colors, lights, etc
            void DrawElement( const cElement*, LevelOfSDetail ); //!< Draws a single element


            struct ObjectClassifier
            {
                LevelOfSDetail m_LOD;           //!< Calculated level of detail
                bool           m_bVisible;      //!< The element is (potentially) visible
                bool           m_bTreeVisible;  //!< The element and its thescendants are (potentially) visible
            };

            void ClassifyElement( const cElement*, ObjectClassifier& );  //!< Classify visibility against the viewport
            bool OccludesCompletely( const cElement*, const cElement* ); //!< Checks if an element cooludes the other completely
    };
}

#endif
