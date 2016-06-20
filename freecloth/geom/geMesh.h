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

#ifndef freecloth_geom_geMesh_h
#define freecloth_geom_geMesh_h

#ifndef freecloth_geom_package_h
#include <freecloth/geom/package.h>
#endif

#ifndef freecloth_base_vector
#include <freecloth/base/vector>
#endif

#ifndef freecloth_resmgt_rcBase_h
#include <freecloth/resmgt/rcBase.h>
#endif

#ifndef freecloth_geom_gePoint_h
#include <freecloth/geom/gePoint.h>
#endif

#ifndef freecloth_geom_geMeshTypes_h
#include <freecloth/geom/geMeshTypes.h>
#endif

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//

class GeMeshBuilder;
class GeVector;

////////////////////////////////////////////////////////////////////////////////
/*!
 * \class GeMesh freecloth/geom/geMesh.h
 * \brief A class for triangle meshes.
 *
 * A mesh is represented by vertices, texture vertices and faces. Vertices
 * and texture vertices are stored in arrays, and faces use array indices
 * (VertexId, TextureVertexId) to refer to vertices. A mesh with no
 * texture vertices is considered to have no texture.
 *
 * Must be constructed by GeMeshBuilder. Allows changes to geometry
 * (vertices, texture vertices) after construction, but does not allow
 * changes to topology (faces).
 *
 * Retrieval and storage of faces is a little odd. Faces are stored as
 * GeMesh::Face objects, containing only face data. Clients are presented
 * with the alternate GeMesh::FaceWrapper class, which is given access to
 * vertex and texture vertex data; this is more useful for client use.
 * (Design pattern: facade.) A special iterator class allows iteration over
 * the faces using FaceWrappers.
 *
 * A winged-edge representation can be built with GeMeshWingedEdge. This
 * representation allows better traversal of the mesh topology, but has
 * too much overhead for everyday use.
 */

// Note - RCBase *must* be first parent class here... weird compiler
// bugs due to multiple inheritance otherwise. FIXME: which platform?
class GeMesh : public RCBase, public GeMeshTypes {

public:

    // ----- classes -----

    //! Custom ConstIterator for faces.
    class FaceConstIterator;
    //! Facade for faces.
    class FaceWrapper;
    
    // ----- types and enumerations -----
    
    //! Facade for faces.
    typedef FaceWrapper FaceType;

    // ----- member functions -----
    
    // Default copy constructor is fine.
    // Default assignment operator is fine.
    
    UInt32 getNbFaces() const;
    FaceType getFace( FaceId ) const;
    FaceConstIterator beginFace() const;
    FaceConstIterator endFace() const;

    UInt32 getNbVertices() const;
    const VertexType& getVertex( VertexId ) const;
    VertexType& getVertex( VertexId );
    const VertexType* getVertexArray() const;
    VertexIterator beginVertex();
    VertexIterator endVertex();
    VertexConstIterator beginVertex() const;
    VertexConstIterator endVertex() const;

    bool hasTexture() const;
    UInt32 getNbTextureVertices() const;
    const TextureVertexType& getTextureVertex( TextureVertexId ) const;
    TextureVertexType& getTextureVertex( TextureVertexId );
    const TextureVertexType* getTextureVertexArray() const;
    TextureVertexIterator beginTextureVertex();
    TextureVertexIterator endTextureVertex();
    TextureVertexConstIterator beginTextureVertex() const;
    TextureVertexConstIterator endTextureVertex() const;


private:

    // ----- friends -----

    friend class FaceWrapper;
    friend class GeMeshBuilder;

    // ----- classes -----

    class Face;

    // ----- types and enumerations -----
    
    typedef std::vector<Face> FaceContainer;

    // ----- member functions -----
    GeMesh();

    // ----- data members -----
    
    FaceContainer           _faces;
    VertexContainer         _vertices;
    TextureVertexContainer  _textureVertices;
};


////////////////////////////////////////////////////////////////////////////////
/*!
 * \class GeMesh::Face freecloth/geom/geMesh.h
 * \brief A private data structure used to store information associated with
 * mesh faces.
 *
 * This class has no knowledge of other parts of the mesh (vertices,
 * texture vertices), and hence can only perform a very limited number of
 * operations.
 *
 * Vertices are assumed to be in counter-clockwise order.
 */
class GeMesh::Face : public GeMeshTypes {

public:
    // ----- types and enumerations -----
    enum {
        //! Maximum number of vertices in a face.
        NB_VERTICES = 3
    };

