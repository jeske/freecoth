//////////////////////////////////////////////////////////////////////
// Copyright (c) 2002 David Pritchard <drpritch@alumni.uwaterloo.ca>
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

#ifndef freecloth_gfx_gfxGLWindowGLUT_h
#define freecloth_gfx_gfxGLWindowGLUT_h

#ifndef freecloth_gfx_package_h
#include <freecloth/gfx/package.h>
#endif

#ifndef freecloth_gfx_gfxGLWindow_h
#include <freecloth/gfx/gfxGLWindow.h>
#endif

#ifndef freecloth_base_map
#include <freecloth/base/map>
#endif

////////////////////////////////////////////////////////////////////////////////
// FORWARD DECLARATIONS

class GfxConfig;

////////////////////////////////////////////////////////////////////////////////
/*!
 * \class GfxGLWindowGLUT freecloth/gfx/gfxGLWindowGLUT.h
 * \brief Window class for GLUT
 *
 */
class GfxGLWindowGLUT : public GfxGLWindow
{
public:

    //----- types and enumerations -----
    typedef GfxGLWindow BaseClass;

    //----- member functions -----

    GfxGLWindowGLUT(
        const GfxConfig&,
        const String& title
    );
    ~GfxGLWindowGLUT();

    void enableIdleEvents( bool );

    void close();
    //! Run events, forever
    void eventLoop();
    //! Can't be implemented using GLUT...
    void handleEvents();

    UInt32 getWidth() const;
    UInt32 getHeight() const;
    void swapBuffers();
    void postRedisplay();
    
protected:

    //----- static member functions -----

    static KeyID translateKey( unsigned char key );
    static ModBitfield getModifiers();
    static ButtonID translateButton( Int32 button );

    //! Retrieve the current GLUT window
    static GfxGLWindowGLUT& getWindow();

    //! Protected, to allow GLUI to access it.
    static void idleCB();
    static void displayCB();
    static void keyboardCB( unsigned char key, int x, int y );
    static void mouseCB( int button, int state, int x, int y );
    static void motionCB( int x, int y );
    static void passiveMotionCB( int x, int y );
    static void specialCB( int key, int x, int y );
    static void reshapeCB( int w, int h );

    //----- member functions -----

    //! Virtual, to allow GLUI to override.
    virtual void registerIdleCB();
    virtual void unregisterIdleCB();
    virtual void registerCBs();
    //! See base.
    virtual void closeReceived();

    //! GLUT doesn't support some important useful events, such as closing
    //! a window. This allows us to handle these events.
    void handleMiscEvents();

    //----- data members -----
    Int32   _windowId;
    bool    _idleEvents;
    UInt32 _width;
    UInt32 _height;

private:
    
    //----- types and enumerations -----

    typedef std::map<Int32,GfxGLWindowGLUT*> WindowMap;

    //----- member functions -----

    //! Disallowed.
    GfxGLWindowGLUT( const GfxGLWindowGLUT& );
    //! Disallowed.
    GfxGLWindowGLUT& operator = ( const GfxGLWindowGLUT& );

    //----- static data members -----

    //! Map from window ids to GfxGLWindowGLUT pointers.
    static WindowMap _windowMap;
    static UInt32 _nbIdleRegistered;

    //----- data members -----

    bool    _closeReceived;
};

#endif
