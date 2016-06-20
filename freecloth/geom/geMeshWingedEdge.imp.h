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

#ifndef freecloth_geom_geMeshWingedEdge_imp_h
#define freecloth_geom_geMeshWingedEdge_imp_h

#ifndef freecloth_geom_geMeshWingedEdge_h
#include <freecloth/geom/geMeshWingedEdge.h>
#endif

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// CLASS GeMeshWingedEdge::EdgeIteratorBase
//

//------------------------------------------------------------------------------

template <bool HALF>
TEMPLATE_NAMESPACE_PREFIX
GeMeshWingedEdge::EdgeIteratorBase<HALF>::EdgeIteratorBase()
{
}

//------------------------------------------------------------------------------

template <bool HALF>
TEMPLATE_NAMESPACE_PREFIX
GeMeshWingedEdge::EdgeIteratorBase<HALF>::EdgeIteratorBase(
    const GeMeshWingedEdge& meshwe,
    HalfEdgeId halfEdgeId
) : _wrapper( meshwe, halfEdgeId )
{
}

//------------------------------------------------------------------------------

template <bool HALF>
TEMPLATE_NAMESPACE_PREFIX
GeMeshWingedEdge::EdgeIteratorBase<HALF>::EdgeIteratorBase(
    const HalfEdgeWrapper& wrapper
) : _wrapper( wrapper )
{
}

//------------------------------------------------------------------------------

template <bool HALF>
TEMPLATE_NAMESPACE_PREFIX GeMeshWingedEdge::EdgeIteratorBase<HALF>
TEMPLATE_NAMESPACE_PREFIX GeMeshWingedEdge::EdgeIteratorBase<HALF>::begin(
    const GeMeshWingedEdge& meshwe
) {
    return EdgeIteratorBase( meshwe, 0 );
}

//------------------------------------------------------------------------------

template <bool HALF>
TEMPLATE_NAMESPACE_PREFIX GeMeshWingedEdge::EdgeIteratorBase<HALF>&
TEMPLATE_NAMESPACE_PREFIX
GeMeshWingedEdge::EdgeIteratorBase<HALF>::operator++()
{
    do {
        ++_wrapper._halfEdgeId;
        if ( HALF ) {
            break;
        }
    } while (
        // If we're doing full-edges, repeat increment until
        // a) we reach the end
        _wrapper._halfEdgeId < _wrapper.getMeshWE().getNbHalfEdges() &&
        // b) we find a half-edge with a lower origin id than tip id (used
        // to ensure that only one half-edge in each edge is iterated)
        _wrapper.getOriginVertexId() > _wrapper.getTipVertexId() &&
        // c) ... unless this half-edge has no twin
        _wrapper.hasTwin()
    );
    return *this;
}

//------------------------------------------------------------------------------

template <bool HALF>
TEMPLATE_NAMESPACE_PREFIX GeMeshWingedEdge::EdgeIteratorBase<HALF>
TEMPLATE_NAMESPACE_PREFIX
GeMeshWingedEdge::EdgeIteratorBase<HALF>::operator++( int )
{
    EdgeIteratorBase copy( *this );
    operator++();
    return copy;
}


////////////////////////////////////////////////////////////////////////////////
// CLASS GeMeshWingedEdge::VertexEdgeIteratorBase
//

//------------------------------------------------------------------------------

template <bool HALF>
TEMPLATE_NAMESPACE_PREFIX
GeMeshWingedEdge::VertexEdgeIteratorBase<HALF>::VertexEdgeIteratorBase()
{
}

//------------------------------------------------------------------------------

template <bool HALF>
TEMPLATE_NAMESPACE_PREFIX
GeMeshWingedEdge::VertexEdgeIteratorBase<HALF>::VertexEdgeIteratorBase(
    const GeMeshWingedEdge& meshwe,
    VertexId vertexId,
    HalfEdgeId halfEdgeId
) : _wrapper( meshwe, halfEdgeId ),
    _vertexId( vertexId )
{
    if ( HALF ) {
        DGFX_ASSERT(
            _wrapper.getOriginVertexId() == _vertexId
        );
    }
}

//------------------------------------------------------------------------------

template <bool HALF>
TEMPLATE_NAMESPACE_PREFIX
GeMeshWingedEdge::VertexEdgeIteratorBase<HALF>::VertexEdgeIteratorBase(
    const HalfEdgeWrapper& wrapper,
    VertexId vertexId
) : _wrapper( wrapper ),
    _vertexId( vertexId )
{
#if 0
    if ( HALF ) {
        DGFX_ASSERT(
            _wrapper.getOriginVertexId() == _vertexId
        );
    }
#endif
}

//------------------------------------------------------------------------------

template <bool HALF>
TEMPLATE_NAMESPACE_PREFIX GeMeshWingedEdge::VertexEdgeIteratorBase<HALF>
TEMPLATE_NAMESPACE_PREFIX
GeMeshWingedEdge::VertexEdgeIteratorBase<HALF>::begin(
    const GeMeshWingedEdge& meshwe,
    VertexId vertexId
) {
    DGFX_ASSERT( vertexId < meshwe._vertexHalfEdgeIds.size() );
    // Not adjacent to any edges
    if ( meshwe._vertexHalfEdgeIds[ vertexId ] == ID_INVALID ) {
        return end( meshwe, vertexId );
    }
    return VertexEdgeIteratorBase(
        meshwe, vertexId, meshwe._vertexHalfEdgeIds[ vertexId ]
    );
}

//------------------------------------------------------------------------------

template <bool HALF>
TEMPLATE_NAMESPACE_PREFIX GeMeshWingedEdge::VertexEdgeIteratorBase<HALF>&
TEMPLATE_NAMESPACE_PREFIX
GeMeshWingedEdge::VertexEdgeIteratorBase<HALF>::operator++()
{
    bool atEnd = false;
    if ( _wrapper.getOriginVertexId() != _vertexId ) {
        atEnd = true;
    }
    else if ( _wrapper.getPrevHalfEdge().hasTwin() ) {
        _wrapper._halfEdgeId = _wrapper.getPrevHalfEdge().getTwinHalfEdgeId();
        DGFX_ASSERT( _wrapper.getOriginVertexId() == _vertexId );
        atEnd = (
            _wrapper._halfEdgeId ==
            _wrapper.getMeshWE()._vertexHalfEdgeIds[ _vertexId ]
        );
    }
    else {
        if ( HALF ) {
            atEnd = true;
        }
        else {
            _wrapper._halfEdgeId = _wrapper.getPrevHalfEdgeId();
            DGFX_ASSERT( _wrapper.getOriginVertexId() != _vertexId );
        }
    }
    if ( atEnd ) {
        _vertexId = ID_INVALID;
        _wrapper._halfEdgeId = ID_INVALID;
    }
    return *this;
}

//------------------------------------------------------------------------------

template <bool HALF>
TEMPLATE_NAMESPACE_PREFIX GeMeshWingedEdge::VertexEdgeIteratorBase<HALF>
TEMPLATE_NAMESPACE_PREFIX
GeMeshWingedEdge::VertexEdgeIteratorBase<HALF>::operator++( int )
{
    VertexEdgeIteratorBase copy( *this );
    operator++();
    return copy;
}

FREECLOTH_NAMESPACE_END

#endif
