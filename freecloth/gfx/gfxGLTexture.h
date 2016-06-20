//////////////////////////////////////////////////////////////////////
// Copyright (c) 2001-2003 David Pritchard <drpritch@alumni.uwaterloo.ca>
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

#ifndef freecloth_gfx_gfxGLTexture_h
#define freecloth_gfx_gfxGLTexture_h

#ifndef freecloth_gfx_package_h
#include <freecloth/gfx/package.h>
#endif

#ifndef freecloth_resmgt_rcShdPtr_h
#include <freecloth/resmgt/rcShdPtr.h>
#endif

#ifndef freecloth_resmgt_rcBase_h
#include <freecloth/resmgt/rcBase.h>
#endif

////////////////////////////////////////////////////////////////////////////////
// FORWARD DECLARATIONS

class GfxImage;

FREECLOTH_NAMESPACE_START
    class GePoint;
FREECLOTH_NAMESPACE_END

////////////////////////////////////////////////////////////////////////////////
/*!
 * \class GfxGLTexture freecloth/gfx/gfxGLTexture.h
 * \brief Wrapper around an image and handle allocation of graphics RAM.
 *
 * Image is padded to a power-of-2 size, as required by OpenGL.
 */
class GfxGLTexture : public RCBase
{

public:
    //----- types and enumerations -----
    typedef RCShdPtr<GfxImage> ImagePtr;

    //----- member functions -----

    GfxGLTexture(
        const ImagePtr& image,
        Int32 format
    );
    virtual ~GfxGLTexture();
    UInt32  getTextureId() const;

    //! Get position of top-right corner of image in texture co-ordinates.
    //! If the input image is a large enough power-of-2 size, this will always
    //! be (1,1).
    //! Otherwise, the image is padded with zeros to a power-of-2 size, and
    //! the top-right corner will have a (u,v) position somewhere between
    //! (.5,.5) and (1,1)
    GePoint getCorner() const;

    void    bind() const;
    void    generateMipmaps();

    //! Get width of texture image prior to power-of-2 related padding
    UInt32 getOriginalWidth() const;
    //! Get height of texture image prior to power-of-2 related padding
    UInt32 getOriginalHeight() const;
    //! Get aspect ratio prior to power-of-2 related padding
    Float getOriginalAspectRatio() const;
    //! Get actual image used for texture
    const GfxImage& getImage() const;

    //----- static member functions -----

    //! Embeds input image in a larger image whose size is a power of two.
    //! Keep in mind that texture co-ordinates need to take into account the
    //! new image size.
    static RCShdPtr<GfxImage> makePow2( const GfxImage& );

private:

    //----- member functions -----
    //! Disallowed.
    GfxGLTexture( const GfxGLTexture& );
    //! Disallowed.
    GfxGLTexture& operator = ( const GfxGLTexture& );

    //----- data members -----

    ImagePtr    _image;
    UInt32      _textureId;
    Int32       _format;
    UInt32      _originalWidth;
    UInt32      _originalHeight;
};
#endif
