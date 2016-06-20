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

#ifndef freecloth_geom_geMesh_inline_h
#define freecloth_geom_geMesh_inline_h

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// CLASS GeMesh
//

//------------------------------------------------------------------------------

inline GeMesh::GeMesh()
{
}

//------------------------------------------------------------------------------

inline UInt32 GeMesh::getNbFaces() const
{
    return _faces.size();
}

//------------------------------------------------------------------------------

inline GeMesh::FaceType GeMesh::getFace( FaceId faceId) const
{
    return FaceWrapper( *this, faceId );
}

//------------------------------------------------------------------------------

inline GeMesh::FaceConstIterator GeMesh::beginFace() const
{
    return FaceConstIterator::begin( *this );
}

//------------------------------------------------------------------------------

inline GeMesh::FaceConstIterator GeMesh::endFace() const
{
    return FaceConstIterator::end( *this );
}

//------------------------------------------------------------------------------

inline UInt32 GeMesh::getNbVertices() const
{
    return _vertices.size();
}

//------------------------------------------------------------------------------

inline GeMesh::VertexType& GeMesh::getVertex( VertexId vertexId )
{
    DGFX_ASSERT( vertexId < getNbVertices() );
    return _vertices[ vertexId ];
}

//------------------------------------------------------------------------------

inline const GeMesh::VertexType& GeMesh::getVertex( VertexId vertexId ) const
{
    DGFX_ASSERT( vertexId < getNbVertices() );
    return _vertices[ vertexId ];
}

//------------------------------------------------------------------------------

inline GeMesh::VertexIterator GeMesh::beginVertex()
{
    return _vertices.begin();
}

//------------------------------------------------------------------------------

inline GeMesh::VertexIterator GeMesh::endVertex()
{
    return _vertices.end();
}

//------------------------------------------------------------------------------

inline GeMesh::VertexConstIterator GeMesh::beginVertex() const
{
    return _vertices.begin();
}

//------------------------------------------------------------------------------

inline GeMesh::VertexConstIterator GeMesh::endVertex() const
{
    return _vertices.end();
}

//------------------------------------------------------------------------------

inline bool GeMesh::hasTexture() const
{
    return getNbTextureVertices() > 0;
}

//------------------------------------------------------------------------------

inline UInt32 GeMesh::getNbTextureVertices() const
{
    return _textureVertices.size();
}

//------------------------------------------------------------------------------

inline GeMesh::TextureVertexType& GeMesh::getTextureVertex(
    TextureVertexId textureVertexId
) {
    DGFX_ASSERT( textureVertexId < getNbTextureVertices() );
    return _textureVertices[ textureVertexId ];
}

//------------------------------------------------------------------------------

inline const GeMesh::TextureVertexType& GeMesh::getTextureVertex(
    TextureVertexId textureVertexId
) const {
    DGFX_ASSERT( textureVertexId < getNbTextureVertices() );
    return _textureVertices[ textureVertexId ];
}

//------------------------------------------------------------------------------

inline GeMesh::TextureVertexIterator GeMesh::beginTextureVertex()
{
    return _textureVertices.begin();
}

//------------------------------------------------------------------------------

inline GeMesh::TextureVertexIterator GeMesh::endTextureVertex()
{
    return _textureVertices.end();
}

//------------------------------------------------------------------------------

inline GeMesh::TextureVertexConstIterator GeMesh::beginTextureVertex() const
{
    return _textureVertices.begin();
}

//------------------------------------------------------------------------------

inline GeMesh::TextureVertexConstIterator GeMesh::endTextureVertex() const
{
    return _textureVertices.end();
}



////////////////////////////////////////////////////////////////////////////////
// CLASS GeMesh::Face
//

//------------------------------------------------------------------------------

inline GeMesh::Face::Face(
    VertexId vid1, VertexId vid2, VertexId vid3,
    TextureVertexId tvid1, TextureVertexId tvid2, TextureVertexId tvid3
) {
    _vertexIds[ 0 ] = vid1;
    _vertexIds[ 1 ] = vid2;
    _vertexIds[ 2 ] = vid3;
    _textureVertexIds[ 0 ] = tvid1;
    _textureVertexIds[ 1 ] = tvid2;
    _textureVertexIds[ 2 ] = tvid3;
}

//------------------------------------------------------------------------------

inline UInt32 GeMesh::Face::getNbVertices() const
{
    return NB_VERTICES;
}

//------------------------------------------------------------------------------

inline GeMeshTypes::VertexId GeMesh::Face::getVertexId(
    FaceVertexId faceVertexId
) const {
    DGFX_ASSERT( faceVertexId < NB_VERTICES );
    return _vertexIds[ faceVertexId ];
}

//------------------------------------------------------------------------------

inline GeMeshTypes::TextureVertexId GeMesh::Face::getTextureVertexId(
    FaceVertexId faceVertexId
) const {
    DGFX_ASSERT( faceVertexId < NB_VERTICES );
    return _textureVertexIds[ faceVertexId ];
}



////////////////////////////////////////////////////////////////////////////////
// CLASS GeMesh::FaceWrapper
//

//------------------------------------------------------------------------------

inline GeMesh::FaceWrapper::FaceWrapper()
    : _mesh( 0 ), _faceId( 0 )
{
}

