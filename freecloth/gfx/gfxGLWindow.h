//////////////////////////////////////////////////////////////////////
// Copyright (c) 2001-2002 David Pritchard <drpritch@alumni.uwaterloo.ca>
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

#ifndef freecloth_gfx_gfxGLWindow_h
#define freecloth_gfx_gfxGLWindow_h

#ifndef freecloth_gfx_package_h
#include <freecloth/gfx/package.h>
#endif

#ifndef freecloth_gfx_gfxWindow_h
#include <freecloth/gfx/gfxWindow.h>
#endif

////////////////////////////////////////////////////////////////////////////////
// FORWARD DECLARATIONS

class GfxConfig;

FREECLOTH_NAMESPACE_START
    template <class T> class RCShdPtr;
FREECLOTH_NAMESPACE_END

////////////////////////////////////////////////////////////////////////////////
/*!
 * \class GfxGLWindow freecloth/gfx/gfxGLWindow.h
 * \brief Window for OpenGL operations.
 *
 * This abstract base class defines an interface for implementation of an
 * OpenGL window. It is assumed that all operations will take place within a
 * single thread.
 */
class GfxGLWindow : public GfxWindow
{
public:

    //----- static member functions -----

    static RCShdPtr<GfxGLWindow> create(
        const GfxConfig&,
        const String& title
    );

    //----- member functions -----

    virtual ~GfxGLWindow();

    // FIXME: need a more robust way of handling events.
    // Must achieve:
    // 1) portable
    // 2) works with plugin-type setups (xmms, winamp) where glutMainLoop()
    //    type calls may not be feasible
    // 3) avoid "handleEvents()" type approaches, which are error-prone...
    virtual void handleEvents() = 0;
    virtual void swapBuffers() = 0;
    virtual void eventLoop() = 0;

protected:

    //----- member functions -----

    GfxGLWindow();

private:

};
#endif
