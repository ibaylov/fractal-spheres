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
#include "model.hh"

namespace mvc
{


///////////////////////////////////////////////////////////////
// cElement implementation

cElement::cElement()
    : m_nHierarchyDepth( 0 )
{

}

cElement::cElement( size_t nLevel, const geom::cMatrix3d& matCS )
    : m_matLCS( matCS ), m_nHierarchyDepth( nLevel )
{

}

cElement::~cElement()
{

}

const geom::cMatrix3d&
cElement::GetLocalCS() const
{
    return m_matLCS;
}

size_t
cElement::GetHierarchyDepth() const
{
    return m_nHierarchyDepth;
}


///////////////////////////////////////////////////////////////
// cModel implementation

cModel::cModel()
{

}

cModel::~cModel()
{

}

} // NS end
