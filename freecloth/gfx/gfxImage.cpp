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

#include <freecloth/gfx/gfxImage.h>
#include <freecloth/resmgt/rcShdPtr.h>

////////////////////////////////////////////////////////////////////////////////
// CLASS GfxImage

//------------------------------------------------------------------------------

GfxImage::GfxImage(
    const RawDataPtr& dataPtr,
    UInt32 width,
    UInt32 height,
    Format format
) : _dataPtr( dataPtr ),
    _width( width ),
    _height( height ),
    _format( format )
{
    //DGFX_TRACE_ENTER( "GfxImage ctor" );
    //DGFX_TRACE( "width,height = " << _width << ", " << _height );
    //DGFX_TRACE( "format = " << _format );
    DGFX_ASSERT( dataPtr->size() == getRawSize() );
    DGFX_ASSERT( getBitsPerComponent( format ) == 8 );
}

//------------------------------------------------------------------------------

GfxImage::GfxImage(
    UInt32 width,
    UInt32 height,
    Format format
) : _dataPtr( new RawData ),
    _width( width ),
    _height( height ),
    _format( format )
{
    _dataPtr->resize( getRawSize(), 0 );
    //DGFX_TRACE_ENTER( "GfxImage ctor 2" );
    //DGFX_TRACE( "width,height = " << _width << ", " << _height );
    //DGFX_TRACE( "format = " << _format );
}

//------------------------------------------------------------------------------

GfxImage::GfxImage(
    const GfxImage& rhs
) : _dataPtr( new RawData ),
    _width( rhs._width ),
    _height( rhs._height ),
    _format( rhs._format )
{
    _dataPtr->resize( getRawSize(), 0 );
    ::memcpy( getRawData(), rhs.getRawData(), getRawSize() );
}

//------------------------------------------------------------------------------
GfxImage::~GfxImage()
{
    //DGFX_TRACE_ENTER( "GfxImage dtor" );
}

//------------------------------------------------------------------------------

GfxImage::Format GfxImage::getFormat() const
{
    return _format;
}

//------------------------------------------------------------------------------

UInt32 GfxImage::getBitsPerComponent( Format format )
{
    switch( format ) {
        case GREY8: {
            return 8;
        }

        case RGB24: {
            return 8;
        }
    }
    return 0;
}

//------------------------------------------------------------------------------

UInt32 GfxImage::getNbComponents( Format format )
{
    switch( format ) {
        case GREY8: {
            return 1;
        }

        case RGB24: {
            return 3;
        }
    }
    return 0;
}

//------------------------------------------------------------------------------

UInt32 GfxImage::getBitsPerPixel() const
{
    return getBitsPerComponent( _format ) * getNbComponents( _format );
}

//------------------------------------------------------------------------------

UInt32 GfxImage::getBytesPerPixel() const
{
    return getBitsPerPixel() / 8;
}

//------------------------------------------------------------------------------

UInt32 GfxImage::getBytesPerRow() const
{
    return getBytesPerPixel() * getWidth();
}

//------------------------------------------------------------------------------

UInt32 GfxImage::getNbComponents() const
{
    return getNbComponents( _format );
}

//------------------------------------------------------------------------------

UInt32 GfxImage::getBitsPerComponent() const
{
    return getBitsPerComponent( _format );
}

//------------------------------------------------------------------------------

RCShdPtr<GfxImage> GfxImage::pad(
    UInt32 padX, UInt32 padY, UInt8 r, UInt8 g, UInt8 b
) const {
    DGFX_ASSERT( getFormat() == GfxImage::RGB24 );
    RCShdPtr<GfxImage> result( new GfxImage(
        getWidth() + padX*2,
        getHeight() + padY*2,
        getFormat()
    ) );
    UInt32 x,y;
    for ( y = 0; y < padY; ++y ) {
        UInt32 y2 = y + getHeight() + padY;
        for ( x = 0; x < result->getWidth(); ++x ) {
            result->getData( x, y, 0 ) = result->getData( x, y2, 0 ) = r;
            result->getData( x, y, 1 ) = result->getData( x, y2, 1 ) = g;
            result->getData( x, y, 2 ) = result->getData( x, y2, 2 ) = b;
        }
    }
    for ( y = 0; y < getHeight(); ++y ) {
        UInt32 y2 = y + padY;
        for ( x = 0; x < padX; ++x ) {
            UInt32 x2 = x + getWidth() + padX;
            result->getData( x, y2, 0 ) = result->getData( x2, y2, 0 ) = r;
            result->getData( x, y2, 1 ) = result->getData( x2, y2, 1 ) = g;
            result->getData( x, y2, 2 ) = result->getData( x2, y2, 2 ) = b;
        }
        for ( x = 0; x < getWidth(); ++x ) {
            UInt32 x2 = x + padX;
            result->getData( x2, y2, 0 ) = getData( x, y, 0 );
            result->getData( x2, y2, 1 ) = getData( x, y, 1 );
            result->getData( x2, y2, 2 ) = getData( x, y, 2 );
        }
    }
    return result;
}

//------------------------------------------------------------------------------

RCShdPtr<GfxImage> GfxImage::pad(
    UInt32 padX, UInt32 padY, UInt8 v
) const {
    DGFX_ASSERT( getFormat() == GfxImage::GREY8 );
    RCShdPtr<GfxImage> result( new GfxImage(
        getWidth() + padX*2,
        getHeight() + padY*2,
        getFormat()
    ) );
    UInt32 x,y;
    for ( y = 0; y < padY; ++y ) {
        UInt32 y2 = y + getHeight() + padY;
        for ( x = 0; x < result->getWidth(); ++x ) {
            result->getData( x, y ) = result->getData( x, y2 ) = v;
        }
    }
    for ( y = 0; y < getHeight(); ++y ) {
        UInt32 y2 = y + padY;
        for ( x = 0; x < padX; ++x ) {
            UInt32 x2 = x + getWidth() + padX;
            result->getData( x, y2 ) = result->getData( x2, y2 ) = v;
        }
        for ( x = 0; x < getWidth(); ++x ) {
            UInt32 x2 = x + padX;
            result->getData( x2, y2 ) = getData( x, y );
        }
    }
    return result;
}

