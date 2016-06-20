//////////////////////////////////////////////////////////////////////
// Copyright (c) 2002-2003 David Pritchard <drpritch@alumni.uwaterloo.ca>
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

#ifndef freecloth_geom_geMatrix3_inline_h
#define freecloth_geom_geMatrix3_inline_h

#ifndef freecloth_geom_geVector_h
#include <freecloth/geom/geVector.h>
#endif

#ifndef freecloth_geom_gePoint_h
#include <freecloth/geom/gePoint.h>
#endif

#ifndef freecloth_base_baMath_h
#include <freecloth/base/baMath.h>
#endif

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// CLASS GeMatrix3

//------------------------------------------------------------------------------

inline GeMatrix3::GeMatrix3()
{
}

//------------------------------------------------------------------------------

inline GeMatrix3::GeMatrix3(
    Float d00, Float d01, Float d02,
    Float d10, Float d11, Float d12,
    Float d20, Float d21, Float d22
) {
    _data[ 0 ] = d00; _data[ 3 ] = d01; _data[ 6 ] = d02;
    _data[ 1 ] = d10; _data[ 4 ] = d11; _data[ 7 ] = d12;
    _data[ 2 ] = d20; _data[ 5 ] = d21; _data[ 8 ] = d22;
}

//------------------------------------------------------------------------------

inline GeMatrix3::GeMatrix3( const Float data[ 9 ] )
{
    std::copy( data, data + 9, _data );
}

//------------------------------------------------------------------------------

inline GeMatrix3 GeMatrix3::colMajor( const Float data[ 9 ] )
{
    return GeMatrix3( data );
}

//------------------------------------------------------------------------------

inline GeMatrix3 GeMatrix3::rowMajor( const Float data[ 9 ] )
{
    return GeMatrix3(
        data[ 0 ], data[ 3 ], data[ 6 ],
        data[ 1 ], data[ 4 ], data[ 7 ],
        data[ 2 ], data[ 5 ], data[ 8 ]
    );
}

//------------------------------------------------------------------------------

inline GeMatrix3 GeMatrix3::scaling( Float sx, Float sy, Float sz )
{
    return GeMatrix3(
        sx, 0, 0,
        0, sy, 0,
        0, 0, sz
    );
}

//------------------------------------------------------------------------------

inline GeMatrix3 GeMatrix3::outerProduct( const GeVector& a, const GeVector& b )
{
    return GeMatrix3(
        a._x * b._x, a._x * b._y, a._x * b._z,
        a._y * b._x, a._y * b._y, a._y * b._z,
        a._z * b._x, a._z * b._y, a._z * b._z
    );
}

//------------------------------------------------------------------------------

inline GeMatrix3 GeMatrix3::identity()
{
    static const Float entries[] = { 1, 0, 0,  0, 1, 0,  0, 0, 1 };
    return GeMatrix3( entries );
}

//------------------------------------------------------------------------------

inline GeMatrix3 GeMatrix3::zero()
{
    static const Float entries[] = { 0, 0, 0,  0, 0, 0,  0, 0, 0 };
    return GeMatrix3( entries );
}

//------------------------------------------------------------------------------

inline Float& GeMatrix3::operator()( UInt32 row, UInt32 col )
{
    DGFX_ASSERT( row < 3 && col < 3 );
    return _data[ col * 3 + row ];
}

//------------------------------------------------------------------------------

inline Float GeMatrix3::operator()( UInt32 row, UInt32 col ) const
{
    DGFX_ASSERT( row < 3 && col < 3 );
    return _data[ col * 3 + row ];
}

//------------------------------------------------------------------------------

inline GeMatrix3 GeMatrix3::operator-() const
{
    return GeMatrix3(
        -_data[0], -_data[1], -_data[2],
        -_data[3], -_data[4], -_data[5],
        -_data[6], -_data[7], -_data[8]
    );
}

//------------------------------------------------------------------------------

inline bool GeMatrix3::operator==( const GeMatrix3& rhs ) const
{
    bool result = true;
    for( UInt32 i = 0; i < 9; ++i ) {
        result = result && BaMath::isEqual( _data[ i ], rhs._data[ i ] );
    }
    return result;
}

//------------------------------------------------------------------------------

inline bool GeMatrix3::operator!=( const GeMatrix3& rhs ) const
{
    return !operator==( rhs );
}

//------------------------------------------------------------------------------

inline GeMatrix3& GeMatrix3::operator*=( const GeMatrix3& rhs )
{
    return operator=( operator*( rhs ) );
}

