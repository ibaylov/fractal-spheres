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
#include "fractal-model.hh"
#include "geom-decorator.hh"
#include <cstring>
#include <iostream>
#include "assert.hh"

namespace mvc
{

///////////////////////////////////////////////////////////////////////////////
// cSphere implementation

cSphere::cSphere()
{
    m_sRadius = static_cast<geom::scalar>( 1.0 );
    m_plistSDescendands = nullptr;
}

cSphere::cSphere( size_t nLevel, const geom::cMatrix3d& matCS, geom::scalar sR )
    : cElement(nLevel, matCS), m_sRadius( sR ), m_plistSDescendands( nullptr)
{
}


cSphere::~cSphere()
{
    if( m_plistSDescendands )
    {
        while( m_plistSDescendands->HasData())
            delete m_plistSDescendands->PullHead();
        delete m_plistSDescendands;
    }
}

geom::scalar
cSphere::GetBoundingSphereRadius()  const
{
    // the bounding radius of a sphere is, well, the radius of the sphere itself :)
    return m_sRadius;
}

geom::scalar
cSphere::GetDescendantSphereRadius()  const
{
    //
    // the maximal radius of all possible descendants for our model is  R * sum( 1 / 3 ^ n ), n = 0..inf
    // which is R * 3 / 2
    // maybe this houls be calculated in model?

    return m_sRadius * static_cast<geom::scalar>( 3.0 / 2.0 );
}


utl::cObList<cElement*>*
cSphere::GetDescendandsListPtr()
{
    return m_plistSDescendands;
}

utl::cObList<cElement*>*
cSphere::MakeDescendandsListPtr()
{
    _ASSERT( m_plistSDescendands == nullptr ); // make sure ti is called only for a n empty cache
    m_plistSDescendands = new utl::cObList<cElement*>();
    return m_plistSDescendands;
}

///////////////////////////////////////////////////////////////////////////////
// cFractalcModel implementation


cFractalcModel::cFractalcModel()
    : m_nElementsProduced( 0 ), m_pRootElem( nullptr )
{

}

cFractalcModel::~cFractalcModel()
{
    CollectImpl();
    if( m_pRootElem )
        delete m_pRootElem;
}


// operations
cElement*
cFractalcModel::GetRootElement()
{
    if( ! m_pRootElem )
    {
        // we construct a sphere in the origin of model CS, with a radius of 3 units
        cSphere* pElemSphereRoot = new cSphere( 0, geom::cMatrix3d() , static_cast<geom::scalar>( 3.0));
        m_pRootElem = pElemSphereRoot;
    }
    return m_pRootElem;
}

#ifndef _FV_CACHE_SIZE_
const int nCacheMax = 80000;
#else
const int nCacheMax = (_FV_CACHE_SIZE_);
#endif

utl::cObList<cElement*>
cFractalcModel::GetDescendantElements( cElement* pElem )
{
    cSphere* pElemSphere = dynamic_cast<cSphere*>( pElem );

    if( pElemSphere->GetDescendandsListPtr() ) // we have pre-calculated descendands, so return them
    {
        utl::cObList<cElement*> lstRV;
        lstRV.CopyFrom( *pElemSphere->GetDescendandsListPtr()); // by-elemt copy invoked
        return lstRV;
    }

    // by default we register the new objects in GC map
    utl::cObList<cElement*>* plstRegister = &m_lstElemetsProduced;
    if( m_nElementsProduced < nCacheMax ) // if there's a room for chacing, we retarget the allocation destination here
    {
        m_nElementsProduced += 9;
        plstRegister = pElemSphere->MakeDescendandsListPtr();
    }

    cSphere* pSphereChild = nullptr;

    geom::scalar sR = pElemSphere->GetBoundingSphereRadius();
    size_t nLevel = pElemSphere->GetHierarchyDepth();

    utl::cObList<cElement*> lsrChildren;

    int cChd;

    geom::cMatrix3d matRotateEquator, matBasisChld = pElemSphere->GetLocalCS();
    geom::decorator::LoadRotation( matRotateEquator, geom::Z,  60,  geom::decorator::Degrees );

    // generate the equator child local CS
    geom::cMatrix3d matTranslateOuter;
    geom::decorator::LoadTranslation( matTranslateOuter, geom::cVector3d(sR + sR / 3,  0 ,  0));
    geom::cMatrix3d matRotateNewBasis;
    geom::decorator::LoadRotation( matRotateNewBasis, geom::Y,  90,  geom::decorator::Degrees );

    geom::cMatrix3d matEquator = matTranslateOuter * matRotateNewBasis;

    for( cChd = 0; cChd < 6; cChd ++ )
    {
        geom::cMatrix3d  matIn = matBasisChld * matEquator;
        pSphereChild = new cSphere(  nLevel + 1, matIn, sR / 3 );
        plstRegister->Add(pSphereChild);
        lsrChildren.Add(pSphereChild);
        matBasisChld *= matRotateEquator;
    }

    matBasisChld = pElemSphere->GetLocalCS();
    geom::decorator::LoadRotation( matRotateEquator, geom::Z,  120,  geom::decorator::Degrees );

    // generate the inclined child local CS
    geom::cMatrix3d matRotateLat;
    geom::decorator::LoadRotation( matRotateLat, geom::Y,  -60,  geom::decorator::Degrees );
    geom::cMatrix3d matRotateLon;
    geom::decorator::LoadRotation( matRotateLon, geom::Z,  30,  geom::decorator::Degrees );

    geom::cMatrix3d matInclined = matRotateLon * matRotateLat * matTranslateOuter * matRotateNewBasis;

    for( cChd = 0; cChd < 3; cChd ++ )
    {

        geom::cMatrix3d  matIn = matBasisChld * matInclined;
        pSphereChild = new cSphere(  nLevel + 1, matIn, sR / 3 );
        plstRegister->Add(pSphereChild);
        lsrChildren.Add(pSphereChild);
        matBasisChld *= matRotateEquator;
    }
    return lsrChildren;
}


void
cFractalcModel::Collect()
{
    CollectImpl();
}

void
cFractalcModel::CollectImpl()
{
    while( m_lstElemetsProduced.HasData() )
        delete m_lstElemetsProduced.PullHead();
}


}// NS end
