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

#include <freecloth/resmgt/rcBase.h>

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// CLASS RCBase

//------------------------------------------------------------------------------

RCBase::RCBase()
    : _rcCount( 0 )
{
}

//------------------------------------------------------------------------------

RCBase::RCBase( RCBase const& rhs )
    : _rcCount( 0 )
{
    // While the original object may have had references, *this* object doesn't
    // have any.
}

//------------------------------------------------------------------------------

RCBase::~RCBase()
{
    //DGFX_ASSERT( _rcCount == 1 );
}

FREECLOTH_NAMESPACE_END
