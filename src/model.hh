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
#ifndef _MVC_MODEL_
#define _MVC_MODEL_
#include "oblist.hh"
#include "geom.hh"

/**
@file  model.hh
@brief the cModel is our base class for models.
We are using MVC pattern in our applicaton to separate the data from the presentation.
Our model exports a tree of elements, statring with the root element
*/
namespace mvc
{
    ////////////////////////////////////////////////////////////////////
    /// \brief The cElement class
    /// Represents the abstract model element
    ///
    class cElement
    {
    protected:
        /// \brief m_matLCS - the local coordinate system matrix of the model
        // we suppose that every element has its local coordinats system transform in model space
        geom::cMatrix3d   m_matLCS;

        /// \brief m_nHierarchyDepth - the level in model tree hierarchy the model resides on
        /// we suppose that the model is hierarchical
        size_t            m_nHierarchyDepth;

    public:
        cElement();
        cElement( size_t nLevel, const geom::cMatrix3d& matCS );
        virtual ~cElement(); //!< It will be inherited with overloaded methods, so the virtual destructor
        // operations
        const geom::cMatrix3d& GetLocalCS() const; //!< Rerurns the local CS in model space
                        size_t GetHierarchyDepth() const; //!< Reeturns the level of hierarchy of the element in the model tree

        virtual geom::scalar GetBoundingSphereRadius() const = 0; //!< Retrieves the bounding sphere of the element
        virtual geom::scalar GetDescendantSphereRadius() const = 0; //!< Retrieves the bounding sphere of the element and all its descendants
    };

    ////////////////////////////////////////////////////////////////////
    /// \brief The cModel class
    /// A base class for models used for visualisation
    /// Еxports a tree of elements, statring with the root element
    class cModel
    {
    protected:
    public:

        cModel();
#ifndef _NO_CXX_11_
        cModel( const cModel& ) = delete; //!<< Prevent model direct copy
#endif
        virtual ~cModel(); //!< It will be inherited with overloaded methods, so the virtual destructor
// operations
        virtual cElement* GetRootElement() = 0; //!< Retrieves the root element ot the model
        virtual utl::cObList<cElement*> GetDescendantElements( cElement* ) = 0; //!< Retrieves the direct descendant elements
        // since we will generate a dynamic se of elemets lazy evaluating the model, we will have to clean up the temporary results after that
        virtual void Collect() = 0; //!< Collects the intermediate results produced by the model enymeration
    };
}

#endif
