//////////////////////////////////////////////////////////////////////
// Copyright (c) 2002-2003 David Pritchard <drpritch@alumni.uwaterloo.ca>
// 
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later
// version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#ifndef freecloth_geMeshWingedEdge_inline_h
#define freecloth_geMeshWingedEdge_inline_h

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// CLASS GeMeshWingedEdge
//

//------------------------------------------------------------------------------

inline UInt32 GeMeshWingedEdge::getNbHalfEdges() const
{
    return _halfEdges.size();
}

//------------------------------------------------------------------------------

inline GeMeshWingedEdge::HalfEdgeType GeMeshWingedEdge::getHalfEdge(
    HalfEdgeId halfEdgeId
) const {
    return HalfEdgeWrapper( *this, halfEdgeId );
}

//------------------------------------------------------------------------------

inline GeMeshWingedEdge::HalfEdgeIterator
GeMeshWingedEdge::beginHalfEdge() const
{
    return HalfEdgeIterator::begin( *this );
}

//------------------------------------------------------------------------------

inline GeMeshWingedEdge::HalfEdgeIterator
GeMeshWingedEdge::endHalfEdge() const
{
    return HalfEdgeIterator::end( *this );
}

//------------------------------------------------------------------------------

inline GeMeshWingedEdge::EdgeIterator
GeMeshWingedEdge::beginEdge() const
{
    return EdgeIterator::begin( *this );
}

//------------------------------------------------------------------------------

inline GeMeshWingedEdge::EdgeIterator
GeMeshWingedEdge::endEdge() const
{
    return EdgeIterator::end( *this );
}

//------------------------------------------------------------------------------

inline GeMeshWingedEdge::VertexHalfEdgeIterator
GeMeshWingedEdge::beginVertexHalfEdge( VertexId vertexId ) const
{
    return VertexHalfEdgeIterator::begin( *this, vertexId );
}

//------------------------------------------------------------------------------

inline GeMeshWingedEdge::VertexHalfEdgeIterator
GeMeshWingedEdge::endVertexHalfEdge( VertexId vertexId ) const
{
    return VertexHalfEdgeIterator::end( *this, vertexId );
}

//------------------------------------------------------------------------------

inline GeMeshWingedEdge::VertexEdgeIterator
GeMeshWingedEdge::beginVertexEdge( VertexId vertexId ) const
{
    return VertexEdgeIterator::begin( *this, vertexId );
}

//------------------------------------------------------------------------------

inline GeMeshWingedEdge::VertexEdgeIterator
GeMeshWingedEdge::endVertexEdge( VertexId vertexId ) const
{
    return VertexEdgeIterator::end( *this, vertexId );
}

//------------------------------------------------------------------------------

inline GeMeshWingedEdge::VertexFaceIterator
GeMeshWingedEdge::beginVertexFace( VertexId vertexId ) const
{
    return VertexFaceIterator::begin( *this, vertexId );
}

//------------------------------------------------------------------------------

inline GeMeshWingedEdge::VertexFaceIterator
GeMeshWingedEdge::endVertexFace( VertexId vertexId ) const
{
    return VertexFaceIterator::end( *this, vertexId );
}

//------------------------------------------------------------------------------

inline GeMeshWingedEdge::HalfEdgeId
GeMeshWingedEdge::getHalfEdgeFromFaceId( FaceId faceId ) const
{
    DGFX_ASSERT( faceId < _faceHalfEdgeIds.size() );
    return _faceHalfEdgeIds[ faceId ];
}

//------------------------------------------------------------------------------

inline const GeMesh& GeMeshWingedEdge::getMesh() const
{
    return *_mesh;
}


////////////////////////////////////////////////////////////////////////////////
// CLASS GeMeshWingedEdge::HalfEdge
//

//------------------------------------------------------------------------------

inline GeMeshWingedEdge::HalfEdge::HalfEdge()
{
}

//------------------------------------------------------------------------------

inline GeMeshWingedEdge::HalfEdge::HalfEdge(
    VertexId origin,
    HalfEdgeId twin,
    FaceId face,
    HalfEdgeId next
) : _origin( origin ),
    _twin( twin ),
    _face( face ),
    _next( next )
{
}

//------------------------------------------------------------------------------

inline GeMeshWingedEdge::VertexId
GeMeshWingedEdge::HalfEdge::getOriginVertexId() const
{
    return _origin;
}

//------------------------------------------------------------------------------

