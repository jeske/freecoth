//////////////////////////////////////////////////////////////////////
// Copyright (c) 2002-2003 David Pritchard <drpritch@alumni.uwaterloo.ca>
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

#ifndef freecloth_gfx_gfxGLWindowGLUI_h
#define freecloth_gfx_gfxGLWindowGLUI_h

#ifndef freecloth_gfx_package_h
#include <freecloth/gfx/package.h>
#endif

#ifndef freecloth_gfx_gfxGLWindowGLUT_h
#include <freecloth/gfx/gfxGLWindowGLUT.h>
#endif

////////////////////////////////////////////////////////////////////////////////
// FORWARD DECLARATIONS


FREECLOTH_NAMESPACE_START
    class GeMatrix4;
FREECLOTH_NAMESPACE_END

////////////////////////////////////////////////////////////////////////////////
/*!
 * \class GfxGLWindowGLUI freecloth/gfx/gfxGLWindowGLUI.h
 * \brief Window class for GLUI
 *
 * GLUI has an awful C++ API, and some terrible user interface controls,
 * but it's the simplest portable windowing API I could find.
 *
 * Some of the semantics of the GfxWindow class have been abused here. The
 * getWidth()/getHeight() methods still return the width and height of the
 * entire window, but the observor windowResized() callback now receives the
 * width and height of the window *excluding* the GLUI panel on the right side.
 */
class GfxGLWindowGLUI : public GfxGLWindowGLUT {

public:

    //----- types and enumerations -----
    typedef GfxGLWindowGLUT BaseClass;
    typedef UInt32 ControlID;
    typedef UInt32 PanelID;

    enum TranslateType {
        TR_XY,
        TR_X,
        TR_Y,
        TR_Z
    };
    enum EditType {
        EDIT_TEXT,
        EDIT_INT,
        EDIT_FLOAT
    };
    enum BorderType {
        BORDER_NONE,
        BORDER_RAISED,
        BORDER_EMBOSSED
    };
    enum {
        PANEL_NONE = ~0U,
        CTRL_NONE = ~0U
    };

    //----- member functions -----

    GfxGLWindowGLUI(
        const GfxConfig&,
        const String& title
    );
    ~GfxGLWindowGLUI();

    // Add UI controls
    void addGroup( BorderType, PanelID, PanelID parent = PANEL_NONE );
    void addPanel( const String& label, PanelID, PanelID parent = PANEL_NONE );
    void addRollout(
        const String& label,
        PanelID,
        bool open,
        PanelID parent = PANEL_NONE
    );
    void addColumn( bool showDivider = true, PanelID = PANEL_NONE );
    void addButton( const String& label, ControlID, PanelID = PANEL_NONE );
    void addRotate( const String& label, ControlID, PanelID = PANEL_NONE );
    void addTranslate(
        const String& label,
        TranslateType,
        ControlID,
        PanelID = PANEL_NONE
    );
    void addText(
        const String& label,
        ControlID = CTRL_NONE,
        PanelID = PANEL_NONE
    );
    void addEditText( const String& label, ControlID, PanelID = PANEL_NONE );
    void addEditInt( const String& label, ControlID, PanelID = PANEL_NONE );
    void addEditFloat( const String& label, ControlID, PanelID = PANEL_NONE );
    void addEditBox(
        const String& label,
        EditType,
        ControlID,
        PanelID = PANEL_NONE
    );
    void addCheckbox( const String& label, ControlID, PanelID = PANEL_NONE );
    void addRadioGroup( ControlID, PanelID = PANEL_NONE );
    void addRadioButton( ControlID group, const String& label );

    void setRotate( ControlID, const GeMatrix4& );
    void setTranslateX( ControlID, Float );
    void setTranslateY( ControlID, Float );
    void setTranslateZ( ControlID, Float );
    void setText( ControlID, const String& );
    void setCheckbox( ControlID, bool );
    void setRadioGroup( ControlID, UInt32 );
    void setEditText( ControlID, const String& );
    void setEditInt( ControlID, Int32 );
    void setEditFloat( ControlID, Float );
    void setEditIntLimits(
        ControlID,
        Int32 low,
        Int32 high,
        bool wrap = false
    );
    void setEditFloatLimits(
        ControlID,
        Float low,
        Float high,
        bool wrap = false
    );

    GeMatrix4 getRotate( ControlID ) const;
    Float getTranslateX( ControlID ) const;
    Float getTranslateY( ControlID ) const;
    Float getTranslateZ( ControlID ) const;
    bool getCheckbox( ControlID ) const;
    UInt32 getRadioGroup( ControlID ) const;
    String getEditText( ControlID ) const;
    Int32 getEditInt( ControlID ) const;
    Float getEditFloat( ControlID ) const;

    void enable( ControlID );
    void disable( ControlID );

protected:

    //----- static member functions -----

    static void reshapeCB( int w, int h );
    static void controlCB( int control );

    //----- member functions -----

    virtual void registerIdleCB();
    virtual void registerCBs();

private:

    //----- classes -----
    class Imp;

    //----- member functions -----

    // Disallowed.
    GfxGLWindowGLUI( const GfxGLWindowGLUI& );
    GfxGLWindowGLUI& operator = ( const GfxGLWindowGLUI& );

    //----- data members -----

    Imp* _imp;
};

#endif
