//////////////////////////////////////////////////////////////////////
// Copyright (c) 2002-2003 David Pritchard <drpritch@alumni.uwaterloo.ca>
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

#include <freecloth/geom/geMatrix4.h>

////////////////////////////////////////////////////////////////////////////////
// LOCAL DECLARATIONS

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// CLASS GeMatrix4

//------------------------------------------------------------------------------

GeMatrix4 GeMatrix4::operator*( const GeMatrix4& rhs ) const
{
    GeMatrix4 result;
    for ( UInt32 r = 0; r < 4; ++r ) {
        for ( UInt32 c = 0; c < 4; ++c ) {
            Float val = 0;
            for ( UInt32 i = 0; i < 4; ++i ) {
                val += operator()( r, i ) * rhs( i, c );
            }
            result( r, c ) = val;
        }
    }
    return result;
}


////////////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTIONS
//

//------------------------------------------------------------------------------

std::ostream& operator<<( std::ostream& out, const GeMatrix4& m )
{
    out << "M4(" << std::endl;
    out << "  " << m(0,0) << " " << m(0,1) << " "
                << m(0,2) << " " << m(0,3) << std::endl;
    out << "  " << m(1,0) << " " << m(1,1) << " "
                << m(1,2) << " " << m(1,3) << std::endl;
    out << "  " << m(2,0) << " " << m(2,1) << " "
                << m(2,2) << " " << m(2,3) << std::endl;
    out << "  " << m(3,0) << " " << m(3,1) << " "
                << m(3,2) << " " << m(3,3) << std::endl;
    out << ")";
    return out;
}

FREECLOTH_NAMESPACE_END
