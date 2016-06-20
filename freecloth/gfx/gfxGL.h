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

#ifndef freecloth_gfx_gfxGL_h
#define freecloth_gfx_gfxGL_h

#ifndef freecloth_gfx_package_h
#include <freecloth/gfx/package.h>
#endif

#ifndef freecloth_base_GL_gl_h
#include <freecloth/base/GL_gl.h>
#endif

////////////////////////////////////////////////////////////////////////////////
// FORWARD DECLARATIONS

class ColColourRGB;

FREECLOTH_NAMESPACE_START
    class GePoint;
    class GeVector;
    class GeMatrix4;
FREECLOTH_NAMESPACE_END

////////////////////////////////////////////////////////////////////////////////
/*!
 * \class GL freecloth/gfx/gfxGL.h
 * \brief OpenGL bindings for our utility classes.
 *
 * Calls to standard OpenGL functions (\p glColor, \p glVertex, etc.) using
 * our utility classes (\p ColColourRGB, \p GePoint, etc.)
 */
class GL {

public:
    // ----- static member functions -----

    static void colour( const ColColourRGB& );
    static void colour( const ColColourRGB&, Float alpha );
    static void fog( const ColColourRGB&, Float alpha = 0.f );
    static void multMatrix( const GeMatrix4& );
    static void normal( const GeVector& );
    static void lightPosition( GLenum light, const GeVector& );
    static void light(
        GLenum light,
        GLenum pname,
        const ColColourRGB&,
        Float alpha = 1
    );
    static void material(
        GLenum face,
        GLenum pname,
        const ColColourRGB&,
        Float alpha = 1.f
    );
    static void texCoord2( const GePoint& );
    static void translate( const GeVector& );
    static void translate( const GePoint& );
    static void vertex( const GePoint& );
};
#endif
