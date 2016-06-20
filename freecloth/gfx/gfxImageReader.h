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

#ifndef freecloth_gfx_gfxImageReader_h
#define freecloth_gfx_gfxImageReader_h

#ifndef freecloth_gfx_package_h
#include <freecloth/gfx/package.h>
#endif

#ifndef freecloth_resmgt_rcBase_h
#include <freecloth/resmgt/rcBase.h>
#endif

////////////////////////////////////////////////////////////////////////////////
// FORWARD DECLARATIONS

class GfxImage;

FREECLOTH_NAMESPACE_START
    template <class T> class RCShdPtr;
FREECLOTH_NAMESPACE_END

////////////////////////////////////////////////////////////////////////////////
/*!
 * \class GfxImageReader freecloth/gfx/gfxImagReader.h
 * \brief Interface for image reader classes.
 *
 * Very poorly thought out, so far.
 */
class GfxImageReader : public RCBase
{

public:

    //----- types and enumerations -----

    typedef RCShdPtr< GfxImage > ImagePtr;

    //----- member functions -----

    virtual ~GfxImageReader();
    virtual ImagePtr readImage() const = 0;
};

#endif
