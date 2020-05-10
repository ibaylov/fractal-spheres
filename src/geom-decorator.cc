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
#include "geom-decorator.hh"
#include <cmath>

namespace geom
{
namespace decorator
{

scalar ScalarToRadians( scalar sIn,  AngleUnit unitIn )
{
    if( unitIn == Degrees )
        return sIn * M_PI / static_cast<scalar>(180.0);
    return sIn;
}

scalar RadiansToScalar( scalar sRads,  AngleUnit unitIn )
{
    if( unitIn == Degrees )
        return static_cast<scalar>(180.0) * sRads / M_PI;
    return sRads;
}

inline void LoadRotationX (  cMatrix3d& matIn , scalar sCos , scalar sSin )
{
    matIn(1)(1) =   sCos;
    matIn(1)(2) = - sSin;

    matIn(2)(1) =  sSin;
    matIn(2)(2) =  sCos;
} 

inline void LoadRotationY (  cMatrix3d& matIn , scalar sCos , scalar sSin )
{
    matIn(0)(0) =   sCos;
    matIn(0)(2) =   sSin;

    matIn(2)(0) =  -sSin;
    matIn(2)(2) =   sCos;
} 

inline void LoadRotationZ (  cMatrix3d& matIn , scalar sCos , scalar sSin )
{
    matIn(0)(0) =   sCos;
    matIn(0)(1) = - sSin;

    matIn(1)(0) =   sSin;
    matIn(1)(1) =   sCos;
} 


void LoadRotation( cMatrix3d& matIn , Axis axisRotate, scalar sAngle , AngleUnit unitAngle )
{
    sAngle = ScalarToRadians( sAngle, unitAngle );
    scalar sCos = cos( sAngle );
    scalar sSin = sin( sAngle );
    matIn.LoadIdentity();
    switch( axisRotate )
    {
        case X:
            LoadRotationX( matIn, sCos, sSin );
        break;
        case Y:
            LoadRotationY( matIn, sCos, sSin );
        break;
        case Z:
            LoadRotationZ( matIn, sCos, sSin );
        break;
        //!!! error handling
    }
}

//!!! I was too lzy to calculate this myself.
//!!! Stolen from here: https://community.khronos.org/t/implementing-rotation-function-like-glrotate/68603

void LoadRotation( cMatrix3d& matIn , const cVector3d& vecAxis, scalar sAngle , AngleUnit unitAngle )
{
    sAngle = ScalarToRadians( sAngle, unitAngle );
    scalar sCos = cos( sAngle );
    scalar sSin = sin( sAngle );
    matIn.LoadIdentity();
    scalar x = vecAxis[X];
    scalar y = vecAxis[Y];
    scalar z = vecAxis[Z];
    scalar  xx = x * x;
    scalar  yy = y * y;
    scalar  zz = z * z;
    scalar  xy = x * y;
    scalar  yz = y * z;
    scalar  zx = z * x;
    scalar  xs = x * sSin;
    scalar  ys = y * sSin;
    scalar  zs = z * sSin;
    scalar  oneMinusCos = static_cast<scalar>(1.0) - sCos;

      matIn(0)(0) = (oneMinusCos * xx) + sCos;
      matIn(0)(1) = (oneMinusCos * xy) - zs;
      matIn(0)(2) = (oneMinusCos * zx) + ys;

      matIn(1)(0) = (oneMinusCos * xy) + zs;
      matIn(1)(1) = (oneMinusCos * yy) + sCos;
      matIn(1)(2) = (oneMinusCos * yz) - xs;

      matIn(2)(0) = (oneMinusCos * zx) - ys;
      matIn(2)(1) = (oneMinusCos * yz) + xs;
      matIn(2)(2) = (oneMinusCos * zz) + sCos;

}

void LoadTranslation( cMatrix3d&  matIn, const cVector3d& vecTrans)
{
    matIn.LoadIdentity();
    matIn(0)(3) = vecTrans[X];
    matIn(1)(3) = vecTrans[Y];
    matIn(2)(3) = vecTrans[Z];
}

void LoadScale( cMatrix3d&  matIn, const cVector3d& vecScale )
{
    matIn.LoadIdentity();
    matIn(0)(0) = vecScale[X];
    matIn(1)(1) = vecScale[Y];
    matIn(2)(2) = vecScale[Z];
}

void ExportOGLMatrix( scalar* psOGL, const cMatrix3d& matOut )
{
    //!!! checks on input data
    int cRow, cCol;
    for( cRow = 0; cRow < gnDim3d; cRow ++ )
        for( cCol = 0; cCol < gnDim3d; cCol ++ )
            *psOGL ++ = matOut[cCol][cRow];     
}

void ImportOGLMatrix(  cMatrix3d& matIn, const scalar* psOGL )
{
    //!!! checks on input data
    int cRow, cCol;
    for( cRow = 0; cRow < gnDim3d; cRow ++ )
        for( cCol = 0; cCol < gnDim3d; cCol ++ )
            matIn(cCol)(cRow) = *psOGL ++ ;

}

cTuple3d ElementSumMul( const cTuple3d& tupleA, scalar sCoefA, const cTuple3d& tupleB, scalar sCoefB )
{
    cTuple3d tplOut;
    int cElem;
    for( cElem = 0; cElem < gnDim3d; cElem ++ )
        tplOut( cElem ) = tupleA[ cElem ] * sCoefA + tupleB[ cElem ] * sCoefB;
    return tplOut;    
}


} // NS end
} // NS end
