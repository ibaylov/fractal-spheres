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
#ifndef _GEOM_H_
#define _GEOM_H_
#include <cstdlib>
namespace geom
{

/**
@file geom.hh
@brief We decalre here the base scalar type for geometry calculations and the geometry classes for 3D calculations
Since we wil not use floats and doubles simultaneously, we dont use template but a definition instead

We use some lean c++  11 code to speeed  up thiings where possible, but this can be disabled

@note Compilation control macros:

_GEOM_FLOAT_ - define to use float scalar
_NO_CXX_11_  - define to exclude the c++ 11 + specific code
_DEBUG_DUMP_ - define to enable debug features and dumping
*/


#ifdef _GEOM_FLOAT_
typedef float  scalar;
#else
typedef double scalar;
#endif
///////////////////////////////////////////////////
/// \brief gnDim3d - the size of homogeneous tuples
///
const int gnDim3d = 4;

class cVector3d;
class cPoint3d;
class cMatrix3d;

//////////////////////////////////////////////////
/// \brief The Axis enum - enumerating axis names for convenience
///
enum Axis
{
    X = 0,
    Y = 1,
    Z = 2,
    W = 3
};

//////////////////////////////////////////////////
/// \brief The cTuple3d class
/// We encapsulate the base functionality for homogeneous coordinates calculations here
/// We derive then the cPoint3d and cVector3d to restrict specific operations, e.g nt allowing point * point

class cTuple3d
{
    protected:
        //////////////////////////////////////////////////
        /// \brief m_sVector - the inner vector of the tuple
        ///
        scalar m_sVector[ gnDim3d ];
    public:


        cTuple3d( ); //!< default constructor, zeroes the duqdruple
        cTuple3d( scalar, scalar, scalar, scalar ); //!< four scalars constructor
        cTuple3d( const  cTuple3d& );               //!< copy constructor
        #ifndef _NO_CXX_11_
        cTuple3d( cTuple3d&& );                     //!< We define this for possible future usage should we change the internals;
                                                    //   there's no direct benefit from rvalue reference argument right now
        #endif
        const cTuple3d& operator = ( const  cTuple3d& ); //!< assignment
        ~cTuple3d();  //!< The destructor;  We don't have virtual methods here so no virtual destructor

        const scalar& operator [] ( size_t nPos) const; //!< Read-only  element access
        scalar&       operator () ( size_t nPos) ;      //!< Read-write element access

    // relationships
        friend    scalar    operator * ( const  cVector3d& , const  cVector3d& ); //!< Dot product

        friend    cVector3d operator * ( const  cVector3d& , scalar ); //!< Multiplication by scalar value
        friend    cVector3d operator ^ ( const  cVector3d& , const  cVector3d& ); //!< Cross product
        friend    cVector3d operator + ( const  cVector3d& , const  cVector3d& ); //!< Vector - vector sum
        friend    cVector3d operator - ( const  cPoint3d&  , const  cPoint3d&  ); //!< Point - point subtraction

        friend    cPoint3d  operator + ( const  cPoint3d& , const  cVector3d& ); //!< Vector - point sum
        friend    cPoint3d  operator + ( const  cVector3d&, const  cPoint3d&  ); //!< Point - vector  sum


        friend    cTuple3d   operator * ( const cMatrix3d&, const cTuple3d& );    //!< Matrix - tuple  multiplication, this order only
        friend    cMatrix3d  operator * ( const  cMatrix3d&, const  cMatrix3d& );  //!< Matrix - matrix  multiplication
        friend    cMatrix3d  operator + ( const  cMatrix3d&, const  cMatrix3d& );  //!< Matrix - matrix  addition
    // operations
        scalar  Homogenize(); //!< Performs homogenization and returns the previous value of W
    protected:    
    // Inline or DSP optimization candidates
    // We declare this protected to restrict possible the misuse
        static scalar  DotProductImpl(   const cTuple3d&, const cTuple3d& ); //!< Dot product internal implementation
        static void    TupleSumImpl(     const cTuple3d&, const cTuple3d&, cTuple3d& tupleOut ); //!< element-wize sum of a tuple
        static void    CrossProductImpl( const cTuple3d&, const cTuple3d&, cTuple3d& tupleOut ); //!< vector crossproduct implementation
    public:
        static scalar gsEps; //!< constant for calculations zero limit
#ifdef _DEBUG_DUMP_
    void Dump() const;
#else
    inline void Dump() const{} //!<< We are using this for debug dunp; release version is inline doing nothing
#endif                   
};

//////////////////////////////////////////////////
/// \brief The cMatrix3d class
/// the class for linear operator with the bare minimum functionality we need

class cMatrix3d
{
    protected:
        /////////////////////////////////////////////////
        /// \brief m_vecMartixRows - the matrix values
        /// we use row-vector impementation since our clculations assume row-major form
        /// also this may be benefical if DSP/SIMD product is implemented on quples
        cTuple3d m_vecMartixRows[ gnDim3d ];
    public:
        cMatrix3d( );   //!< default constructor, loads identity
        cMatrix3d( const  cMatrix3d& ); //!< copy constructor
        #ifndef _NO_CXX_11_
        cMatrix3d( cMatrix3d&& ); // again, there's no actual benefits from this rigt now
        #endif
        cMatrix3d( scalar, scalar, scalar, scalar,
                   scalar, scalar, scalar, scalar,
                   scalar, scalar, scalar, scalar,
                   scalar, scalar, scalar, scalar ); //<! direct row-major initialization

