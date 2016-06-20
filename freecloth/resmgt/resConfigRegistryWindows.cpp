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

#include <freecloth/resmgt/resConfigRegistryWindows.h>
#include <freecloth/resmgt/rcShdPtr.h>

////////////////////////////////////////////////////////////////////////////////
// LOCAL DECLARATIONS

FREECLOTH_NAMESPACE_START
    const String BASE_PATH = "Software\\Winamp\\Plugins";
};

namespace freecloth {

////////////////////////////////////////////////////////////////////////////////
// CLASS ResConfigRegistryW

//------------------------------------------------------------------------------
RCShdPtr<ResConfigRegistryW> ResConfigRegistryW::create(
    const String& groupName
) {
    return RCShdPtr<ResConfigRegistryW>(
        new ResConfigRegistryWWindows( BASE_PATH + "\\" + groupName )
    );
}

////////////////////////////////////////////////////////////////////////////////
// CLASS ResConfigRegistryR

//------------------------------------------------------------------------------
RCShdPtr<ResConfigRegistryR> ResConfigRegistryR::create(
    const String& groupName
) {
    return RCShdPtr<ResConfigRegistryR>(
        new ResConfigRegistryRWindows( BASE_PATH + "\\" + groupName )
    );
}

////////////////////////////////////////////////////////////////////////////////
// CLASS ResConfigRegistryWWindows

//------------------------------------------------------------------------------

ResConfigRegistryWWindows::ResConfigRegistryWWindows(
    const String& keyName
) {
    HRESULT result;
    
    result = ::RegCreateKeyEx(
        HKEY_CURRENT_USER,
        keyName.c_str(),
        0,
        NULL,
        0,
        KEY_WRITE,
        NULL,
        &_hkey,
        NULL
    );

    DGFX_ASSERT( result == 0 );
}

//------------------------------------------------------------------------------

ResConfigRegistryWWindows::~ResConfigRegistryWWindows()
{
    ::RegCloseKey( _hkey );
}

//------------------------------------------------------------------------------

void ResConfigRegistryWWindows::writeString(
    const String& key,
    const String& value
) {
    HRESULT result;
    result = ::RegSetValueEx(
        _hkey,
        key.c_str(),
        0,
        REG_SZ,
        reinterpret_cast<const BYTE*>( value.c_str() ),
        value.length()
    );
    DGFX_ASSERT( result == 0 );
}

//------------------------------------------------------------------------------

void ResConfigRegistryWWindows::writeUInt32(
    const String& key,
    UInt32 value
) {
    HRESULT result;
    result = ::RegSetValueEx(
        _hkey,
        key.c_str(),
        0,
        REG_DWORD,
        reinterpret_cast<const BYTE*>( &value ),
        sizeof( value )
    );
    DGFX_ASSERT( result == 0 );
}

//------------------------------------------------------------------------------

void ResConfigRegistryWWindows::writeFloat(
    const String& key,
    Float value
) {
    writeUInt32( key, *reinterpret_cast<UInt32*>( &value ) );
}

////////////////////////////////////////////////////////////////////////////////
// CLASS ResConfigRegistryRWindows

//------------------------------------------------------------------------------

ResConfigRegistryRWindows::ResConfigRegistryRWindows(
    const String& keyName
) {
    HRESULT result;

    result = ::RegOpenKeyEx(
        HKEY_CURRENT_USER,
        keyName.c_str(),
        0,
        KEY_READ,
        &_hkey
    );

    // Key not found
    if ( result != 0 ) {
        _hkey = NULL;
    }
}

//------------------------------------------------------------------------------

ResConfigRegistryRWindows::~ResConfigRegistryRWindows()
{
    ::RegCloseKey( _hkey );
}

//------------------------------------------------------------------------------

bool ResConfigRegistryRWindows::hasKey(
    const String& key
) const {
    bool retVal = false;

    if ( _hkey != NULL ) {
        DWORD size;
        DWORD type;
        HRESULT result = ::RegQueryValueEx(
            _hkey, key.c_str(), 0, &type, NULL, &size
        );
        if ( result == 0 ) {
            retVal = true;
        }
    }
    return retVal;
}

//------------------------------------------------------------------------------

String ResConfigRegistryRWindows::readString(
    const String& key,
    const String& def
) const {
    String retVal = def;

    DWORD size = 0;
    DWORD type;
    HRESULT result = ::RegQueryValueEx(
        _hkey, key.c_str(), 0, &type, NULL, &size
    );
    if ( result == 0 && type == REG_SZ ) {
        char* buf = new char[ size + 1 ];
        HRESULT result = ::RegQueryValueEx(
            _hkey, key.c_str(), 0, &type, reinterpret_cast<BYTE*>( buf ), &size
        );
        if ( result == 0 ) {
            retVal = buf;
        }
        delete[] buf;
    }

    return retVal;
}

//------------------------------------------------------------------------------

UInt32 ResConfigRegistryRWindows::readUInt32(
    const String& key,
    UInt32 def
) const {
    UInt32 retVal = def;

    if ( _hkey != NULL ) {
        DWORD value;
        DWORD size = sizeof( value );
        DWORD type;
        HRESULT result = ::RegQueryValueEx(
            _hkey,
            key.c_str(),
            0,
            &type,
            reinterpret_cast<BYTE*>( &value ),
            &size
        );
        if ( result == 0 && type == REG_DWORD ) {
            retVal = value;
        }
    }
    return retVal;
}

//------------------------------------------------------------------------------

Float ResConfigRegistryRWindows::readFloat(
    const String& key,
    Float def
) const {
    DGFX_ASSERT( sizeof(Float) == sizeof(UInt32) );

    UInt32 retVal = readUInt32( key, *reinterpret_cast<UInt32*>( &def ) );
    return *reinterpret_cast<Float*>( &retVal );
}

FREECLOTH_NAMESPACE_END
