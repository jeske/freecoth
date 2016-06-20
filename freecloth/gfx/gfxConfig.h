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

#ifndef freecloth_gfx_gfxConfig_h
#define freecloth_gfx_gfxConfig_h

#ifndef freecloth_gfx_package_h
#include <freecloth/gfx/package.h>
#endif

#ifndef freecloth_resmgt_resConfig_h
#include <freecloth/resmgt/resConfig.h>
#endif

////////////////////////////////////////////////////////////////////////////////
// FORWARD DECLARATIONS

////////////////////////////////////////////////////////////////////////////////
/*!
 * \class GfxConfig freecloth/gfx/gfxConfig.h
 * \brief Configuration information for graphics applications.
 *
 * Includes window size and position, or fullscreen video mode information.
 */
class GfxConfig : public ResConfig
{
public:
    //----- classes -----
    /*!
     * \class WindowInfo freecloth/gfx/gfxConfig.h
     * \brief Graphics configuration for windowed applications
     */
    class WindowInfo
    {
    public:
        //----- data members -----
        UInt32 _width;
        UInt32 _height;
        UInt32 _x;
        UInt32 _y;
    };

    /*!
     * \class VideoModeInfo freecloth/gfx/gfxConfig.h
     * \brief Graphics configuration for fullscreen applications
     */
    class VideoModeInfo
    {
    public:
        //----- data members -----
        UInt32 _width;
        UInt32 _height;
        UInt32 _bitsPerPixel;
    };

    //----- member functions -----

    GfxConfig();

    // See base.
    virtual void load( ResConfigRegistryR& reg );
    virtual void save( ResConfigRegistryW& reg ) const;
    // FIXME: contravariant return type.
    virtual RCShdPtr<ResConfig> clone() const;

    //@{
    //! Accessor
    const WindowInfo& getWindowInfo() const;
    const VideoModeInfo& getVideoModeInfo() const;
    bool getFullscreen() const;
    //@}

    //@{
    //! Mutator
    void setWindowInfo( const WindowInfo& );
    void setVideoModeInfo( const VideoModeInfo& );
    void setFullscreen( bool );
    //@}

protected:
    //----- member functions -----

    GfxConfig( const GfxConfig& );

private:
    //----- member functions -----

    // Disallowed.
    GfxConfig& operator=( const GfxConfig& );

    //----- data members -----

    WindowInfo      _windowInfo;
    VideoModeInfo   _videoModeInfo;
    bool            _fullscreen;
};
#endif
