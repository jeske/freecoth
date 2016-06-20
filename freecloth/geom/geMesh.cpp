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

#include <freecloth/geom/geMesh.h>
#include <freecloth/geom/geVector.h>

////////////////////////////////////////////////////////////////////////////////
// LOCAL DECLARATIONS

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// CLASS GeMesh

//------------------------------------------------------------------------------

const GeMesh::VertexType* GeMesh::getVertexArray() const
{
    return &_vertices.front();
}

//------------------------------------------------------------------------------

const GeMesh::TextureVertexType* GeMesh::getTextureVertexArray() const
{
    return &_textureVertices.front();
}


////////////////////////////////////////////////////////////////////////////////
// CLASS GeMesh::FaceWrapper
//

//------------------------------------------------------------------------------

Float GeMesh::FaceWrapper::calcArea() const
{
    GeVector v1( getVertex( 0 ), getVertex( 1 ) );
    GeVector v2( getVertex( 1 ), getVertex( 2 ) );
    return v1.cross( v2 ).length() / 2;
}

//------------------------------------------------------------------------------

GeVector GeMesh::FaceWrapper::calcNonUnitNormal() const
{
    GeVector v1( getVertex( 0 ), getVertex( 1 ) );
    GeVector v2( getVertex( 1 ), getVertex( 2 ) );
    return v1.cross( v2 );
}

//------------------------------------------------------------------------------

GeVector GeMesh::FaceWrapper::calcNormal() const
{
    return calcNonUnitNormal().getUnit();
}

//------------------------------------------------------------------------------

GePoint GeMesh::FaceWrapper::calcBarycentric( const GePoint& p ) const
{
    return calcBarycentric( p, getVertex( 0 ), getVertex( 1 ), getVertex( 2 ) );
}

//------------------------------------------------------------------------------

GePoint GeMesh::FaceWrapper::calcBarycentric(
    const GePoint& p,
    const GePoint& v1,
    const GePoint& v2,
    const GePoint& v3
) {
    Float denom =
        ( v2._x - v1._x ) * ( v3._y - v1._y ) -
        ( v3._x - v1._x ) * ( v2._y - v1._y );
    // If this fails, the triangle is zero-area.
    //DGFX_ASSERT( ! BaMath::isEqual( denom, 0 ) );

    return GePoint(
        ( ( v2._x - p._x ) * ( v3._y - p._y ) -
          ( v3._x - p._x ) * ( v2._y - p._y ) ) / denom,
        ( ( v3._x - p._x ) * ( v1._y - p._y ) -
          ( v1._x - p._x ) * ( v3._y - p._y ) ) / denom,
        ( ( v1._x - p._x ) * ( v2._y - p._y ) -
          ( v2._x - p._x ) * ( v1._y - p._y ) ) / denom
    );
}


////////////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTIONS
//

//------------------------------------------------------------------------------

std::ostream& operator<<( std::ostream& os, const GeMesh& mesh )
{
    os << "GeMesh {" << std::endl;

    GeMesh::VertexConstIterator vi;
    UInt32 i;

    os << "vertices = " << std::endl;
    for ( vi = mesh.beginVertex(), i = 0; vi != mesh.endVertex(); ++vi, ++i ) {
        os << "[" << i << "] " << *vi << std::endl;
    }

    GeMesh::TextureVertexConstIterator tvi;
    os << "texture vertices = " << std::endl;
    for (
        tvi = mesh.beginTextureVertex(), i = 0;
        tvi != mesh.endTextureVertex();
        ++tvi, ++i
    ) {
        os << "[" << i << "] " << *tvi << std::endl;
    }

    GeMesh::FaceConstIterator fi;
    os << "faces = " << std::endl;
    for ( fi = mesh.beginFace(), i = 0; fi != mesh.endFace(); ++fi, ++i ) {
        os << "[" << i << "] " << *fi << std::endl;
    }
    os << "}";
    return os;
}

//------------------------------------------------------------------------------

std::ostream& operator<<( std::ostream& os, const GeMesh::FaceWrapper& face )
{
    os << "{ vids={ "
        << face.getVertexId( 0 ) << ","
        << face.getVertexId( 1 ) << ","
        << face.getVertexId( 2 )
        << " }, tvids={ "
        << face.getTextureVertexId( 0 ) << ","
        << face.getTextureVertexId( 1 ) << ","
        << face.getTextureVertexId( 2 )
        << " } }";
    return os;
}

FREECLOTH_NAMESPACE_END