        const cMatrix3d& operator = ( const  cMatrix3d& ); //!< assignemt operator
        ~cMatrix3d(); // again, we don't have virtual methods here so no virtual destructor

        const cTuple3d& operator [] ( size_t nPos) const; //!< Read-only row access 
              cTuple3d& operator () ( size_t nPos) ;      //!< Read-write row access 

        void LoadIdentity();
    // relationships
    // operations
        const cMatrix3d& operator *= ( const  cMatrix3d& );
#ifdef _DEBUG_DUMP_
    void Dump() const;
#else
    inline void Dump() const {} // same as in tuple
#endif                   

};

//////////////////////////////////////////////////
/// \brief The cPoint3d class
/// We defive this from the tuple to restrict functionality to points-only valid operations
class cPoint3d : public cTuple3d
{
    public:
        cPoint3d(  );                //!< default, makes a finite point by settinh W to 1.0
        cPoint3d( const cTuple3d& ); //!< copy
        #ifndef _NO_CXX_11_
        cPoint3d( cTuple3d&& );      //!< rvalue
        #endif
        cPoint3d( scalar, scalar, scalar ); //!< direct initialization, sets W to 1.0
    // operations
        const cPoint3d& operator  += ( const  cVector3d& ); //!< Point-vector addition
              cVector3d operator  -= ( const  cPoint3d& );  //!< Point subtraction produces vector

};

//////////////////////////////////////////////////
/// \brief The cVector3d class
/// We defive this from the tuple to restrict functionality to vector-specific operations
class cVector3d : public cTuple3d
{
    public:
        cVector3d(  );                  //!< , makes an infinite point by settinh W to 0.0
        cVector3d( const cTuple3d& );   //!< copy
        #ifndef _NO_CXX_11_
        cVector3d(cTuple3d&& );         //!< value
        #endif
        cVector3d( scalar, scalar, scalar ); //!< direct initialization, sets W to 0.0
    // operations
        const cVector3d& operator += ( const  cVector3d& ); //!< Per-element addition
        const cVector3d& operator ^= ( const  cVector3d& ); //!< Crossproduct
        scalar Normalize(); //!< Normalizes the vector and returns the Euclidean norm
        const cVector3d& operator *= ( scalar ); //!< Multiplication by scalar value
};

//////////////////////////////////////////////////
/// \brief The cPlane3d class
/// We defive the plane as utility class for 3d operations
/// We use point and normal representation, maybe not the most efficient, but convenient
class cPlane3d
{
    protected:
        cVector3d m_vecNormal; //!< Normal vector to the plane
        cPoint3d  m_ptOrigin;  //!< Point on the plane
    public:
        cPlane3d();            //!< default
        const cPlane3d&  operator = ( const cPlane3d& );                 //!< copy
        cPlane3d( const cVector3d& , const cPoint3d& );                  //!< Vector and point form
        cPlane3d( const cPoint3d& , const cPoint3d&, const cPoint3d&  ); //!< Three point definition
        cPlane3d( scalar, scalar, scalar, scalar );                      //!< Normal equation definition 

        scalar   PointDistance( const cPoint3d& ); //!< Euclidean norm. Returns positive if point is on the sode normal vector points to, ~= 0 on plane, negative o.w.
        cPoint3d ProjectPoint( const cPoint3d& ); //!< Projects the argument onto plane

        ~cPlane3d(); //!< destructor, not virtual
#ifdef _DEBUG_DUMP_
    void Dump() const;
#else
    inline void Dump() const {}
#endif                   
};

// object returning operations

scalar    operator * ( const  cVector3d& , const  cVector3d& ); //!< Dot product

cVector3d operator * ( const  cVector3d& , scalar ); //!< Multiplication by scalar value
cVector3d operator ^ ( const  cVector3d& , const  cVector3d& ); //!< Cross product
cVector3d operator + ( const  cVector3d& , const  cVector3d& ); //!< Vector - vector sum
cVector3d operator - ( const  cPoint3d&  , const  cPoint3d&  ); //!< Point - point subtraction

cPoint3d  operator + ( const  cPoint3d& , const  cVector3d& ); //!< Vector - point sum
cPoint3d  operator + ( const  cVector3d&, const  cPoint3d&  ); //!< Point - vector  sum

cTuple3d   operator * ( const cMatrix3d&, const cTuple3d& );    //!< Matrix - tuple  multiplication, this order only
cMatrix3d  operator * ( const  cMatrix3d&, const  cMatrix3d& );  //!< Matrix - matrix  multiplication

cMatrix3d  operator + ( const  cMatrix3d&, const  cMatrix3d& );  //!< Matrix - matrix  addition


} // namespace end

#endif
