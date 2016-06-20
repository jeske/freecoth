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

#include <freecloth/geom/gePoint.h>
#include <freecloth/geom/geVector.h>
#include <freecloth/base/baMath.h>

////////////////////////////////////////////////////////////////////////////////
// LOCAL DECLARATIONS

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// CLASS GePoint

const GePoint::CompX GePoint::_compX;
const GePoint::CompY GePoint::_compY;
const GePoint::CompZ GePoint::_compZ;
const GePoint GePoint::NIL( ~0, ~0, ~0 );
const GePoint GePoint::ZERO( 0, 0, 0 );

//------------------------------------------------------------------------------

GePoint::GePoint( const GePoint& a, const GePoint& b, Float fracA )
    : _x( a._x * fracA + b._x * (1-fracA) ),
    _y( a._y * fracA + b._y * (1-fracA) ),
    _z( a._z * fracA + b._z * (1-fracA) )
{
}

//------------------------------------------------------------------------------

GePoint::GePoint( const GePoint& a, const GePoint& b, const GePoint& c, Float fracA, Float fracB )
    : _x( a._x * fracA + b._x * fracB + c._x * (1-fracA-fracB) ),
    _y( a._y * fracA + b._y * fracB + c._y * (1-fracA-fracB) ),
    _z( a._z * fracA + b._z * fracB + c._z * (1-fracA-fracB) )
{
}

//------------------------------------------------------------------------------

GePoint& GePoint::operator+=( const GeVector& rhs )
{
    _x += rhs._x;
    _y += rhs._y;
    _z += rhs._z;
    return *this;
}

//------------------------------------------------------------------------------

GePoint& GePoint::operator-=( const GeVector& rhs )
{
    _x -= rhs._x;
    _y -= rhs._y;
    _z -= rhs._z;
    return *this;
}

//------------------------------------------------------------------------------

GePoint GePoint::operator+( const GeVector& rhs ) const
{
    GePoint temp( *this );
    temp += rhs;
    return temp;
}

//------------------------------------------------------------------------------

GePoint GePoint::operator-( const GeVector& rhs ) const
{
    GePoint temp( *this );
    temp -= rhs;
    return temp;
}

//------------------------------------------------------------------------------

GeVector GePoint::operator-( const GePoint& rhs ) const
{
    return GeVector( rhs, *this );
}


////////////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTIONS
//

//------------------------------------------------------------------------------

std::ostream& operator<<( std::ostream& os, const GePoint& p )
{
    os << "p(" << p._x << "," << p._y << "," << p._z << ")";
    return os;
}

FREECLOTH_NAMESPACE_END
