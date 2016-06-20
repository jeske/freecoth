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

#include <freecloth/resmgt/resConfigRegistryFile.h>
#include <freecloth/resmgt/rcShdPtr.h>

#include <freecloth/base/baStringUtil.h>
#include <freecloth/base/stdlib.h>

////////////////////////////////////////////////////////////////////////////////
// LOCAL DECLARATIONS

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// CLASS ResConfigRegistryWFile

//------------------------------------------------------------------------------

ResConfigRegistryWFile::ResConfigRegistryWFile(
    const String& filename
) : out( filename.c_str() )
{
}

//------------------------------------------------------------------------------

ResConfigRegistryWFile::~ResConfigRegistryWFile()
{
}

//------------------------------------------------------------------------------

void ResConfigRegistryWFile::writeString(
    const String& key,
    const String& value
) {
    out << key << "=" << value << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
// CLASS ResConfigRegistryRFile

//------------------------------------------------------------------------------

ResConfigRegistryRFile::ResConfigRegistryRFile(
    const String& filename
) {
    //DGFX_TRACE_ENTER( "ResConfigRegistryRFile ctor " << filename );
    std::ifstream in( filename.c_str() );

    while ( in.good() ) {
        char buf[ 1024 ];
        in.getline( buf, sizeof( buf ) );
        if ( buf[ 0 ] != '#' ) {
            const String s( buf );
            UInt32 equalsPos = s.find( '=' );
            String key;
            String value;
            if ( equalsPos != String::npos ) {
                const char* whitespace = " \t";
                UInt32 nonWSPos;
                nonWSPos = s.find_last_not_of( whitespace, equalsPos - 1 );
                if ( nonWSPos != String::npos ) {
                    key = s.substr( 0, nonWSPos+1 );
                }
                nonWSPos = s.find_first_not_of( whitespace, equalsPos + 1 );
                if ( nonWSPos != String::npos ) {
                    value = s.substr( nonWSPos );
                }
            }
            if (key.length() > 0) {
                _keys[ key ] = value;
            }
        }
        // TODO: store dead keys
    }
}

//------------------------------------------------------------------------------

ResConfigRegistryRFile::~ResConfigRegistryRFile()
{
}

//------------------------------------------------------------------------------

bool ResConfigRegistryRFile::hasKey(
    const String& key
) const {
    //DGFX_TRACE_ENTER( "ResConfigRegistryRFile::hasKey" );
    //DGFX_TRACE( "key = " << key );
    std::map<String,String>::const_iterator it;
    it = _keys.find( key );
    //DGFX_TRACE( "answer = " << ( it != _keys.end() ) );
    return it != _keys.end();
}

//------------------------------------------------------------------------------

String ResConfigRegistryRFile::readString(
    const String& key,
    const String& def
) const {
    std::map<String,String>::const_iterator it;
    it = _keys.find( key );
    if ( it != _keys.end() ) {
        return it->second;
    }
    else {
        return def;
    }
}

FREECLOTH_NAMESPACE_END
