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

#ifndef freecloth_resmgt_rcShdPtr_h
#define freecloth_resmgt_rcShdPtr_h

#ifndef freecloth_base_typeinfo
#include <freecloth/base/typeinfo>
#endif

#ifndef freecloth_resmgt_package_h
#include <freecloth/resmgt/package.h>
#endif

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// FORWARD DECLARATIONS

template <class T>
class RCShdPtr;

class RCBase;

////////////////////////////////////////////////////////////////////////////////
/*!
 * \class RCShdPtrBase freecloth/resmgt/rcShdPtr.h
 * \brief Base class for reference counted smart pointer.
 *
 * This is not intended for general use. Use RCShdPtr.
 */
class RCShdPtrBase
{
public:
    //----- member functions -----
    bool operator==( const RCBase* ) const;
    bool operator!=( const RCBase* ) const;

protected:
    //----- member functions -----
    RCShdPtrBase();

    void acquire( RCBase* );
    void release();

    //----- data members -----
    RCBase* _ptr;
};

////////////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTIONS
//
bool operator==( const RCBase*, const RCShdPtrBase& );
bool operator!=( const RCBase*, const RCShdPtrBase& );


////////////////////////////////////////////////////////////////////////////////
/*!
 * \class RCShdPtr freecloth/resmgt/rcShdPtr.h
 * \brief Reference-counted shared smart pointer.
 *
 * This is a general-purpose smart pointer class. It takes ownership of the
 * input pointer on construction, and deletes the input pointer when its
 * reference count drops to zero.
 *
 * The reference count is incremented every time RCShdPtr's copy constructor
 * is called. The count is decremented every time RCShdPtr's destructor is
 * called. Copies of this object will point to the same data, hence "shared"
 * smart pointer; this is not a copy-on-write pointer.
 *
 * The template parameter must be derived from RCBase. At present, it should
 * not be of const type.
 */

// FIXME: separate into RCBase<> and RCProxy<> style for more allocation
// efficiency (a la discreet)
// FIXME: need to support const
//
// Modelled on Yonat Sharon's code.
//
template <class T>
class RCShdPtr : public RCShdPtrBase
{
public:
    //----- types and enumerations -----
    typedef T element_type;

    //----- member functions -----
    explicit RCShdPtr( T* = 0 );
    //! Copy constructor. Both rhs and this will point to the same data.
    RCShdPtr( const RCShdPtr& rhs );
    ~RCShdPtr();

    RCShdPtr& operator=( const RCShdPtr& rhs );

#ifdef HAVE_MEMBER_TEMPLATES
    //template <class X> friend class RCShdPtr<X>;
    template <class X>
    RCShdPtr( const RCShdPtr<X>& rhs )
    {
        // verify that X* can be converted to T* (i.e. X* is const T*, or
        // T is a base class of X)
        X* x;
        T* t = x;

        acquire( rhs.get() );
    }

    template <class X>
    RCShdPtr& operator=( const RCShdPtr<X>& rhs )
    {
        // verify that X* can be converted to T* (i.e. X* is const T*, or
        // T is a base class of X)
        X* x;
        T* t = x;
        if ( rhs.get() != _ptr ) {
            release();
            acquire( rhs.get() );
        }
        return *this;
    }
    template <class X>
    bool operator==( const RCShdPtr<X>& rhs ) const
    {
        return _ptr == static_cast<RCBase*>( rhs.get() );
    }
    template <class X>
    bool operator!=( const RCShdPtr<X>& rhs ) const
    {
        return _ptr != static_cast<RCBase*>( rhs.get() );
    }
#endif

    bool isNull() const;
    T& operator*() const;
    T* operator->() const;
    T* get() const;
    bool operator==( const RCShdPtr& ) const;
    bool operator!=( const RCShdPtr& ) const;
    bool operator==( const RCBase* ) const;
    bool operator!=( const RCBase* ) const;

private:
};

////////////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTIONS
//

template <class T>
std::ostream& operator<<( std::ostream&, const RCShdPtr<T>& );

FREECLOTH_NAMESPACE_END

#include <freecloth/resmgt/rcShdPtr.inline.h>

#endif
