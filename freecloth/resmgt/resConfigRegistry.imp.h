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

#ifndef freecloth_resmgt_resConfigRegistry_imp_h
#define freecloth_resmgt_resConfigRegistry_imp_h

#include <freecloth/resmgt/resConfigRegistry.h>
#include <freecloth/base/baStringUtil.h>

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES

//------------------------------------------------------------------------------

template <class T>
void rFloats(
    const ResConfigRegistryR& reg,
    const String& key,
    T& result,
    const T& def
) {
    if ( reg.hasKey( key ) ) {
        result.clear();
        String s = reg.readString( key );
        String::size_type startPos = 0;
        String::size_type spacePos = s.find_first_of( ' ', startPos );
        while ( spacePos != String::npos ) {
            result.push_back(
                BaStringUtil::toFloat( s.substr( startPos, spacePos ) )
            );
            startPos = spacePos + 1;
            spacePos = s.find_first_of( ' ', startPos );
        }
        result.push_back(
            BaStringUtil::toFloat( s.substr( startPos ) )
        );
    }
    else {
        result = def;
    }
}

//------------------------------------------------------------------------------

template <class T>
void wFloats(
    ResConfigRegistryW& reg,
    const String& key,
    const T& values
) {
    typename T::const_iterator i = values.begin();
    String string;
    if ( i != values.end() ) {
        string = BaStringUtil::fromFloat( *i );
        for ( ++i; i != values.end(); ++i ) {
            string += " " + BaStringUtil::fromFloat( *i );
        }
    }
    reg.writeString( key, string );
}


////////////////////////////////////////////////////////////////////////////////
// CLASS ResConfigRegistryW

//------------------------------------------------------------------------------

#ifdef HAVE_MEMBER_TEMPLATES
template <class T>
void ResConfigRegistryW::writeFloats(
    const String& key,
    const T& values
) {
    wFloats( *this, key, values );
}
#endif

////////////////////////////////////////////////////////////////////////////////
// CLASS ResConfigRegistryR

//------------------------------------------------------------------------------

#ifdef HAVE_MEMBER_TEMPLATES
template <class T>
void ResConfigRegistryR::readFloats(
    const String& key,
    T& result,
    const T& def
) const {
    rFloats( *this, key, result, def );
}
#endif

FREECLOTH_NAMESPACE_END

#endif
