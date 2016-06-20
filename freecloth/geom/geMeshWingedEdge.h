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

#ifndef freecloth_geom_geMeshWingedEdge_h
#define freecloth_geom_geMeshWingedEdge_h

#ifndef freecloth_geom_package_h
#include <freecloth/geom/package.h>
#endif

#ifndef freecloth_geom_geMesh_h
#include <freecloth/geom/geMesh.h>
#endif

#ifndef freecloth_resmgt_rcBase_h
#include <freecloth/resmgt/rcBase.h>
#endif

#ifndef freecloth_resmgt_rcShdPtr_h
#include <freecloth/resmgt/rcShdPtr.h>
#endif

#ifndef freecloth_base_vector
#include <freecloth/base/vector>
#endif

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// FORWARD DECLARATIONS

template <class T> class RCShdPtr;

////////////////////////////////////////////////////////////////////////////////
/*!
 * \class GeMeshWingedEdge freecloth/geom/geMeshWingedEdge.h
 * \brief Add-on for GeMesh class to store winged-edge data structure.
 *
 * Allows more general traversal of a mesh. The winged-edge representation
 * focuses on an entity known as the "half edge," where each edge separating
 * two faces of the mesh is represented by two independent twinned half-edges,
 * one associated with each face. Each half edge points from an "origin" vertex
 * to a "tip" vertex, in a counter-clockwise direction within the face.
 *
 * In computational geometry, the "planar subdivision" data structure operates
 * in a similar manner in two dimensions, but adds a special face for the
 * outside world beyond the mesh. We do not have any such outside face, and
 * there may yet be some bugs in the code due to this limitation.
 *
 * This approach can only handle a limited set of topologies. Disc and
 * sphere topology should be fine. Only one component is allowed, and faces
 * must all point in the same direction. There aren't much of any assertions to
 * enforce this.
 *
 * Using the winged-edge data structure, we can easily
 * - retrieve edges associated with a vertex or face
 * - retrieve faces associated with an edge
 *
 * We allow iteration over both half-edges and "edges". Edge iteration
 * still iterates over half-edge objects; it just changes the iteration
 * metaphor to only include one of each pair of twinned half-edges.
 *
 * The winged edge data structure extends the standard mesh data structure,
 * and we hence keep a pointer to the source mesh. The winged edge data is only
 * dependent upon the mesh topology, which is immutable after construction.
 *
 * Wrapper/ConstIterator classes are direct rips of
 * GeMesh::FaceWrapper / ConstIterator. Some day, I should try to make a
 * general-purpose class to do this stuff...
 */

// FIXME: should we just derive off of GeMesh, and allow GeMeshBuilder to
// construct one or the other off the bat?
// RCBase must be first parent. See GeMesh.
class GeMeshWingedEdge : public RCBase, public GeMeshTypes
{
public:

    // ----- classes -----
    //! Half edge facade class
    class HalfEdgeWrapper;
    //! Iterator for edges/halfedges.
    template <bool HALF> class EdgeIteratorBase;
    //! Iterator for edges/halfedges incident on vertex.
    template <bool HALF> class VertexEdgeIteratorBase;
    //! Iterator for faces touching vertex.
    class VertexFaceIterator;

    // All are const iterators.
    //! Half edge iterator class.
    typedef EdgeIteratorBase<true> HalfEdgeIterator;
    //! Edge iterator class.
    typedef EdgeIteratorBase<false> EdgeIterator;
    //! Iterator for halfedges incident on vertex
    typedef VertexEdgeIteratorBase<true> VertexHalfEdgeIterator;
    //! Iterator for edges incident on vertex
    typedef VertexEdgeIteratorBase<false> VertexEdgeIterator;

    // ----- types and enumerations -----

    //! Half edge index
    typedef UInt32          HalfEdgeId;
    //! Half edge facade class
    typedef HalfEdgeWrapper HalfEdgeType;

    // ----- member functions -----
    explicit GeMeshWingedEdge( const RCShdPtr<GeMesh>& );

