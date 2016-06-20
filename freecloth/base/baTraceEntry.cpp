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

#include <freecloth/base/baTraceEntry.h>
#include <freecloth/base/baTraceStream.h>

FREECLOTH_NAMESPACE_START

#ifndef NDEBUG
////////////////////////////////////////////////////////////////////////////////
// CLASS BaTraceEntry

//------------------------------------------------------------------------------

BaTraceEntry::BaTraceEntry()
{
    BaTraceStream& stream = BaTraceStream::theTraceStream();

    stream.getStream()
        << stream.startl( BaTraceStream::LINE_INDENT );
}

//------------------------------------------------------------------------------

BaTraceEntry::~BaTraceEntry()
{
    BaTraceStream& stream = BaTraceStream::theTraceStream();

    stream.unindent();
    stream.getStream()
        << stream.startl( BaTraceStream::LINE_UNINDENT )
        << std::endl;
}

//------------------------------------------------------------------------------

std::ostream& BaTraceEntry::getStream()
{
    return BaTraceStream::theTraceStream().getStream();
}

//------------------------------------------------------------------------------

void BaTraceEntry::doneOutput()
{
    BaTraceStream& stream = BaTraceStream::theTraceStream();

    stream.getStream() << std::endl;
    stream.indent();
}
#endif

FREECLOTH_NAMESPACE_END
