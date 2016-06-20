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

#ifndef freecloth_resmgt_resConfigRegistryFile_h
#define freecloth_resmgt_resConfigRegistryFile_h

#ifndef freecloth_resmgt_package_h
#include <freecloth/resmgt/package.h>
#endif

#ifndef freecloth_resmgt_resConfigRegistry_h
#include <freecloth/resmgt/resConfigRegistry.h>
#endif

#ifndef freecloth_base_map
#include <freecloth/base/map>
#endif

#ifndef freecloth_base_fstream
#include <freecloth/base/fstream>
#endif

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// FORWARD DECLARATIONS

////////////////////////////////////////////////////////////////////////////////
// CLASS ResConfigRegistryWFile
//
// Configuration writer using a file, in Unix "rc-file" style.
//
class ResConfigRegistryWFile : public ResConfigRegistryW {

public:
    //----- member functions -----

    ResConfigRegistryWFile( const String& filename );
    virtual ~ResConfigRegistryWFile();

    virtual void writeString(
        const String& key,
        const String& value
    );

private:
    //----- data members -----

    std::ofstream out;
};

////////////////////////////////////////////////////////////////////////////////
// CLASS ResConfigRegistryRFile
//
// Configuration reader using a file, in Unix "rc-file" style
//
class ResConfigRegistryRFile : public ResConfigRegistryR {

public:
    //----- member functions -----

    ResConfigRegistryRFile( const String& filename );
    virtual ~ResConfigRegistryRFile();

    virtual bool hasKey( const String& key ) const;

    virtual String readString(
        const String& key,
        const String& def
    ) const;

private:
    std::map<String,String> _keys;
};

FREECLOTH_NAMESPACE_END

#endif
