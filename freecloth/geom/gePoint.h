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

#ifndef freecloth_geom_gePoint_h
#define freecloth_geom_gePoint_h

#ifndef freecloth_geom_package_h
#include <freecloth/geom/package.h>
#endif

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// FORWARD DECLARATIONS

class GeVector;

////////////////////////////////////////////////////////////////////////////////
/*!
 * \class GePoint freecloth/geom/gePoint.h
 * \brief A class for 3D points.
 *
 * In keeping with the principles of affine spaces, we distinguish between
 * vectors and points, which represent direction and position, respectively.
 */
class GePoint
{
public:

    // ----- classes -----

    //! Comparison functors for sorting or working with points
    //  TODO: add combined comparators that treat first one co-ord,
    //  then the other.
    class CompX {
    public:
        CompX() { }
        inline bool operator() ( const GePoint& a, const GePoint& b ) const
        { return a._x < b._x; }
    };
    //! Comparison functors for sorting or working with points
    class CompY {
    public:
        CompY() { }
        inline bool operator() ( const GePoint& a, const GePoint& b ) const
        { return a._y < b._y; }
    };
    //! Comparison functors for sorting or working with points
    class CompZ {
    public:
        CompZ() { }
        inline bool operator() ( const GePoint& a, const GePoint& b ) const
        { return a._z < b._z; }
    };

    // ----- member functions -----
    
    GePoint();
    GePoint( Float x, Float y, Float z );
    //! Construct from linear combination. Result = fracA * a + (1-fracA) * b
    GePoint( const GePoint& a, const GePoint& b, Float fracA );
    //! Construct from linear combination. fracA and fracB constitute two of
    //! three barycentric co-ordinates; (fracA,fracB,1-fracA-fracB) are the
    //! full three.
    GePoint(
        const GePoint& a, const GePoint& b, const GePoint& c,
        Float fracA, Float fracB
    );
    // Default copy constructor is fine.

    // Default assignment operator is fine.
    GePoint& operator+=( const GeVector& );
    GePoint& operator-=( const GeVector& );
    GePoint operator+( const GeVector& ) const;
    GePoint operator-( const GeVector& ) const;
    GeVector operator-( const GePoint& ) const;
    bool operator==( const GePoint& ) const;
    bool operator!=( const GePoint& ) const;
    Float operator[]( UInt32 ) const;
    Float& operator[]( UInt32 );

    // ----- static data members -----

    static const CompX _compX;
    static const CompY _compY;
    static const CompZ _compZ;

    static const GePoint NIL;
    static const GePoint ZERO;

    // ----- data members -----

    Float _x,_y,_z;
};


////////////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTIONS
//
std::ostream& operator<<( std::ostream&, const GePoint& );

FREECLOTH_NAMESPACE_END

#include <freecloth/geom/gePoint.inline.h>

#endif
