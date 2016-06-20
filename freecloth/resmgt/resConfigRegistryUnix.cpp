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

#include <freecloth/resmgt/resConfigRegistryUnix.h>
#include <freecloth/resmgt/rcShdPtr.h>

#include <freecloth/base/baStringUtil.h>
#include <freecloth/base/stdlib.h>

////////////////////////////////////////////////////////////////////////////////
// LOCAL DECLARATIONS

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// CLASS ResConfigRegistryW

//------------------------------------------------------------------------------
RCShdPtr<ResConfigRegistryW> ResConfigRegistryW::create(
    const String& groupName
) {
    //DGFX_TRACE_ENTER( "ResConfigRegistryW::create" );

    // FIXME: get rid of spaces
    const String rcfile( BaStringUtil::toLower(
        String( ::getenv( "HOME" ) ) + "/." + groupName + "rc"
    ) );

    return RCShdPtr<ResConfigRegistryW>(
        new ResConfigRegistryWUnix( rcfile )
    );
}

////////////////////////////////////////////////////////////////////////////////
// CLASS ResConfigRegistryR

//------------------------------------------------------------------------------
RCShdPtr<ResConfigRegistryR> ResConfigRegistryR::create(
    const String& groupName
) {
    //DGFX_TRACE_ENTER( "ResConfigRegistryR::create " << groupName );
    const String rcfile( BaStringUtil::toLower(
        String( ::getenv( "HOME" ) ) + "/." + groupName + "rc"
    ) );

    return RCShdPtr<ResConfigRegistryR>(
        new ResConfigRegistryRUnix( rcfile )
    );
}

FREECLOTH_NAMESPACE_END
