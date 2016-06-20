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

#ifndef freecloth_gfx_gfxWindow_h
#define freecloth_gfx_gfxWindow_h

#ifndef freecloth_gfx_package_h
#include <freecloth/gfx/package.h>
#endif

#ifndef freecloth_resmgt_rcBase_h
#include <freecloth/resmgt/rcBase.h>
#endif

#ifndef freecloth_base_list
#include <freecloth/base/list>
#endif

////////////////////////////////////////////////////////////////////////////////
// FORWARD DECLARATIONS

class GfxWindowObserver;

////////////////////////////////////////////////////////////////////////////////
/*!
 * \class GfxWindow freecloth/gfx/gfxWindow.h
 * \brief Interface for window creation and messages.
 *
 */
class GfxWindow : public RCBase {
public:

    //----- types and enumerations -----

    //! Key definitions.
    enum KeyID {

        // The only pattern here is to follow the ASCII conventions as
        // much as possible.

        KB_NONE = 0,

        KB_BACKSPACE = 0x08,
        KB_TAB = '\t',      // 0x09
        KB_RETURN = '\n',   // 0x0d
        KB_ENTER = KB_RETURN,
        KB_ESCAPE = 0x1B,
        KB_SPACE = ' ',     // 0x20

        KB_LEFT,
        KB_RIGHT,
        KB_UP,
        KB_DOWN,

        // 0x30 - 0x39
        KB_0 = '0', KB_1 = '1', KB_2 = '2', KB_3 = '3',
        KB_4 = '4', KB_5 = '5', KB_6 = '6', KB_7 = '7',
        KB_8 = '8', KB_9 = '9',

        // 0x41 - 0x5a
        KB_A = 'A', KB_B = 'B', KB_C = 'C', KB_D = 'D',
        KB_E = 'E', KB_F = 'F', KB_G = 'G', KB_H = 'H',
        KB_I = 'I', KB_J = 'J', KB_K = 'K', KB_L = 'L',
        KB_M = 'M', KB_N = 'N', KB_O = 'O', KB_P = 'P',
        KB_Q = 'Q', KB_R = 'R', KB_S = 'S', KB_T = 'T',
        KB_U = 'U', KB_V = 'V', KB_W = 'W', KB_X = 'X',
        KB_Y = 'Y', KB_Z = 'Z'
    };
    //! Modifiers, combined in a bitfield.
    enum {
        KBM_ALT = 0x01,
        KBM_CONTROL = 0x02,
        KBM_SHIFT = 0x04
    };
    //! Mouse button ids
    //! Sadly, MB_ prefix conflicts with Win32 constants
    enum ButtonID {
        MBN_NONE = 0,

        MBN_LEFT,
        MBN_RIGHT,
        MBN_MIDDLE
    };
    //! Modifier bitfield. (Not the same as the enum since it can contain
    //! arbitrary combinations of the above enum entries.)
    typedef UInt32  ModBitfield;

    //----- static member functions -----

    //! Add a static observer.
    //! The observer must live longer than window.
    // FIXME: use a shared-pointer to assure that this condition is
    // satisfied.
    // FIXME: kill these off; they're an awful hack.
    static void addStaticObserver( GfxWindowObserver& );
    static void removeStaticObserver( GfxWindowObserver& );

    //----- member functions -----

    GfxWindow();
    virtual ~GfxWindow();

    virtual UInt32 getWidth() const = 0;
    virtual UInt32 getHeight() const = 0;
    Float getAspectRatio() const;
    virtual void close() = 0;

    //! Add an observer.
    //! The observer must live longer than window.
    // FIXME: use a shared-pointer to assure that this condition is
    // satisfied.
    virtual void addObserver( GfxWindowObserver& );
    virtual void removeObserver( GfxWindowObserver& );

protected:
    //----- member functions -----

    //! Just prior to closing the window. Not sent to static observers.
    void closeReceived();
    //! Redraw requested...
    void displayReceived();
    void idleReceived();
    void keyPressed( KeyID, ModBitfield );
    void keyDownReceived( KeyID, ModBitfield );
    void keyUpReceived( KeyID, ModBitfield );
    void mouseDownReceived( UInt32 x, UInt32 y, ButtonID );
    void mouseUpReceived( UInt32 x, UInt32 y, ButtonID );
    void mouseMoveReceived( UInt32 x, UInt32 y );
    void windowResized( UInt32 x, UInt32 y, UInt32 width, UInt32 height );
    void uiReceived( UInt32 uid );

private:

    //----- classes -----
    class ObserverIterator;

    //----- friends -----
    friend class ObserverIterator;

    //----- types and enumerations -----
    typedef std::list<GfxWindowObserver*> ObserverList;

    //----- member functions -----
    ObserverIterator allObserversBegin();
    ObserverIterator allObserversEnd();

    //----- data members -----

    // @{
    //! Prohibited.
    GfxWindow( GfxWindow const& );
    GfxWindow& operator=( GfxWindow const& );
    // @}

    ObserverList    _observers;

    //----- static data members -----

    static ObserverList _staticObservers;
};
#endif
