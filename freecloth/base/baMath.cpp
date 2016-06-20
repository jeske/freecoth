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

#include <freecloth/base/baMath.h>
#include <freecloth/base/debug.h>
#include <freecloth/base/algorithm>

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// CLASS BaMath

const Float BaMath::TOLERANCE = 10e-4f;

//------------------------------------------------------------------------------

Int8 BaMath::ceilInt8( Float x )
{
    return static_cast<Int8>( ceil( x ) );
}

//------------------------------------------------------------------------------

Int8 BaMath::floorInt8( Float x )
{
    return static_cast<Int8>( floor( x ) );
}

//------------------------------------------------------------------------------

Int8 BaMath::roundInt8( Float x )
{
    return static_cast<Int8>( round( x ) );
}

//------------------------------------------------------------------------------

UInt8 BaMath::ceilUInt8( Float x )
{
    return static_cast<UInt8>( ceil( x ) );
}

//------------------------------------------------------------------------------

UInt8 BaMath::floorUInt8( Float x )
{
    return static_cast<UInt8>( floor( x ) );
}

//------------------------------------------------------------------------------

UInt8 BaMath::roundUInt8( Float x )
{
    return static_cast<UInt8>( round( x ) );
}

//------------------------------------------------------------------------------

Int16 BaMath::ceilInt16( Float x )
{
    return static_cast<Int16>( ceil( x ) );
}

//------------------------------------------------------------------------------

Int16 BaMath::floorInt16( Float x )
{
    return static_cast<Int16>( floor( x ) );
}

//------------------------------------------------------------------------------

Int16 BaMath::roundInt16( Float x )
{
    return static_cast<Int16>( round( x ) );
}

//------------------------------------------------------------------------------

UInt16 BaMath::ceilUInt16( Float x )
{
    return static_cast<UInt16>( ceil( x ) );
}

//------------------------------------------------------------------------------

UInt16 BaMath::floorUInt16( Float x )
{
    return static_cast<UInt16>( floor( x ) );
}

//------------------------------------------------------------------------------

UInt16 BaMath::roundUInt16( Float x )
{
    return static_cast<UInt16>( round( x ) );
}

//------------------------------------------------------------------------------

Int32 BaMath::ceilInt32( Float x )
{
    return static_cast<Int32>( ceil( x ) );
}

//------------------------------------------------------------------------------

Int32 BaMath::floorInt32( Float x )
{
    return static_cast<Int32>( floor( x ) );
}

//------------------------------------------------------------------------------

Int32 BaMath::roundInt32( Float x )
{
    return static_cast<Int32>( round( x ) );
}

//------------------------------------------------------------------------------

UInt32 BaMath::ceilUInt32( Float x )
{
    return static_cast<UInt32>( ceil( x ) );
}

//------------------------------------------------------------------------------

UInt32 BaMath::floorUInt32( Float x )
{
    return static_cast<UInt32>( floor( x ) );
}

//------------------------------------------------------------------------------

UInt32 BaMath::roundUInt32( Float x )
{
    return static_cast<UInt32>( round( x ) );
}

//------------------------------------------------------------------------------

UInt8 BaMath::roundTruncUInt8( Float x )
{
    return static_cast<UInt8>(
        std::min(
            static_cast<Float>( 0xff ),
            std::max( static_cast<Float>( 0x00 ), round( x ) )
        )
    );
}

//------------------------------------------------------------------------------

Int8 BaMath::roundTruncInt8( Float x )
{
    return static_cast<Int8>(
        std::min(
            static_cast<Float>( 0x7f ),
            std::max( static_cast<Float>( -0x80 ), round( x ) )
        )
    );
}

//------------------------------------------------------------------------------

UInt16 BaMath::roundTruncUInt16( Float x )
{
    return static_cast<UInt16>(
        std::min(
            static_cast<Float>( 0xffff ),
            std::max( static_cast<Float>( 0x0000 ), round( x ) )
        )
    );
}

//------------------------------------------------------------------------------

Int16 BaMath::roundTruncInt16( Float x )
{
    return static_cast<Int16>(
        std::min(
            static_cast<Float>( 0x7fff ),
            std::max( static_cast<Float>( -0x8000 ), round( x ) )
        )
    );
}

//------------------------------------------------------------------------------

Float BaMath::abs( Float x )
{
    return static_cast<Float>( ::fabs( x ) );
}

//------------------------------------------------------------------------------

bool BaMath::isPow2( UInt32 x )
{
    return x == roundUpPow2( x );
}

//------------------------------------------------------------------------------

UInt32 BaMath::roundUpPow2( UInt32 x )
{
    for ( UInt32 i = 1 << 31; i > 0; i >>= 1 ) {
        if ( x > i ) {
            return i * 2;
        }
    }
    return 0;
}

//------------------------------------------------------------------------------

Float BaMath::sinc( Float x )
{
    if ( isEqual( x, 0 ) ) {
        return 1.f;
    }
    return sin( M_PI * x ) / ( M_PI * x );
}

//------------------------------------------------------------------------------

UInt32 BaMath::randomUInt32()
{
    // We only know how to handle two different cases of RAND_MAX
    if ( RAND_MAX == 0xffffffff ) {
        return rand();
    } else if ( RAND_MAX == 0x7fffffff ) {
        return rand() + rand();
    }
    DGFX_ASSERT( RAND_MAX == 0xffffffff );
    return 0;
}

//------------------------------------------------------------------------------

UInt32 BaMath::randomUInt32( UInt32 nbVals )
{
    return randomUInt32() % nbVals;
}

//------------------------------------------------------------------------------

Float BaMath::randomFloat()
{
    return rand() / Float( RAND_MAX );
}

//------------------------------------------------------------------------------

bool BaMath::isEqual( Float a, Float b, Float tol )
{
    return fabs( a - b ) < tol;
}

//------------------------------------------------------------------------------

bool BaMath::isGreater( Float a, Float b, Float tol )
{
    return a - b > tol;
}

//------------------------------------------------------------------------------

bool BaMath::isLess( Float a, Float b, Float tol )
{
    return b - a > tol;
}

FREECLOTH_NAMESPACE_END
