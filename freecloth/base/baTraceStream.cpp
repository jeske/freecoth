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

#include <freecloth/base/baTraceStream.h>
#include <freecloth/base/debug.h>
#include <freecloth/base/fstream>

FREECLOTH_NAMESPACE_START

#ifndef NDEBUG
////////////////////////////////////////////////////////////////////////////////
// CLASS BaTraceStream::Imp

class BaTraceStream::Imp {
public:
    // ----- member functions -----
    Imp();

    // ----- data members -----
    std::ofstream   _stream;
    UInt32          _nbIndents;
};

//------------------------------------------------------------------------------

BaTraceStream::Imp::Imp()
    : _stream( "debug_trace.txt" ),
    _nbIndents( 0 )
{
    // FIXME: include date, time, etc. in header
    _stream << "// debug_trace.txt  " << std::endl;
    _stream << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
// CLASS BaTraceStream

BaTraceStream BaTraceStream::_theTraceStream;

//------------------------------------------------------------------------------

BaTraceStream& BaTraceStream::theTraceStream()
{
    return _theTraceStream;
}

//------------------------------------------------------------------------------

// FIXME: don't generate file for non-debug builds.
BaTraceStream::BaTraceStream()
    : _imp( new Imp )
{
}

//------------------------------------------------------------------------------

BaTraceStream::~BaTraceStream()
{
    delete _imp;
}

//------------------------------------------------------------------------------

std::ostream& BaTraceStream::getStream()
{
    return _imp->_stream;
}

//------------------------------------------------------------------------------

String BaTraceStream::startl( BaTraceStream::LineType lineType ) const
{
    String retVal;
    for ( UInt32 i = 0; i < _imp->_nbIndents; ++i ) {
        retVal += "  ";
    }
    switch ( lineType ) {
        case LINE_NORMAL: {
            retVal += "| ";
        } break;

        case LINE_INDENT: {
            retVal += "\\ ";
        } break;

        case LINE_UNINDENT: {
            retVal += "/ ";
        } break;
    }
    return retVal;
}

//------------------------------------------------------------------------------

void BaTraceStream::indent()
{
    ++_imp->_nbIndents;
}

//------------------------------------------------------------------------------

void BaTraceStream::unindent()
{
    DGFX_ASSERT( _imp->_nbIndents > 0 );
    --_imp->_nbIndents;
}
#endif

FREECLOTH_NAMESPACE_END