    UInt32 getNbHalfEdges() const;
    HalfEdgeType getHalfEdge( HalfEdgeId ) const;

    HalfEdgeIterator beginHalfEdge() const;
    HalfEdgeIterator endHalfEdge() const;
    EdgeIterator beginEdge() const;
    EdgeIterator endEdge() const;

    //! Iterate over half-edges whose origin is vertexId. Note that this may
    //! not include all edges - any vertex that lies on the boundary will
    //! have two edges w/o twins, and one of these will not be included.
    VertexHalfEdgeIterator beginVertexHalfEdge( VertexId ) const;
    VertexHalfEdgeIterator endVertexHalfEdge( VertexId ) const;

    //!! Iterate over all edges that touch vertexId. This will only iterate once
    // over each half-edge pair.
    VertexEdgeIterator beginVertexEdge( VertexId ) const;
    VertexEdgeIterator endVertexEdge( VertexId ) const;

    VertexFaceIterator beginVertexFace( VertexId ) const;
    VertexFaceIterator endVertexFace( VertexId ) const;

    //HalfEdgeId getHalfEdgeFromVertexId( VertexId ) const;
    HalfEdgeId getHalfEdgeFromFaceId( FaceId ) const;

    const RCShdPtr<GeMesh>& getMeshPtr() const;
    const GeMesh& getMesh() const;

private:

    // ----- classes -----
    class HalfEdge;
    typedef std::vector< HalfEdge > HalfEdgeContainer;

    // ----- friends -----
    friend class HalfEdgeWrapper;
#ifdef HAVE_TEMPLATE_FRIENDS
    template <bool HALF> friend class VertexEdgeIteratorBase;
#else
    friend class VertexEdgeIteratorBase<false>;
    friend class VertexEdgeIteratorBase<true>;
#endif
    friend class VertexFaceIterator;

    // ----- member functions -----
    //! Construction-time function to fill data members.
    void build( const GeMesh& );

    // ----- data members -----
    RCShdPtr<GeMesh> _mesh;
    HalfEdgeContainer   _halfEdges;
    //! Array containing the index of one half-edge for each vertex.
    std::vector< HalfEdgeId > _vertexHalfEdgeIds;
    //! Array containing the index of one half-edge for each face.
    std::vector< HalfEdgeId > _faceHalfEdgeIds;
};


////////////////////////////////////////////////////////////////////////////////
/*!
 * \class GeMeshWingedEdge::HalfEdge freecloth/geom/geMeshWingedEdge.h
 * \brief Half-edge data structure.
 * This is a private class, only used for data storage. The HalfEdgeWrapper
 * class is a facade that is exposed to clients.
 */
class GeMeshWingedEdge::HalfEdge {
public:
    // ----- types and enumerations -----
    enum { ID_INVALID = ~0U };

    // ----- member functions -----
    HalfEdge();
    HalfEdge(
        VertexId origin,
        HalfEdgeId twin,
        FaceId face,
        HalfEdgeId next
    );
    // Default copy constructor is fine.
    // Default assignment operator is fine.

    VertexId getOriginVertexId() const;
    HalfEdgeId getTwinHalfEdgeId() const;
    FaceId getFaceId() const;
    HalfEdgeId getNextHalfEdgeId() const;

    bool hasTwin() const;

private:

    // ----- friends -----
    friend class GeMeshWingedEdge;

    // ----- data members -----
    VertexId _origin;
    HalfEdgeId _twin;
    FaceId _face;
    HalfEdgeId _next;
    // No need for a previous edge member - since all faces are triangular,
    // this can be easily found.
};


////////////////////////////////////////////////////////////////////////////////
/*!
 * \class GeMeshWingedEdge::HalfEdgeWrapper freecloth/geom/geMeshWingedEdge.h
 * \brief Facade class for access to half edge data, while drawing on
 * information from the vertex and texture vertex arrays.
 *
 * Each face contains indices into the vertex and texture vertex arrays.
 * From the vertices, other information can be calculated.
 *
 * At present, all faces are triangular.
 *
 * \pattern Facade
 */
