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

#include <freecloth/base/baTime.h>
#include <freecloth/base/windows.h>
#include <freecloth/base/debug.h>

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// CLASS BaTime

//------------------------------------------------------------------------------

BaTime::Instant BaTime::getTime()
{
    return ::timeGetTime();
}

//------------------------------------------------------------------------------

BaTime::Duration BaTime::getDuration( Instant start, Instant end )
{
    DGFX_ASSERT( end >= start );
    return end - start;
}

FREECLOTH_NAMESPACE_END
