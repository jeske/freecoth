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

#ifndef freecloth_base_baStringUtil_h
#define freecloth_base_baStringUtil_h

#ifndef freecloth_base_package_h
#include <freecloth/base/package.h>
#endif

#ifndef freecloth_base_types_h
#include <freecloth/base/types.h>
#endif

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
/*!
 * \class BaStringUtil freecloth/base/baStringUtil.h
 * \brief Utilities for manipulating strings.
 *
 * These are very basic, aren't compliant with any good text processing
 * standards, can't handle Unicode, etc.
 */
class BaStringUtil {
public:
    // ----- static member functions -----

    //! Convert to lowercase.
    static String toLower( const String& );
    //! Convert to uppercase.
    static String toUpper( const String& );
    //! Convert to Float
    static Float toFloat( const String& );
    //! Convert to Int32
    static Int32 toInt32( const String& );
    //! Convert from UInt32
    static String fromUInt32( UInt32 );
    //! Convert from UInt32
    static String fromUInt32( UInt32, UInt32 zeroPad );
    //! Convert from Float
    static String fromFloat( Float );
    //! Convert from Float
    static String fromFloat( Float, UInt32 nbFracDigits );
};

FREECLOTH_NAMESPACE_END
 
#endif
