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

#include <freecloth/geom/geMatrix3.h>

////////////////////////////////////////////////////////////////////////////////
// LOCAL DECLARATIONS

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// CLASS GeMatrix3

//------------------------------------------------------------------------------

GeMatrix3 GeMatrix3::rotation( const GeVector& axis, Float theta )
{
    const Float s = BaMath::sin( theta );
    const Float c = BaMath::cos( theta );
    const Float t = 1-c;
    const Float x = axis._x, y = axis._y, z = axis._z;
    return GeMatrix3(
        t*x*x + c,   t*x*y - s*z, t*x*z + s*y,
        t*x*y + s*z, t*y*y + c,   t*y*z - s*x,
        t*x*z - s*y, t*y*z + s*x, t*z*z + c
    );
}

//------------------------------------------------------------------------------

GeMatrix3 GeMatrix3::operator*( const GeMatrix3& rhs ) const
{
    GeMatrix3 result;
    for ( UInt32 r = 0; r < 3; ++r ) {
        for ( UInt32 c = 0; c < 3; ++c ) {
            Float val = 0;
            for ( UInt32 i = 0; i < 3; ++i ) {
                val += operator()( r, i ) * rhs( i, c );
            }
            result( r, c ) = val;
        }
    }
    return result;
}

//------------------------------------------------------------------------------

GeMatrix3 GeMatrix3::getInverse() const
{
    GeMatrix3 result(
        operator()(1,1) * operator()(2,2) - operator()(1,2) * operator()(2,1),
        operator()(0,2) * operator()(2,1) - operator()(0,1) * operator()(2,2),
        operator()(0,1) * operator()(1,2) - operator()(0,2) * operator()(1,1),

        operator()(1,2) * operator()(2,0) - operator()(1,0) * operator()(2,2),
        operator()(0,0) * operator()(2,2) - operator()(0,2) * operator()(2,0),
        operator()(0,2) * operator()(1,0) - operator()(0,0) * operator()(1,2),

        operator()(1,0) * operator()(2,1) - operator()(1,1) * operator()(2,0),
        operator()(0,1) * operator()(2,0) - operator()(0,0) * operator()(2,1),
        operator()(0,0) * operator()(1,1) - operator()(0,1) * operator()(1,0)
    );

    Float det =
        operator()(0,0) * result(0,0) +
        operator()(0,1) * result(1,0) +
        operator()(0,2) * result(2,0);

    DGFX_ASSERT( ! BaMath::isEqual( BaMath::abs( det ), 0 ) );

    Float invDet = 1.0f / det;
    for( UInt32 i = 0; i < 9; ++i ) {
        result._data[ i ] *= invDet;
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTIONS
//

//------------------------------------------------------------------------------

std::ostream& operator<<( std::ostream& out, const GeMatrix3& m )
{
    out << "M3(" << std::endl;
    out << "  " << m(0,0) << " " << m(0,1) << " " << m(0,2) << std::endl;
    out << "  " << m(1,0) << " " << m(1,1) << " " << m(1,2) << std::endl;
    out << "  " << m(2,0) << " " << m(2,1) << " " << m(2,2) << std::endl;
    out << ")";
    return out;
}

FREECLOTH_NAMESPACE_END
