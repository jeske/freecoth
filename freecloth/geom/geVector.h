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

#ifndef freecloth_geom_geVector_h
#define freecloth_geom_geVector_h

#ifndef freecloth_geom_package_h
#include <freecloth/geom/package.h>
#endif

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// FORWARD DECLARATIONS

class GePoint;

////////////////////////////////////////////////////////////////////////////////
/*!
 * \class GeVector freecloth/geom/geVector.h
 * \brief A class for 3D vectors.
 *
 * In keeping with the principles of affine spaces, we distinguish between
 * vectors and points, which represent direction and position, respectively.
 */
class GeVector {

public:

    // ----- static member functions -----

    //! Named constructor: retrieve zero vector. More efficient than the
    //! ZERO static data member.
    static GeVector zero();
    //! Named constructor: retrieve vector for nth axis
    static GeVector axis( UInt32 n );
    //! Named constructor: retrieve vector for x axis
    static GeVector xAxis();
    //! Named constructor: retrieve vector for y axis
    static GeVector yAxis();
    //! Named constructor: retrieve vector for z axis
    static GeVector zAxis();

    // ----- member functions -----
    GeVector();
    GeVector( Float x, Float y, Float z );
    //! Construct a vector from p2-p1
    GeVector( const GePoint& p1, const GePoint& p2 );
    // Default copy constructor is fine.

    // Default assignment operator is fine.
    GeVector& operator+=( const GeVector& );
    GeVector& operator-=( const GeVector& );
    GeVector& operator+=( Float );
    GeVector& operator-=( Float );
    GeVector& operator*=( Float );
    GeVector& operator/=( Float );
    GeVector operator-() const;
    GeVector operator+( const GeVector& ) const;
    GeVector operator-( const GeVector& ) const;
    GeVector operator+( Float ) const;
    GeVector operator-( Float ) const;
    GeVector operator*( Float ) const;
    GeVector operator/( Float ) const;
    bool operator==( const GeVector& ) const;
    bool operator!=( const GeVector& ) const;
    Float operator[]( UInt32 ) const;
    Float& operator[]( UInt32 );

    //! Dot product
    Float dot( const GeVector& ) const;
    //! Cross product
    GeVector cross( const GeVector& ) const;

    //! Vector length (L2 norm)
    Float length() const;
    Float squaredLength() const;
    //! max(|x|,|y|,|z|)
    Float infinityNorm() const;

    bool isUnit() const;
    GeVector getUnit() const;

    // FIXME: this is more of a 2d thing... kill it.
    Float getAngle( const GeVector& ) const;

    // ----- static data members -----
    static const GeVector ZERO;

    // ----- data members -----
    Float _x,_y,_z;
};


////////////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTIONS
//
std::ostream& operator<<( std::ostream&, const GeVector& );
GeVector operator*( Float, GeVector const& );

FREECLOTH_NAMESPACE_END

#include <freecloth/geom/geVector.inline.h>

#endif
