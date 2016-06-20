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

#ifndef freecloth_base_baTraceEntry_h
#define freecloth_base_baTraceEntry_h

#ifndef freecloth_base_package_h
#include <freecloth/base/package.h>
#endif

#ifndef freecloth_base_types_h
#include <freecloth/base/types.h>
#endif

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
/*!
 * \internal
 * \class BaTraceEntry freecloth/base/baTraceEntry.h
 * \brief Utility class used by DGFX_TRACE_ENTER macro.
 *
 * On construction, increases the trace indent level, and on destruction
 * decreases it.
 */
class BaTraceEntry {

public:
    // ----- member functions -----
    BaTraceEntry();
    ~BaTraceEntry();

    std::ostream& getStream();
    void doneOutput();

private:

    // ----- data members -----

};

FREECLOTH_NAMESPACE_END
#endif