class GeMeshWingedEdge::HalfEdgeWrapper : public GeMeshTypes
{
public:
    // ----- member functions -----
    HalfEdgeWrapper( const GeMeshWingedEdge&, HalfEdgeId );
    // Default copy constructor is fine.

    HalfEdgeId getHalfEdgeId() const;

    //! Get id of vertex at origin of this half-edge
    VertexId getOriginVertexId() const;
    const VertexType& getOriginVertex() const;
    //! Get id of vertex at tip of this half-edge
    VertexId getTipVertexId() const;
    const VertexType& getTipVertex() const;
    //! Get id of vertex opposite this half-edge
    VertexId getOppositeVertexId() const;
    const VertexType& getOppositeVertex() const;

    HalfEdgeId getTwinHalfEdgeId() const;
    HalfEdgeWrapper getTwinHalfEdge() const;
    FaceId getFaceId() const;
    GeMesh::FaceWrapper getFace() const;
    HalfEdgeId getNextHalfEdgeId() const;
    HalfEdgeWrapper getNextHalfEdge() const;
    HalfEdgeId getPrevHalfEdgeId() const;
    HalfEdgeWrapper getPrevHalfEdge() const;

    bool hasTwin() const;

private:
    // ----- friends -----
#ifdef HAVE_TEMPLATE_FRIENDS
    template <bool HALF> friend class GeMeshWingedEdge::EdgeIteratorBase;
    template <bool HALF> friend class GeMeshWingedEdge::VertexEdgeIteratorBase;
#else
    friend class GeMeshWingedEdge::EdgeIteratorBase<false>;
    friend class GeMeshWingedEdge::EdgeIteratorBase<true>;
    friend class GeMeshWingedEdge::VertexEdgeIteratorBase<false>;
    friend class GeMeshWingedEdge::VertexEdgeIteratorBase<true>;
#endif
    friend class VertexFaceIterator;

    // ----- member functions -----

    //! For use by iterator - only it can construct an invalid wrapper.
    HalfEdgeWrapper();
    HalfEdgeWrapper( const GeMeshWingedEdge*, HalfEdgeId );
    bool isValid() const;
    const GeMeshWingedEdge& getMeshWE() const;
    const GeMesh& getMesh() const;

    // ----- data members -----

    //! This must be a pointer to allow modification by the iterator classes.
    const GeMeshWingedEdge* _meshwe;
    HalfEdgeId              _halfEdgeId;
};


////////////////////////////////////////////////////////////////////////////////
/*!
 * \class GeMeshWingedEdge::EdgeIteratorBase freecloth/geom/geMeshWingedEdge.h
 * \brief Class for both edge and halfedge iterators.
 *
 * Template parameter HALF is true for halfedge iterator, false for edge
 * iterator.
 * 
 * \pattern Iterator
 *
 */
template <bool HALF>
class GeMeshWingedEdge::EdgeIteratorBase : public GeMeshTypes {

public:
    // ----- static member functions -----
    //! Named constructor
    //! @{
    static EdgeIteratorBase begin( const GeMeshWingedEdge& );
    static EdgeIteratorBase end( const GeMeshWingedEdge& );
    //! @}

    // ----- member functions -----
    EdgeIteratorBase();
    EdgeIteratorBase( const GeMeshWingedEdge&, HalfEdgeId );
    // Default copy constructor is fine.
    // Default assignment operator is fine.

    EdgeIteratorBase& operator++();
    EdgeIteratorBase operator++( int );

    bool operator==( const EdgeIteratorBase& ) const;
    bool operator!=( const EdgeIteratorBase& ) const;

    const HalfEdgeWrapper& operator*() const;
    const HalfEdgeWrapper* operator->() const;

private:
    // ----- member functions -----
    explicit EdgeIteratorBase( const HalfEdgeWrapper& );

    // ----- data members -----
    GeMeshWingedEdge::HalfEdgeWrapper _wrapper;
};


