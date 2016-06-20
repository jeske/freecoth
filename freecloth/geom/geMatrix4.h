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

#ifndef freecloth_geom_geMatrix4_h
#define freecloth_geom_geMatrix4_h

#ifndef freecloth_geom_package_h
#include <freecloth/geom/package.h>
#endif

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// FORWARD DECLARATIONS

class GeVector;
class GePoint;
class GeMatrix3;

////////////////////////////////////////////////////////////////////////////////
/*!
 * \class GeMatrix4 freecloth/geom/geMatrix4.h
 * \brief A simple 4x4 matrix class.
 */
class GeMatrix4
{
public:
    // ----- static member functions -----
    
    //! Named constructor: construct from float array, column-major storage
    static GeMatrix4 colMajor( const Float data[ 16 ] );
    //! Named constructor: construct from float array, row-major storage
    static GeMatrix4 rowMajor( const Float data[ 16 ] );
    //! Named constructor: construct a rotation matrix: the top-left 3x3 block
    //! filled with the given data.
    static GeMatrix4 rotation( const GeMatrix3& );
    //! Named constructor: construct a rotation matrix, representing a
    //! rotation of theta about the given axis.
    static GeMatrix4 rotation( const GeVector& axis, Float theta );
    //! Named constructor: construct a translation matrix: an identity matrix,
    //! plus the given vector in the fourth column.
    static GeMatrix4 translation( const GeVector& );
    //! Named constructor: construct a scaling matrix, with [sx sy sz 1] along
    //! the main diagonal
    static GeMatrix4 scaling( Float sx, Float sy, Float sz );
    //! Named constructor: create matrix M = a * b^T
    static GeMatrix4 outerProduct( const GeVector& a, const GeVector& b );
    //! Named constructor: embeds 3x3 matrix in the top-left of an identity
    //! matrix
    static GeMatrix4 from3x3( const GeMatrix3& );
    //! Named constructor: create identity matrix. This is implemented this
    //! way (instead of as a static member) so that the compiler knows the
    //! contents of the matrix and can optimise it out.
    static GeMatrix4 identity();
    //! Named constructor: create zero matrix
    static GeMatrix4 zero();


    // ----- member functions -----
    
    GeMatrix4();
    GeMatrix4(
        Float entry00, Float entry01, Float entry02, Float entry03,
        Float entry10, Float entry11, Float entry12, Float entry13,
        Float entry20, Float entry21, Float entry22, Float entry23,
        Float entry30, Float entry31, Float entry32, Float entry33
    );

    // Default copy constructor is fine.

    // Default assignment operator is fine.
    Float operator()( UInt32 row, UInt32 col ) const;
    Float& operator()( UInt32 row, UInt32 col );
    GeMatrix4 operator-() const;
    GeMatrix4& operator*=( const GeMatrix4& );
    GeMatrix4& operator*=( Float );
    GeMatrix4& operator+=( const GeMatrix4& );
    GeMatrix4& operator-=( const GeMatrix4& );
    GeMatrix4 operator*( const GeMatrix4& ) const;
    //! Matrix-vector multiplication. Treats vector as 4x1, with last element
    //! zero.
    GeVector operator*( const GeVector& ) const;
    //! Matrix-vector multiplication. Treats point as 4x1, with last element
    //! one.
    GePoint operator*( const GePoint& ) const;
    GeMatrix4 operator*( Float ) const;
    GeMatrix4 operator+( const GeMatrix4& ) const;
    GeMatrix4 operator-( const GeMatrix4& ) const;
    bool operator==( const GeMatrix4& ) const;
    bool operator!=( const GeMatrix4& ) const;

    //! Sum of diagonal elements.
    Float getTrace() const;
    const Float* asColMajor() const;
    GeMatrix4 getTranspose() const;

private:
    // ----- member functions -----

    //! Private. Clients should use named constructors colMajor or rowMajor.
    GeMatrix4( const Float data[ 16 ] );

    // ----- data members -----

    // Stored in column-major format.
    Float _data[ 16 ];
};


////////////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTIONS
//

std::ostream& operator<<( std::ostream&, const GeMatrix4& );
//! Scalar-matrix multiplication.
GeMatrix4 operator*( Float, const GeMatrix4& );

FREECLOTH_NAMESPACE_END

#include <freecloth/geom/geMatrix4.inline.h>

#endif
