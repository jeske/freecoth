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

#ifndef freecloth_base_types_h
#define freecloth_base_types_h

#ifndef freecloth_base_package_h
#include <freecloth/base/package.h>
#endif

#ifndef freecloth_base_string
#include <freecloth/base/string>
#endif

FREECLOTH_NAMESPACE_START

typedef unsigned int    UInt32;
typedef signed int      Int32;
typedef unsigned short  UInt16;
typedef signed short    Int16;
typedef unsigned char   UInt8;
typedef signed char     Int8;

typedef float           Float;
typedef std::string     String;

FREECLOTH_NAMESPACE_END

#endif
