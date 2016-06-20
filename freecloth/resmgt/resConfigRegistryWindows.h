//////////////////////////////////////////////////////////////////////
// Copyright (c) 2001-2002 David Pritchard <drpritch@alumni.uwaterloo.ca>
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

#ifndef freecloth_resmgt_resConfigRegistryWindows_h
#define freecloth_resmgt_resConfigRegistryWindows_h

#ifndef freecloth_resmgt_package_h
#include <freecloth/resmgt/package.h>
#endif

#ifndef freecloth_resmgt_resConfigRegistry_h
#include <freecloth/resmgt/resConfigRegistry.h>
#endif

#ifndef freecloth_base_windows_h
#include <freecloth/base/windows.h>
#endif

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// FORWARD DECLARATIONS

////////////////////////////////////////////////////////////////////////////////
// CLASS ResConfigRegistryWWindows
//
// Configuration writer for Windows, using registry keys.
//
class ResConfigRegistryWWindows : public ResConfigRegistryW {

public:
    //----- member functions -----

    ResConfigRegistryWWindows( const String& keyName );
    virtual ~ResConfigRegistryWWindows();

    virtual void writeString(
        const String& key,
        const String&
    );
    virtual void writeUInt32(
        const String& key,
        UInt32
    );
    virtual void writeFloat(
        const String& key,
        Float
    );

private:
    HKEY _hkey;
};

////////////////////////////////////////////////////////////////////////////////
// CLASS ResConfigRegistryRWindows
//
// Configuration reader for Windows, using registry keys.
//
class ResConfigRegistryRWindows : public ResConfigRegistryR {

public:
    //----- member functions -----

    ResConfigRegistryRWindows( const String& keyName );
    virtual ~ResConfigRegistryRWindows();

    virtual bool hasKey( const String& key ) const;

    virtual String readString(
        const String& key,
        const String& def
    ) const;
    virtual UInt32 readUInt32(
        const String& key,
        UInt32 def
    ) const;
    virtual Float readFloat(
        const String& key,
        Float def
    ) const;

private:
    HKEY _hkey;
};

FREECLOTH_NAMESPACE_END

#endif
