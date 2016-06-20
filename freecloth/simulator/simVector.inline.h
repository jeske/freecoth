//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 David Pritchard <drpritch@alumni.uwaterloo.ca>
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

#ifndef freecloth_simulator_simVector_inline_h
#define freecloth_simulator_simVector_inline_h

#include <freecloth/base/baMath.h>
#include <freecloth/base/algorithm>

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// CLASS SimVector

//------------------------------------------------------------------------------

inline SimVector SimVector::zero( UInt32 size )
{
    SimVector result( size );
    std::fill( result.begin(), result.end(), GeVector::zero() );
    return result;
}

//------------------------------------------------------------------------------

inline SimVector::SimVector()
{}

//------------------------------------------------------------------------------

inline SimVector::SimVector( UInt32 size )
    : _data( size )
{}

//------------------------------------------------------------------------------

inline SimVector::const_iterator SimVector::begin() const
{
    return _data.begin();
}

//------------------------------------------------------------------------------

inline SimVector::const_iterator SimVector::end() const
{
    return _data.end();
}

//------------------------------------------------------------------------------

inline SimVector::iterator SimVector::begin()
{
    return _data.begin();
}

//------------------------------------------------------------------------------

inline SimVector::iterator SimVector::end()
{
    return _data.end();
}

//------------------------------------------------------------------------------

inline UInt32 SimVector::size() const
{
    return _data.size();
}

//------------------------------------------------------------------------------

inline SimVector& SimVector::operator+=( const SimVector& rhs )
{
    DGFX_ASSERT( size() == rhs.size() );
    iterator i;
    const_iterator j;
    for ( i = begin(), j = rhs.begin(); i != end(); ++i, ++j ) {
        (*i) += (*j);
    }
    return *this;
}

//------------------------------------------------------------------------------

inline SimVector& SimVector::operator-=( const SimVector& rhs )
{
    DGFX_ASSERT( size() == rhs.size() );
    iterator i;
    const_iterator j;
    for ( i = begin(), j = rhs.begin(); i != end(); ++i, ++j ) {
        (*i) -= (*j);
    }
    return *this;
}

//------------------------------------------------------------------------------

inline SimVector& SimVector::operator+=( Float k )
{
    for ( iterator i = begin(); i != end(); ++i ) {
        (*i) += k;
    }
    return *this;
}

//------------------------------------------------------------------------------

inline SimVector& SimVector::plusEqualsScaled( Float k, const SimVector& rhs )
{
    DGFX_ASSERT( size() == rhs.size() );
    iterator i;
    const_iterator j;
    for ( i = begin(), j = rhs.begin(); i != end(); ++i, ++j ) {
        (*i) += k * (*j);
    }
    return *this;
}

//------------------------------------------------------------------------------

inline SimVector& SimVector::operator-=( Float k )
{
    return operator+=( -k );
}

//------------------------------------------------------------------------------

inline SimVector& SimVector::operator*=( Float k )
{
    for ( iterator i = begin(); i != end(); ++i ) {
        (*i) *= k;
    }
    return *this;
}

//------------------------------------------------------------------------------

inline SimVector& SimVector::operator/=( Float k )
{
    return operator*=( 1 / k );
}

//------------------------------------------------------------------------------

inline SimVector SimVector::operator-() const
{
    SimVector result( *this );
    for ( iterator i = result.begin(); i != result.end(); ++i ) {
        (*i) = -(*i);
    }
    return result;
}

//------------------------------------------------------------------------------

inline SimVector SimVector::operator+( const SimVector& rhs ) const
{
    SimVector temp( *this );
    temp += rhs;
    return temp;
}

//------------------------------------------------------------------------------

inline SimVector SimVector::operator-( const SimVector& rhs ) const
{
    SimVector temp( *this );
    temp -= rhs;
    return temp;
}

//------------------------------------------------------------------------------

inline SimVector SimVector::operator+( Float k ) const
{
    SimVector temp( *this );
    temp += k;
    return temp;
}

//------------------------------------------------------------------------------

inline SimVector SimVector::operator-( Float k ) const
{
    SimVector temp( *this );
    temp -= k;
    return temp;
}

//------------------------------------------------------------------------------

inline SimVector SimVector::operator*( Float k ) const
{
    SimVector temp( *this );
    temp *= k;
    return temp;
}

//------------------------------------------------------------------------------

inline SimVector SimVector::operator/( Float k ) const
{
    SimVector temp( *this );
    temp /= k;
    return temp;
}

//------------------------------------------------------------------------------

inline Float SimVector::dot( const SimVector& rhs ) const
{
    DGFX_ASSERT( size() == rhs.size() );
    Float result = 0;
    for ( const_iterator i = begin(), j = rhs.begin(); i != end(); ++i, ++j ) {
        result += i->dot( *j );
    }
    return result;
}

//------------------------------------------------------------------------------

inline Float SimVector::length() const
{
    Float sum = 0;
    for ( const_iterator i = begin(); i != end(); ++i ) {
        sum += i->_x * i->_x + i->_y * i->_y + i->_z * i->_z;
    }
    return BaMath::sqrt( sum );
}

//------------------------------------------------------------------------------

inline const GeVector& SimVector::operator[]( UInt32 i ) const
{
    DGFX_ASSERT( i < _data.size() );
    return _data[ i ];
}

//------------------------------------------------------------------------------

inline GeVector& SimVector::operator[]( UInt32 i )
{
    DGFX_ASSERT( i < _data.size() );
    return _data[ i ];
}

//------------------------------------------------------------------------------

inline void SimVector::clear()
{
    std::fill( begin(), end(), GeVector::ZERO );
}


////////////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTIONS
//

//------------------------------------------------------------------------------

inline SimVector operator*( Float k, const SimVector& rhs )
{
    return rhs * k;
}

FREECLOTH_NAMESPACE_END

#endif
