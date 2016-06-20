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

#include <freecloth/geom/geMeshWingedEdge.imp.h>
#include <freecloth/base/map>

////////////////////////////////////////////////////////////////////////////////
// LOCAL DECLARATIONS

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// CLASS GeMeshWingedEdge

//------------------------------------------------------------------------------

GeMeshWingedEdge::GeMeshWingedEdge( const RCShdPtr<GeMesh>& mesh )
  : _mesh( mesh )
{
    DGFX_ASSERT( !mesh.isNull() );
    build( *mesh );
}

//------------------------------------------------------------------------------

void GeMeshWingedEdge::build( const GeMesh& mesh )
{
    typedef std::pair< VertexId, VertexId > VertexIdPair;
    typedef std::map< VertexIdPair, HalfEdgeId > EdgeMap;
    EdgeMap edgeMap;

    GeMesh::FaceConstIterator fi;
    _halfEdges.resize( mesh.getNbFaces() * 3 );

    _vertexHalfEdgeIds.resize( mesh.getNbVertices() );
    std::fill(
        _vertexHalfEdgeIds.begin(),
        _vertexHalfEdgeIds.end(),
        static_cast<HalfEdgeId>( HalfEdge::ID_INVALID )
    );
    _faceHalfEdgeIds.resize( mesh.getNbFaces() );
    std::fill(
        _faceHalfEdgeIds.begin(),
        _faceHalfEdgeIds.end(),
        static_cast<HalfEdgeId>( HalfEdge::ID_INVALID )
    );

    HalfEdgeId hid;
    FaceId fid;
    for (
        fi = mesh.beginFace(), hid = 0, fid = 0;
        fi != mesh.endFace();
        hid += fi->getNbVertices(), ++fi, ++fid
    ) {
        DGFX_ASSERT( fi->getNbVertices() == 3 );
        for ( FaceVertexId fvid = 0; fvid < fi->getNbVertices(); ++fvid ) {
            // Next face vertex id
            FaceVertexId fvid2 = (fvid + 1) % 3;
            // Corresponding vertex ids
            VertexId vid = fi->getVertexId( fvid );
            VertexId vid2 = fi->getVertexId( fvid2 );
            // Corresponding halfedge ids
            HalfEdgeId heid = hid + fvid;
            HalfEdgeId heid2 = hid + fvid2;

#if 0
            if ( vid == 0 ) {
                std::cout << "Face " << fid
                    << " vertex ids " << fi->getVertexId( 0 ) << "," << fi->getVertexId( 1 ) << "," << fi->getVertexId( 2 )
                    << " vertices " << fi->getVertex( 0 ) << "," << fi->getVertex( 1 ) << "," << fi->getVertex( 2 )
                    << std::endl;
            }
#endif
            _halfEdges[ heid ] = HalfEdge(
                vid,                    // vertex
                HalfEdge::ID_INVALID,   // twin
                fid,                    // face
                heid2                   // next half-edge
            );
            _vertexHalfEdgeIds[ vid ] = heid;
            _faceHalfEdgeIds[ fid ] = heid;
            if ( vid < vid2 ) {
                edgeMap[ VertexIdPair( vid, vid2 ) ] = heid;
            }
        }
    }

    // Make another pass and hook twins up together.
    EdgeMap::const_iterator ei;
    for (
        fi = mesh.beginFace(), hid = 0, fid = 0;
        fi != mesh.endFace();
        hid += fi->getNbVertices(), ++fi, ++fid
    ) {
        for ( FaceVertexId fvid = 0; fvid < fi->getNbVertices(); ++fvid ) {
            FaceVertexId fvid2 = (fvid + 1) % 3;
            VertexId vid = fi->getVertexId( fvid );
            VertexId vid2 = fi->getVertexId( fvid2 );
            if ( vid > vid2 ) {
                ei = edgeMap.find( VertexIdPair( vid2, vid ) );
                if ( ei != edgeMap.end() ) {
                    _halfEdges[ hid + fvid ]._twin = ei->second;
                    _halfEdges[ ei->second ]._twin = hid + fvid;
                }
            }
        }
    }

    // Make a final pass and fill in vertex half-edge ids
    for (
        fi = mesh.beginFace(), hid = 0, fid = 0;
        fi != mesh.endFace();
        hid += fi->getNbVertices(), ++fi, ++fid
    ) {
        for ( FaceVertexId fvid = 0; fvid < fi->getNbVertices(); ++fvid ) {
            // Corresponding vertex ids
            VertexId vid = fi->getVertexId( fvid );
            // Corresponding halfedge ids
            HalfEdgeId heid = hid + fvid;

            // If there's a half-edge with no twin, then that should be the
            // first half-edge used for iterating half-edges about vertex.
            if ( ! _halfEdges[ heid ].hasTwin() ) {
                _vertexHalfEdgeIds[ vid ] = heid;
            }
        }
    }
}

