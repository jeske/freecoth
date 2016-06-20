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

#ifndef freecloth_gfx_gfxImage_inline_h
#define freecloth_gfx_gfxImage_inline_h

////////////////////////////////////////////////////////////////////////////////
// CLASS GfxImage

//------------------------------------------------------------------------------

inline UInt32 GfxImage::getWidth() const
{
    return _width;
}

//------------------------------------------------------------------------------

inline UInt32 GfxImage::getHeight() const
{
    return _height;
}

//------------------------------------------------------------------------------

inline Float GfxImage::getAspectRatio() const
{
    return static_cast<Float>( _width ) / _height;
}

//------------------------------------------------------------------------------

inline UInt8* GfxImage::getRawData()
{
    return &_dataPtr->front();
}

//------------------------------------------------------------------------------

inline const UInt8* GfxImage::getRawData() const
{
    return &_dataPtr->front();
}

//------------------------------------------------------------------------------

inline UInt8 GfxImage::getData( UInt32 x, UInt32 y, UInt32 component ) const
{
    DGFX_ASSERT( getBitsPerComponent() == 8 );
    DGFX_ASSERT(
        x < getWidth() && y < getHeight() && component < getNbComponents()
    );
    return (*_dataPtr)[
        y * getBytesPerRow() + x * getBytesPerPixel() + component
    ];
}

//------------------------------------------------------------------------------

inline UInt8& GfxImage::getData( UInt32 x, UInt32 y, UInt32 component )
{
    DGFX_ASSERT( getBitsPerComponent() == 8 );
    DGFX_ASSERT(
        x < getWidth() && y < getHeight() && component < getNbComponents()
    );
    return (*_dataPtr)[
        y * getBytesPerRow() + x * getBytesPerPixel() + component
    ];
}

//------------------------------------------------------------------------------

inline UInt32 GfxImage::getRawSize() const
{
    return getBytesPerRow() * getHeight();
}

#endif
