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

#ifndef freecloth_base_baMath_h
#define freecloth_base_baMath_h

#ifndef freecloth_base_package_h
#include <freecloth/base/package.h>
#endif

#ifndef freecloth_base_types_h
#include <freecloth/base/types.h>
#endif

#ifndef freecloth_base_math_h
#include <freecloth/base/math.h>
#endif

#ifndef M_PI
#define M_PI 3.14159f
#endif

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
/*!
 * \class BaMath freecloth/base/baMath.h
 * \brief Container for a variety of useful mathematics functions.
 *
 * This class is intended to provide clean platform-independent access to
 * standard routines. It also groups a number of useful new routines, such
 * as floating-point comparisons using tolerances.
 */

class BaMath
{
public:
    // ----- static data members -----

    static const Float TOLERANCE;

    // ----- static member functions -----

    static Float ceil( Float );
    static Float floor( Float );
    static Float round( Float );

    //@{
    //! Note that no truncation is performed. If the value is out of
    //! range, it will be wrapped around.
    static Int8 ceilInt8( Float );
    static Int8 floorInt8( Float );
    static Int8 roundInt8( Float );
    static UInt8 ceilUInt8( Float );
    static UInt8 floorUInt8( Float );
    static UInt8 roundUInt8( Float );

    static Int16 ceilInt16( Float );
    static Int16 floorInt16( Float );
    static Int16 roundInt16( Float );
    static UInt16 ceilUInt16( Float );
    static UInt16 floorUInt16( Float );
    static UInt16 roundUInt16( Float );

    static Int32 ceilInt32( Float );
    static Int32 floorInt32( Float );
    static Int32 roundInt32( Float );
    static UInt32 ceilUInt32( Float );
    static UInt32 floorUInt32( Float );
    static UInt32 roundUInt32( Float );
    //@}

    //! This will cap values to the range [-128,127]
    static Int8 roundTruncInt8( Float );
    //! This will cap values to the range [0,255]
    static UInt8 roundTruncUInt8( Float );
    //! This will cap values to the range [-32768,32767]
    static Int16 roundTruncInt16( Float );
    //! This will cap values to the range [0,65535]
    static UInt16 roundTruncUInt16( Float );

    static Float abs( Float );

    //! Returns true if input is an integer (or within tolerance)
    static bool isInteger( Float );
    //! Returns true if input is a power of two.
    static bool isPow2( UInt32 );
    //! Rounds input up to nearest power of two.
    static UInt32 roundUpPow2( UInt32 );

    static Float sqr( Float );
    static Float sqrt( Float );

    static Float cos( Float );
    static Float sin( Float );
    static Float tan( Float );
    static Float arccos( Float );
    static Float arcsin( Float );
    static Float arctan( Float );
    static Float arctan2( Float y, Float x );
    static Float sinc( Float );

    static Float exp( Float );
    static Float log( Float );
    static Float log10( Float );

    //! Returns a 32-bit value containing random bits, according to a 
    //! a pseudo-random uniform distribution.
    static UInt32 randomUInt32();
    //! Returns a 32-bit value in the range [0,nbVals) using a pseudo-random
    //! uniform distribution.
    static UInt32 randomUInt32( UInt32 nbVals );

    //! Returns a value between 0 and 1, in a pseudo-random uniform
    //! distribution.
    static Float randomFloat();

    //@{
    //! Values a and b are considered equal if they differ by less than the
    //! tolerance.
    static bool isEqual( Float a, Float b, Float tol = TOLERANCE );
    static bool isGreater( Float a, Float b, Float tol = TOLERANCE );
    static bool isLess( Float a, Float b, Float tol = TOLERANCE );
    //@}
};

FREECLOTH_NAMESPACE_END

#include <freecloth/base/baMath.inline.h>

#endif
