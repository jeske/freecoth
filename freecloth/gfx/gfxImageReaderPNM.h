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

#ifndef freecloth_gfx_gfxImageReaderPNM_h
#define freecloth_gfx_gfxImageReaderPNM_h

#ifndef freecloth_gfx_package_h
#include <freecloth/gfx/package.h>
#endif

#ifndef freecloth_gfx_gfxImageReader_h
#include <freecloth/gfx/gfxImageReader.h>
#endif

////////////////////////////////////////////////////////////////////////////////
// FORWARD DECLARATIONS

template <class T> class GfxImageReaderFactorySimple;

/*!
 * \class GfxImageReaderPNM freecloth/gfx/gfxImagReaderPNM.h
 * \brief Class to read Portable Anymap Files (bitmaps, greymap, and pixmaps)
 * from disk.
 */
class GfxImageReaderPNM : public GfxImageReader
{

public:

    //----- types and enumerations -----
    typedef GfxImageReaderFactorySimple< GfxImageReaderPNM > Factory;

    //----- static member functions -----
    static bool canRead( const String& path );

    //----- member functions -----
    explicit GfxImageReaderPNM( const String& path );

    //! See base class.
    virtual ImagePtr readImage() const;

    ImagePtr readImage( std::istream& ) const;

private:

    //----- data members -----
    const String _path;
};

#endif
