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

#ifndef freecloth_base_debug_h
#define freecloth_base_debug_h

#ifndef freecloth_base_package_h
#include <freecloth/base/package.h>
#endif

// Since the debug build will need this for tracing, we'll also include it
// in the release build. We want the two to have similar levels of
// knowledge of standard headers.
#include <freecloth/base/iostream>

#ifdef NDEBUG

#define DGFX_ASSERT(x)
#define DGFX_TRACE_ENTER(x)
#define DGFX_TRACE(x)

#else

#include <assert.h>

#ifndef freecloth_base_baTraceEntry_h
#include <freecloth/base/baTraceEntry.h>
#endif

#ifndef freecloth_base_baTraceStream_h
#include <freecloth/base/baTraceStream.h>
#endif

//! Custom assertion macro.
#define DGFX_ASSERT(x) assert( x )

//! Do a DGFX_TRACE of x at execution, and another one when the current
//! scope ends. Useful for tracking function entry/exit. Indentation
//! of trace output is increased from here to end of scope.
#define DGFX_TRACE_ENTER(x)                             \
    BaTraceEntry theBaTraceEntry;                       \
    theBaTraceEntry.getStream() << x;                   \
    theBaTraceEntry.doneOutput();

//! Dump \p x to tracing stream, typically debug_trace.txt. Only happens in
//! debug builds with NDEBUG flag undefined.
#define DGFX_TRACE(x)                                   \
    BaTraceStream::theTraceStream().getStream()         \
        << BaTraceStream::theTraceStream().startl()     \
        << x                                            \
        << std::endl

#endif


#endif
