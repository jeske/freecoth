//////////////////////////////////////////////////////////////////////
// Copyright (c) 2002 David Pritchard <drpritch@alumni.uwaterloo.ca>
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

#ifndef freecloth_base_baTime_inline_h
#define freecloth_base_baTime_inline_h

#include <freecloth/base/baMath.h>

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// CLASS BaTime
//

//------------------------------------------------------------------------------

inline Float BaTime::instantAsSeconds( Instant instant )
{
    return instant / Float( S );
}

//------------------------------------------------------------------------------

inline Float BaTime::durationAsSeconds( Duration duration )
{
    return duration / Float( S );
}

//------------------------------------------------------------------------------

inline BaTime::Instant BaTime::floatAsInstant( Float t )
{
    return BaMath::roundUInt32( t * Float( S ) );
}

//------------------------------------------------------------------------------

inline BaTime::Duration BaTime::floatAsDuration( Float t )
{
    return BaMath::roundInt32( t * Float( S ) );
}

FREECLOTH_NAMESPACE_END

#endif
