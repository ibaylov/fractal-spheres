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
