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

#ifndef freecloth_simulator_simMatrix_inline_h
#define freecloth_simulator_simMatrix_inline_h

#include <freecloth/simulator/simVector.h>

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// CLASS SimMatrix::Element

//------------------------------------------------------------------------------

inline SimMatrix::Element::Element( const GeMatrix3& data, UInt32 column )
  : _data( data ),
    _column( column )
{
}

////////////////////////////////////////////////////////////////////////////////
// CLASS SimMatrix::Row

//------------------------------------------------------------------------------

inline SimMatrix::ColumnIterator SimMatrix::Row::beginColumn()
{
    return _columns.begin();
}

//------------------------------------------------------------------------------

inline SimMatrix::ColumnIterator SimMatrix::Row::endColumn()
{
    return _columns.end();
}

//------------------------------------------------------------------------------

inline SimMatrix::ColumnConstIterator SimMatrix::Row::beginColumn() const
{
    return _columns.begin();
}

//------------------------------------------------------------------------------

inline SimMatrix::ColumnConstIterator SimMatrix::Row::endColumn() const
{
    return _columns.end();
}

//------------------------------------------------------------------------------

inline GeMatrix3& SimMatrix::Row::operator[] ( UInt32 col )
{
    // FIXME: need a way to assert this
    //DGFX_ASSERT( col < nbColumns() );
    ColumnIterator cit( beginColumn() );
    while( cit != endColumn() && cit->_column < col ) {
        ++cit;
    }
    if ( cit == endColumn() || cit->_column != col ) {
        cit = _columns.insert( cit, Element( GeMatrix3::zero(), col ) );
    }
    DGFX_ASSERT( cit != endColumn() && cit->_column == col );
    return cit->_data;
}

//------------------------------------------------------------------------------

inline const GeMatrix3& SimMatrix::Row::operator[] ( UInt32 col ) const
{
    // FIXME: need a way to assert this
    //DGFX_ASSERT( col < nbColumns() );
    ColumnConstIterator cit( beginColumn() );
    while( cit != endColumn() && cit->_column < col ) {
        ++cit;
    }
    if ( cit == endColumn() || cit->_column != col ) {
        static const GeMatrix3 zero( GeMatrix3::zero() );
        return zero;
    }
    return cit->_data;
}


////////////////////////////////////////////////////////////////////////////////
// CLASS SimMatrix

//------------------------------------------------------------------------------

inline SimMatrix::SimMatrix()
  : _nbRows( 0 ),
    _nbColumns( 0 )
{}

//------------------------------------------------------------------------------

inline SimMatrix::SimMatrix( UInt32 nbRows, UInt32 nbColumns )
  : _nbRows( nbRows ),
    _nbColumns( nbColumns ),
    _rows( nbRows )
{}

//------------------------------------------------------------------------------

inline UInt32 SimMatrix::nbRows() const
{
    return _nbRows;
}

//------------------------------------------------------------------------------

inline UInt32 SimMatrix::nbColumns() const
{
    return _nbColumns;
}

//------------------------------------------------------------------------------

inline SimMatrix::RowIterator SimMatrix::beginRow()
{
    return _rows.begin();
}

//------------------------------------------------------------------------------

inline SimMatrix::RowIterator SimMatrix::endRow()
{
    return _rows.end();
}

//------------------------------------------------------------------------------

inline SimMatrix::RowConstIterator SimMatrix::beginRow() const
{
    return _rows.begin();
}

//------------------------------------------------------------------------------

inline SimMatrix::RowConstIterator SimMatrix::endRow() const
{
    return _rows.end();
}

//------------------------------------------------------------------------------

inline SimMatrix::Row& SimMatrix::operator[] ( UInt32 row )
{
    DGFX_ASSERT( row < nbRows() );
    return _rows[ row ];
}

//------------------------------------------------------------------------------

inline const SimMatrix::Row& SimMatrix::operator[] ( UInt32 row ) const
{
    DGFX_ASSERT( row < nbRows() );
    return _rows[ row ];
}

//------------------------------------------------------------------------------

inline GeMatrix3& SimMatrix::operator() ( UInt32 row, UInt32 col )
{
    DGFX_ASSERT( row < nbRows() );
    return operator[]( row )[ col ];
}

//------------------------------------------------------------------------------

inline const GeMatrix3& SimMatrix::operator() ( UInt32 row, UInt32 col ) const
{
    DGFX_ASSERT( row < nbRows() );
    return operator[]( row )[ col ];
}

//------------------------------------------------------------------------------

inline SimMatrix& SimMatrix::operator*=( Float rhs )
{
    for( RowIterator r = beginRow(); r != endRow(); ++r ) {
        for( ColumnIterator c = r->beginColumn(); c != r->endColumn(); ++c ) {
            c->_data *= rhs;
        }
    }
    return *this;
}

//------------------------------------------------------------------------------

inline SimMatrix SimMatrix::operator*( Float rhs ) const
{
    SimMatrix temp( *this );
    temp *= rhs;
    return temp;
}

//------------------------------------------------------------------------------

inline SimVector SimMatrix::operator*( const SimVector& rhs ) const
{
    SimVector result;
    multiply( result, *this, rhs );
    return result;
}

//------------------------------------------------------------------------------

inline SimMatrix SimMatrix::operator+( const SimMatrix& rhs ) const
{
    SimMatrix temp( *this );
    temp += rhs;
    return temp;
}


////////////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTIONS
//

//------------------------------------------------------------------------------

inline SimMatrix operator*( Float lhs, const SimMatrix& rhs )
{
    return rhs * lhs;
}

FREECLOTH_NAMESPACE_END

#endif