inline GeMeshWingedEdge::HalfEdgeId
GeMeshWingedEdge::HalfEdge::getTwinHalfEdgeId() const
{
    DGFX_ASSERT( hasTwin() );
    return _twin;
}

//------------------------------------------------------------------------------

inline GeMeshWingedEdge::FaceId GeMeshWingedEdge::HalfEdge::getFaceId() const
{
    return _face;
}

//------------------------------------------------------------------------------

inline GeMeshWingedEdge::HalfEdgeId
GeMeshWingedEdge::HalfEdge::getNextHalfEdgeId() const
{
    return _next;
}

//------------------------------------------------------------------------------

inline bool GeMeshWingedEdge::HalfEdge::hasTwin() const
{
    return _twin != ID_INVALID;
}


////////////////////////////////////////////////////////////////////////////////
// CLASS GeMeshWingedEdge::HalfEdgeWrapper
//

//------------------------------------------------------------------------------

inline GeMeshWingedEdge::HalfEdgeWrapper::HalfEdgeWrapper()
  : _meshwe( 0 ),
    _halfEdgeId( 0 )
{
}

//------------------------------------------------------------------------------

inline GeMeshWingedEdge::HalfEdgeWrapper::HalfEdgeWrapper(
    const GeMeshWingedEdge& meshwe,
    HalfEdgeId halfEdgeId
) : _meshwe( &meshwe ),
    _halfEdgeId( halfEdgeId )
{
    DGFX_ASSERT( isValid() );
}

//------------------------------------------------------------------------------

inline GeMeshWingedEdge::HalfEdgeWrapper::HalfEdgeWrapper(
    const GeMeshWingedEdge* meshwe,
    HalfEdgeId halfEdgeId
) : _meshwe( meshwe ),
    _halfEdgeId( halfEdgeId )
{
    // Pointer variant is private and unchecked - allows efficient construction
    // of invalid wrappers, i.e. for end() iterator.
}

//------------------------------------------------------------------------------

inline const GeMeshWingedEdge&
GeMeshWingedEdge::HalfEdgeWrapper::getMeshWE() const
{
    DGFX_ASSERT( _meshwe != 0 );
    return *_meshwe;
}

//------------------------------------------------------------------------------

inline const GeMesh& GeMeshWingedEdge::HalfEdgeWrapper::getMesh() const
{
    return getMeshWE().getMesh();
}

//------------------------------------------------------------------------------

inline GeMeshWingedEdge::HalfEdgeId
GeMeshWingedEdge::HalfEdgeWrapper::getHalfEdgeId() const
{
    return _halfEdgeId;
}

//------------------------------------------------------------------------------

inline bool GeMeshWingedEdge::HalfEdgeWrapper::isValid() const
{
    return _meshwe != 0 && _halfEdgeId < getMeshWE().getNbHalfEdges();
}

//------------------------------------------------------------------------------

inline GeMeshWingedEdge::VertexId
GeMeshWingedEdge::HalfEdgeWrapper::getOriginVertexId() const
{
    return getMeshWE()._halfEdges[ _halfEdgeId ].getOriginVertexId();
}

//------------------------------------------------------------------------------

inline const GeMeshWingedEdge::VertexType& 
GeMeshWingedEdge::HalfEdgeWrapper::getOriginVertex() const
{
    return getMesh().getVertex( getOriginVertexId() );
}

//------------------------------------------------------------------------------

inline GeMeshWingedEdge::VertexId
GeMeshWingedEdge::HalfEdgeWrapper::getTipVertexId() const
{
    return getNextHalfEdge().getOriginVertexId();
}

//------------------------------------------------------------------------------

inline const GeMeshWingedEdge::VertexType& 
GeMeshWingedEdge::HalfEdgeWrapper::getTipVertex() const
{
    return getMesh().getVertex( getTipVertexId() );
}

//------------------------------------------------------------------------------

inline GeMeshWingedEdge::VertexId
GeMeshWingedEdge::HalfEdgeWrapper::getOppositeVertexId() const
{
    return getNextHalfEdge().getTipVertexId();
}

//------------------------------------------------------------------------------

inline const GeMeshWingedEdge::VertexType& 
GeMeshWingedEdge::HalfEdgeWrapper::getOppositeVertex() const
{
    return getMesh().getVertex( getOppositeVertexId() );
}

//------------------------------------------------------------------------------

