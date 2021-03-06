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
#ifndef _GEOM_DECORATOR_H_
#define _GEOM_DECORATOR_H_

#include "geom.hh"

/**
@file geom-decorator.hh
@brief Decorators add the specific operations to geometry objects without adding methods to them
*/

namespace geom
{
    namespace decorator
    {
        /////////////////////////////////////////////////
        /// \brief The AngleUnit enum
        /// used as a companion argument for angle values
        enum AngleUnit
        {
            Radians,
            Degrees
        };

        /////////////////////////////////////////////////
        /// \brief ScalarToRadians converts an arbitrary angle value to radians
        /// \param sIn      - the argument value
        /// \param unitIn   - the argument ubits
        /// \return         - the angle in radians
        ///
        scalar ScalarToRadians( scalar sIn,    AngleUnit unitIn );

        /////////////////////////////////////////////////
        /// \brief RadiansToScalar - converts angle value in radians to arbitrary unit
        /// \param sRads    - the angle value in radians
        /// \param unitOut   - the units desired
        /// \return         - the angle value in unitOut
        ///
        scalar RadiansToScalar( scalar sRads,  AngleUnit unitOut );

        /////////////////////////////////////////////////
        /// \brief LoadRotation - loads the argument matrix with an orthogonal matrix of rotation against WCS axis
        /// \param matArg       - the matrix to be loaded
        /// \param axisRotate   - the WCS axis of rotation
        /// \param sAngle       - the angle value
        /// \param unitAngle    - the angle units
        ///
        void LoadRotation( cMatrix3d& matArg , Axis axisRotate, scalar sAngle, AngleUnit unitAngle );

        /////////////////////////////////////////////////
        /// \brief LoadRotation - loads the argument matrix with an orthogonal matrix of rotation against arbitrary axis
        /// \param matArg       - the matrix to be loaded
        /// \param axisRotate   - the axis vector
        /// \param sAngle       - the angle value
        /// \param unitAngle    - the angle units
        ///
        void LoadRotation( cMatrix3d& matArg, const cVector3d& vecAxis, scalar sAngle , AngleUnit angleUnit ); //!< Arbitrary axis rotation

        /////////////////////////////////////////////////
        /// \brief LoadTranslation - loads a translation operator into the argument matrix
        /// \param matArg          - the matrix to be loaded
        /// \param vacTranslate    - the translation vector
        ///
        void LoadTranslation( cMatrix3d& matArg , const cVector3d& vacTranslate );

        /////////////////////////////////////////////////
        /// \brief LoadScale       - loads a dilatation operator into the argument matrix
        /// \param matArg          - the matrix to be loaded
        /// \param vacScale        - the dilatation values
        ///
        void LoadScale( cMatrix3d& matArg, const cVector3d& vecScale );

        /////////////////////////////////////////////////
        /// \brief ExportOGLMatrix - Exports a matrix to OpenGL array
        /// \param psOGL           - the OGL matrix
        /// \param matOut          - the matrix to be exported
        ///
        void ExportOGLMatrix( scalar* psOGL, const cMatrix3d& matOut ); //!< Export to  OpenGL matrix

        /////////////////////////////////////////////////
        /// \brief ImportOGLMatrix - Imports a matrix from OpenGL array
        /// \param matIn           - the matrix to be imported
        /// \param psOGL           - the OpenGL matrix
        ///
        void ImportOGLMatrix(  cMatrix3d& matIn , const scalar* psOGL ); //!< Import from  OpenGL matrix

        /////////////////////////////////////////////////
        /// \brief ElementSumMul   - calculates per-element sum of two tuples, weighted by scalar coefficients
        /// \param tupleA          - the first tuple
        /// \param sCoefA          - the first tuple coefficient
        /// \param tupleB          - the second tuple
        /// \param sCoefB          - the second tuple coefficient
        /// \return                - the weighted sum tuple
        ///
        cTuple3d ElementSumMul( const cTuple3d& tupleA, scalar sCoefA, const cTuple3d& tupleB, scalar sCoefB );

    }

}

#endif
