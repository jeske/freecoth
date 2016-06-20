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

#ifndef freecloth_geom_geMeshBuilder_h
#define freecloth_geom_geMeshBuilder_h

#ifndef freecloth_geom_package_h
#include <freecloth/geom/package.h>
#endif

#ifndef freecloth_resmgt_rcShdPtr_h
#include <freecloth/resmgt/rcShdPtr.h>
#endif

#ifndef freecloth_geom_geMesh_h
#include <freecloth/geom/geMesh.h>
#endif

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//

template <class T> class RCShdPtr;

////////////////////////////////////////////////////////////////////////////////
/*!
 * \class GeMeshBuilder freecloth/geom/geMeshBuilder.h
 * \brief Class to construct a GeMesh object.
 *
 * Vertices, texture vertices and faces can be added, and the mesh will be
 * created with a final call to createMesh().
 *
 * For added efficiency, the prealloc* routines can be called before the
 * add* routines to reserve space for vertices, texture vertices or faces.
 *
 * During construction, vertices and texture vertices are given temporary
 * IDs and can be retrieved from the partially constructed mesh.
 *
 * \pattern Builder
 */
class GeMeshBuilder : public GeMeshTypes {

public:

    // ----- types and enumerations -----

    //! ConstIterator for faces.
    typedef GeMesh::FaceConstIterator FaceConstIterator;
    
    // ----- member functions -----
    
    GeMeshBuilder();

    //! Reserve space for the given number of vertices.
    void preallocVertices( UInt32 );
    //! Reserve space for the given number of texture vertices.
    void preallocTextureVertices( UInt32 );
    //! Reserve space for the given number of faces.
    void preallocFaces( UInt32 nb );

    //! Add a vertex to the mesh. Returns the id of the new vertex.
    VertexId addVertex( const VertexType& );
    //! Add the range [beginIt,endIt). Returns the id of the first vertex.
    VertexId addVertices(
        VertexConstIterator const& beginIt,
        VertexConstIterator const& endIt
    );
    //! Retrieve number of vertices added to mesh so far.
    UInt32 getNbVertices() const;
    //! Retrieve a vertex that has already been added.
    const VertexType& getVertex( VertexId ) const; 

    //! Add a texture vertex to the mesh. Returns the id of the new texture
    //! vertex.
    TextureVertexId addTextureVertex( const TextureVertexType& );
    //! Add the range [beginIt,endIt). Returns the id of the first texture
    //! vertex.
    TextureVertexId addTextureVertices(
        TextureVertexConstIterator const& beginIt,
        TextureVertexConstIterator const& endIt
    );
    //! Retrieve number of texture vertices added to mesh so far.
    UInt32 getNbTextureVertices() const;
    //! Retrieve a texture vertex that has already been added.
    const TextureVertexType& getTextureVertex( TextureVertexId ) const;

    //! Add a face to the mesh. Returns the id of the new face.
    FaceId addFace(
        VertexId v1, VertexId v2, VertexId v3,
        TextureVertexId tv1, TextureVertexId tv2, TextureVertexId tv3
    );
    //! Add a face to the mesh. Returns the id of the new face.
    FaceId addFace(
        VertexId vids[ GeMesh::FaceType::NB_VERTICES ],
        TextureVertexId tvids[ GeMesh::FaceType::NB_VERTICES ]
    );
    //! Add the range [beginIt,endIt). Returns the id of the first face.
    FaceId addFaces(
        FaceConstIterator const& beginIt,
        FaceConstIterator const& endIt
    );
    //! Retrieve number of faces added to mesh so far.
    UInt32 getNbFaces() const;

    //! Creates a GeMesh object using the vertices, texture vertices and
    //! faces added to the builder so far. Returns a pointer to the built
    //! GeMesh object.
    //!
    //! Subsequently, resets the builder in preparation for the construction
    //! of a new GeMesh object.
    RCShdPtr<GeMesh> createMesh();


private:

    // ----- member functions -----

    //! Disallowed.
    GeMeshBuilder( const GeMeshBuilder& );
    //! Disallowed.
    GeMeshBuilder& operator=( const GeMeshBuilder& );

    // ----- data members -----
    
    RCShdPtr<GeMesh>        _meshPtr;
};

FREECLOTH_NAMESPACE_END
#endif
