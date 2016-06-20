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

#ifndef freecloth_resmgt_rcShdPtr_inline_h
#define freecloth_resmgt_rcShdPtr_inline_h

#ifndef freecloth_resmgt_rcBase_h
#include <freecloth/resmgt/rcBase.h>
#endif

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// CLASS RCShdPtrBase

//------------------------------------------------------------------------------

inline bool RCShdPtrBase::operator==( const RCBase* rhs ) const
{
    return _ptr == rhs;
}

//------------------------------------------------------------------------------

inline bool RCShdPtrBase::operator!=( const RCBase* rhs ) const
{
    return _ptr != rhs;
}

//------------------------------------------------------------------------------

inline RCShdPtrBase::RCShdPtrBase()
    : _ptr( 0 )
{
}

//------------------------------------------------------------------------------

inline void RCShdPtrBase::acquire( RCBase* ptr )
{
#if RCSHDPTR_DEBUG
    DGFX_TRACE_ENTER( "RCShdPtrBase::acquire" );
    DGFX_TRACE( "this = " << this );
    DGFX_TRACE( "ptr = " << static_cast<void *>( ptr ) );
    DGFX_TRACE( "count = " << (ptr ? ptr->_rcCount : 0 ) );
#endif
    _ptr = ptr;
    if ( 0 != _ptr ) {
        ++_ptr->_rcCount;
    }
}

//------------------------------------------------------------------------------

inline void RCShdPtrBase::release()
{
#if RCSHDPTR_DEBUG
    DGFX_TRACE_ENTER( "RCShdPtrBase::release" );
    DGFX_TRACE( "this = " << this );
    DGFX_TRACE( "ptr = " << static_cast<void *>( _ptr ) );
    DGFX_TRACE( "count = " << ( _ptr ? _ptr->_rcCount : 0 ) );
#endif
    if ( _ptr ) {
        if ( _ptr->_rcCount == 1 ) {
            delete _ptr;
        }
        else {
            --_ptr->_rcCount;
        }
        
        _ptr = 0;
    }
}

//------------------------------------------------------------------------------

inline bool operator==( const RCBase* lhs, const RCShdPtrBase& rhs )
{
    return rhs == lhs;
}

//------------------------------------------------------------------------------

inline bool operator!=( const RCBase* lhs, const RCShdPtrBase& rhs )
{
    return rhs != lhs;
}


////////////////////////////////////////////////////////////////////////////////
// CLASS RCShdPtr

//------------------------------------------------------------------------------

template <class T>
inline RCShdPtr<T>::RCShdPtr( T* ptr )
{
#if RCSHDPTR_DEBUG
    DGFX_TRACE_ENTER( "RCShdPtr<" << typeid(T).name() << "> ctor" );
    DGFX_TRACE( "ptr = " << static_cast<void *>( ptr ) );
    DGFX_TRACE( "this = " << this );
#endif
    acquire( ptr );
    //DGFX_TRACE( "count = " << ( _ptr ? _ptr->_rcCount : 0 ) );
}

//------------------------------------------------------------------------------

template <class T>
inline RCShdPtr<T>::RCShdPtr( const RCShdPtr& rhs )
{
#if RCSHDPTR_DEBUG
    DGFX_TRACE_ENTER( "RCShdPtr<" << typeid(T).name() << "> copy ctor" );
    DGFX_TRACE( "rhs = " << &rhs );
#endif
    acquire( rhs._ptr );
}

//------------------------------------------------------------------------------

template <class T>
inline RCShdPtr<T>::~RCShdPtr()
{
#if RCSHDPTR_DEBUG
    DGFX_TRACE_ENTER( "RCShdPtr<" << typeid(T).name() << "> dtor" );
    DGFX_TRACE( "this = " << this );
#endif
    release();
}

//------------------------------------------------------------------------------

template <class T>
inline RCShdPtr<T>&
RCShdPtr<T>::operator=( const RCShdPtr<T>& rhs )
{
#if RCSHDPTR_DEBUG
    DGFX_TRACE_ENTER( "RCShdPtr<" << typeid(T).name() << ">::operator=" );
    DGFX_TRACE( "this = " << this );
    DGFX_TRACE( "rhs = " << &rhs );
#endif
    if ( this != &rhs ) {
        release();
        acquire( rhs._ptr );
    }
    return *this;
}

//------------------------------------------------------------------------------

template <class T>
inline bool RCShdPtr<T>::operator==( const RCShdPtr<T>& rhs ) const
{
    return _ptr == rhs._ptr;
}

//------------------------------------------------------------------------------

template <class T>
inline bool RCShdPtr<T>::operator!=( const RCShdPtr<T>& rhs ) const
{
    return _ptr != rhs._ptr;
}

//------------------------------------------------------------------------------

template <class T>
inline bool RCShdPtr<T>::operator==( const RCBase* rhs ) const
{
    return _ptr == rhs;
}

//------------------------------------------------------------------------------

template <class T>
inline bool RCShdPtr<T>::operator!=( const RCBase* rhs ) const
{
    return _ptr != rhs;
}

//------------------------------------------------------------------------------

template <class T>
inline bool RCShdPtr<T>::isNull() const
{
    return _ptr == 0;
}

//------------------------------------------------------------------------------

template <class T>
inline T& RCShdPtr<T>::operator*() const
{
    DGFX_ASSERT( ! isNull() );
#if RCSHDPTR_DEBUG
    DGFX_ASSERT( 0 != _ptr );
#endif
    // FIXME: using dynamic_cast
    return *reinterpret_cast<T*>( _ptr );
}

//------------------------------------------------------------------------------

template <class T>
inline T* RCShdPtr<T>::operator->() const
{
    DGFX_ASSERT( ! isNull() );
#if RCSHDPTR_DEBUG
    DGFX_ASSERT( 0 != _ptr );
#endif
    return reinterpret_cast<T*>( _ptr );
}

//------------------------------------------------------------------------------

template <class T>
inline T* RCShdPtr<T>::get() const
{
    return reinterpret_cast<T*>( _ptr );
}



////////////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTIONS

//------------------------------------------------------------------------------

template <class T>
inline std::ostream& operator<<( std::ostream& out, const RCShdPtr<T>& ptr )
{
    if ( ptr.isNull() ) {
        out << "(null)";
    }
    else {
        out << *ptr;
    }
    return out;
}

FREECLOTH_NAMESPACE_END

#endif
