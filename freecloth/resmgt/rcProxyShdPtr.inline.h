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

#ifndef freecloth_resmgt_rcProxyShdPtr_inline_h
#define freecloth_resmgt_rcProxyShdPtr_inline_h

#ifndef freecloth_base_debug_h
#include <freecloth/base/debug.h>
#endif

FREECLOTH_NAMESPACE_START

////////////////////////////////////////////////////////////////////////////////
// CLASS RCProxyShdPtrBase

//------------------------------------------------------------------------------
inline RCProxyShdPtrBase::RCProxyShdPtrBase()
    : _data( 0 )
{
}

////////////////////////////////////////////////////////////////////////////////
// CLASS RCProxyShdPtrBase::Data

//------------------------------------------------------------------------------
inline RCProxyShdPtrBase::Data::Data(
    void* ptr,
    UInt32 count
) : _ptr( ptr ),
    _count( count )
{
}


////////////////////////////////////////////////////////////////////////////////
// CLASS RCProxyShdPtr

//------------------------------------------------------------------------------
template <class T>
inline RCProxyShdPtr<T>::RCProxyShdPtr( T* ptr )
{
    //DGFX_TRACE_ENTER( "RCProxyShdPtr<" << typeid(T).name() << "> ctor" );
    //DGFX_TRACE( "ptr = " << static_cast<void *>( ptr ) );
    //DGFX_TRACE( "this = " << this );
    if ( 0 != ptr ) {
        _data = new Data( ptr );
    }
    //DGFX_TRACE( "count = " << ( _data ? _data->_count : 0 ) );
}

//------------------------------------------------------------------------------
template <class T>
inline RCProxyShdPtr<T>::RCProxyShdPtr( const RCProxyShdPtr& rhs )
{
    //DGFX_TRACE_ENTER( "RCProxyShdPtr<" << typeid(T).name() << "> copy ctor" );
    //DGFX_TRACE( "rhs = " << &rhs );
    acquire( rhs._data );
}

//------------------------------------------------------------------------------
template <class T>
inline RCProxyShdPtr<T>::~RCProxyShdPtr()
{
    //DGFX_TRACE_ENTER( "RCProxyShdPtr<" << typeid(T).name() << "> dtor" );
    //DGFX_TRACE( "this = " << this );
    release();
}

//------------------------------------------------------------------------------
template <class T>
inline RCProxyShdPtr<T>&
RCProxyShdPtr<T>::operator=( const RCProxyShdPtr<T>& rhs )
{
    //DGFX_TRACE_ENTER( "RCProxyShdPtr<" << typeid(T).name() << ">::operator=" );
    //DGFX_TRACE( "this = " << this );
    //DGFX_TRACE( "rhs = " << &rhs );
    if ( this != &rhs ) {
        release();
        acquire( rhs._data );
    }
    return *this;
}

//------------------------------------------------------------------------------
template <class T>
inline bool RCProxyShdPtr<T>::operator==( const RCProxyShdPtr<T>& rhs ) const
{
    return _data == rhs._data;
}

//------------------------------------------------------------------------------
template <class T>
inline bool RCProxyShdPtr<T>::operator!=( const RCProxyShdPtr<T>& rhs ) const
{
    return _data != rhs._data;
}

#if 0
//------------------------------------------------------------------------------
template <class T,class X>
inline RCProxyShdPtr<T>::RCProxyShdPtr( const RCProxyShdPtr<X>& rhs )
{
    // verify that X* can be converted to T* (i.e. X* is const T*, or
    // T is a base class of X)
    X* x;
    T* t = x;

    // FIXME: will this work?
    acquire( rhs._data );
}

//------------------------------------------------------------------------------
template <class T,class X>
inline RCProxyShdPtr<T>&
RCProxyShdPtr<T>::operator=( const RCProxyShdPtr<X>& rhs )
{
    DGFX_ASSERT( this != &rhs );

    // verify that X* can be converted to T* (i.e. X* is const T*, or
    // T is a base class of X)
    X* x;
    T* t = x;

    if ( this != &rhs ) {
        release();
        acquire( rhs._data );
    }
    return *this;
}
#endif

//------------------------------------------------------------------------------
template <class T>
inline bool RCProxyShdPtr<T>::isNull() const
{
    return _data == 0;
}

//------------------------------------------------------------------------------
template <class T>
inline T& RCProxyShdPtr<T>::operator*() const
{
    DGFX_ASSERT( ! isNull() );
    DGFX_ASSERT( 0 != _data->_ptr );
    return *static_cast<T*>( _data->_ptr );
}

//------------------------------------------------------------------------------
template <class T>
inline T* RCProxyShdPtr<T>::operator->() const
{
    DGFX_ASSERT( ! isNull() );
    DGFX_ASSERT( 0 != _data->_ptr );
    return static_cast<T*>( _data->_ptr );
}

//------------------------------------------------------------------------------
template <class T>
inline T* RCProxyShdPtr<T>::get() const
{
    return static_cast<T*>( 0 != _data ? _data->_ptr : 0 );
}

//------------------------------------------------------------------------------
template <class T>
inline void RCProxyShdPtr<T>::acquire( Data* data )
{
    //DGFX_TRACE_ENTER( "RCProxyShdPtr<" << typeid(T).name() << ">::acquire" );
    //DGFX_TRACE( "this = " << this );
    //DGFX_TRACE( "ptr = " << static_cast<void *>( data ? data->_ptr : 0 ) );
    //DGFX_TRACE( "count = " << ( data ? data->_count : 0 ) );
    _data = data;
    if ( 0 != _data ) {
        ++_data->_count;
    }
}

//------------------------------------------------------------------------------
template <class T>
inline void RCProxyShdPtr<T>::release()
{
    //DGFX_TRACE_ENTER( "RCProxyShdPtr<" << typeid(T).name() << ">::release" );
    //DGFX_TRACE( "this = " << this );
    //DGFX_TRACE( "ptr = " << static_cast<void *>( isNull() ? 0 : _data->_ptr) );
    //DGFX_TRACE( "count = " << ( _data ? _data->_count : 0 ) );
    if ( _data ) {
        --_data->_count;
        if ( _data->_count == 0 ) {
            delete static_cast<T*>( _data->_ptr );
            delete _data;
        }
        _data = 0;
    }
}



////////////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTIONS

//------------------------------------------------------------------------------
template <class T>
inline std::ostream& operator<<(
    std::ostream& out,
    const RCProxyShdPtr<T>& ptr
) {
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
