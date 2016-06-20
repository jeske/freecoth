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

#ifndef freecloth_base_baTraceStream_h
#define freecloth_base_baTraceStream_h

#ifndef freecloth_base_package_h
#include <freecloth/base/package.h>
#endif

#ifndef freecloth_base_types_h
#include <freecloth/base/types.h>
#endif

#ifndef freecloth_base_iostream
#include <freecloth/base/iostream>
#endif

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//

////////////////////////////////////////////////////////////////////////////////
/*!
 * \internal
 * \class BaTraceStream freecloth/base/baTraceStream.h
 * \brief Output stream for DGFX_TRACE_* macros.
 *
 * Maintains indentation level, etc.
 */
class BaTraceStream {

public:
    // ----- types and enumerations -----
    enum LineType {
        LINE_NORMAL,
        LINE_INDENT,
        LINE_UNINDENT
    };

    // ----- static member functions -----
    static BaTraceStream& theTraceStream();

    // ----- member functions -----
    ~BaTraceStream();
    std::ostream& getStream();
    String startl( LineType = LINE_NORMAL ) const;
    void indent();
    void unindent();

private:

    // ----- classes -----
    class Imp;

    // ----- member functions -----
    BaTraceStream();

    // ----- static data members -----
    static BaTraceStream _theTraceStream;

    // ----- data members -----
    Imp* _imp;

};

FREECLOTH_NAMESPACE_END
#endif
