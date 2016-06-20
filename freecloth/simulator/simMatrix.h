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

#ifndef freecloth_sim_simMatrix_h
#define freecloth_sim_simMatrix_h

#ifndef freecloth_simulator_package_h
#include <freecloth/simulator/package.h>
#endif

#ifndef freecloth_geom_geMatrix3_h
#include <freecloth/geom/geMatrix3.h>
#endif

#ifndef freecloth_base_list
#include <freecloth/base/list>
#endif

#ifndef freecloth_base_vector
#include <freecloth/base/vector>
#endif

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// FORWARD DECLARATIONS

class SimVector;

////////////////////////////////////////////////////////////////////////////////
/*!
 * \class SimMatrix freecloth/simulator/simMatrix.h
 * \brief Sparse matrix class.
 *
 * Uses compressed row storage format, with each entry being a
 * 3x3 GeMatrix3 matrix.
 *
 * This implements the bare minimum necessary for the cloth simulation class,
 * and is not really intended as a general purpose sparse matrix class.
 *
 * Formerly, both MTL and uBLAS were used for this purpose.
 * - MTL: poor code clarity (looks like assembly), not easily portable to
 *   Visual C++, no support for 3x3 matrix as base element.
 * - uBLAS: good code clarity, easily portable, poor support for 3x3 matrix as
 *   base element, sparse matrices are second class citizens, with dense
 *   matrices getting best testing. Doesn't appear to use sparse matrices in
 *   debug mode. In the end, given maintenance issues and the difficulty of
 *   setting up Boost, this was abandoned to make the library easier to
 *   compile.
 */

class SimMatrix
{
public:
    // ----- classes -----

    // FIXME 2003/04/23: out-of-line these classes. At present, MSVC6 cannot
    // handle out-of-line nested classes.

    ////////////////////////////////////////////////////////////////////////////
    /*!
     * \class Element freecloth/simulator/simMatrix.h
     * \brief Entry in a sparse matrix.
     */
    class Element {
    public:
        Element( const GeMatrix3& data, UInt32 column );
        GeMatrix3 _data;
        UInt32 _column;
    };

    // ----- types and enumerations -----
    typedef std::list<Element> ColumnContainer;
    typedef ColumnContainer::iterator ColumnIterator;
    typedef ColumnContainer::const_iterator ColumnConstIterator;

    // ----- classes -----
    ////////////////////////////////////////////////////////////////////////////
    /*!
     * \class Row freecloth/simulator/simMatrix.h
     * \brief Row in a sparse matrix.
     */
    class Row {
    public:
        ColumnContainer _columns;

        ColumnIterator beginColumn();
        ColumnIterator endColumn();
        ColumnConstIterator beginColumn() const;
        ColumnConstIterator endColumn() const;

        const GeMatrix3& operator[]( UInt32 column ) const;
        // Will insert a new entry into row if one does not exist.
        GeMatrix3& operator[]( UInt32 column );
    };


    // ----- types and enumerations -----
    typedef std::vector<Row> RowContainer;
    typedef RowContainer::iterator RowIterator;
    typedef RowContainer::const_iterator RowConstIterator;
    
    // ----- static member functions -----
    
    static void multiply(
        SimVector& destV,
        const SimMatrix& srcM,
        const SimVector& srcV
    );
    
    // ----- member functions -----
    
    SimMatrix();
    SimMatrix( UInt32 nbRows, UInt32 nbColumns );
    // Default copy constructor is fine.

    UInt32 nbRows() const;
    UInt32 nbColumns() const;
    RowIterator beginRow();
    RowIterator endRow();
    RowConstIterator beginRow() const;
    RowConstIterator endRow() const;

    // Default assignment operator is fine.
    const Row& operator[]( UInt32 row ) const;
    Row& operator[]( UInt32 row );
    const GeMatrix3& operator()( UInt32 row, UInt32 col ) const;
    // Will insert a new entry into matrix if one does not exist.
    GeMatrix3& operator()( UInt32 row, UInt32 col );
    SimMatrix& operator*=( Float );
    SimMatrix& operator+=( const SimMatrix& );
    SimVector operator*( const SimVector& ) const;
    SimMatrix operator*( Float ) const;
    SimMatrix operator+( const SimMatrix& ) const;

    // ----- static data members -----

private:
    // ----- member functions -----

    // ----- data members -----

    UInt32 _nbRows, _nbColumns;
    RowContainer _rows;
};

////////////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTIONS
//
std::ostream& operator<<( std::ostream&, const SimMatrix& );
SimMatrix operator*( Float, const SimMatrix& );

FREECLOTH_NAMESPACE_END

#include <freecloth/simulator/simMatrix.inline.h>

#endif
