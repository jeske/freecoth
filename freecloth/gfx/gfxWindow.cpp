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

#include <freecloth/gfx/gfxWindow.h>
#include <freecloth/gfx/gfxWindowObserver.h>

////////////////////////////////////////////////////////////////////////////////
// LOCAL DECLARATIONS

////////////////////////////////////////////////////////////////////////////////
// CLASS GfxWindow::ObserverIterator
class GfxWindow::ObserverIterator
{
public:

    //----- types and enumerations -----

    typedef GfxWindow::ObserverList::value_type Item;

    //----- static member functions -----

    // Named constructor.
    static ObserverIterator begin( GfxWindow& window )
    {
        return ObserverIterator(
            window,
            GfxWindow::_staticObservers.begin(),
            window._observers.begin()
        );
    }

    // Name constructor.
    static ObserverIterator end( GfxWindow& window )
    {
        return ObserverIterator(
            window,
            GfxWindow::_staticObservers.end(),
            window._observers.end()
        );
    }

    //----- member functions -----

    // Default constructor. Iterators are invalid!
    inline ObserverIterator()
    { }

    // Copy constructor.
    inline ObserverIterator( const ObserverIterator& other )
        : _si( other._si ),
        _i( other._i )
    { }

    // Compiler default operator= is fine.

    // Standard iterator operators.
    // <group>
    inline ObserverIterator& operator ++ ()
    {
        if( inStatic() ) {
            ++_si;
        }
        else {
            ++_i;
        }
        return *this;
    }
    inline Item& operator * ()
    {
        return inStatic() ? *_si : *_i;
    }
    inline const Item& operator * () const
    {
        return inStatic() ? *_si : *_i;
    }
    inline bool operator == ( const ObserverIterator& other ) const
    {
        return
            _si == other._si &&
            _i == other._i;
    }
    inline bool operator != ( const ObserverIterator& other ) const
    {
        return ! operator==( other );
    }
    // </group>

private:

    //----- member functions -----

    // Private constructor.
    inline ObserverIterator(
        GfxWindow& window,
        const GfxWindow::ObserverList::iterator& si,
        const GfxWindow::ObserverList::iterator& i
    ) : _si( si ),
        _i( i )
    { }

    inline bool inStatic() const {
        return _si != GfxWindow::_staticObservers.end();
    }

    //----- data members -----

    ObserverList::iterator _si;
    ObserverList::iterator _i;

};

////////////////////////////////////////////////////////////////////////////////
// CLASS GfxWindow

GfxWindow::ObserverList GfxWindow::_staticObservers;

//------------------------------------------------------------------------------

GfxWindow::GfxWindow()
{
}

//------------------------------------------------------------------------------

GfxWindow::~GfxWindow()
{
    ObserverList o2( _observers );
    ObserverList::iterator i;
    for( i = o2.begin(); i != o2.end(); ++i ) {
        (*i)->destroyed( *this );
    }
}

//------------------------------------------------------------------------------

Float GfxWindow::getAspectRatio() const
{
    return static_cast<Float>( getWidth() ) / getHeight();
}

//------------------------------------------------------------------------------

void GfxWindow::addStaticObserver( GfxWindowObserver& observer )
{
    _staticObservers.push_back( &observer );
}

//------------------------------------------------------------------------------

void GfxWindow::removeStaticObserver( GfxWindowObserver& observer )
{
    _staticObservers.remove( &observer );
}

//------------------------------------------------------------------------------

void GfxWindow::addObserver( GfxWindowObserver& observer )
{
    _observers.push_back( &observer );
}

//------------------------------------------------------------------------------

void GfxWindow::removeObserver( GfxWindowObserver& observer )
{
    _observers.remove( &observer );
}

//------------------------------------------------------------------------------

GfxWindow::ObserverIterator GfxWindow::allObserversBegin()
{
    return ObserverIterator::begin( *this );
}

//------------------------------------------------------------------------------

GfxWindow::ObserverIterator GfxWindow::allObserversEnd()
{
    return ObserverIterator::end( *this );
}

//------------------------------------------------------------------------------

void GfxWindow::closeReceived()
{
    ObserverList o2( _observers );
    ObserverList::iterator i;
    for( i = o2.begin(); i != o2.end(); ++i ) {
        (*i)->closeReceived( *this );
    }
}

//------------------------------------------------------------------------------

void GfxWindow::displayReceived()
{
    ObserverIterator i;
    for( i = allObserversBegin(); i != allObserversEnd(); ++i ) {
        (*i)->displayReceived( *this );
    }
}

//------------------------------------------------------------------------------

void GfxWindow::idleReceived()
{
    ObserverIterator i;
    for( i = allObserversBegin(); i != allObserversEnd(); ++i ) {
        (*i)->idleReceived();
    }
}

//------------------------------------------------------------------------------

void GfxWindow::keyPressed( KeyID keyId, ModBitfield modbits )
{
    DGFX_TRACE_ENTER( "GfxWindow::keyPressed" );
    ObserverIterator i;
    for( i = allObserversBegin(); i != allObserversEnd(); ++i ) {
        (*i)->keyPressed( *this, keyId, modbits );
    }
}

//------------------------------------------------------------------------------

void GfxWindow::keyDownReceived( KeyID keyId, ModBitfield modbits )
{
    ObserverIterator i;
    for( i = allObserversBegin(); i != allObserversEnd(); ++i ) {
        (*i)->keyDownReceived( *this, keyId, modbits );
    }
}

//------------------------------------------------------------------------------

void GfxWindow::keyUpReceived( KeyID keyId, ModBitfield modbits )
{
    ObserverIterator i;
    for( i = allObserversBegin(); i != allObserversEnd(); ++i ) {
        (*i)->keyUpReceived( *this, keyId, modbits );
    }
}

//------------------------------------------------------------------------------

void GfxWindow::mouseDownReceived( UInt32 x, UInt32 y, ButtonID button )
{
    ObserverIterator i;
    for( i = allObserversBegin(); i != allObserversEnd(); ++i ) {
        (*i)->mouseDownReceived( *this, x, y, button );
    }
}

//------------------------------------------------------------------------------

void GfxWindow::mouseUpReceived( UInt32 x, UInt32 y, ButtonID button )
{
    ObserverIterator i;
    for( i = allObserversBegin(); i != allObserversEnd(); ++i ) {
        (*i)->mouseUpReceived( *this, x, y, button );
    }
}

//------------------------------------------------------------------------------

void GfxWindow::mouseMoveReceived( UInt32 x, UInt32 y )
{
    ObserverIterator i;
    for( i = allObserversBegin(); i != allObserversEnd(); ++i ) {
        (*i)->mouseMoveReceived( *this, x, y );
    }
}

//------------------------------------------------------------------------------

void GfxWindow::windowResized( UInt32 x, UInt32 y, UInt32 width, UInt32 height )
{
    ObserverIterator i;
    for( i = allObserversBegin(); i != allObserversEnd(); ++i ) {
        (*i)->windowResized( *this, x, y, width, height );
    }
}

//------------------------------------------------------------------------------

void GfxWindow::uiReceived( UInt32 uid )
{
    ObserverIterator i;
    for( i = allObserversBegin(); i != allObserversEnd(); ++i ) {
        (*i)->uiReceived( *this, uid );
    }
}
