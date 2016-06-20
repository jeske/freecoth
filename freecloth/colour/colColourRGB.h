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

#ifndef col_colColourRGB_h
#define col_colColourRGB_h

#ifndef col_package_h
#include <freecloth/colour/package.h>
#endif

////////////////////////////////////////////////////////////////////////////////
// FORWARD DECLARATIONS

////////////////////////////////////////////////////////////////////////////////
/*!
 * \class ColColourRGB freecloth/colour/colColourRGB.h
 * \brief Definition of a colour in an RGB colourspace.
 *
 * Very rudimentary.
 */
class ColColourRGB
{
public:

    // ----- classes -----

    // ----- member functions -----
    
    ColColourRGB();
    ColColourRGB( Float r, Float g, Float b );
    //! Linear combination of two colours.
    ColColourRGB(
        const ColColourRGB& colA,
        const ColColourRGB& colB,
        Float fracA
    );
    // Default copy constructor is fine.
    // Default assignment operator is fine.

    ColColourRGB& operator*=( Float val );
    ColColourRGB operator*( Float val ) const;


    // ----- static data members -----

    static const ColColourRGB BLACK;
    static const ColColourRGB WHITE;
    static const ColColourRGB RED;
    static const ColColourRGB GREEN;
    static const ColColourRGB BLUE;
    static const ColColourRGB CYAN;
    static const ColColourRGB MAGENTA;
    static const ColColourRGB YELLOW;

    // ----- data members -----

    Float _r,_g,_b;
};

////////////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTIONS
//
std::ostream& operator<<( std::ostream&, const ColColourRGB& );

#endif
