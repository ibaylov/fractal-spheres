#ifndef _OGL_VIEWPORT_H_
#define _OGL_VIEWPORT_H_
#include "geom.hh"
#include "geom-decorator.hh"

/**
@file viewport.hh
@brief The viewport sets up the OpenGL Projection and View matrices and exports visibility functions
*/


namespace ogl
{

const size_t gnClipPlanes = 6;
class cViewport
{
    public:

        enum ClipPlane
        {
            Left    = 0,
            Right   = 1,
            Top     = 2,
            Bottom  = 3,
            Far     = 4,
            Near    = 5                          
        };

    protected:
        geom::cPoint3d      m_ptEye;        //!< Camere origin
        geom::cVector3d     m_vecView;      //!< Camera direction
        geom::cVector3d     m_vecUp;        //!< The vecxtor to help set up the camera local OCS
        geom::scalar        m_sFOV;         //!< the Field of view in radians
        int                 m_nW;           //!< Rendering surface width in pixels
        int                 m_nH;           //!< Rendering surface height in pixels

        geom::cPlane3d      m_arrPlanesClip[ gnClipPlanes ]; //!< The clip planes array

    public:


        cViewport(); //!< Default
        cViewport( const geom::cPoint3d& ptEye, const geom::cVector3d& vecView, const geom::cVector3d& vecUp, 
                   geom::scalar sFOV, geom::decorator::AngleUnit,  int nPortWidth, int nPortHeight ); //!< Set up the viewport, camera coordinate systems and perspective
        ~cViewport(); //!< Non-virtual destructor, we don't plan to derive from viewport

    // accessors
        const geom::cPoint3d GetEyePoint(); //!< retrieves the virtual camera's eye point
        geom::scalar GetFOV() const;        //!<  retrieves the virtual camera's field of view in radians
    // scene operations
        void Reset ( const geom::cPoint3d& ptEye, const geom::cVector3d& vecView, const geom::cVector3d& vecUp,
                     geom::scalar sFOV, geom::decorator::AngleUnit );       //!< reinitializes the virtual camera
        void OrbitHorz( geom::scalar sAngle, geom::decorator::AngleUnit );  //!< orbits against the center of WCS around the camera Up vector
        void OrbitVert( geom::scalar sAngle, geom::decorator::AngleUnit );  //!< orbits against the center of WCS around the camera Right vector
        void Pitch( geom::scalar sAngle, geom::decorator::AngleUnit );      //!< Rotates the camera LCS around camera's Right vector
        void Yaw( geom::scalar sAngle, geom::decorator::AngleUnit );        //!< Rotates the camera LCS around camera's Up vector
        void MoveInViewDir( geom::scalar sMove );                           //!< Moves the camera's LCS into camera View direction
        void AddFOV( geom::scalar sAngle, geom::decorator::AngleUnit );     //!< Increments the camrea's FOV
        void SetExtents( int nPortWidth, int nPortHeight );                 //!< Changes viewport ectents and recomputes the aspect
    // visibility operations
        geom::scalar SegmentVisibleAngle( const geom::cPoint3d& ptOrg, geom::scalar sLen ); //!< Claculates the viewing angle of a segment of line sLen prependicular to view dirtvion to ptOrg
        geom::scalar SegmentVisibleCosine( const geom::cPoint3d& ptOrg, geom::scalar sLen ); //!< Claculates the cosine value of viewing angle of a segment of line sLen prependicular to view dirtvion to ptOrg
        bool PointInFrustum( const geom::cPoint3d& );                                        //!< Chexks if a point is inside the frustim planes
        bool SphereInFrustum( const geom::cPoint3d&, geom::scalar sR );                      //!<  Chexks if a sphere is inside the frustim planes
        geom::scalar PointMinimalFrustumDistance( const geom::cPoint3d& );                   //!<  Calculates the minimum distance of a point to all frustum planes
    protected:
        void SetupOGLViev( ); //!< recreates the internal objects and sets up the OGL matrices
        void TransformBasis( const geom::cMatrix3d& ); //!< Transforms the LCS bu the argument matrix
};

} 

#endif