//------------------------------------------------------------------------------

inline GeMesh::FaceWrapper::FaceWrapper(
    const GeMesh& mesh,
    GeMeshTypes::FaceId faceId
) : _mesh( &mesh ),
    _faceId( faceId )
{
    DGFX_ASSERT( isValid() );
}

//------------------------------------------------------------------------------

inline GeMesh::FaceWrapper::FaceWrapper(
    const GeMesh* mesh,
    GeMeshTypes::FaceId faceId
) : _mesh( mesh ),
    _faceId( faceId )
{
    // Pointer variant is private and unchecked - allows efficient construction
    // of invalid wrappers, i.e. for end() iterator.
}

//------------------------------------------------------------------------------

inline GeMesh::FaceId GeMesh::FaceWrapper::getFaceId() const
{
    return _faceId;
}

//------------------------------------------------------------------------------

inline UInt32 GeMesh::FaceWrapper::getNbVertices() const
{
    return _mesh->_faces[ _faceId ].getNbVertices();
}

//------------------------------------------------------------------------------

inline GeMeshTypes::VertexId GeMesh::FaceWrapper::getVertexId(
    FaceVertexId faceVertexId
) const {
    return _mesh->_faces[ _faceId ].getVertexId( faceVertexId );
}

//------------------------------------------------------------------------------

inline const GeMeshTypes::VertexType& GeMesh::FaceWrapper::getVertex(
    FaceVertexId faceVertexId
) const {
    return _mesh->getVertex( getVertexId( faceVertexId ) );
}

//------------------------------------------------------------------------------

inline GeMeshTypes::TextureVertexId GeMesh::FaceWrapper::getTextureVertexId(
    FaceVertexId faceVertexId
) const {
    return _mesh->_faces[ _faceId ].getTextureVertexId( faceVertexId );
}

//------------------------------------------------------------------------------

inline const GeMeshTypes::TextureVertexType&
GeMesh::FaceWrapper::getTextureVertex(
    FaceVertexId faceVertexId
) const {
    return _mesh->getTextureVertex( getTextureVertexId( faceVertexId ) );
}

//------------------------------------------------------------------------------

inline bool GeMesh::FaceWrapper::isValid() const
{
    return _mesh != 0 && _faceId < _mesh->getNbFaces();
}


////////////////////////////////////////////////////////////////////////////////
// CLASS GeMesh::FaceConstIterator
//

//------------------------------------------------------------------------------

inline GeMesh::FaceConstIterator::FaceConstIterator()
{
}

//------------------------------------------------------------------------------

inline GeMesh::FaceConstIterator::FaceConstIterator(
    const GeMesh& mesh,
    GeMeshTypes::FaceId faceId
) : _wrapper( mesh, faceId )
{
}

//------------------------------------------------------------------------------

inline GeMesh::FaceConstIterator::FaceConstIterator(
    const FaceWrapper& wrapper
) : _wrapper( wrapper )
{
}

//------------------------------------------------------------------------------

inline GeMesh::FaceConstIterator GeMesh::FaceConstIterator::begin(
    const GeMesh& mesh
) {
    return FaceConstIterator( mesh, 0 );
}

//------------------------------------------------------------------------------

inline GeMesh::FaceConstIterator GeMesh::FaceConstIterator::end(
    const GeMesh& mesh
) {
    // Pointer variant of constructor is unchecked.
    return FaceConstIterator( FaceWrapper( &mesh, mesh.getNbFaces() ) );
}

//------------------------------------------------------------------------------

inline GeMesh::FaceConstIterator& GeMesh::FaceConstIterator::operator++()
{
    ++_wrapper._faceId;
    return *this;
}

//------------------------------------------------------------------------------

inline GeMesh::FaceConstIterator GeMesh::FaceConstIterator::operator++( int )
{
    FaceConstIterator copy( *this );
    ++_wrapper._faceId;
    return copy;
}

//------------------------------------------------------------------------------

inline GeMesh::FaceConstIterator& GeMesh::FaceConstIterator::operator--()
{
    --_wrapper._faceId;
    return *this;
}

//------------------------------------------------------------------------------

inline GeMesh::FaceConstIterator GeMesh::FaceConstIterator::operator--( int )
{
    FaceConstIterator copy( *this );
    --_wrapper._faceId;
    return copy;
}

//------------------------------------------------------------------------------

inline bool GeMesh::FaceConstIterator::operator == (
    const FaceConstIterator& rhs
) const {
    return ( _wrapper._mesh == rhs._wrapper._mesh ) &&
        ( _wrapper._faceId == rhs._wrapper._faceId );
}

//------------------------------------------------------------------------------

inline bool GeMesh::FaceConstIterator::operator != (
    const FaceConstIterator& rhs
) const {
    return !operator == ( rhs );
}

//------------------------------------------------------------------------------

inline const GeMesh::FaceWrapper& GeMesh::FaceConstIterator::operator*() const
{
    DGFX_ASSERT( _wrapper.isValid() );
    return _wrapper;
}

//------------------------------------------------------------------------------

inline const GeMesh::FaceWrapper* GeMesh::FaceConstIterator::operator->() const
{
    DGFX_ASSERT( _wrapper.isValid() );
    return &_wrapper;
}

FREECLOTH_NAMESPACE_END

#endif
