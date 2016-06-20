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

#include <freecloth/gfx/gfxGL.h>
#include <freecloth/geom/gePoint.h>
#include <freecloth/geom/geVector.h>
#include <freecloth/geom/geMatrix4.h>
#include <freecloth/colour/colColourRGB.h>

////////////////////////////////////////////////////////////////////////////////
// LOCAL DECLARATIONS

////////////////////////////////////////////////////////////////////////////////
// CLASS GfxGL

//------------------------------------------------------------------------------

void GL::colour( const ColColourRGB& c )
{
    if ( sizeof(Float) == sizeof(GLfloat) ) {
        ::glColor3fv( reinterpret_cast<const GLfloat*>( &c._r ) );
    }
    else {
        ::glColor3dv( reinterpret_cast<const GLdouble*>( &c._r ) );
    }
}

//------------------------------------------------------------------------------

void GL::colour( const ColColourRGB& c, Float alpha )
{
    ::glColor4f( c._r, c._g, c._b, alpha );
}

//------------------------------------------------------------------------------

void GL::fog( const ColColourRGB& c, Float alpha )
{
    GLfloat f[ 4 ] = { c._r, c._g, c._b, alpha };
    ::glFogfv( GL_FOG_COLOR, f );
}

//------------------------------------------------------------------------------

void GL::lightPosition( GLenum light, const GeVector& dir )
{
    const GLfloat data[] = { dir._x, dir._y, dir._z, 0 };
    ::glLightfv( light, GL_POSITION, data );
}

//------------------------------------------------------------------------------

void GL::light( GLenum light, GLenum pname, const ColColourRGB& c, Float alpha )
{
    DGFX_ASSERT(
        pname == GL_AMBIENT || pname == GL_DIFFUSE || pname == GL_SPECULAR
    );
    const Float data[] = { c._r, c._g, c._b, alpha };
    ::glLightfv( light, pname, data );
}

//------------------------------------------------------------------------------

void GL::material(
    GLenum face,
    GLenum pname,
    const ColColourRGB& c,
    Float alpha
) {
    DGFX_ASSERT(
        pname == GL_AMBIENT || pname == GL_DIFFUSE || pname == GL_SPECULAR ||
        pname == GL_EMISSION || pname == GL_SHININESS ||
        pname == GL_AMBIENT_AND_DIFFUSE
    );
    const Float data[] = { c._r, c._g, c._b, alpha };
    ::glMaterialfv( face, pname, data );
}

//------------------------------------------------------------------------------

void GL::multMatrix( const GeMatrix4& m )
{
    if ( sizeof(Float) == sizeof(GLfloat) ) {
        ::glMultMatrixf( reinterpret_cast<const GLfloat*>( m.asColMajor() ) );
    }
    else {
        ::glMultMatrixd( reinterpret_cast<const GLdouble*>( m.asColMajor() ) );
    }
}

//------------------------------------------------------------------------------

void GL::normal( const GeVector& v )
{
    if ( sizeof(Float) == sizeof(GLfloat) ) {
        ::glNormal3fv( reinterpret_cast<const GLfloat*>( &v._x ) );
    }
    else {
        ::glNormal3dv( reinterpret_cast<const GLdouble*>( &v._x ) );
    }
}

//------------------------------------------------------------------------------

void GL::texCoord2( const GePoint& p )
{
    ::glTexCoord2f( p._x, p._y );
}

//------------------------------------------------------------------------------

void GL::translate( const GePoint& p )
{
    ::glTranslatef( p._x, p._y, p._z );
}

//------------------------------------------------------------------------------

void GL::translate( const GeVector& v )
{
    ::glTranslatef( v._x, v._y, v._z );
}

//------------------------------------------------------------------------------

void GL::vertex( const GePoint& p )
{
    if ( sizeof(Float) == sizeof(GLfloat) ) {
        ::glVertex3fv( reinterpret_cast<const GLfloat*>( &p._x ) );
    }
    else {
        ::glVertex3dv( reinterpret_cast<const GLdouble*>( &p._x ) );
    }
}

