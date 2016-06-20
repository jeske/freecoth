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

#include <freecloth/gfx/gfxGLWindowGLUT.h>
#include <freecloth/gfx/gfxConfig.h>
#include <freecloth/resmgt/rcShdPtr.h>
#include <freecloth/base/GL_glut.h>

////////////////////////////////////////////////////////////////////////////////
// CLASS GfxGLWindowGLUT

GfxGLWindowGLUT::WindowMap GfxGLWindowGLUT::_windowMap;
UInt32 GfxGLWindowGLUT::_nbIdleRegistered = 0;

//------------------------------------------------------------------------------

GfxGLWindowGLUT::GfxGLWindowGLUT(
    const GfxConfig& config,
    const String& title
) : _idleEvents( false ),
    _closeReceived( false )
{
    // FIXME: need some way of getting argc, argv to glutInit
    ::glutInitDisplayMode(
        GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE
    );

    ::glutInitWindowSize( 
        config.getWindowInfo()._width, config.getWindowInfo()._height
    );
    _width = config.getWindowInfo()._width;
    _height = config.getWindowInfo()._height;
    ::glutInitWindowPosition( 
        config.getWindowInfo()._x, config.getWindowInfo()._y
    );

    _windowId = ::glutCreateWindow( title.c_str() );
    _windowMap[ _windowId ] = this;

    // FIXME: need a way to select the video mode for fullscreen...
    if ( config.getFullscreen() ) {
        ::glutFullScreen();
    }
}

//------------------------------------------------------------------------------

GfxGLWindowGLUT::~GfxGLWindowGLUT()
{
    DGFX_ASSERT( _closeReceived );
    if ( _idleEvents && --_nbIdleRegistered == 0 ) {
        unregisterIdleCB();
    }
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUT::enableIdleEvents( bool idleEvents )
{
    if ( idleEvents == _idleEvents ) {
        return;
    }
    _idleEvents = idleEvents;
    if ( _idleEvents && _nbIdleRegistered++ == 0 ) {
        registerIdleCB();
    }
    else if ( ! _idleEvents && --_nbIdleRegistered == 0 ) {
        unregisterIdleCB();
    }
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUT::close()
{
    // This will be handled later
    // FIXME: does it really *need* to be handled later?
    _closeReceived = true;
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUT::handleEvents()
{
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUT::eventLoop()
{
    registerCBs();
    ::glutMainLoop();
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUT::swapBuffers()
{
    ::glutSwapBuffers();
}

//------------------------------------------------------------------------------

UInt32 GfxGLWindowGLUT::getWidth() const
{
    return _width;
}

//------------------------------------------------------------------------------

UInt32 GfxGLWindowGLUT::getHeight() const
{
    return _height;
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUT::postRedisplay()
{
    ::glutSetWindow( _windowId );
    ::glutPostRedisplay();
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUT::registerIdleCB()
{
    ::glutIdleFunc( idleCB );
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUT::unregisterIdleCB()
{
    ::glutIdleFunc( 0 );
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUT::registerCBs()
{
    ::glutDisplayFunc( displayCB );
    ::glutKeyboardFunc( keyboardCB );
    ::glutMouseFunc( mouseCB );
    ::glutMotionFunc( motionCB );
    ::glutPassiveMotionFunc( passiveMotionCB );
    ::glutSpecialFunc( specialCB );
    ::glutReshapeFunc( reshapeCB );
    // Idle is registered later, to allow a virtual method
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUT::closeReceived()
{
    BaseClass::closeReceived();
    if ( _windowId >= 0 ) {
        _windowMap.erase( _windowId );
        ::glutDestroyWindow( _windowId );
        _windowId = -1;
    }
}

//------------------------------------------------------------------------------

GfxGLWindowGLUT& GfxGLWindowGLUT::getWindow()
{
    WindowMap::iterator wi = _windowMap.find( ::glutGetWindow() );
    DGFX_ASSERT( wi != _windowMap.end() );
    return *(wi->second);
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUT::displayCB()
{
    GfxGLWindowGLUT& window = getWindow();
    window.displayReceived();
    window.handleMiscEvents();
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUT::idleCB()
{
    WindowMap::iterator wi;
    UInt32 size = _windowMap.size();
    for ( wi = _windowMap.begin(); wi != _windowMap.end(); ++wi ) {
        wi->second->idleReceived();
        wi->second->handleMiscEvents();
        if ( size != _windowMap.size() ) {
            // If the size changes, the iterators are invalid and bad
            // things we likely happen.
            return;
        }
    }
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUT::keyboardCB( unsigned char key, int x, int y )
{
    GfxGLWindowGLUT& window = getWindow();

    KeyID keyId = translateKey( key );
    ModBitfield mods = getModifiers();

    // FIXME: what does GLUT's key CB correspond to? keydown, or 
    // key press?
    //window.keyDownReceived( keyId, mods );
    window.keyPressed( keyId, mods );
    window.handleMiscEvents();
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUT::mouseCB( int button, int state, int x, int y )
{
    GfxGLWindowGLUT& window = getWindow();

    ButtonID buttonId = translateButton( button );
    if ( buttonId == MBN_NONE ) {
        return;
    }
    if ( state == GLUT_DOWN ) {
        window.mouseDownReceived( x, y, buttonId );
    }
    else {
        DGFX_ASSERT( state == GLUT_UP );
        window.mouseUpReceived( x, y, buttonId );
    }
    window.handleMiscEvents();
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUT::motionCB( int x, int y )
{
    GfxGLWindowGLUT& window = getWindow();
    window.mouseMoveReceived( x, y );
    window.handleMiscEvents();
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUT::passiveMotionCB( int x, int y )
{
    GfxGLWindowGLUT& window = getWindow();
    window.mouseMoveReceived( x, y );
    window.handleMiscEvents();
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUT::specialCB( int key, int x, int y )
{
    GfxGLWindowGLUT& window = getWindow();
    // FIXME: add extended key handling
    window.handleMiscEvents();
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUT::reshapeCB( int width, int height )
{
    GfxGLWindowGLUT& window = getWindow();
    window.windowResized( 0, 0, width, height );
    window.handleMiscEvents();
    window._width = width;
    window._height = height;
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUT::handleMiscEvents()
{
    if ( _closeReceived ) {
        closeReceived();
    }
}

//------------------------------------------------------------------------------

GfxWindow::KeyID GfxGLWindowGLUT::translateKey(
    unsigned char key
) {
    // As it happens, our key map is *exactly* like ASCII... funny how that
    // accidentally happens.
    return static_cast<KeyID>( key );
}

//------------------------------------------------------------------------------

GfxWindow::ModBitfield GfxGLWindowGLUT::getModifiers()
{
    ModBitfield result = 0;
    int mod = ::glutGetModifiers();
    // FIXME: GLUT counts capslock as shift...
    if ( mod & GLUT_ACTIVE_SHIFT ) {
        result |= KBM_SHIFT;
    }
    if ( mod & GLUT_ACTIVE_CTRL ) {
        result |= KBM_CONTROL;
    }
    if ( mod & GLUT_ACTIVE_ALT ) {
        result |= KBM_ALT;
    }
    return result;
}

//------------------------------------------------------------------------------

GfxWindow::ButtonID GfxGLWindowGLUT::translateButton(
    Int32 button
) {
    switch( button ) {
        case GLUT_LEFT_BUTTON:
            return MBN_LEFT;
        case GLUT_MIDDLE_BUTTON:
            return MBN_MIDDLE;
        case GLUT_RIGHT_BUTTON:
            return MBN_RIGHT;
    }
    return MBN_NONE;
}
