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

#include <freecloth/gfx/gfxImageReaderPNM.h>
#include <freecloth/gfx/gfxImage.h>
#include <freecloth/resmgt/rcShdPtr.h>
#include <freecloth/base/fstream>
#include <freecloth/base/ctype.h>
#include <freecloth/base/limits.h>


////////////////////////////////////////////////////////////////////////////////
// LOCAL DECLARATIONS

namespace {

//------------------------------------------------------------------------------

void stripSpaceComments( std::istream& in )
{
    while( 1 ) {
        while ( isspace( in.peek() ) ) in.get();
        if (in.peek() == '#') {
            in.ignore( INT_MAX, '\n' );
        }
        else {
            return;
        }
    }
}

}

////////////////////////////////////////////////////////////////////////////////
// CLASS GfxImageReaderPNM

//------------------------------------------------------------------------------

bool GfxImageReaderPNM::canRead( const String& path )
{
    // Note that string comparison is case-sensitive.
    //
    // Also note that string::compare w/ 3 args is not implemented in a standard
    // manner in gcc 3.0 right now. gcc 2.95 complies to standard.
    // (why...???)
    return
        path.length() >= 4 &&
        ( path.substr( path.length() - 4, 4 ).compare( ".ppm" ) == 0 ||
          path.substr( path.length() - 4, 4 ).compare( ".pgm" ) == 0 ||
          path.substr( path.length() - 4, 4 ).compare( ".pnm" ) == 0);
}

//------------------------------------------------------------------------------

GfxImageReaderPNM::GfxImageReaderPNM( const String& path )
    : _path( path )
{
}

//------------------------------------------------------------------------------

GfxImageReader::ImagePtr GfxImageReaderPNM::readImage() const
{
    DGFX_TRACE_ENTER( "GfxImageReaderPNM::readImage()" );
    DGFX_TRACE( "path = " << _path );

    if ( _path == "-" ) {
        return readImage( std::cin );
    }
    else {
        // FIXME: non-portable: need some way of switching ascii/binary for
        // binary files under Win32
        std::ifstream in( _path.c_str() );
        if ( in.good() ) {
            return readImage( in );
        }
        else {
            return ImagePtr();
        }
    }
}

//------------------------------------------------------------------------------

GfxImageReader::ImagePtr GfxImageReaderPNM::readImage(
    std::istream& in
) const {
    stripSpaceComments( in );
    char c;
    c = in.get();
    DGFX_ASSERT( c == 'P' );

    char type;
    UInt32 width, height, maxCol;

    type = in.get();
    stripSpaceComments( in );
    in >> width;
    stripSpaceComments( in );
    in >> height;
    if ( type == '4' ) {
        maxCol = 1;
    }
    else {
        stripSpaceComments( in );
        in >> maxCol;
        DGFX_ASSERT( maxCol == 0xff || maxCol == 0xffff );
    }

    bool isBinary = false;
    GfxImage::Format format = GfxImage::GREY8;

    switch ( type ) {
        case '2': {
            isBinary = false;
            format = GfxImage::GREY8;
            DGFX_ASSERT( maxCol == 0xff );
        } break;

        case '3': {
            isBinary = false;
            format = GfxImage::RGB24;
            DGFX_ASSERT( maxCol == 0xff );
        } break;

        case '4': {
            isBinary = true;
            // No 1-bit format yet
            format = GfxImage::GREY8;
            DGFX_ASSERT( maxCol == 1 );
        } break;

        case '5': {
            isBinary = true;
            format = GfxImage::GREY8;
            DGFX_ASSERT( maxCol == 0xff );
        } break;

        case '6': {
            isBinary = true;
            format = GfxImage::RGB24;
            DGFX_ASSERT( maxCol == 0xff );
        } break;

        default: {
            DGFX_ASSERT( 0 );
        } break;
    }
    c = in.get();
    DGFX_ASSERT( isspace( c ) );

    ImagePtr image( new GfxImage( width, height, format ) );
    UInt32 i,j;
    UInt32 bits = maxCol == 1 ? 1 : image->getBitsPerComponent();
    switch ( bits ) {
        case 1: {
            DGFX_ASSERT( isBinary );
            UInt8* data = image->getRawData();
            for ( j = 0; j < height; ++j ) {
                for ( i = 0; i < width; ) {
                    UInt8 val = in.get();
                    for ( UInt8 k = 0x80; k > 0 && i < width; k >>= 1,++i) {
                        *data = (val & k) == 0 ? 255 : 0;
                        ++data;
                    }
                }
            }
        } break;

        case 8: {
            if ( isBinary ) {
                in.read(
                    reinterpret_cast<char*>( image->getRawData() ),
                    image->getRawSize()
                );
            }
            else {
                UInt32 val;
                UInt8* data = image->getRawData();
                for ( i = 0; i < image->getRawSize(); ++i ) {
                    in >> val;
                    *data = val;
                    ++data;
                }
            }
        } break;

        default: {
            DGFX_ASSERT( 0 );
        } break;
    }
    return image;
}
