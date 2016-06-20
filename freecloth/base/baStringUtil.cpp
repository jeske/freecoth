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

#include <freecloth/base/baStringUtil.h>
#include <freecloth/base/debug.h>
#include <freecloth/base/string>
#include <freecloth/base/ctype.h>
#include <freecloth/base/stdio.h>

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// CLASS BaStringUtil

//------------------------------------------------------------------------------

String BaStringUtil::toLower( const String& s )
{
    String result( s );

    for( UInt32 i = 0; i < s.length(); ++i ) {
        if( isupper( s[ i ] ) ) {
            result[ i ] = tolower( s[ i ] );
        }
    }
    return result; 
} 
 
//------------------------------------------------------------------------------

String BaStringUtil::toUpper( const String& s )
{
    String result( s );

    for( UInt32 i = 0; i < s.length(); ++i ) {
        if( islower( s[ i ] ) ) {
            result[ i ] = toupper( s[ i ] );
        }
    }
    return result; 
} 

//------------------------------------------------------------------------------

Float BaStringUtil::toFloat( const String& s )
{
    return ::atof( s.c_str() );
} 

//------------------------------------------------------------------------------

Int32 BaStringUtil::toInt32( const String& s )
{
    return ::atol( s.c_str() );
} 

//------------------------------------------------------------------------------

String BaStringUtil::fromUInt32( UInt32 n )
{
    String result;

    if ( n == 0 ) {
        result = "0";
    }
    else {
        char temp[ 2 ] = "1";
        // FIXME: reserve space in result in advance...
        while ( n != 0 ) {
            temp[ 0 ] = n % 10 + '0';
            result = temp + result;
            n /= 10;
        }
    }
    return result;
}

//------------------------------------------------------------------------------

String BaStringUtil::fromUInt32( UInt32 n, UInt32 zeroPad )
{
    char buf[ 256 ];
    DGFX_ASSERT( zeroPad < sizeof(buf) );

    buf[ zeroPad ] = 0;
    for ( UInt32 i = zeroPad; i > 0; --i ) {
        buf[ i - 1 ] = n % 10 + '0';
        n /= 10;
    }
    return String( buf );
}

//------------------------------------------------------------------------------

String BaStringUtil::fromFloat( Float n )
{
    char buf[ 64 ];
    ::sprintf( buf, "%f", n );
    return String( buf );
}

//------------------------------------------------------------------------------

String BaStringUtil::fromFloat( Float n, UInt32 nbFracDigits )
{
    String fmt( "%." + fromUInt32( nbFracDigits ) + "f" );
    char buf[ 64 ];
    ::sprintf( buf, fmt.c_str(), n );
    return String( buf );
}

FREECLOTH_NAMESPACE_END
