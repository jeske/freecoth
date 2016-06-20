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

#ifndef freecloth_resmgt_rcBase_h
#define freecloth_resmgt_rcBase_h

#ifndef freecloth_resmgt_package_h
#include <freecloth/resmgt/package.h>
#endif

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// FORWARD DECLARATIONS

class RCShdPtrBase;

////////////////////////////////////////////////////////////////////////////////
/*!
 * \class RCBase freecloth/resmgt/rcBase.h
 * \brief Base class for reference-counted objects.
 *
 * This class contains a reference count, for use with the RCShdPtr<>
 * smart pointer class. Any object that is intended to be usable as a
 * smart-pointed object should derive from this.
 */
class RCBase
{
protected:
    UInt32 _rcCount;

    //----- member functions -----
    RCBase();
    RCBase( RCBase const& );
    virtual ~RCBase();

    friend class RCShdPtrBase;
};

FREECLOTH_NAMESPACE_END
#endif
