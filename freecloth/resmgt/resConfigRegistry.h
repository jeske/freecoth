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

#ifndef freecloth_resmgt_resConfigRegistry_h
#define freecloth_resmgt_resConfigRegistry_h

#ifndef freecloth_resmgt_package_h
#include <freecloth/resmgt/package.h>
#endif

#ifndef freecloth_resmgt_rcBase_h
#include <freecloth/resmgt/rcBase.h>
#endif

#ifndef freecloth_base_vector
#include <freecloth/base/vector>
#endif

#ifndef freecloth_base_baTime_h
#include <freecloth/base/baTime.h>
#endif

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// FORWARD DECLARATIONS

template <class T> class RCShdPtr;

////////////////////////////////////////////////////////////////////////////////
/*!
 * \class ResConfigRegistryW freecloth/resmgt/resConfigRegistryW
 * \brief Interface for configuration writing.
 *
 * ResConfig objects save their contents using this object, which should store
 * in a manner than can be restored by ResConfigRegistryR.
 */
class ResConfigRegistryW : public RCBase
{
public:
    //----- static member functions -----

    //! Implemented in derived class' source.
    // FIXME: implement this as an OS-dependent factory/abstract factory setup
    static RCShdPtr<ResConfigRegistryW> create( const String& groupName );

    //----- member functions -----

    virtual ~ResConfigRegistryW();

    virtual void writeString(
        const String& key,
        const String&
    ) = 0;
    virtual void writeUInt32(
        const String& key,
        UInt32
    );
    virtual void writeFloat(
        const String& key,
        Float
    );
    void writeFloats(
        const String& key,
        const std::vector<Float>&
    );
#ifdef HAVE_MEMBER_TEMPLATES
    template <class T>
    void writeFloats(
        const String& key,
        const T&
    );
#endif

    virtual void writeEnum(
        const String& key,
        UInt32
    );
    virtual void writeBool(
        const String& key,
        bool
    );
    virtual void writeTimeDuration(
        const String& key,
        BaTime::Duration
    );
};

////////////////////////////////////////////////////////////////////////////////
/*!
 * \class ResConfigRegistryR freecloth/resmgt/resConfigRegistryR
 * \brief Interface for configuration reading.
 *
 * ResConfig objects load their contents using this object, which should
 * retrieve from the format stored by ResConfigRegistryW.
 */
class ResConfigRegistryR : public RCBase {

public:
    //----- static member functions -----

    //! Implemented in derived class' source.
    static RCShdPtr<ResConfigRegistryR> create( const String& groupName );

    //----- member functions -----

    virtual ~ResConfigRegistryR();

    virtual bool hasKey( const String& key ) const = 0;

    virtual String readString(
        const String& key,
        const String& def = ""
    ) const = 0;
    virtual UInt32 readUInt32(
        const String& key,
        UInt32 def = 0
    ) const;
    virtual Float readFloat(
        const String& key,
        Float def = 0.f
    ) const;
    void readFloats(
        const String& key,
        std::vector<Float>&,
        const std::vector<Float>& def = std::vector<Float>()
    ) const;
#ifdef HAVE_MEMBER_TEMPLATES
    template <class T>
    void readFloats(
        const String& key,
        T&,
        const T& def = T()
    ) const;
#endif

    // No default to prevent accidental misuse
    UInt32 readEnum(
        const String& key,
        UInt32 nbEntries,
        UInt32 def
    ) const;
    bool readBool(
        const String& key,
        bool def = false
    ) const;
    BaTime::Duration readTimeDuration(
        const String& key,
        BaTime::Duration def = 0
    ) const;
};

FREECLOTH_NAMESPACE_END

#endif
