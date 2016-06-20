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

#include <freecloth/simulator/simMatrix.h>

////////////////////////////////////////////////////////////////////////////////
// LOCAL DECLARATIONS

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// CLASS SimMatrix

//------------------------------------------------------------------------------

SimMatrix& SimMatrix::operator+=( const SimMatrix& rhs )
{
    DGFX_ASSERT( nbRows() == rhs.nbRows() );
    DGFX_ASSERT( nbColumns() == rhs.nbColumns() );
    RowIterator r1;
    RowConstIterator r2;
    for( r1 = beginRow(), r2 = rhs.beginRow(); r1 != endRow(); ++r1, ++r2 ) {
        ColumnIterator c1 = r1->beginColumn();
        ColumnConstIterator c2;
        for( c2 = r2->beginColumn(); c2 != r2->endColumn(); ++c2 ) {
            while ( c1 != r1->endColumn() && c1->_column < c2->_column ) {
                ++c1;
            }
            if ( c1 == r1->endColumn() ) {
                r1->_columns.push_back( *c2 );
                continue;
            } else if ( c2->_column < c1->_column ) {
                r1->_columns.insert( c1, *c2 );
                continue;
            }
            DGFX_ASSERT( c1->_column == c2->_column );
            c1->_data += c2->_data;
        }
    }
    return *this;
}

//------------------------------------------------------------------------------

void SimMatrix::multiply(
    SimVector& destV,
    const SimMatrix& srcM,
    const SimVector& srcV
) {
    DGFX_ASSERT( srcM.nbColumns() == srcV.size() );
    if ( destV.size() != srcM.nbRows() ) {
        destV = SimVector::zero( srcM.nbRows() );
    }
    else {
        destV.clear();
    }
    for ( RowConstIterator rit = srcM.beginRow(); rit != srcM.endRow(); ++rit ){
        UInt32 row = rit - srcM.beginRow();
        GeVector& v = destV[ row ];
        ColumnConstIterator c;
        for ( c = rit->beginColumn(); c != rit->endColumn(); ++c ) {
            v += c->_data * srcV[ c->_column ];
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTIONS
//

//------------------------------------------------------------------------------

std::ostream& operator<<( std::ostream& out, const SimMatrix& m )
{
    out << "M(" << std::endl;
    for ( UInt32 r = 0; r < m.nbRows(); ++r ) {
        out << "  ";
        for ( UInt32 c = 0; c < m.nbColumns(); ++c ) {
            out << m[ r ][ c ] << " ";
        }
        out << std::endl;
    }
    out << ")";
    return out;
}

FREECLOTH_NAMESPACE_END