////////////////////////////////////////////////////////////////////////////////
/*!
 * \class GeMeshWingedEdge::VertexEdgeIteratorBase freecloth/geom/geMeshWingedEdge.h
 * \brief Class for both edge and halfedge iterators touching a vertex.
 *
 * Template parameter HALF is true for halfedge iterator, false for edge
 * iterator.
 *
 * \pattern Iterator
 */
template <bool HALF>
class GeMeshWingedEdge::VertexEdgeIteratorBase : public GeMeshTypes {

public:
    // ----- static member functions -----
    //! Named constructor
    //! @{
    static VertexEdgeIteratorBase begin(
        const GeMeshWingedEdge&,
        VertexId vertexId
    );
    static VertexEdgeIteratorBase end(
        const GeMeshWingedEdge&,
        VertexId vertexId
    );
    //! @}

    // ----- member functions -----
    VertexEdgeIteratorBase();
    VertexEdgeIteratorBase(
        const GeMeshWingedEdge&,
        VertexId vertexId,
        HalfEdgeId halfEdgeId
    );
    // Default copy constructor is fine.
    // Default assignment operator is fine.

    VertexEdgeIteratorBase& operator++();
    VertexEdgeIteratorBase operator++( int );

    bool operator==( const VertexEdgeIteratorBase& ) const;
    bool operator!=( const VertexEdgeIteratorBase& ) const;

    const HalfEdgeWrapper& operator*() const;
    const HalfEdgeWrapper* operator->() const;

private:
    // ----- types and enumerations-----
    enum {
        ID_INVALID = GeMeshWingedEdge::HalfEdge::ID_INVALID
    };

    // ----- member functions -----
    VertexEdgeIteratorBase( const HalfEdgeWrapper&, VertexId );

    // ----- data members -----
    GeMeshWingedEdge::HalfEdgeWrapper _wrapper;
    VertexId _vertexId;
};


////////////////////////////////////////////////////////////////////////////////
/*!
 * \class GeMeshWingedEdge::VertexFaceIterator freecloth/geom/geMeshWingedEdge.h
 * \brief Class for iterating over faces touching a vertex.
 *
 * \pattern Iterator
 */
class GeMeshWingedEdge::VertexFaceIterator : public GeMeshTypes {

public:
    // ----- static member functions -----
    // Named constructors
    static VertexFaceIterator begin(
        const GeMeshWingedEdge&,
        VertexId vertexId
    );
    static VertexFaceIterator end(
        const GeMeshWingedEdge&,
        VertexId vertexId
    );

    // ----- member functions -----
    VertexFaceIterator();
    VertexFaceIterator(
        const GeMeshWingedEdge&,
        VertexId vertexId,
        HalfEdgeId halfEdgeId
    );
    // Default copy constructor is fine.
    // Default assignment operator is fine.

    VertexFaceIterator& operator++();
    VertexFaceIterator operator++( int );

    bool operator==( const VertexFaceIterator& ) const;
    bool operator!=( const VertexFaceIterator& ) const;

    const GeMesh::FaceWrapper& operator*() const;
    const GeMesh::FaceWrapper* operator->() const;

private:
    // ----- types and enumerations-----
    enum {
        ID_INVALID = GeMeshWingedEdge::HalfEdge::ID_INVALID
    };

    // ----- member functions -----
    VertexFaceIterator( const HalfEdgeWrapper&, VertexId );

    // ----- data members -----
    GeMeshWingedEdge::HalfEdgeWrapper _wrapper;
    VertexId _vertexId;
    GeMesh::FaceWrapper _faceWrapper;
};


////////////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTIONS
//

std::ostream& operator<<( std::ostream&, const GeMeshWingedEdge& );
std::ostream& operator<<(
    std::ostream&,
    const GeMeshWingedEdge::HalfEdgeWrapper&
);

FREECLOTH_NAMESPACE_END

#include <freecloth/geom/geMeshWingedEdge.inline.h>

#endif
