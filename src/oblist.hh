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
#ifndef _OBLIST_HH_
#define _OBLIST_HH_
#include "assert.hh"

/**
@file  oblist.hh
@brief A template-based object list
We are restricted in our STL usage, but necessity is the mother of invention.
*/

namespace utl
{
    //////////////////////////////////////////////////
    /// \brief The cObList template
    /// implements a singly-linked list
    template<typename Data> class cObList
    {
        protected:
            struct Elem
            {
                Data    m_Data;     //!< data member ot template argument type
                Elem*   m_pNext;    //!< pointer to the next element
            } * m_pList;            //!< the list start element
        public:
            //////////////////////////////////////////////////
            /// \brief cObList::cObList
            /// The default ampty list constructor
            cObList()
                : m_pList( 0 )
            {

            }

            /////////////////////////////////////////////////
            /// \brief cObList::~cObList
            ///  We remove the linked elemens; we don't do any special processing on the data
            ~cObList()
            {
                while( m_pList ) // enumerating elements
                {
                    Elem* pNext = m_pList->m_pNext;
                    delete m_pList;
                    m_pList = pNext;
                }
            }

            /////////////////////////////////////////////////
            /// \brief cObList::cObList
            /// The copy constructor
            cObList( const  cObList& rOther )
            {
                CopyFrom( rOther ); // it's always nice to have an explicit copy on hand
            }

            #ifndef _NO_CXX_11_
            /////////////////////////////////////////////////
            /// \brief cObList::cObList
            /// The rvalue assignment constructor
            cObList( cObList&& rOther)
            {
                m_pList = rOther.m_pList;
                rOther.m_pList = 0;
            }
            #endif

            /////////////////////////////////////////////////
            /// \brief cObList::CopyFrom
            /// The explicit copy function
            /// \param rOther -  list we are copying from
            const  cObList& CopyFrom( const  cObList& rOther )
            {
                Elem* pOther = rOther.m_pList;
                // adding the elemets in reverse order, but that's ok for our particulat purpose
                for( ; pOther; pOther = pOther->m_pNext )
                    Add( pOther->m_Data );
                return *this;
            }

            /////////////////////////////////////////////////
            /// \brief cObList::Add
            /// Add (actually prepend) a data element to the list
            /// \param dataIn -  the data to be added to the list
            void Add( Data dataIn )
            {
                Elem* pElem = new Elem;
                pElem->m_pNext =  m_pList;
                m_pList = pElem;
                pElem->m_Data = dataIn;
            }

            /////////////////////////////////////////////////
            /// \brief cObList::HasData
            /// Predicate for non-empty list
            bool HasData( )
            {
                return m_pList != 0;
            }

            /////////////////////////////////////////////////
            /// \brief cObList::PullHead
            /// Pulls the (first) elememt from the list and returns the data associated
            /// \returns the pulled elemet data
            Data PullHead( )
            {
                _ASSERT( m_pList ); // make sure we are pulling off elements only if the list is not empty
                Data  dRV = m_pList->m_Data;
                Elem* pNext = m_pList->m_pNext;
                delete m_pList;
                m_pList = pNext;
                return dRV;
            }

    };
}

#endif
