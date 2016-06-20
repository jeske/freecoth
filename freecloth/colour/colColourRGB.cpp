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

#include <freecloth/colour/colColourRGB.h>

////////////////////////////////////////////////////////////////////////////////
// LOCAL DECLARATIONS

////////////////////////////////////////////////////////////////////////////////
// CLASS ColColourRGB

const ColColourRGB ColColourRGB::BLACK( 0.f, 0.f, 0.f );
const ColColourRGB ColColourRGB::WHITE( 1.f, 1.f, 1.f );
const ColColourRGB ColColourRGB::RED( 1.f, 0.f, 0.f );
const ColColourRGB ColColourRGB::GREEN( 0.f, 1.f, 0.f );
const ColColourRGB ColColourRGB::BLUE( 0.f, 0.f, 1.f );
const ColColourRGB ColColourRGB::CYAN( 0.f, 1.f, 1.f );
const ColColourRGB ColColourRGB::MAGENTA( 1.f, 0.f, 1.f );
const ColColourRGB ColColourRGB::YELLOW( 1.f, 1.f, 0.f );

//------------------------------------------------------------------------------

ColColourRGB::ColColourRGB()
{}

//------------------------------------------------------------------------------

ColColourRGB::ColColourRGB( Float r, Float g, Float b )
    : _r( r ), _g( g ), _b( b )
{}

//------------------------------------------------------------------------------

ColColourRGB::ColColourRGB(
    const ColColourRGB& colA,
    const ColColourRGB& colB,
    Float fracA
) : _r( colA._r * fracA + colB._r * ( 1 - fracA ) ),
    _g( colA._g * fracA + colB._g * ( 1 - fracA ) ),
    _b( colA._b * fracA + colB._b * ( 1 - fracA ) )
{
}

//------------------------------------------------------------------------------

ColColourRGB& ColColourRGB::operator*=( Float val )
{
    _r *= val;
    _g *= val;
    _b *= val;
    return *this;
}

//------------------------------------------------------------------------------

ColColourRGB ColColourRGB::operator*( Float val ) const
{
    ColColourRGB result( *this );
    result *= val;
    return result;
}

////////////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTIONS
//

//------------------------------------------------------------------------------

std::ostream& operator<<( std::ostream& os, const ColColourRGB& c )
{
    os << "rgb(" << c._r << "," << c._g << "," << c._b << ")";
    return os;
}
