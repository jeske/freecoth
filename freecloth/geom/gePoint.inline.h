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

#ifndef freecloth_geom_gePoint_inline_h
#define freecloth_geom_gePoint_inline_h

#include <freecloth/base/baMath.h>

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// CLASS GePoint

//------------------------------------------------------------------------------

inline GePoint::GePoint()
{}

//------------------------------------------------------------------------------

inline GePoint::GePoint( Float x, Float y, Float z)
    : _x( x ), _y( y ), _z( z )
{}

//------------------------------------------------------------------------------

inline bool GePoint::operator==( const GePoint& rhs ) const
{
    return
        BaMath::isEqual( _x, rhs._x ) &&
        BaMath::isEqual( _y, rhs._y ) &&
        BaMath::isEqual( _z, rhs._z );
}

//------------------------------------------------------------------------------

inline bool GePoint::operator!=( const GePoint& rhs ) const
{
    return !operator==( rhs );
}


//------------------------------------------------------------------------------

inline Float GePoint::operator[]( UInt32 i ) const
{
    DGFX_ASSERT( i < 3 );
    switch( i ) {
        case 0: return _x;
        case 1: return _y;
        case 2: return _z;
    }
    return 0;
}

//------------------------------------------------------------------------------

inline Float& GePoint::operator[]( UInt32 i )
{
    DGFX_ASSERT( i < 3 );
    switch( i ) {
        case 0: return _x;
        case 1: return _y;
        case 2: return _z;
    }
    // Failure.
    return _x;
}


////////////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTIONS
//

FREECLOTH_NAMESPACE_END

#endif
