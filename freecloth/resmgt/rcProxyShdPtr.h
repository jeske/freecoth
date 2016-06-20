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

#ifndef freecloth_resmgt_rcProxyShdPtr_h
#define freecloth_resmgt_rcProxyShdPtr_h

#ifndef freecloth_resmgt_package_h
#include <freecloth/resmgt/package.h>
#endif

#ifndef freecloth_base_typeinfo
#include <freecloth/base/typeinfo>
#endif

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// FORWARD DECLARATIONS

template <class T>
class RCProxyShdPtr;

////////////////////////////////////////////////////////////////////////////////
/*!
 * \class RCProxyShdPtrBase freecloth/resmgt/rcProxyShdPtr.h
 * \brief Base class for reference counted smart pointer proxy.
 *
 * This is not intended for general use. Use RCProxyShdPtr.
 */
class RCProxyShdPtrBase
{
protected:
    //----- classes -----
    class Data;
    //----- member functions -----
    RCProxyShdPtrBase();

public:
    // FIXME: g++ doesn't like templated friends. Must use this as a
    // substitute for now.

    //----- data members -----
    Data*   _data;

};

////////////////////////////////////////////////////////////////////////////////
/*!
 * \class RCProxyShdPtrBase::Data freecloth/resmgt/rcProxyShdPtr.h
 * \brief Internal data for RCProxyShdPtrBase.
 */
class RCProxyShdPtrBase::Data
{
public:
    //----- member functions -----
    Data( void* ptr = 0, UInt32 count = 1 );

    //----- data members -----
    void*   _ptr;
    UInt32  _count;
};

////////////////////////////////////////////////////////////////////////////////
/*!
 * \class RCProxyShdPtr freecloth/resmgt/rcProxyShdPtr.h
 * \brief Reference-counted shared smart pointer for non-RCBase objects.
 *
 * When using external classes, such as the STL or a commercial library,
 * it is not possible to change these classes to derive from RCBase, for use
 * with RCShdPtr. Instead, RCProxyShdPtr can be used, although it is not
 * quite as powerful as RCShdPtr.
 */

// FIXME: separate into RCBase<> and RCProxy<> style for more allocation
// efficiency (a la discreet)
//
// Modelled on Yonat Sharon's code.
template <class T>
class RCProxyShdPtr : public RCProxyShdPtrBase
{
public:
    //----- types and enumerations -----
    typedef T element_type;

    //----- member functions -----
    explicit RCProxyShdPtr( T* = 0 );
    RCProxyShdPtr( const RCProxyShdPtr& rhs );
    ~RCProxyShdPtr();

    RCProxyShdPtr& operator=( const RCProxyShdPtr& rhs );

#ifdef HAVE_MEMBER_TEMPLATES
    //template <class X> friend class RCProxyShdPtr<X>;
    template <class X>
    RCProxyShdPtr( const RCProxyShdPtr<X>& rhs )
    {
        // verify that X* can be converted to T* (i.e. X* is const T*, or
        // T is a base class of X)
        X* x;
        T* t = x;

        acquire( rhs._data );
    }

    template <class X>
    RCProxyShdPtr& operator=( const RCProxyShdPtr<X>& rhs )
    {
        // verify that X* can be converted to T* (i.e. X* is const T*, or
        // T is a base class of X)
        X* x;
        T* t = x;
        if ( rhs._data != _data ) {
            release();
            acquire( rhs._data );
        }
        return *this;
    }
    template <class X>
    bool operator==( const RCProxyShdPtr<X>& rhs ) const
    {
        return _data == rhs._data;
    }
    template <class X>
    bool operator!=( const RCProxyShdPtr<X>& rhs ) const
    {
        return _data != rhs._data;
    }
#endif

    bool isNull() const;
    T& operator*() const;
    T* operator->() const;
    T* get () const;
    bool operator==( const RCProxyShdPtr& ) const;
    bool operator!=( const RCProxyShdPtr& ) const;

private:

    //----- member functions -----
    void acquire( Data* );
    void release();
};

////////////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTIONS
//

template <class T>
std::ostream& operator<<( std::ostream&, const RCProxyShdPtr<T>& );

FREECLOTH_NAMESPACE_END

#include <freecloth/resmgt/rcProxyShdPtr.inline.h>

#endif
