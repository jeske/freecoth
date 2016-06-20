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

#include <freecloth/geom/geMeshBuilder.h>
#include <freecloth/base/algorithm>

////////////////////////////////////////////////////////////////////////////////
// LOCAL DECLARATIONS

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// CLASS GeMeshBuilder

//------------------------------------------------------------------------------

GeMeshBuilder::GeMeshBuilder()
    : _meshPtr( RCShdPtr<GeMesh>( new GeMesh ) )
{
}

//------------------------------------------------------------------------------

void GeMeshBuilder::preallocVertices( UInt32 nb )
{
    _meshPtr->_vertices.reserve( nb );
}

//------------------------------------------------------------------------------

void GeMeshBuilder::preallocTextureVertices( UInt32 nb )
{
    _meshPtr->_textureVertices.reserve( nb );
}

//------------------------------------------------------------------------------

void GeMeshBuilder::preallocFaces( UInt32 nb )
{
    _meshPtr->_faces.reserve( nb );
}

//------------------------------------------------------------------------------

GeMeshBuilder::VertexId GeMeshBuilder::addVertex( const VertexType& vertex )
{
    VertexId result = _meshPtr->getNbVertices();
    _meshPtr->_vertices.push_back( vertex );
    return result;
}

//------------------------------------------------------------------------------

GeMeshBuilder::VertexId GeMeshBuilder::addVertices(
    VertexConstIterator const& beginIt,
    VertexConstIterator const& endIt
) {
    VertexId result = _meshPtr->getNbVertices();
    _meshPtr->_vertices.insert( _meshPtr->_vertices.end(), beginIt, endIt );
    return result;
}

//------------------------------------------------------------------------------

UInt32 GeMeshBuilder::getNbVertices() const
{
    return _meshPtr->getNbVertices();
}

//------------------------------------------------------------------------------

const GeMeshBuilder::VertexType& GeMeshBuilder::getVertex( VertexId vid ) const
{
    return _meshPtr->getVertex( vid );
}

//------------------------------------------------------------------------------

GeMeshBuilder::TextureVertexId GeMeshBuilder::addTextureVertex(
    const TextureVertexType& textureVertex
)
{
    TextureVertexId result = _meshPtr->getNbTextureVertices();
    _meshPtr->_textureVertices.push_back( textureVertex );
    return result;
}

//------------------------------------------------------------------------------

GeMeshBuilder::TextureVertexId GeMeshBuilder::addTextureVertices(
    TextureVertexConstIterator const& beginIt,
    TextureVertexConstIterator const& endIt
) {
    TextureVertexId result = _meshPtr->getNbTextureVertices();
    _meshPtr->_textureVertices.insert(
        _meshPtr->_textureVertices.end(),
        beginIt, endIt
    );
    return result;
}

//------------------------------------------------------------------------------

UInt32 GeMeshBuilder::getNbTextureVertices() const
{
    return _meshPtr->getNbTextureVertices();
}

//------------------------------------------------------------------------------

const GeMeshBuilder::TextureVertexType& GeMeshBuilder::getTextureVertex(
    TextureVertexId vid
) const {
    return _meshPtr->getTextureVertex( vid );
}

//------------------------------------------------------------------------------

GeMeshBuilder::FaceId GeMeshBuilder::addFace(
    VertexId v1, VertexId v2, VertexId v3,
    TextureVertexId tv1, TextureVertexId tv2, TextureVertexId tv3
) {
    FaceId result = _meshPtr->getNbFaces();
    _meshPtr->_faces.push_back(
        GeMesh::Face( v1, v2, v3, tv1, tv2, tv3 )
    );
    return result;
}

//------------------------------------------------------------------------------

GeMeshBuilder::FaceId GeMeshBuilder::addFace(
    VertexId vids[ GeMesh::FaceType::NB_VERTICES ],
    TextureVertexId tvids[ GeMesh::FaceType::NB_VERTICES ]
) {
    FaceId result = _meshPtr->getNbFaces();
    _meshPtr->_faces.push_back(
        GeMesh::Face(
            vids[ 0 ], vids[ 1 ], vids[ 2 ],
            tvids[ 0 ], tvids[ 1 ], tvids[ 2 ]
        )
    );
    return result;
}

//------------------------------------------------------------------------------

GeMeshBuilder::FaceId GeMeshBuilder::addFaces(
    FaceConstIterator const& beginIt,
    FaceConstIterator const& endIt
) {
    FaceId result = _meshPtr->getNbFaces();
    std::vector<GeMesh::Face>::const_iterator realBeginIt, realEndIt;
    realBeginIt = beginIt._wrapper._mesh->_faces.begin()
        + beginIt._wrapper._faceId;
    realEndIt = endIt._wrapper._mesh->_faces.begin()
        + endIt._wrapper._faceId;

    _meshPtr->_faces.insert(
        _meshPtr->_faces.end(),
        realBeginIt,
        realEndIt
    );
    return result;
}

//------------------------------------------------------------------------------

UInt32 GeMeshBuilder::getNbFaces() const
{
    return _meshPtr->getNbFaces();
}

//------------------------------------------------------------------------------

RCShdPtr<GeMesh> GeMeshBuilder::createMesh()
{
    VertexId maxvid = 0;
    TextureVertexId maxtvid = 0;
    FaceConstIterator fi;

    for ( fi = _meshPtr->beginFace(); fi != _meshPtr->endFace(); ++fi ) {
        for ( FaceVertexId fvid = 0; fvid < fi->getNbVertices(); ++fvid ) {
            maxvid = std::max( maxvid, fi->getVertexId( fvid ) );
            maxtvid = std::max( maxtvid, fi->getTextureVertexId( fvid ) );
        }
    }

    DGFX_ASSERT( _meshPtr->getNbFaces() > 0 );
    DGFX_ASSERT( maxvid < _meshPtr->getNbVertices() );
    DGFX_ASSERT(
        ! _meshPtr->hasTexture() || maxtvid < _meshPtr->getNbTextureVertices()
    );

    RCShdPtr<GeMesh> result( _meshPtr );
    // Start a new mesh
    _meshPtr = RCShdPtr<GeMesh>( new GeMesh );
    return result;
}

FREECLOTH_NAMESPACE_END
