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

#ifndef freecloth_geom_geVector_inline_h
#define freecloth_geom_geVector_inline_h

#include <freecloth/base/baMath.h>
#include <freecloth/base/algorithm>

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// CLASS GeVector

//------------------------------------------------------------------------------

inline GeVector::GeVector()
{}

//------------------------------------------------------------------------------

inline GeVector::GeVector( Float x, Float y, Float z)
    : _x( x ), _y( y ), _z( z )
{}

//------------------------------------------------------------------------------

inline GeVector GeVector::zero()
{
    return GeVector( 0, 0, 0 );
}

//------------------------------------------------------------------------------

inline GeVector GeVector::xAxis()
{
    return GeVector( 1, 0, 0 );
}

//------------------------------------------------------------------------------

inline GeVector GeVector::yAxis()
{
    return GeVector( 0, 1, 0 );
}

//------------------------------------------------------------------------------

inline GeVector GeVector::zAxis()
{
    return GeVector( 0, 0, 1 );
}

//------------------------------------------------------------------------------

inline GeVector GeVector::axis( UInt32 n )
{
    DGFX_ASSERT( n < 3 );
    switch( n ) {
        case 0: {
            return xAxis();
        }
        case 1: {
            return yAxis();
        }
        case 2: {
            return zAxis();
        }
    }
    return GeVector();
}

//------------------------------------------------------------------------------

inline GeVector& GeVector::operator+=( const GeVector& rhs )
{
    _x += rhs._x;
    _y += rhs._y;
    _z += rhs._z;
    return *this;
}

//------------------------------------------------------------------------------

inline GeVector& GeVector::operator-=( const GeVector& rhs )
{
    _x -= rhs._x;
    _y -= rhs._y;
    _z -= rhs._z;
    return *this;
}

//------------------------------------------------------------------------------

inline GeVector& GeVector::operator+=( Float k )
{
    _x += k;
    _y += k;
    _z += k;
    return *this;
}

//------------------------------------------------------------------------------

inline GeVector& GeVector::operator-=( Float k )
{
    _x -= k;
    _y -= k;
    _z -= k;
    return *this;
}

//------------------------------------------------------------------------------

inline GeVector& GeVector::operator*=( Float k )
{
    _x *= k;
    _y *= k;
    _z *= k;
    return *this;
}

//------------------------------------------------------------------------------

inline GeVector& GeVector::operator/=( Float k )
{
    _x /= k;
    _y /= k;
    _z /= k;
    return *this;
}

//------------------------------------------------------------------------------

inline GeVector GeVector::operator-() const
{
    return GeVector( -_x, -_y, -_z );
}

//------------------------------------------------------------------------------

inline GeVector GeVector::operator+( const GeVector& rhs ) const
{
    GeVector temp( *this );
    temp += rhs;
    return temp;
}

//------------------------------------------------------------------------------

inline GeVector GeVector::operator-( const GeVector& rhs ) const
{
    GeVector temp( *this );
    temp -= rhs;
    return temp;
}

//------------------------------------------------------------------------------

inline GeVector GeVector::operator+( Float k ) const
{
    GeVector temp( *this );
    temp += k;
    return temp;
}

//------------------------------------------------------------------------------

inline GeVector GeVector::operator-( Float k ) const
{
    GeVector temp( *this );
    temp -= k;
    return temp;
}

//------------------------------------------------------------------------------

inline GeVector GeVector::operator*( Float k ) const
{
    GeVector temp( *this );
    temp *= k;
    return temp;
}

//------------------------------------------------------------------------------

inline GeVector GeVector::operator/( Float k ) const
{
    GeVector temp( *this );
    temp /= k;
    return temp;
}

//------------------------------------------------------------------------------

inline bool GeVector::operator==( const GeVector& rhs ) const
{
    return
        BaMath::isEqual( _x, rhs._x ) &&
        BaMath::isEqual( _y, rhs._y ) &&
        BaMath::isEqual( _z, rhs._z );
}

//------------------------------------------------------------------------------

inline bool GeVector::operator!=( const GeVector& rhs ) const
{
    return !operator==( rhs );
}

//------------------------------------------------------------------------------

inline Float GeVector::dot( const GeVector& rhs ) const
{
    return _x * rhs._x + _y * rhs._y + _z * rhs._z;
}

//------------------------------------------------------------------------------

inline GeVector GeVector::cross( const GeVector& rhs ) const
{
    return GeVector(
        _y * rhs._z - _z * rhs._y,
        _z * rhs._x - _x * rhs._z,
        _x * rhs._y - _y * rhs._x
    );
}

//------------------------------------------------------------------------------

inline Float GeVector::squaredLength() const
{
    return _x*_x + _y*_y + _z*_z;
}

//------------------------------------------------------------------------------

inline Float GeVector::length() const
{
    return BaMath::sqrt( squaredLength() );
}

//------------------------------------------------------------------------------

inline Float GeVector::infinityNorm() const
{
    return std::max(
        std::max( BaMath::abs( _x ), BaMath::abs( _y ) ),
        BaMath::abs( _z )
    );
}

//------------------------------------------------------------------------------

inline bool GeVector::isUnit() const
{
    return BaMath::isEqual( squaredLength(), 1 );
}

//------------------------------------------------------------------------------

inline GeVector GeVector::getUnit() const
{
    return (*this) / length();
}

//------------------------------------------------------------------------------

inline Float GeVector::operator[]( UInt32 i ) const
{
    DGFX_ASSERT( i < 4 );
    switch( i ) {
        case 0: return _x;
        case 1: return _y;
        case 2: return _z;
        case 3: return 0;
    }
    return 0;
}

//------------------------------------------------------------------------------

inline Float& GeVector::operator[]( UInt32 i )
{
    DGFX_ASSERT( i < 3 );
    switch( i ) {
        case 0: return _x;
        case 1: return _y;
        case 2: return _z;
    }
    static Float dummy;
    return dummy;
}


////////////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTIONS
//

//------------------------------------------------------------------------------

inline GeVector operator*( Float k, const GeVector& rhs )
{
    return rhs * k;
}

FREECLOTH_NAMESPACE_END

#endif
