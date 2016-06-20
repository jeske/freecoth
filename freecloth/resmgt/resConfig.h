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

#ifndef freecloth_resmgt_resConfig_h
#define freecloth_resmgt_resConfig_h

#ifndef freecloth_resmgt_package_h
#include <freecloth/resmgt/package.h>
#endif

#ifndef freecloth_resmgt_rcBase_h
#include <freecloth/resmgt/rcBase.h>
#endif

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// FORWARD DECLARATIONS

class ResConfigRegistryR;
class ResConfigRegistryW;
template <class T> class RCShdPtr;

////////////////////////////////////////////////////////////////////////////////
/*!
 * \class ResConfig freecloth/resmgt/resConfig.h
 * \brief Application configuration data.
 *
 * Used for storing persistent application configuration data, which can be
 * is stored on disk/registry/whatever.
 */
class ResConfig : public RCBase {

public:
    //----- member functions -----

    virtual RCShdPtr<ResConfig> clone() const = 0;
    virtual void load( ResConfigRegistryR& reg ) = 0;
    virtual void save( ResConfigRegistryW& reg ) const = 0;

};

FREECLOTH_NAMESPACE_END
#endif
