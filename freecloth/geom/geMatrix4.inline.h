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

#ifndef freecloth_geom_geMatrix4_inline_h
#define freecloth_geom_geMatrix4_inline_h

#ifndef freecloth_geom_geVector_h
#include <freecloth/geom/geVector.h>
#endif

#ifndef freecloth_geom_gePoint_h
#include <freecloth/geom/gePoint.h>
#endif

#ifndef freecloth_geom_geMatrix3_h
#include <freecloth/geom/geMatrix3.h>
#endif

#ifndef freecloth_base_baMath_h
#include <freecloth/base/baMath.h>
#endif


FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// CLASS GeMatrix4

//------------------------------------------------------------------------------

inline GeMatrix4::GeMatrix4()
{}

//------------------------------------------------------------------------------

inline GeMatrix4::GeMatrix4(
    Float d00, Float d01, Float d02, Float d03,
    Float d10, Float d11, Float d12, Float d13,
    Float d20, Float d21, Float d22, Float d23,
    Float d30, Float d31, Float d32, Float d33
) {
    _data[ 0 ] = d00; _data[ 4 ] = d01; _data[ 8 ] = d02; _data[ 12 ] = d03;
    _data[ 1 ] = d10; _data[ 5 ] = d11; _data[ 9 ] = d12; _data[ 13 ] = d13;
    _data[ 2 ] = d20; _data[ 6 ] = d21; _data[ 10 ] = d22; _data[ 14 ] = d23;
    _data[ 3 ] = d30; _data[ 7 ] = d31; _data[ 11 ] = d32; _data[ 15 ] = d33;
}

//------------------------------------------------------------------------------

inline GeMatrix4::GeMatrix4( const Float data[ 16 ] )
{
    std::copy( data, data + 16, _data );
}

//------------------------------------------------------------------------------

inline GeMatrix4 GeMatrix4::colMajor( const Float data[ 16 ] )
{
    return GeMatrix4( data );
}

//------------------------------------------------------------------------------

inline GeMatrix4 GeMatrix4::rowMajor( const Float data[ 16 ] )
{
    return GeMatrix4(
        data[ 0 ], data[ 4 ], data[ 8 ], data[ 12 ],
        data[ 1 ], data[ 5 ], data[ 9 ], data[ 13 ],
        data[ 2 ], data[ 6 ], data[ 10 ],data[ 14 ],
        data[ 3 ], data[ 7 ], data[ 11 ],data[ 15 ]
    );
}

//------------------------------------------------------------------------------

inline GeMatrix4 GeMatrix4::rotation( const GeMatrix3& m3 )
{
    return GeMatrix4(
        m3(0,0), m3(0,1), m3(0,2), 0,
        m3(1,0), m3(1,1), m3(1,2), 0,
        m3(2,0), m3(2,1), m3(2,2), 0,
        0, 0, 0, 1
    );
}

//------------------------------------------------------------------------------

inline GeMatrix4 GeMatrix4::rotation( const GeVector& axis, Float theta )
{
    return rotation( GeMatrix3::rotation( axis, theta ) );
}

//------------------------------------------------------------------------------

inline GeMatrix4 GeMatrix4::translation( const GeVector& v )
{
    return GeMatrix4(
        1, 0, 0, v._x,
        0, 1, 0, v._y,
        0, 0, 1, v._z,
        0, 0, 0, 1
    );
}

//------------------------------------------------------------------------------

inline GeMatrix4 GeMatrix4::scaling( Float sx, Float sy, Float sz )
{
    return GeMatrix4(
        sx, 0, 0, 0,
        0, sy, 0, 0,
        0, 0, sz, 0,
        0, 0, 0, 1
    );
}

//------------------------------------------------------------------------------

inline GeMatrix4 GeMatrix4::outerProduct( const GeVector& a, const GeVector& b )
{
    return GeMatrix4(
        a._x * b._x, a._x * b._y, a._x * b._z, 0,
        a._y * b._x, a._y * b._y, a._y * b._z, 0,
        a._z * b._x, a._z * b._y, a._z * b._z, 0,
        0, 0, 0, 1
    );
}

//------------------------------------------------------------------------------

inline GeMatrix4 GeMatrix4::from3x3( const GeMatrix3& m3 )
{
    return GeMatrix4(
        m3( 0, 0 ), m3( 0, 1 ), m3( 0, 2 ), 0,
        m3( 1, 0 ), m3( 1, 1 ), m3( 1, 2 ), 0,
        m3( 2, 0 ), m3( 2, 1 ), m3( 2, 2 ), 0,
        0, 0, 0, 1
    );
}

//------------------------------------------------------------------------------

inline GeMatrix4 GeMatrix4::identity()
{
    static const Float entries[] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    return GeMatrix4( entries );
}

//------------------------------------------------------------------------------

inline GeMatrix4 GeMatrix4::zero()
{
    static const Float entries[] = {
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
    };
    return GeMatrix4( entries );
}

//------------------------------------------------------------------------------