inline GeMeshWingedEdge::HalfEdgeId 
GeMeshWingedEdge::HalfEdgeWrapper::getTwinHalfEdgeId() const
{
    return getMeshWE()._halfEdges[ _halfEdgeId ].getTwinHalfEdgeId();
}

//------------------------------------------------------------------------------

inline GeMeshWingedEdge::HalfEdgeWrapper
GeMeshWingedEdge::HalfEdgeWrapper::getTwinHalfEdge() const
{
    return getMeshWE().getHalfEdge( getTwinHalfEdgeId() );
}

//------------------------------------------------------------------------------

inline GeMeshWingedEdge::FaceId 
GeMeshWingedEdge::HalfEdgeWrapper::getFaceId() const
{
    return getMeshWE()._halfEdges[ _halfEdgeId ].getFaceId();
}

//------------------------------------------------------------------------------

inline GeMesh::FaceType
GeMeshWingedEdge::HalfEdgeWrapper::getFace() const
{
    return getMesh().getFace( getFaceId() );
}

//------------------------------------------------------------------------------

inline GeMeshWingedEdge::HalfEdgeId 
GeMeshWingedEdge::HalfEdgeWrapper::getNextHalfEdgeId() const
{
    return getMeshWE()._halfEdges[ _halfEdgeId ].getNextHalfEdgeId();
}

//------------------------------------------------------------------------------

inline GeMeshWingedEdge::HalfEdgeWrapper 
GeMeshWingedEdge::HalfEdgeWrapper::getNextHalfEdge() const
{
    return getMeshWE().getHalfEdge( getNextHalfEdgeId() );
}

//------------------------------------------------------------------------------

inline GeMeshWingedEdge::HalfEdgeId 
GeMeshWingedEdge::HalfEdgeWrapper::getPrevHalfEdgeId() const
{
    // Assumes triangles, of course
    return getNextHalfEdge().getNextHalfEdgeId();
}

//------------------------------------------------------------------------------

inline GeMeshWingedEdge::HalfEdgeWrapper 
GeMeshWingedEdge::HalfEdgeWrapper::getPrevHalfEdge() const
{
    return getMeshWE().getHalfEdge( getPrevHalfEdgeId() );
}

//------------------------------------------------------------------------------

inline bool GeMeshWingedEdge::HalfEdgeWrapper::hasTwin() const
{
    return getMeshWE()._halfEdges[ _halfEdgeId ].hasTwin();
}


////////////////////////////////////////////////////////////////////////////////
// CLASS GeMeshWingedEdge::EdgeIteratorBase
//

//------------------------------------------------------------------------------

template <bool HALF>
inline TEMPLATE_NAMESPACE_PREFIX GeMeshWingedEdge::EdgeIteratorBase<HALF>
TEMPLATE_NAMESPACE_PREFIX GeMeshWingedEdge::EdgeIteratorBase<HALF>::end(
    const GeMeshWingedEdge& meshwe
) {
    // Pointer variant of constructor is unchecked.
    return EdgeIteratorBase(
        HalfEdgeWrapper( &meshwe, meshwe.getNbHalfEdges() )
    );
}

//------------------------------------------------------------------------------

template <bool HALF>
inline bool TEMPLATE_NAMESPACE_PREFIX
GeMeshWingedEdge::EdgeIteratorBase<HALF>::operator==(
    const EdgeIteratorBase& rhs
) const {
    return ( _wrapper._meshwe == rhs._wrapper._meshwe ) &&
        ( _wrapper._halfEdgeId == rhs._wrapper._halfEdgeId );
}

//------------------------------------------------------------------------------

template <bool HALF>
inline bool TEMPLATE_NAMESPACE_PREFIX
GeMeshWingedEdge::EdgeIteratorBase<HALF>::operator!=(
    const EdgeIteratorBase& rhs
) const {
    return !operator==( rhs );
}

//------------------------------------------------------------------------------

template <bool HALF>
inline const TEMPLATE_NAMESPACE_PREFIX GeMeshWingedEdge::HalfEdgeWrapper&
TEMPLATE_NAMESPACE_PREFIX
GeMeshWingedEdge::EdgeIteratorBase<HALF>::operator*() const
{
    DGFX_ASSERT( _wrapper.isValid() );
    return _wrapper;
}

//------------------------------------------------------------------------------

