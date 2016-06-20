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

#ifndef freecloth_gfx_gfxWindowObserver_h
#define freecloth_gfx_gfxWindowObserver_h

#ifndef freecloth_gfx_package_h
#include <freecloth/gfx/package.h>
#endif

#ifndef freecloth_gfx_gfxWindow_h
#include <freecloth/gfx/gfxWindow.h>
#endif

////////////////////////////////////////////////////////////////////////////////
// FORWARD DECLARATIONS

////////////////////////////////////////////////////////////////////////////////
/*!
 * \class GfxWindowObserver freecloth/gfx/gfxWindowObserver.h
 * \brief Interface for classes that are interested in events coming from
 * a window.
 *
 * \pattern Observer
 */
class GfxWindowObserver {

public:

    //----- member functions -----

    virtual ~GfxWindowObserver();

    virtual void idleReceived();

    //! These two are only sent to actual observers, not static observers.
    //! An observer may only be removed during one of these two methods, our
    //! outside a *received call.
    virtual void destroyed( GfxWindow& );
    virtual void closeReceived( GfxWindow& );

    virtual void displayReceived( GfxWindow& );
    virtual void keyPressed(
        GfxWindow&,
        GfxWindow::KeyID,
        GfxWindow::ModBitfield
    );
    virtual void keyDownReceived(
        GfxWindow&,
        GfxWindow::KeyID,
        GfxWindow::ModBitfield
    );
    virtual void keyUpReceived(
        GfxWindow&,
        GfxWindow::KeyID,
        GfxWindow::ModBitfield
    );
    virtual void mouseDownReceived(
        GfxWindow&,
        UInt32 x,
        UInt32 y,
        GfxWindow::ButtonID
    );
    virtual void mouseUpReceived(
        GfxWindow&,
        UInt32 x,
        UInt32 y,
        GfxWindow::ButtonID
    );
    virtual void mouseMoveReceived(
        GfxWindow&,
        UInt32 x,
        UInt32 y
    );
    virtual void windowResized(
        GfxWindow&,
        UInt32 viewportX, UInt32 viewportY,
        UInt32 viewportWidth, UInt32 viewportHeight
    );

    //! GLUI user interface event.
    // FIXME: need a better way of defining events (e.g. double-dispatch),
    // so that GfxWindow and GfxGLWindowGLUI events don't need to be mixed up.
    virtual void uiReceived(
        GfxWindow&, UInt32 uid
    );

};
#endif
