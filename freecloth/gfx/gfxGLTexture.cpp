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

#include <freecloth/gfx/gfxGLTexture.h>
#include <freecloth/gfx/gfxImage.h>
#include <freecloth/geom/gePoint.h>
#include <freecloth/base/algorithm>
#include <freecloth/base/baMath.h>
#include <freecloth/base/GL_gl.h>
#include <freecloth/base/GL_glu.h>


////////////////////////////////////////////////////////////////////////////////
// CLASS GfxGLTexture

//------------------------------------------------------------------------------

GfxGLTexture::GfxGLTexture( const ImagePtr& image, Int32 format )
  : _format( format )
{
    DGFX_ASSERT( ! image.isNull() );
    _originalWidth = image->getWidth();
    _originalHeight = image->getHeight();
    if (
        BaMath::isPow2( _originalWidth ) &&
        BaMath::isPow2( _originalHeight ) &&
        _originalWidth >= 64 && _originalHeight >= 64
    ) {
        _image = image;
    }
    else {
        _image = makePow2( *image );
    }

    ::glGenTextures( 1, &_textureId );

    // Create linear filtered texture
    ::glBindTexture( GL_TEXTURE_2D, _textureId );
    ::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    ::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    ::glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    ::glTexImage2D(
        GL_TEXTURE_2D,
        0,                      // level-of-detail
        _image->getNbComponents(),
        _image->getWidth(),
        _image->getHeight(),
        0,                      // border
        _format,
        GL_UNSIGNED_BYTE,
        _image->getRawData()
    );
}

//------------------------------------------------------------------------------

GfxGLTexture::~GfxGLTexture()
{
    ::glDeleteTextures( 1, &_textureId );
}

//------------------------------------------------------------------------------

void GfxGLTexture::generateMipmaps()
{
    ::gluBuild2DMipmaps(
        GL_TEXTURE_2D,
        _image->getNbComponents(),
        _image->getWidth(),
        _image->getHeight(),
        _format,
        GL_UNSIGNED_BYTE,
        _image->getRawData()
    );
}

//------------------------------------------------------------------------------

UInt32 GfxGLTexture::getTextureId() const
{
    return _textureId;
}

//------------------------------------------------------------------------------

void GfxGLTexture::bind() const
{
    ::glBindTexture( GL_TEXTURE_2D, _textureId );
}

//------------------------------------------------------------------------------

const GfxImage& GfxGLTexture::getImage() const
{
    return *_image;
}

//------------------------------------------------------------------------------

GePoint GfxGLTexture::getCorner() const
{
    return GePoint(
        static_cast< Float >( _originalWidth ) / _image->getWidth(),
        static_cast< Float >( _originalHeight ) / _image->getHeight(),
        0
    );
}

//------------------------------------------------------------------------------

UInt32 GfxGLTexture::getOriginalWidth() const
{
    return _originalWidth;
}

//------------------------------------------------------------------------------

UInt32 GfxGLTexture::getOriginalHeight() const
{
    return _originalHeight;
}

//------------------------------------------------------------------------------

Float GfxGLTexture::getOriginalAspectRatio() const
{
    return static_cast<Float>( _originalWidth ) / _originalHeight;
}

//------------------------------------------------------------------------------

RCShdPtr<GfxImage> GfxGLTexture::makePow2( const GfxImage& image )
{
    RCShdPtr<GfxImage> result( new GfxImage(
        std::max( BaMath::roundUpPow2( image.getWidth() ), 64U ),
        std::max( BaMath::roundUpPow2( image.getHeight() ), 64U ),
        image.getFormat()
    ) );
    const UInt8* in = image.getRawData();
    UInt8* out = result->getRawData();
    UInt32 y;
    for ( y = 0; y < image.getHeight(); ++y ) {
        UInt32 iw = image.getWidth() * image.getNbComponents();
        UInt32 x;
        for ( x = 0; x < iw; ++x ) {
            *out++ = *in++;
        }
        UInt32 ow = result->getWidth() * result->getNbComponents();
        for ( x = iw; x < ow; ++x ) {
            *out++ = 0;
        }
    }
    for ( y = image.getHeight(); y < result->getHeight(); ++y ) {
        UInt32 ow = result->getWidth() * result->getNbComponents();
        for ( UInt32 x = 0; x < ow; ++x ) {
            *out++ = 0;
        }
    }
    return result;
}