inline Float& GeMatrix4::operator()( UInt32 row, UInt32 col )
{
    DGFX_ASSERT( row < 4 && col < 4 );
    return _data[ col * 4 + row ];
}

//------------------------------------------------------------------------------

inline Float GeMatrix4::operator()( UInt32 row, UInt32 col ) const
{
    DGFX_ASSERT( row < 4 && col < 4 );
    return _data[ col * 4 + row ];
}

//------------------------------------------------------------------------------

inline GeMatrix4 GeMatrix4::operator-() const
{
    return GeMatrix4(
        -_data[0], -_data[1], -_data[2], -_data[3],
        -_data[4], -_data[5], -_data[6], -_data[7],
        -_data[8], -_data[9], -_data[10],-_data[11],
        -_data[12],-_data[13],-_data[14],-_data[15]
    );
}

//------------------------------------------------------------------------------

inline bool GeMatrix4::operator==( const GeMatrix4& rhs ) const
{
    bool result = true;
    for( UInt32 i = 0; i < 16; ++i ) {
        result = result && BaMath::isEqual( _data[ i ], rhs._data[ i ] );
    }
    return result;
}

//------------------------------------------------------------------------------

inline bool GeMatrix4::operator!=( const GeMatrix4& rhs ) const
{
    return !operator==( rhs );
}

//------------------------------------------------------------------------------

inline GeMatrix4& GeMatrix4::operator*=( const GeMatrix4& rhs )
{
    return operator=( operator*( rhs ) );
}

//------------------------------------------------------------------------------

inline GeMatrix4& GeMatrix4::operator*=( Float rhs )
{
    for( UInt32 i = 0; i < 16; ++i ) {
        _data[ i ] *= rhs;
    }
    return *this;
}

//------------------------------------------------------------------------------

inline GeMatrix4& GeMatrix4::operator+=( const GeMatrix4& rhs )
{
    for( UInt32 i = 0; i < 16; ++i ) {
        _data[ i ] += rhs._data[ i ];
    }
    return *this;
}

//------------------------------------------------------------------------------

inline GeMatrix4& GeMatrix4::operator-=( const GeMatrix4& rhs )
{
    for( UInt32 i = 0; i < 16; ++i ) {
        _data[ i ] -= rhs._data[ i ];
    }
    return *this;
}

//------------------------------------------------------------------------------

inline GeMatrix4 GeMatrix4::operator*( Float rhs ) const
{
    GeMatrix4 temp( *this );
    temp *= rhs;
    return temp;
}

//------------------------------------------------------------------------------

inline GeVector GeMatrix4::operator*( const GeVector& rhs ) const
{
    // _data[ c*4+r ]
    return GeVector(
        _data[ 0*4+0 ]*rhs._x + _data[ 1*4+0 ]*rhs._y + _data[ 2*4+0 ]*rhs._z,
        _data[ 0*4+1 ]*rhs._x + _data[ 1*4+1 ]*rhs._y + _data[ 2*4+1 ]*rhs._z,
        _data[ 0*4+2 ]*rhs._x + _data[ 1*4+2 ]*rhs._y + _data[ 2*4+2 ]*rhs._z
    );
}

//------------------------------------------------------------------------------

inline GePoint GeMatrix4::operator*( const GePoint& rhs ) const
{
    // _data[ c*4+r ]
    return GePoint(
        _data[ 0*4+0 ]*rhs._x + _data[ 1*4+0 ]*rhs._y + _data[ 2*4+0 ]*rhs._z
            + _data[ 3*4+0 ],
        _data[ 0*4+1 ]*rhs._x + _data[ 1*4+1 ]*rhs._y + _data[ 2*4+1 ]*rhs._z
            + _data[ 3*4+1 ],
        _data[ 0*4+2 ]*rhs._x + _data[ 1*4+2 ]*rhs._y + _data[ 2*4+2 ]*rhs._z
            + _data[ 3*4+2 ]
    );
}

//------------------------------------------------------------------------------

inline GeMatrix4 GeMatrix4::operator+( const GeMatrix4& rhs ) const
{
    GeMatrix4 temp( *this );
    temp += rhs;
    return temp;
}

//------------------------------------------------------------------------------

inline GeMatrix4 GeMatrix4::operator-( const GeMatrix4& rhs ) const
{
    GeMatrix4 temp( *this );
    temp -= rhs;
    return temp;
}

//------------------------------------------------------------------------------

inline Float GeMatrix4::getTrace() const
{
    return _data[ 0 ] + _data[ 5 ] + _data[ 10 ] + _data[ 15 ];
}

//------------------------------------------------------------------------------

inline GeMatrix4 GeMatrix4::getTranspose() const
{
    return GeMatrix4::rowMajor( _data );
}

//------------------------------------------------------------------------------

inline const Float* GeMatrix4::asColMajor() const
{
    return _data;
}


////////////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTIONS
//

//------------------------------------------------------------------------------

inline GeMatrix4 operator*( Float lhs, const GeMatrix4& rhs )
{
    return rhs * lhs;
}

FREECLOTH_NAMESPACE_END

#endif
