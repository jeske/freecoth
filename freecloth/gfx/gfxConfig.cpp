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

#include <freecloth/gfx/gfxConfig.h>
#include <freecloth/resmgt/resConfigRegistry.h>
#include <freecloth/resmgt/rcShdPtr.h>

////////////////////////////////////////////////////////////////////////////////
// LOCAL DECLARATIONS

namespace {
    const char* WINDOW_WIDTH_NAME = "WindowWidth";
    const char* WINDOW_HEIGHT_NAME = "WindowHeight";
    const char* WINDOW_X_NAME = "WindowX";
    const char* WINDOW_Y_NAME = "WindowY";
    const char* VIDEO_MODE_WIDTH_NAME = "VideoModeWidth";
    const char* VIDEO_MODE_HEIGHT_NAME = "VideoModeHeight";
    const char* VIDEO_MODE_BITS_PER_PIXEL_NAME = "VideoModeBitsPerPixel";
    const char* FULLSCREEN_NAME = "Fullscreen";

    const GfxConfig::WindowInfo WINDOW_INFO_DEFAULT =
    { 640, 480, 0, 0 };
    const GfxConfig::VideoModeInfo VIDEO_MODE_INFO_DEFAULT =
    { 640, 480, 32 };
    bool FULLSCREEN_DEFAULT = false;
};

////////////////////////////////////////////////////////////////////////////////
// CLASS GfxConfig

//------------------------------------------------------------------------------

GfxConfig::GfxConfig()
  : _windowInfo( WINDOW_INFO_DEFAULT ),
    _videoModeInfo( VIDEO_MODE_INFO_DEFAULT ),
    _fullscreen( FULLSCREEN_DEFAULT )
{
}

//------------------------------------------------------------------------------

GfxConfig::GfxConfig( const GfxConfig& rhs )
  : _windowInfo( rhs._windowInfo ),
    _videoModeInfo( rhs._videoModeInfo ),
    _fullscreen( rhs._fullscreen )
{
}

//------------------------------------------------------------------------------

RCShdPtr<ResConfig> GfxConfig::clone() const
{
    return RCShdPtr<ResConfig>( new GfxConfig( *this ) );
}

//------------------------------------------------------------------------------

const GfxConfig::WindowInfo& GfxConfig::getWindowInfo() const
{
    return _windowInfo;
}

//------------------------------------------------------------------------------

const GfxConfig::VideoModeInfo& GfxConfig::getVideoModeInfo() const
{
    return _videoModeInfo;
}

//------------------------------------------------------------------------------

bool GfxConfig::getFullscreen() const
{
    return _fullscreen;
}

//------------------------------------------------------------------------------

void GfxConfig::setWindowInfo( const WindowInfo& value )
{
    _windowInfo = value;
}

//------------------------------------------------------------------------------

void GfxConfig::setVideoModeInfo( const VideoModeInfo& value )
{
    // TODO: verify
    _videoModeInfo = value;
}

//------------------------------------------------------------------------------

void GfxConfig::setFullscreen( bool value )
{
    _fullscreen = value;
}

//------------------------------------------------------------------------------

void GfxConfig::load( ResConfigRegistryR& reg )
{
    WindowInfo windowInfo;
    VideoModeInfo videoModeInfo;

    windowInfo._width =
        reg.readUInt32( WINDOW_WIDTH_NAME, WINDOW_INFO_DEFAULT._width );
    windowInfo._height =
        reg.readUInt32( WINDOW_HEIGHT_NAME, WINDOW_INFO_DEFAULT._height );
    windowInfo._x =
        reg.readUInt32( WINDOW_X_NAME, WINDOW_INFO_DEFAULT._x );
    windowInfo._y =
        reg.readUInt32( WINDOW_Y_NAME, WINDOW_INFO_DEFAULT._y );
    setWindowInfo( windowInfo );

    videoModeInfo._width = reg.readUInt32(
        VIDEO_MODE_WIDTH_NAME,
        VIDEO_MODE_INFO_DEFAULT._width
    );
    videoModeInfo._height = reg.readUInt32(
        VIDEO_MODE_HEIGHT_NAME,
        VIDEO_MODE_INFO_DEFAULT._height
    );
    videoModeInfo._bitsPerPixel = reg.readUInt32(
        VIDEO_MODE_BITS_PER_PIXEL_NAME,
        VIDEO_MODE_INFO_DEFAULT._bitsPerPixel
    );
    setVideoModeInfo( videoModeInfo );

    setFullscreen( reg.readBool( FULLSCREEN_NAME, FULLSCREEN_DEFAULT) );
}

//------------------------------------------------------------------------------

void GfxConfig::save( ResConfigRegistryW& reg ) const
{
    reg.writeUInt32( WINDOW_WIDTH_NAME, _windowInfo._width );
    reg.writeUInt32( WINDOW_HEIGHT_NAME, _windowInfo._height );
    reg.writeUInt32( WINDOW_X_NAME, _windowInfo._x );
    reg.writeUInt32( WINDOW_Y_NAME, _windowInfo._y );
    reg.writeUInt32( VIDEO_MODE_WIDTH_NAME, _videoModeInfo._width );
    reg.writeUInt32( VIDEO_MODE_HEIGHT_NAME, _videoModeInfo._height );
    reg.writeUInt32(
        VIDEO_MODE_BITS_PER_PIXEL_NAME,
        _videoModeInfo._bitsPerPixel
    );
    reg.writeBool( FULLSCREEN_NAME, _fullscreen );
}