//------------------------------------------------------------------------------

inline const RCShdPtr<GeMesh>& GeMeshWingedEdge::getMeshPtr() const
{
    return _mesh;
}


////////////////////////////////////////////////////////////////////////////////
// CLASS GeMeshWingedEdge::VertexFaceIterator
//

//------------------------------------------------------------------------------

GeMeshWingedEdge::VertexFaceIterator::VertexFaceIterator()
{
}

//------------------------------------------------------------------------------

GeMeshWingedEdge::VertexFaceIterator::VertexFaceIterator(
    const GeMeshWingedEdge& meshwe,
    VertexId vertexId,
    HalfEdgeId halfEdgeId
) : _wrapper( meshwe, halfEdgeId ),
    _vertexId( vertexId )
{
    DGFX_ASSERT(
        _wrapper.getOriginVertexId() == _vertexId
    );
}

//------------------------------------------------------------------------------

GeMeshWingedEdge::VertexFaceIterator::VertexFaceIterator(
    const HalfEdgeWrapper& wrapper,
    VertexId vertexId
) : _wrapper( wrapper ),
    _vertexId( vertexId )
{
}

//------------------------------------------------------------------------------

GeMeshWingedEdge::VertexFaceIterator
GeMeshWingedEdge::VertexFaceIterator::begin(
    const GeMeshWingedEdge& meshwe,
    VertexId vertexId
) {
    DGFX_ASSERT( vertexId < meshwe._vertexHalfEdgeIds.size() );
    // Not adjacent to any edges
    if ( meshwe._vertexHalfEdgeIds[ vertexId ] == ID_INVALID ) {
        return end( meshwe, vertexId );
    }
    return VertexFaceIterator(
        meshwe, vertexId, meshwe._vertexHalfEdgeIds[ vertexId ]
    );
}

//------------------------------------------------------------------------------

GeMeshWingedEdge::VertexFaceIterator&
GeMeshWingedEdge::VertexFaceIterator::operator++()
{
    DGFX_ASSERT( _vertexId != ID_INVALID );

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
        atEnd = true;
    }
    if ( atEnd ) {
        _vertexId = ID_INVALID;
        _wrapper._halfEdgeId = ID_INVALID;
    }
    else {
        _faceWrapper = _wrapper.getFace();
    }
    return *this;
}

//------------------------------------------------------------------------------

GeMeshWingedEdge::VertexFaceIterator
GeMeshWingedEdge::VertexFaceIterator::operator++( int )
{
    VertexFaceIterator copy( *this );
    operator++();
    return copy;
}


////////////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTIONS
//

//------------------------------------------------------------------------------

std::ostream& operator<< ( std::ostream& os, const GeMeshWingedEdge& m )
{
    os << "GeMeshWingedEdge" << std::endl;
    GeMeshWingedEdge::HalfEdgeIterator hei;
    UInt32 i;
    for( hei = m.beginHalfEdge(), i = 0; hei != m.endHalfEdge(); ++hei, ++i ) {
        os << "[" << i << "] " << *hei << std::endl;
    }
    return os;
}

//------------------------------------------------------------------------------

std::ostream& operator<< (
    std::ostream& os,
    const GeMeshWingedEdge::HalfEdgeWrapper& he
) {
    os << "he( origin_vid=" << he.getOriginVertexId();
    os << " twin_heid=";
    if ( he.hasTwin() ) {
        os << he.getTwinHalfEdgeId();
    }
    else {
        os << "(nil)";
    }
    os << " face_id=" << he.getFaceId();
    os << " next_heid=" << he.getNextHalfEdgeId();

    return os;
}

////////////////////////////////////////////////////////////////////////////////
// TEMPLATE INSTANTIATIONS
//

template class GeMeshWingedEdge::EdgeIteratorBase<false>;
template class GeMeshWingedEdge::EdgeIteratorBase<true>;
template class GeMeshWingedEdge::VertexEdgeIteratorBase<false>;
template class GeMeshWingedEdge::VertexEdgeIteratorBase<true>;

FREECLOTH_NAMESPACE_END
