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

#ifndef freecloth_gfx_gfxImageWriterPNM_h
#define freecloth_gfx_gfxImageWriterPNM_h

#ifndef freecloth_gfx_package_h
#include <freecloth/gfx/package.h>
#endif

#ifndef freecloth_gfx_gfxImageWriter
#include <freecloth/gfx/gfxImageWriter.h>
#endif

////////////////////////////////////////////////////////////////////////////////
// FORWARD DECLARATIONS

////////////////////////////////////////////////////////////////////////////////
/*!
 * \class GfxImageWriterPNM freecloth/gfx/gfxImageWriterPNM.h
 * \brief Class to write Portable Anymap Files (bitmaps, greymap, and pixmaps)
 * to disk.
 *
 */
class GfxImageWriterPNM : public GfxImageWriter
{

public:

    //----- types and enumerations -----
    enum FormatType {
        ASCII,
        BINARY
    };

    //----- member functions -----

    explicit GfxImageWriterPNM( FormatType );

    virtual void writeImage(
        const String& filename,
        const GfxImage&
    );
    virtual void writeImage(
        std::ostream&,
        const GfxImage&
    );

private:
    FormatType _formatType;
};

#endif
