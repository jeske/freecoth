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

#include <freecloth/gfx/gfxImageWriterPNM.h>
#include <freecloth/gfx/gfxImage.h>
#include <freecloth/base/fstream>

////////////////////////////////////////////////////////////////////////////////
// CLASS GfxImageWriterPNM

//------------------------------------------------------------------------------

GfxImageWriterPNM::GfxImageWriterPNM(
    FormatType formatType
) : _formatType( formatType )
{
}

//------------------------------------------------------------------------------

void GfxImageWriterPNM::writeImage(
    const String& filename,
    const GfxImage& image
) {
    // FIXME: ios::binary seems to do bad stuff...
    
    //if ( _formatType == ASCII ) {
    if ( 1 ) {
        std::ofstream os( filename.c_str() );
        writeImage( os, image );
    }
    else {
        std::ofstream os( filename.c_str(), std::ios::binary );
        writeImage( os, image );
    }
}

//------------------------------------------------------------------------------

void GfxImageWriterPNM::writeImage(
    std::ostream& os,
    const GfxImage& image
) {
    DGFX_ASSERT( os.good() );

    switch( image.getFormat() ) {
        case GfxImage::GREY8: {
            if ( _formatType == ASCII ) {
                os << "P2";
            }
            else {
                os << "P5";
            }
        } break;
        case GfxImage::RGB24: {
            if ( _formatType == ASCII ) {
                os << "P3";
            }
            else {
                os << "P6";
            }
        } break;
    }
    os << std::endl;
    os << image.getWidth() << " " << image.getHeight() << std::endl;
    switch ( image.getBitsPerComponent() ) {
        case 8: {
            os << "255 ";
        } break;
        default: {
            DGFX_ASSERT( 0 );
        }
    }

    if ( _formatType == ASCII ) {
        UInt32 x, y, c;
        switch ( image.getBitsPerComponent() ) {
            case 8: {
                for ( y = 0; y < image.getHeight(); ++y ) {
                    for ( x = 0; x < image.getWidth(); ++x ) {
                        for ( c = 0; c < image.getNbComponents(); ++c ) {
                            os << static_cast<UInt32>( image.getData( x, y, c ) );
                            os << " ";
                        }
                        os << " ";
                    }
                    os << std::endl;
                }
            } break;

            default: {
                DGFX_ASSERT( 0 );
            }
        }
    }
    else {
        // FIXME: dependent upon endianness!
        os.write(
            reinterpret_cast<const char*>( image.getRawData() ),
            image.getRawSize()
        );
    }
}
