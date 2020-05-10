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
