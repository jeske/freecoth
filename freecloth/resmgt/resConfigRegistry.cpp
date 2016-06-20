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

#include <freecloth/resmgt/resConfigRegistry.imp.h>
#include <freecloth/base/stdio.h>

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// CLASS ResConfigRegistryW

//------------------------------------------------------------------------------

ResConfigRegistryW::~ResConfigRegistryW()
{
}

//------------------------------------------------------------------------------

void ResConfigRegistryW::writeUInt32(
    const String& key,
    UInt32 value
) {
    writeString( key, BaStringUtil::fromUInt32( value ) );
}

//------------------------------------------------------------------------------

void ResConfigRegistryW::writeFloat(
    const String& key,
    Float value
) {
    writeString( key, BaStringUtil::fromFloat( value ) );
}

//------------------------------------------------------------------------------

void ResConfigRegistryW::writeEnum(
    const String& key,
    UInt32 value
) {
    writeUInt32( key, value );
}

//------------------------------------------------------------------------------

void ResConfigRegistryW::writeBool(
    const String& key,
    bool value
) {
    writeUInt32( key, value ? 1 : 0 );
}

//------------------------------------------------------------------------------

void ResConfigRegistryW::writeTimeDuration(
    const String& key,
    BaTime::Duration value
) {
    writeUInt32( key, value );
}

//------------------------------------------------------------------------------

void ResConfigRegistryW::writeFloats(
    const String& key,
    const std::vector<Float>& values
) {
    wFloats( *this, key, values );
}


////////////////////////////////////////////////////////////////////////////////
// CLASS ResConfigRegistryR

//------------------------------------------------------------------------------

ResConfigRegistryR::~ResConfigRegistryR()
{
}

//------------------------------------------------------------------------------

UInt32 ResConfigRegistryR::readUInt32(
    const String& key,
    UInt32 def
) const {
    UInt32 result = def;

    if ( hasKey( key ) ) {
        String s = readString( key );
        ::sscanf( s.c_str(), "%d", &result );
    }
    return result;
}

//------------------------------------------------------------------------------

Float ResConfigRegistryR::readFloat(
    const String& key,
    Float def
) const {
    Float result = def;

    if ( hasKey( key ) ) {
        String s = readString( key );
        ::sscanf( s.c_str(), "%f", &result );
    }
    return result;
}

//------------------------------------------------------------------------------

UInt32 ResConfigRegistryR::readEnum(
    const String& key,
    UInt32 nbEntries,
    UInt32 def
) const {
    UInt32 result = def;

    if ( hasKey( key ) ) {
        result = readUInt32( key, def );
        if ( result >= nbEntries ) {
            result = def;
        }
    }
    return result;
}

//------------------------------------------------------------------------------

bool ResConfigRegistryR::readBool(
    const String& key,
    bool def
) const {
    bool result = def;

    if ( hasKey( key ) ) {
        result = readUInt32( key, def ? 1 : 0 ) != 0 ? true : false;
    }
    return result;
}

//------------------------------------------------------------------------------

BaTime::Duration ResConfigRegistryR::readTimeDuration(
    const String& key,
    BaTime::Duration def
) const {
    return readUInt32( key, def );
}

//------------------------------------------------------------------------------

void ResConfigRegistryR::readFloats(
    const String& key,
    std::vector<Float>& result,
    const std::vector<Float>& def
) const {
    rFloats( *this, key, result, def );
}


////////////////////////////////////////////////////////////////////////////////
// TEMPLATE INSTANTIATIONS

FREECLOTH_NAMESPACE_END