template <bool HALF>
inline const TEMPLATE_NAMESPACE_PREFIX GeMeshWingedEdge::HalfEdgeWrapper*
TEMPLATE_NAMESPACE_PREFIX
GeMeshWingedEdge::EdgeIteratorBase<HALF>::operator->() const
{
    DGFX_ASSERT( _wrapper.isValid() );
    return &_wrapper;
}


////////////////////////////////////////////////////////////////////////////////
// CLASS GeMeshWingedEdge::VertexEdgeIteratorBase
//

//------------------------------------------------------------------------------

template <bool HALF>
inline TEMPLATE_NAMESPACE_PREFIX GeMeshWingedEdge::VertexEdgeIteratorBase<HALF>
TEMPLATE_NAMESPACE_PREFIX GeMeshWingedEdge::VertexEdgeIteratorBase<HALF>::end(
    const GeMeshWingedEdge& meshwe,
    VertexId
) {
    return VertexEdgeIteratorBase(
        HalfEdgeWrapper( &meshwe, ID_INVALID ),
        ID_INVALID
    );
}

//------------------------------------------------------------------------------

template <bool HALF>
inline bool TEMPLATE_NAMESPACE_PREFIX
GeMeshWingedEdge::VertexEdgeIteratorBase<HALF>::operator==(
    const VertexEdgeIteratorBase& rhs
) const {
    return ( _wrapper._meshwe == rhs._wrapper._meshwe ) &&
        ( _vertexId == rhs._vertexId ) &&
        ( _wrapper._halfEdgeId == rhs._wrapper._halfEdgeId );
}

//------------------------------------------------------------------------------

template <bool HALF>
inline bool TEMPLATE_NAMESPACE_PREFIX
GeMeshWingedEdge::VertexEdgeIteratorBase<HALF>::operator!=(
    const VertexEdgeIteratorBase& rhs
) const {
    return !operator==( rhs );
}

//------------------------------------------------------------------------------

template <bool HALF>
inline const TEMPLATE_NAMESPACE_PREFIX GeMeshWingedEdge::HalfEdgeWrapper&
TEMPLATE_NAMESPACE_PREFIX
GeMeshWingedEdge::VertexEdgeIteratorBase<HALF>::operator*() const
{
    DGFX_ASSERT( _vertexId != ID_INVALID && _wrapper.isValid() );
    return _wrapper;
}

//------------------------------------------------------------------------------

template <bool HALF>
inline const TEMPLATE_NAMESPACE_PREFIX GeMeshWingedEdge::HalfEdgeWrapper*
TEMPLATE_NAMESPACE_PREFIX
GeMeshWingedEdge::VertexEdgeIteratorBase<HALF>::operator->() const
{
    DGFX_ASSERT( _vertexId != ID_INVALID && _wrapper.isValid() );
    return &_wrapper;
}


////////////////////////////////////////////////////////////////////////////////
// CLASS GeMeshWingedEdge::VertexFaceIterator
//

//------------------------------------------------------------------------------

inline GeMeshWingedEdge::VertexFaceIterator
GeMeshWingedEdge::VertexFaceIterator::end(
    const GeMeshWingedEdge& meshwe,
    VertexId
) {
    return VertexFaceIterator(
        HalfEdgeWrapper( &meshwe, ID_INVALID ),
        ID_INVALID
    );
}

//------------------------------------------------------------------------------

inline bool GeMeshWingedEdge::VertexFaceIterator::operator==(
    const VertexFaceIterator& rhs
) const {
    return ( _wrapper._meshwe == rhs._wrapper._meshwe ) &&
        ( _vertexId == rhs._vertexId ) &&
        ( _wrapper._halfEdgeId == rhs._wrapper._halfEdgeId );
}

//------------------------------------------------------------------------------

inline bool GeMeshWingedEdge::VertexFaceIterator::operator!=(
    const VertexFaceIterator& rhs
) const {
    return !operator==( rhs );
}

//------------------------------------------------------------------------------

inline const GeMesh::FaceWrapper&
GeMeshWingedEdge::VertexFaceIterator::operator*() const
{
    DGFX_ASSERT( _vertexId != ID_INVALID && _wrapper.isValid() );
    return _faceWrapper;
}

//------------------------------------------------------------------------------

inline const GeMesh::FaceWrapper*
GeMeshWingedEdge::VertexFaceIterator::operator->() const
{
    DGFX_ASSERT( _vertexId != ID_INVALID && _wrapper.isValid() );
    return &_faceWrapper;
}

FREECLOTH_NAMESPACE_END

#endif
