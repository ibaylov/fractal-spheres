#ifndef _MVC_VIEW_
#define _MVC_VIEW_
#include "model.hh"

/**
@file  view.hh
@brief  The cView class defines tha basic View functionality
The cView defines tha view side of the MVC pattern for our project
*/

namespace mvc
{
    ////////////////////////////////////////////////////////////////////////////
    /// \brief The cView class - base class for model visualization
    ///
    class cView
    {
        public:
            ////////////////////////////////////////////////////////////////////
            /// \brief The cElementStencil class.
            /// All cView objects may have an instance of cElementStencil injected; the Decorate method is called before each element is rendered
            class cElementStencil
            {
            public:
                cElementStencil();          //!< Default
                virtual ~cElementStencil(); //!< The destructor is virtual, since we plan to inherit the class
                virtual void Apply( const cElement* pElem ) = 0;    //!< In this method we apply the stencil to the object
            };
        protected:
            /// \brief m_pModel - the model we are to visu<lize
            ///
            cModel*             m_pModel;
            /// \brief m_pDecorator - the decorator instance pointer, used before rendering each element
            ///
            cElementStencil*  m_pStencil;
        public:
            cView();    //!< Default
            #ifndef _NO_CXX_11_
                    cView( const cView& ) = delete; //!<< Prevent model direct copy
            #endif
            virtual ~cView(); //!< it's abstract class and should be overrided
            // operations
            void AssociateModel( cModel* ); //!< Associates model with the view
            void SetStencil( cElementStencil* ); //!< Sets and replaces the current stencil

            void Display() ; //!< caleld to display the model. Calls Dism=playImpl and runs the garbage collection after
        protected:
            virtual void DisplayImpl() = 0; //!< override this to do specific drawind
    };
}

#endif
