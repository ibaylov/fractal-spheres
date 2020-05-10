#ifdef _DEBUG_DUMP_
#include <iostream>
#endif
#include <cmath>
#include "geom.hh"
#include "assert.hh"
namespace geom
{

scalar cTuple3d::gsEps = static_cast<scalar>(1e-7);

cTuple3d::cTuple3d( )
{
    int cElem;
    for( cElem = 0; cElem < gnDim3d; cElem ++ )
        m_sVector[ cElem ] = static_cast<scalar>(0.0);
}

cTuple3d::cTuple3d( scalar sX, scalar sY, scalar sZ , scalar sW )
{
    m_sVector[ X ] = sX;
    m_sVector[ Y ] = sY;
    m_sVector[ Z ] = sZ;
    m_sVector[ W ] = sW;
}


cTuple3d::cTuple3d( const  cTuple3d& rOther)
{
    this->operator = ( rOther );
}


#ifndef _NO_CXX_11_
cTuple3d::cTuple3d( cTuple3d&& rTemp )
{
    int cElem;
    for( cElem = 0; cElem < gnDim3d; cElem ++ )
        m_sVector[ cElem ] = rTemp.m_sVector[ cElem ];
}

#endif

const cTuple3d& 
cTuple3d::operator = ( const  cTuple3d& rOther)
{
    int cElem;
    for( cElem = 0; cElem < gnDim3d; cElem ++ )
        m_sVector[ cElem ] = rOther.m_sVector[ cElem ];
    return *this;
}

cTuple3d::~cTuple3d()
{
    // nothing special about it
}

const scalar& 
cTuple3d::operator [] ( size_t nPos) const
{
    // size checks here
    return m_sVector[ nPos ];
}

scalar&       
cTuple3d::operator () ( size_t nPos) 
{
    return m_sVector[ nPos ];
}


// relationships
// operations
scalar  
cTuple3d::Homogenize()
{
    int cElem;
    scalar sW = m_sVector[ gnDim3d - 1 ]; // the W component is the last one
    scalar sFactor = static_cast<scalar>( 1.0 ) / sW; //!!! checks here
    for( cElem = 0; cElem < gnDim3d; cElem ++ )
        m_sVector[ cElem ] *= sFactor;
    return sW;
}

scalar  
cTuple3d::DotProductImpl(   const cTuple3d& tplA, const cTuple3d&  tplB )
{
    int cElem;
    scalar sProduct = static_cast<scalar>( 0.0 ); 
    for( cElem = 0; cElem < gnDim3d; cElem ++ )
        sProduct += tplA[ cElem ] * tplB[ cElem ]; // function call avoidable :)
    return sProduct;

}
           

void    
cTuple3d::TupleSumImpl(   const cTuple3d& tplA, const cTuple3d& tplB, cTuple3d& rtplOut )
{
    int cElem;
    for( cElem = 0; cElem < gnDim3d; cElem ++ )
        rtplOut ( cElem ) = tplA[ cElem ] + tplB[ cElem ]; // function call avoidable :)
}
           

void    
cTuple3d::CrossProductImpl( const cTuple3d& tplA, const cTuple3d& tplB, cTuple3d& rtplOut )
{
  //!!! check the tuples are homogenized and infinite points
  _ASSERT( fabs(tplA[W])<gsEps);
  _ASSERT( fabs(tplB[W])<gsEps);

  rtplOut( X ) =  tplA[ Y ] * tplB[ Z ] - tplA[ Z ] * tplB[ Y ];// y1*z2 - z1*y2; 
  rtplOut( Y ) =  tplA[ Z ] * tplB[ X ] - tplA[ X ] * tplB[ Z ]; //z1*x2 - x1*z2; 
  rtplOut( Z ) =  tplA[ X ] * tplB[ Y ] - tplA[ Y ] * tplB[ X ]; // x1*y2 - y1*x2;
  rtplOut( W ) =  static_cast<scalar>(0.0);
}
           

cMatrix3d::cMatrix3d( )
{
    int cRow;
    // load identity matrix here
    for( cRow = 0; cRow < gnDim3d; cRow ++ )
        m_vecMartixRows[ cRow ]( cRow ) = static_cast<scalar>(1.0);
}

cMatrix3d::cMatrix3d( const  cMatrix3d& matOther )
{
    this->operator = (matOther);
}

cMatrix3d::cMatrix3d( scalar v00, scalar v01, scalar v02, scalar v03,  
                      scalar v10, scalar v11, scalar v12, scalar v13, 
                      scalar v20, scalar v21, scalar v22, scalar v23, 
                      scalar v30, scalar v31, scalar v32, scalar v33 )
{
    m_vecMartixRows[ 0 ] = cTuple3d(v00, v01, v02, v03);
    m_vecMartixRows[ 1 ] = cTuple3d(v10, v11, v12, v13);
    m_vecMartixRows[ 2 ] = cTuple3d(v20, v21, v22, v23);
    m_vecMartixRows[ 3 ] = cTuple3d(v30, v31, v32, v33);
}


#ifndef _NO_CXX_11_
cMatrix3d::cMatrix3d( cMatrix3d&& matTemp )
{
    this->operator = (matTemp);
}

#endif
const cMatrix3d& 
cMatrix3d::operator = ( const  cMatrix3d& matOther )
{
    int cRow;
    for( cRow = 0; cRow < gnDim3d; cRow ++ )
        m_vecMartixRows[ cRow ] = matOther[ cRow ];

    return * this;
}
 
cMatrix3d::~cMatrix3d()
{

}


const cTuple3d& 
cMatrix3d::operator [] ( size_t nPos) const
{
    //!!! index checks here
    return m_vecMartixRows[ nPos ];
}

        
cTuple3d& 
cMatrix3d::operator () ( size_t nPos) 
{
    //!!! index checks here
    return m_vecMartixRows[ nPos ];
}

const cMatrix3d& 
cMatrix3d::operator *= ( const  cMatrix3d& matOther )
{
    *this = *this * matOther;
    return * this;
}

void 
cMatrix3d ::LoadIdentity()
{
    int cRow, cCol;
    for( cRow = 0; cRow < gnDim3d; cRow ++ )
        for( cCol = 0; cCol < gnDim3d; cCol ++ )
            (*this)( cRow )( cCol ) = cRow == cCol ? static_cast<scalar>(1.0) : static_cast<scalar>(0.0) ; 
}


/////////////////////////////////////////////////////////////////////////////////////////
// cPoint3d implementation

cPoint3d::cPoint3d()
{
    m_sVector[ W ] = static_cast<scalar>(1.0); 
}

cPoint3d::cPoint3d( scalar sX, scalar sY, scalar sZ  )
: cTuple3d( sX, sY, sZ, static_cast<scalar>(1.0) ) // points are finite
{

}

cPoint3d::cPoint3d( const cTuple3d& rOther)
{
    int cElem;
    for( cElem = 0; cElem < gnDim3d; cElem ++ )
        m_sVector[ cElem ] = rOther[ cElem ];
}
#ifndef _NO_CXX_11_
cPoint3d::cPoint3d( cTuple3d&& rOther )
{
    int cElem;
    for( cElem = 0; cElem < gnDim3d; cElem ++ )
        m_sVector[ cElem ] = rOther[ cElem ];
}
#endif


const cPoint3d& 
cPoint3d::operator  += ( const  cVector3d& vecTrans )
{
    TupleSumImpl( *this, vecTrans, *this );
    return *this;
}


cVector3d 
cPoint3d::operator  -= ( const  cPoint3d& ptOther )
{
    cVector3d vecOut;
    int cElem;
    for( cElem = 0; cElem < gnDim3d; cElem ++ )
        vecOut( cElem ) = m_sVector[ cElem ] - ptOther[ cElem ];
    
    return vecOut;
}

/////////////////////////////////////////////////////////////////////////////////////////
// cVector3d implementation

cVector3d::cVector3d()
{
    m_sVector[ W ] = static_cast<scalar>(0.0); 
}

cVector3d::cVector3d( scalar sX, scalar sY, scalar sZ  )
: cTuple3d( sX, sY, sZ, static_cast<scalar>(0.0) ) // vectors are infinite
{

}

cVector3d::cVector3d( const cTuple3d& rOther)
{
    int cElem;
    for( cElem = 0; cElem < gnDim3d; cElem ++ )
        m_sVector[ cElem ] = rOther[ cElem ];
}
#ifndef _NO_CXX_11_
cVector3d::cVector3d( cTuple3d&& rOther )
{
    int cElem;
    for( cElem = 0; cElem < gnDim3d; cElem ++ )
        m_sVector[ cElem ] = rOther[ cElem ];
}
#endif


const cVector3d& 
cVector3d::operator += ( const  cVector3d&  vecOther)
{
    TupleSumImpl( *this, vecOther, *this );
    return *this;
}


const cVector3d& 
cVector3d::operator ^= ( const  cVector3d& vecOther )
{
    cVector3d vecTemp;
    CrossProductImpl( *this, vecOther, vecTemp);
    *this = vecTemp;
    return *this;
}

/////////////////////////////////////////////////////
/// \brief cVector3d::Normalize
/// \return 1 / sqrt( |x| )
/// the infamous inverse square root at work. Use Newton method / Carmack for speedup, currently not ast the top of profile
///
scalar 
cVector3d::Normalize()
{
    scalar sNorm = DotProductImpl( *this, *this );
    sNorm = sqrt( sNorm );
    _ASSERT( sNorm > gsEps );
    //!!! check the norm is non-zero
    scalar sFactor = static_cast<scalar>( 1.0 ) / sNorm;
    int cElem;
    for( cElem = 0; cElem < gnDim3d; cElem ++ )
        m_sVector[ cElem ] *= sFactor;
    return sNorm;    
}

const cVector3d& 
cVector3d::operator *= ( scalar sX )
{
    int cElem;
    for( cElem = 0; cElem < gnDim3d; cElem ++ )
        m_sVector[ cElem ] *= sX;
    return *this;    
}

/////////////////////////////////////////////////////////////////////////////////////////
// cPlane3d implementation

cPlane3d::cPlane3d()
{

}

cPlane3d::cPlane3d( const cVector3d& vecNormal , const cPoint3d& ptOrg )
   : m_vecNormal(vecNormal), m_ptOrigin( ptOrg)
{

}

cPlane3d::cPlane3d( const cPoint3d& ptA, const cPoint3d& ptB, const cPoint3d& ptC  )
{
    //!!! colinear points case is handled by normalize
    cVector3d vecAB =  ptA - ptB;
    cVector3d vecCB =  ptC - ptB;

    m_vecNormal = vecAB ^ vecCB;
    m_vecNormal.Normalize();
    m_ptOrigin  = ptB;
}

cPlane3d::cPlane3d( scalar sA, scalar sB, scalar sC, scalar sD )
{
    m_vecNormal = cVector3d( sA, sB, sC );
    scalar sN =  m_vecNormal.Normalize();
    // project CS origin on plane
    m_ptOrigin = cPoint3d( 0, 0, 0) + m_vecNormal * - (sD / sN);
} 

const cPlane3d&  
cPlane3d::operator = ( const cPlane3d& rplOther  )
{
    m_vecNormal = rplOther .m_vecNormal; 
    m_ptOrigin = rplOther .m_ptOrigin;  
    return *this;
}

cPlane3d::~cPlane3d()
{

}

scalar 
cPlane3d::PointDistance( const cPoint3d& ptIn)
{
    return m_vecNormal * ( ptIn - m_ptOrigin );
}

cPoint3d 
cPlane3d::ProjectPoint( const cPoint3d& ptIn )
{
    return ptIn + m_vecNormal *  - PointDistance( ptIn );
}

#ifdef _DEBUG_DUMP_
void 
cPlane3d::Dump() const
{
    m_ptOrigin.Dump();
    m_vecNormal.Dump();
}
#endif                   


/////////////////////////////////////////////////////////////////////////////

scalar    operator * ( const  cVector3d& vecA, const  cVector3d& vecB )
{
    return cTuple3d::DotProductImpl( vecA, vecB );
}

cVector3d operator * ( const  cVector3d&  vecA, scalar sX  )
{
    cVector3d vecOut = vecA;
    vecOut *= sX;
    return vecOut;
}

cVector3d operator ^ ( const  cVector3d& vecA, const  cVector3d&  vecB )
{
    cVector3d vecOut;
    cTuple3d::CrossProductImpl( vecA, vecB, vecOut);
    return vecOut;
}

cVector3d operator + (  const  cVector3d& vecA, const  cVector3d&  vecB )
{
    cVector3d vecOut;
    cTuple3d::TupleSumImpl( vecA, vecB, vecOut);
    return vecOut;
}

cVector3d operator - ( const  cPoint3d&  ptA, const  cPoint3d& ptB )
{
    cVector3d vecOut;
    int cElem;
    for( cElem = 0; cElem < gnDim3d; cElem ++ )
        vecOut( cElem ) = ptA[ cElem ] - ptB[ cElem ];
    
    return vecOut;
}


cPoint3d  operator + ( const  cPoint3d&  ptOrg, const  cVector3d& vecTran)
{
    cPoint3d ptOut;
    cTuple3d::TupleSumImpl( ptOrg, vecTran, ptOut);
    return ptOut;
}

cPoint3d  operator + ( const  cVector3d& vecTran, const  cPoint3d& ptOrg  )
{
    cPoint3d ptOut;
    cTuple3d::TupleSumImpl( ptOrg, vecTran, ptOut);
    return ptOut;
}


cTuple3d   operator * ( const cMatrix3d& matOper, const cTuple3d& tplTrans)
{
    cTuple3d tplOut;
#if 0
    int cRow;
    for( cRow = 0; cRow < gnDim3d; cRow ++ )
        tplOut( cRow ) = cTuple3d::DotProductImpl( matOper[ cRow], tplTrans );
#endif
    // optimization after profile
    int cRow, cProd;
    tplOut(W) = static_cast<scalar>(0.0);
    for ( cProd = 0; cProd < gnDim3d; cProd++ )
       for ( cRow = 0; cRow < gnDim3d; cRow++ )
           tplOut(cRow) += matOper[ cRow ][ cProd ] * tplTrans[ cProd ];
    return tplOut;
}


cMatrix3d  operator * ( const  cMatrix3d& matA, const  cMatrix3d& matB)
{
    cMatrix3d matOut;
#if 0
    int cRow, cCol;
    // prepare matrix B columns
    cTuple3d arrColsB[gnDim3d];
    for( cRow = 0; cRow < gnDim3d; cRow ++ )
        for( cCol = 0; cCol < gnDim3d; cCol ++ )
            arrColsB[  cCol ]( cRow ) = matB[cRow][cCol]; 

    // do the product
    for( cRow = 0; cRow < gnDim3d; cRow ++ )
        for( cCol = 0; cCol < gnDim3d; cCol ++ )
            matOut(cRow)(cCol) = cTuple3d::DotProductImpl(matA[cRow], arrColsB[cCol]) ;
#endif
    // speed optimization after profiling
    // idea taken from here:https://yazeng.wordpress.com/2014/09/09/speed-up-the-matrix-multiplication-in-c/
    int cRow, cCol, cProd;
    for ( cProd = 0; cProd < gnDim3d; cProd++ )
        matOut(cProd)(cProd) = static_cast<scalar>(0.0); // remove identity
    for ( cProd = 0; cProd < gnDim3d; cProd++ )
       for ( cRow = 0; cRow < gnDim3d; cRow++ )
          for ( cCol = 0; cCol < gnDim3d; cCol ++ )
             matOut(cRow)(cCol) += matA[cRow][cProd]*matB[cProd][cCol];
    return matOut;
}


cMatrix3d  operator + ( const  cMatrix3d& matA, const  cMatrix3d& matB)
{
    cMatrix3d matOut;
    int cRow;
    for( cRow = 0; cRow < gnDim3d; cRow ++ )
        cTuple3d::TupleSumImpl( matA[ cRow ], matB[ cRow ], matOut( cRow ));
    return matOut;
}


#ifdef _DEBUG_DUMP_
void 
cTuple3d::Dump()  const
{
    std::cerr << __FUNCTION__ << ": ";
    std::cerr << (void*)this << " [";

    int cElem;
    for( cElem = 0; cElem < gnDim3d; cElem ++ )
        std::cerr << (*this)[ cElem] << ((cElem < gnDim3d - 1 ) ? " , " : "");
    std::cerr << "]" << std::endl;    
}

void 
cMatrix3d::Dump()  const
{
    std::cerr << __FUNCTION__ << ": " << std::endl;
    std::cerr << (void*)this << " ["  << std::endl;
    int cRow, cCol;
    for( cRow = 0; cRow < gnDim3d; cRow ++ )
    {
        std::cerr <<  " [";
        for( cCol = 0; cCol < gnDim3d; cCol ++ )
            std::cerr << (*this)[ cRow] [ cCol ]<< ((cCol < gnDim3d - 1 ) ? " , " : "");
        std::cerr << " ] " << std::endl;
    }
    std::cerr << "]" << std::endl;    
}

#endif


} // NS end