//------------------------------------------------------------------------------

inline GeMatrix3& GeMatrix3::operator*=( Float rhs )
{
    for( UInt32 i = 0; i < 9; ++i ) {
        _data[ i ] *= rhs;
    }
    return *this;
}

//------------------------------------------------------------------------------

inline GeMatrix3& GeMatrix3::operator+=( const GeMatrix3& rhs )
{
    for( UInt32 i = 0; i < 9; ++i ) {
        _data[ i ] += rhs._data[ i ];
    }
    return *this;
}

//------------------------------------------------------------------------------

inline GeMatrix3& GeMatrix3::operator-=( const GeMatrix3& rhs )
{
    for( UInt32 i = 0; i < 9; ++i ) {
        _data[ i ] -= rhs._data[ i ];
    }
    return *this;
}

//------------------------------------------------------------------------------

inline GeMatrix3 GeMatrix3::operator*( Float rhs ) const
{
    GeMatrix3 temp( *this );
    temp *= rhs;
    return temp;
}

//------------------------------------------------------------------------------

inline GeVector GeMatrix3::operator*( const GeVector& rhs ) const
{
    // _data[ r+c*3 ]
    return GeVector(
        _data[ 0+0*3 ]*rhs._x + _data[ 0+1*3 ]*rhs._y + _data[ 0+2*3 ]*rhs._z,
        _data[ 1+0*3 ]*rhs._x + _data[ 1+1*3 ]*rhs._y + _data[ 1+2*3 ]*rhs._z,
        _data[ 2+0*3 ]*rhs._x + _data[ 2+1*3 ]*rhs._y + _data[ 2+2*3 ]*rhs._z
    );
}

//------------------------------------------------------------------------------

inline GePoint GeMatrix3::operator*( const GePoint& rhs ) const
{
    // _data[ r+c*3 ]
    return GePoint(
        _data[ 0+0*3 ]*rhs._x + _data[ 0+1*3 ]*rhs._y + _data[ 0+2*3 ]*rhs._z,
        _data[ 1+0*3 ]*rhs._x + _data[ 1+1*3 ]*rhs._y + _data[ 1+2*3 ]*rhs._z,
        _data[ 2+0*3 ]*rhs._x + _data[ 2+1*3 ]*rhs._y + _data[ 2+2*3 ]*rhs._z
    );
}

//------------------------------------------------------------------------------

inline GeMatrix3 GeMatrix3::operator+( const GeMatrix3& rhs ) const
{
    GeMatrix3 temp( *this );
    temp += rhs;
    return temp;
}

//------------------------------------------------------------------------------

inline GeMatrix3 GeMatrix3::operator-( const GeMatrix3& rhs ) const
{
    GeMatrix3 temp( *this );
    temp -= rhs;
    return temp;
}

//------------------------------------------------------------------------------

inline Float GeMatrix3::getTrace() const
{
    return _data[ 0 ] + _data[ 4 ] + _data[ 8 ];
}

//------------------------------------------------------------------------------

inline Float GeMatrix3::infinityNorm() const
{
    return std::max(
        std::max(
            std::max(
                std::max( _data[ 0 ], _data[ 1 ] ),
                std::max( _data[ 2 ], _data[ 3 ] )
            ),
            std::max(
                std::max( _data[ 4 ], _data[ 5 ] ),
                std::max( _data[ 6 ], _data[ 7 ] )
            )
        ),
        _data[ 8 ]
    );
}

//------------------------------------------------------------------------------

inline GeMatrix3 GeMatrix3::getTranspose() const
{
    return GeMatrix3::rowMajor( _data );
}

//------------------------------------------------------------------------------

inline const Float* GeMatrix3::asColMajor() const
{
    return _data;
}


////////////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTIONS
//

//------------------------------------------------------------------------------

inline GeMatrix3 operator*( Float lhs, const GeMatrix3& rhs )
{
    return rhs * lhs;
}

//------------------------------------------------------------------------------

inline GeVector operator*( const GeVector& lhs, const GeMatrix3& rhs )
{
    return GeVector(
        lhs._x * rhs(0,0) + lhs._y * rhs(1,0) + lhs._z * rhs(2,0),
        lhs._x * rhs(0,1) + lhs._y * rhs(1,1) + lhs._z * rhs(2,1),
        lhs._x * rhs(0,2) + lhs._y * rhs(1,2) + lhs._z * rhs(2,2)
    );
}

FREECLOTH_NAMESPACE_END

#endif
