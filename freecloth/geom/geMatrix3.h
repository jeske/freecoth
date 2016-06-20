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

#ifndef freecloth_geom_geMatrix3_h
#define freecloth_geom_geMatrix3_h

#ifndef freecloth_geom_package_h
#include <freecloth/geom/package.h>
#endif

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// FORWARD DECLARATIONS

class GeVector;
class GePoint;

////////////////////////////////////////////////////////////////////////////////
/*!
 * \class GeMatrix3 freecloth/geom/geMatrix3.h
 * \brief A simple 3x3 matrix class.
 */
class GeMatrix3
{
public:
    // ----- static member functions -----
    
    //! Named constructor: construct from float array, column-major storage
    static GeMatrix3 colMajor( const Float data[ 9 ] );
    //! Named constructor: construct from float array, row-major storage
    static GeMatrix3 rowMajor( const Float data[ 9 ] );
    //! Named constructor: construct a scaling matrix, with [sx sy sz] along
    //! the main diagonal
    static GeMatrix3 scaling( Float sx, Float sy, Float sz );
    //! Named constructor: construct a rotation matrix, representing a
    //! rotation of theta about the given axis.
    static GeMatrix3 rotation( const GeVector& axis, Float theta );
    //! Named constructor: create matrix M = a * b^T
    static GeMatrix3 outerProduct( const GeVector& a, const GeVector& b );
    //! Named constructor: create identity matrix. This is implemented this
    //! way (instead of as a static member) so that the compiler knows the
    //! contents of the matrix and can optimise it out.
    static GeMatrix3 identity();
    //! Named constructor: create zero matrix
    static GeMatrix3 zero();

    // ----- member functions -----
    
    GeMatrix3();
    GeMatrix3(
        Float entry00, Float entry01, Float entry02,
        Float entry10, Float entry11, Float entry12,
        Float entry20, Float entry21, Float entry22
    );
    // Default copy constructor is fine.
    
    // Default assignment operator is fine.
    Float operator()( UInt32 row, UInt32 col ) const;
    Float& operator()( UInt32 row, UInt32 col );
    GeMatrix3 operator-() const;
    GeMatrix3& operator*=( const GeMatrix3& );
    GeMatrix3& operator*=( Float );
    GeMatrix3& operator+=( const GeMatrix3& );
    GeMatrix3& operator-=( const GeMatrix3& );
    GeMatrix3 operator*( const GeMatrix3& ) const;

    GeVector operator*( const GeVector& ) const;
    GePoint operator*( const GePoint& ) const;

    GeMatrix3 operator*( Float ) const;
    GeMatrix3 operator+( const GeMatrix3& ) const;
    GeMatrix3 operator-( const GeMatrix3& ) const;
    bool operator==( const GeMatrix3& ) const;
    bool operator!=( const GeMatrix3& ) const;

    //! Sum of diagonal elements.
    Float getTrace() const;
    //! Not the standard definition... max of all elements
    Float infinityNorm() const;
    //! Retrieve data as a flat array, column-major storage
    const Float* asColMajor() const;
    GeMatrix3 getTranspose() const;
    GeMatrix3 getInverse() const;

private:
    // ----- member functions -----

    //! Private. Clients should use named constructors colMajor or rowMajor.
    GeMatrix3( const Float data[ 9 ] );

    // ----- data members -----

    //! Stored in column-major format.
    Float _data[ 9 ];
};


////////////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTIONS
//

//! Debug output.
std::ostream& operator<<( std::ostream&, const GeMatrix3& );
//! Scalar-matrix multiplication
GeMatrix3 operator*( Float, const GeMatrix3& );
//! Multiply row vector by matrix, v^T * M
GeVector operator*( const GeVector&, const GeMatrix3& );

FREECLOTH_NAMESPACE_END

#include <freecloth/geom/geMatrix3.inline.h>

#endif
