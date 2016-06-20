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

#include <freecloth/geom/geVector.h>
#include <freecloth/geom/gePoint.h>

////////////////////////////////////////////////////////////////////////////////
// LOCAL DECLARATIONS

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// CLASS GeVector

const GeVector GeVector::ZERO( 0, 0, 0 );

//------------------------------------------------------------------------------

GeVector::GeVector( const GePoint& p1, const GePoint& p2 )
    : _x( p2._x - p1._x ), _y( p2._y - p1._y), _z( p2._z - p1._z )
{}

//------------------------------------------------------------------------------

Float GeVector::getAngle( const GeVector& rhs ) const
{
    // This one's actually probably better.. revisit someday.
#if 0
    const GeVector lunit( getUnit() );
    const GeVector runit( rhs.getUnit() );
    Float costheta = lunit.dot( runit );
    Float sintheta = lunit.cross( runit );
    return BaMath::arctan2( sintheta, costheta );
#endif
    Float th1 = BaMath::arctan2( _y, _x );
    Float th2 = BaMath::arctan2( rhs._y, rhs._x );
    Float result = th2 - th1;
    while ( BaMath::isLess( result, 0 ) ) result += 2 * M_PI;
    while ( BaMath::isGreater( result, 2 * M_PI ) ) result -= 2 * M_PI;
    return result;
}


////////////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTIONS
//

//------------------------------------------------------------------------------

std::ostream& operator<<( std::ostream& os, const GeVector& v )
{
    os << "v(" << v._x << "," << v._y << "," << v._z << ")";
    return os;
}

FREECLOTH_NAMESPACE_END