    // ----- member functions -----
    Face(
        VertexId, VertexId, VertexId,
        TextureVertexId, TextureVertexId, TextureVertexId
    );
    // Default copy constructor is fine.
    // Default assignment operator is fine.
    UInt32 getNbVertices() const;
    VertexId getVertexId( FaceVertexId ) const;
    TextureVertexId getTextureVertexId( FaceVertexId ) const;

private:

    // ----- friends -----
    
    friend class GeMesh;

    // ----- data members -----

    VertexId        _vertexIds[ NB_VERTICES ];
    TextureVertexId _textureVertexIds[ NB_VERTICES ];
};


////////////////////////////////////////////////////////////////////////////////
/*!
 * \class GeMesh::FaceWrapper freecloth/geom/geMesh.h
 * \brief Facade class for access to face data, while drawing on information
 * from the vertex and texture vertex arrays.
 *
 * Each face contains indices into the vertex and texture vertex arrays.
 * From the vertices, other information can be calculated.
 *
 * At present, all faces are triangular.
 *
 * \pattern Facade
 */
class GeMesh::FaceWrapper : public GeMeshTypes {

public:
    // ----- types and enumerations -----

    enum {
        //! Maximum number of vertices in a face.
        NB_VERTICES = 3
    };

    // ----- member functions -----
    
    //! Construct invalid wrapper - mostly used by iterator classes.
    FaceWrapper();
    FaceWrapper( const GeMesh&, FaceId );
    // Default copy constructor is fine.
    // Default assignment operator is fine.

    FaceId getFaceId() const;
    UInt32 getNbVertices() const;
    VertexId getVertexId( FaceVertexId ) const;
    const VertexType& getVertex( FaceVertexId ) const;
    TextureVertexId getTextureVertexId( FaceVertexId ) const;
    const TextureVertexType& getTextureVertex( FaceVertexId ) const;

    //! Calculate area of face.
    Float calcArea() const;
    //! Calculate the normal for the face. Guaranteed to be a unit vector.
    GeVector calcNormal() const;
    //! Calculate the normal for the face. May not be a unit vector.
    GeVector calcNonUnitNormal() const;
    //! Calculate the barycentric co-ordinates of p.
    //!
    //! Barycentric co-ordinates express p as a linear combination of the
    //! vertices of the face.
    GePoint calcBarycentric( const GePoint& p ) const;

private:
    // ----- friends -----
    friend class GeMesh::FaceConstIterator;
    friend class GeMeshBuilder;

    // ----- static member functions -----
    // FIXME: move to somewhere else... public GePoint member?
    static GePoint calcBarycentric(
        const GePoint& p,
        const GePoint& v1,
        const GePoint& v2,
        const GePoint& v3
    );

    // ----- member functions -----

    FaceWrapper( const GeMesh*, FaceId );
    bool isValid() const;

    // ----- data members -----
    const GeMesh*   _mesh;
    FaceId          _faceId;
};



////////////////////////////////////////////////////////////////////////////////
/*!
 * \class GeMesh::FaceConstIterator freecloth/geom/geMesh.h
 * \brief Iterator for faces, using the FaceWrapper facade.
 *
 * Essentially, this is just like vector<Face>::const_iterator, but returns
 * a FaceWrapper object through the * and -> operators.
 * 
 * \pattern Iterator
 */
class GeMesh::FaceConstIterator : public GeMeshTypes {

public:
    // ----- static member functions -----
    // named constructor
    static FaceConstIterator begin( const GeMesh& );
    static FaceConstIterator end( const GeMesh& );

    // ----- member functions -----
    FaceConstIterator();
    FaceConstIterator( const GeMesh&, FaceId );
    // default copy constructor is fine.

    FaceConstIterator& operator++();
    FaceConstIterator operator++( int );
    FaceConstIterator& operator--();
    FaceConstIterator operator--( int );

    bool operator == ( const FaceConstIterator& ) const;
    bool operator != ( const FaceConstIterator& ) const;

    const FaceWrapper& operator*() const;
    const FaceWrapper* operator->() const;

private:
    // ----- friends -----
    friend class GeMesh;
    friend class GeMeshBuilder;

    // ----- member functions -----

    //! Private constructor used by the public named constructors. 
    explicit FaceConstIterator( const FaceWrapper& );

    // ----- data members -----
    
    //! We have to keep a wrapper as a member for the -> operator to work.
    GeMesh::FaceWrapper _wrapper;
};

////////////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTIONS
//
std::ostream& operator<<( std::ostream&, const GeMesh& );
std::ostream& operator<<( std::ostream&, const GeMesh::FaceWrapper& );


FREECLOTH_NAMESPACE_END

#include <freecloth/geom/geMesh.inline.h>

#endif
