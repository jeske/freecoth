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

#ifndef freecloth_gfx_gfxImage_h
#define freecloth_gfx_gfxImage_h

#ifndef freecloth_gfx_package_h
#include <freecloth/gfx/package.h>
#endif

#ifndef freecloth_resmgt_rcBase_h
#include <freecloth/resmgt/rcBase.h>
#endif

#ifndef freecloth_resmgt_rcProxyShdPtr_h
#include <freecloth/resmgt/rcProxyShdPtr.h>
#endif

#ifndef freecloth_base_vector
#include <freecloth/base/vector>
#endif

////////////////////////////////////////////////////////////////////////////////
// FORWARD DECLARATIONS


FREECLOTH_NAMESPACE_START
    template <class T> class RCShdPtr;
FREECLOTH_NAMESPACE_END

////////////////////////////////////////////////////////////////////////////////
/*!
 * \class GfxImage freecloth/gfx/gfxImage.h
 * \brief Class to hold an image.
 *
 * Image data is stored in row major format, starting with the top-left corner.
 * Rows are not guaranteed to be aligned to 4-byte boundaries.
 * Not very user-friendly yet, especially when dealing with 24-bit data.
 */
class GfxImage : public RCBase
{
public:
    //----- types and enumerations -----

    typedef std::vector<UInt8> RawData;
    // This should really be a private pointer, not a shared pointer.
    typedef RCProxyShdPtr<RawData> RawDataPtr;

    enum Format {
        GREY8,
        RGB24
    };
    enum Component {
        RED = 0,
        GREEN = 1,
        BLUE = 2
    };

    //----- static member functions -----

    static UInt32 getBitsPerComponent( Format format );
    static UInt32 getNbComponents( Format format );

    //----- member functions -----

    GfxImage(
        const RawDataPtr& dataPtr,
        UInt32 width,
        UInt32 height,
        Format format
    );
    GfxImage(
        UInt32 width,
        UInt32 height,
        Format format
    );
    GfxImage(
        const GfxImage&
    );
    virtual ~GfxImage();

    UInt32 getWidth() const;
    UInt32 getHeight() const;
    Float getAspectRatio() const;
    Format getFormat() const;
    UInt32 getBitsPerPixel() const;
    UInt32 getBytesPerPixel() const;
    UInt32 getBytesPerRow() const;
    UInt32 getNbComponents() const;
    UInt32 getBitsPerComponent() const;

    UInt8* getRawData();
    const UInt8* getRawData() const;
    UInt32 getRawSize() const;
    UInt8 getData( UInt32 x, UInt32 y, UInt32 component = 0 ) const;
    UInt8& getData( UInt32 x, UInt32 y, UInt32 component = 0 );

    //! Create a new image with padding around the borders. rgb parameters
    //! specify colour to fill border with.
    RCShdPtr<GfxImage> pad(
        UInt32 padX, UInt32 padY, UInt8 r, UInt8 g, UInt8 b
    ) const;
    //! Create a new image with padding around the borders. value parameter
    //! specifies value to fill border with.
    RCShdPtr<GfxImage> pad(
        UInt32 padX, UInt32 padY, UInt8 value
    ) const;

private:

    //----- member functions -----
    // Disallowed. FIXME: allow it.
    GfxImage& operator=( const GfxImage& );

    //----- data members -----

    const RawDataPtr _dataPtr;
    UInt32 _width;
    UInt32 _height;
    Format _format;
};

#include <freecloth/gfx/gfxImage.inline.h>

#endif
