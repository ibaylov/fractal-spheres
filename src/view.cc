#include "view.hh"

namespace mvc
{

/////////////////////////////////////////////////////////////////////
// cView implementation

cView::cView()
    : m_pModel( nullptr), m_pStencil( nullptr )
{
}

cView::~cView()
{
}

void
cView::AssociateModel( cModel* pModel )
{
    //!!! checks here
    m_pModel = pModel;
}

void
cView::Display()
{
    DisplayImpl();
    m_pModel->Collect();
}

void
cView::SetStencil( cView::cElementStencil* pStencil )
{
    m_pStencil = pStencil;
}

cView::cElementStencil::cElementStencil()
{

}

cView::cElementStencil::~cElementStencil()
{

}

} //NS end
