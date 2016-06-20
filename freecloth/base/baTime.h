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

#ifndef freecloth_base_baTime_h
#define freecloth_base_baTime_h

#ifndef freecloth_base_package_h
#include <freecloth/base/package.h>
#endif

#ifndef freecloth_base_types_h
#include <freecloth/base/types.h>
#endif

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
/*!
 * \class BaTime freecloth/base/baTime.h
 * \brief Basic measurement of time.
 *
 * Defines instants and durations, and methods for retrieving the current time
 * and measuring durations.
 */
class BaTime
{
public:
    //----- types and enumerations -----
    //! A period of time.
    typedef Int32  Duration;
    //! A single point in time.
    typedef UInt32  Instant;

    enum {
        MS = 1,
        S = 1000,
        MIN = S * 60
    };

    // ----- static member functions -----

    //! Retrieve the current time.
    static Instant getTime();

    //! Calculate the elapsed time between two instants.
    static Duration getDuration( Instant start, Instant end );

    //! Return a floating point value representing the given instant as a
    //! number, measured in seconds.
    static Float instantAsSeconds( Instant );
    //! Return a floating point value representing the given duration as a
    //! number, measured in seconds.
    static Float durationAsSeconds( Duration );
    static Instant floatAsInstant( Float );
    static Duration floatAsDuration( Float );
};

FREECLOTH_NAMESPACE_END

#include <freecloth/base/baTime.inline.h>

#endif
