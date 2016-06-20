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

#include <freecloth/gfx/gfxGLWindowGLUI.h>
#include <freecloth/geom/geMatrix4.h>
#include <freecloth/resmgt/rcProxyShdPtr.h>
#include <freecloth/base/vector>
#include <freecloth/base/map>
#include <freecloth/base/GL_glut.h>
#include <freecloth/base/glui.h>

////////////////////////////////////////////////////////////////////////////////
// LOCAL DECLARATIONS

#if COMPILER_MSVC
// MSVC6 has bugs in its dynamic_cast<> implementation, causing crashes.
// The reinterpret_cast<> implementation doesn't have this bug.
#define DYNAMIC_CAST_REF(type,obj) \
    reinterpret_cast<type&>( obj )
#else
#define DYNAMIC_CAST_REF(type,obj) \
    dynamic_cast<type&>( obj )
#endif

////////////////////////////////////////////////////////////////////////////////
// CLASS GfxGLWindowGLUI::Imp

class GfxGLWindowGLUI::Imp {
public:
    //----- types and enumerations -----
    typedef GfxGLWindowGLUI::ControlID ControlID;
    typedef GfxGLWindowGLUI::PanelID PanelID;
    typedef UInt32 FullControlID;
    typedef RCProxyShdPtr< GLUI_Control > ControlPtr;
    typedef RCProxyShdPtr< GLUI_Panel > PanelPtr;
    typedef std::list< ControlPtr > ControlList;
    typedef std::map< ControlID, ControlPtr > ControlMap;
    typedef std::map< PanelID, PanelPtr > PanelMap;

    //----- member functions -----
    FullControlID makeFullControlID( const GfxGLWindowGLUI&, ControlID );
    ControlID getControlID( FullControlID );

    void addControl( GLUI_Control*, ControlID );
    void addPanel( GLUI_Panel*, PanelID );

    GLUI_Control& getControl( GfxGLWindowGLUI::ControlID );
    const GLUI_Control& getControl( GfxGLWindowGLUI::ControlID ) const;
    GLUI_Panel& getPanel( GfxGLWindowGLUI::PanelID );
    
    //----- data members -----
    
    GLUI* _glui;

private:

    //----- data members -----
    
    ControlMap _controlMap;
    PanelMap _panelMap;

    // to hold pointers to id-less controls
    ControlList _controlList;
};

//------------------------------------------------------------------------------

GfxGLWindowGLUI::Imp::FullControlID GfxGLWindowGLUI::Imp::makeFullControlID(
    const GfxGLWindowGLUI&,
    ControlID id
) {
    // Eventually, when we support multiple GLUIs, this should combine window
    // and id
    return id;
}

//------------------------------------------------------------------------------

GfxGLWindowGLUI::Imp::ControlID GfxGLWindowGLUI::Imp::getControlID(
    FullControlID fid
) {
    return fid;
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUI::Imp::addControl( GLUI_Control* control, ControlID id )
{
    DGFX_ASSERT( control != 0 );
    if ( id != GfxGLWindowGLUI::CTRL_NONE ) {
        DGFX_ASSERT( _controlMap.find( id ) == _controlMap.end() );
        _controlMap[ id ] = ControlPtr( control );
    }
    else {
        _controlList.push_back( ControlPtr( control ) );
    }
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUI::Imp::addPanel( GLUI_Panel* panel, PanelID id ) {
    DGFX_ASSERT( panel != 0 && id != GfxGLWindowGLUI::PANEL_NONE );
    DGFX_ASSERT( _panelMap.find( id ) == _panelMap.end() );
    _panelMap[ id ] = PanelPtr( panel );
}

//------------------------------------------------------------------------------

GLUI_Control& GfxGLWindowGLUI::Imp::getControl( ControlID id ) {
    ControlMap::iterator i = _controlMap.find( id );
    DGFX_ASSERT( i != _controlMap.end() );
    return *i->second;
}

//------------------------------------------------------------------------------

const GLUI_Control& GfxGLWindowGLUI::Imp::getControl( ControlID id ) const {
    ControlMap::const_iterator i = _controlMap.find( id );
    DGFX_ASSERT( i != _controlMap.end() );
    return *i->second;
}

//------------------------------------------------------------------------------

GLUI_Panel& GfxGLWindowGLUI::Imp::getPanel( PanelID id ) {
    PanelMap::iterator i = _panelMap.find( id );
    DGFX_ASSERT( i != _panelMap.end() );
    return *i->second;
}


////////////////////////////////////////////////////////////////////////////////
// CLASS GfxGLWindowGLUI

//------------------------------------------------------------------------------

GfxGLWindowGLUI::GfxGLWindowGLUI(
    const GfxConfig& config,
    const String& title
) : GfxGLWindowGLUT( config, title ),
    _imp( new Imp )
{
    _imp->_glui = GLUI_Master.create_glui_subwindow(
        _windowId, GLUI_SUBWINDOW_RIGHT
    );
    _imp->_glui->set_main_gfx_window( _windowId );
}

//------------------------------------------------------------------------------

GfxGLWindowGLUI::~GfxGLWindowGLUI()
{
    delete _imp;
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUI::addGroup(
    BorderType borderType,
    PanelID pid,
    PanelID parent
) {
    UInt32 gtypes[] = {
        GLUI_PANEL_NONE, GLUI_PANEL_RAISED, GLUI_PANEL_EMBOSSED
    };
    DGFX_ASSERT( pid != PANEL_NONE );
    GLUI_Panel* panel;
    if ( parent != PANEL_NONE ) {
        panel = _imp->_glui->add_panel_to_panel(
            &_imp->getPanel( parent ),
            "",
            gtypes[ static_cast<UInt32>( borderType ) ]
        );
    }
    else {
        panel = _imp->_glui->add_panel(
            "",
            gtypes[ static_cast<UInt32>( borderType ) ]
        );
    }
    _imp->addPanel( panel, pid );
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUI::addPanel(
    const String& name,
    PanelID pid,
    PanelID parent
) {
    DGFX_ASSERT( pid != PANEL_NONE );
    GLUI_Panel* panel;
    if ( parent != PANEL_NONE ) {
        panel = _imp->_glui->add_panel_to_panel(
            &_imp->getPanel( parent ),
            const_cast<char*>( name.c_str() ),  // FIXME: stupid GLUI!
            GLUI_PANEL_EMBOSSED
        );
    }
    else {
        panel = _imp->_glui->add_panel(
            const_cast<char*>( name.c_str() ),  // FIXME: stupid GLUI!
            GLUI_PANEL_EMBOSSED
        );
    }
    _imp->addPanel( panel, pid );
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUI::addRollout(
    const String& name,
    PanelID pid,
    bool open,
    PanelID parent
) {
    DGFX_ASSERT( pid != PANEL_NONE );
    GLUI_Panel* panel;
    if ( parent != PANEL_NONE ) {
        panel = _imp->_glui->add_rollout_to_panel(
            &_imp->getPanel( parent ),
            const_cast<char*>( name.c_str() ),  // FIXME: stupid GLUI!
            open
        );
    }
    else {
        panel = _imp->_glui->add_rollout(
            const_cast<char*>( name.c_str() ),  // FIXME: stupid GLUI!
            open
        );
    }
    _imp->addPanel( panel, pid );
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUI::addColumn( bool showDivider, PanelID pid )
{
    if ( pid != PANEL_NONE ) {
        _imp->_glui->add_column_to_panel(
            &_imp->getPanel( pid ),
            showDivider
        );
    }
    else {
        _imp->_glui->add_column( showDivider );
    }
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUI::addButton( const String& name, ControlID id, PanelID pid )
{
    Imp::FullControlID fid = _imp->makeFullControlID( *this, id );
    GLUI_Control* ctrl;
    if ( pid != PANEL_NONE ) {
        ctrl = _imp->_glui->add_button_to_panel(
            &_imp->getPanel( pid ),
            const_cast<char*>( name.c_str() ), // FIXME: stupid GLUI!
            fid,
            controlCB
        );
    }
    else {
        ctrl = _imp->_glui->add_button(
            const_cast<char*>( name.c_str() ), // FIXME: stupid GLUI!
            fid,
            controlCB
        );
    }
    _imp->addControl( ctrl, id );
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUI::addRotate( const String& name, ControlID id, PanelID pid )
{
    Imp::FullControlID fid = _imp->makeFullControlID( *this, id );
    GLUI_Rotation* rot;
    if ( pid != PANEL_NONE ) {
        rot = _imp->_glui->add_rotation_to_panel(
            &_imp->getPanel( pid ),
            const_cast<char*>( name.c_str() ), // FIXME: stupid GLUI!
            NULL,
            fid,
            controlCB
        );
    }
    else {
        rot = _imp->_glui->add_rotation(
            const_cast<char*>( name.c_str() ), // FIXME: stupid GLUI!
            NULL,
            fid,
            controlCB
        );
    }
    rot->reset();
    _imp->addControl( rot, id );
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUI::addTranslate(
    const String& name,
    TranslateType type,
    ControlID id,
    PanelID pid
) {
    UInt32 gtypes[] = {
        GLUI_TRANSLATION_XY,
        GLUI_TRANSLATION_X,
        GLUI_TRANSLATION_Y,
        GLUI_TRANSLATION_Z
    };
    Imp::FullControlID fid = _imp->makeFullControlID( *this, id );
    GLUI_Control* ctrl;
    if ( pid != PANEL_NONE ) {
        ctrl = _imp->_glui->add_translation_to_panel(
            &_imp->getPanel( pid ),
            const_cast<char*>( name.c_str() ), // FIXME: stupid GLUI!
            gtypes[ static_cast<UInt32>( type ) ],
            NULL,
            fid,
            controlCB
        );
    }
    else {
        ctrl = _imp->_glui->add_translation(
            const_cast<char*>( name.c_str() ), // FIXME: stupid GLUI!
            gtypes[ static_cast<UInt32>( type ) ],
            NULL,
            fid,
            controlCB
        );
    }
    _imp->addControl( ctrl, id );
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUI::addText( const String& label, ControlID id, PanelID pid )
{
    GLUI_Control* ctrl;
    if ( pid != PANEL_NONE ) {
        ctrl = _imp->_glui->add_statictext_to_panel(
            &_imp->getPanel( pid ),
            const_cast<char*>( label.c_str() ) // FIXME: stupid GLUI!
        );
    }
    else {
        ctrl = _imp->_glui->add_statictext(
            const_cast<char*>( label.c_str() ) // FIXME: stupid GLUI!
        );
    }
    _imp->addControl( ctrl, id );
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUI::addEditText(
    const String& label,
    ControlID id,
    PanelID pid
) {
    addEditBox( label, EDIT_TEXT, id, pid );
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUI::addEditInt(
    const String& label,
    ControlID id,
    PanelID pid
) {
    addEditBox( label, EDIT_INT, id, pid );
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUI::addEditFloat(
    const String& label,
    ControlID id,
    PanelID pid
) {
    addEditBox( label, EDIT_FLOAT, id, pid );
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUI::addEditBox(
    const String& label,
    EditType type,
    ControlID id,
    PanelID pid
) {
    Imp::FullControlID fid = _imp->makeFullControlID( *this, id );
    UInt32 gtypes[] = {
        GLUI_EDITTEXT_TEXT,
        GLUI_EDITTEXT_INT,
        GLUI_EDITTEXT_FLOAT
    };
    GLUI_Control* ctrl;
    if ( pid != PANEL_NONE ) {
        ctrl = _imp->_glui->add_edittext_to_panel(
            &_imp->getPanel( pid ),
            const_cast<char*>( label.c_str() ), // FIXME: stupid GLUI!
            gtypes[ static_cast<UInt32>( type ) ],
            NULL,
            fid,
            controlCB
        );
    }
    else {
        ctrl = _imp->_glui->add_edittext(
            const_cast<char*>( label.c_str() ), // FIXME: stupid GLUI!
            gtypes[ static_cast<UInt32>( type ) ],
            NULL,
            fid,
            controlCB
        );
    }
    _imp->addControl( ctrl, id );
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUI::addCheckbox(
    const String& label,
    ControlID id,
    PanelID pid
) {
    Imp::FullControlID fid = _imp->makeFullControlID( *this, id );
    GLUI_Control* ctrl;
    if ( pid != PANEL_NONE ) {
        ctrl = _imp->_glui->add_checkbox_to_panel(
            &_imp->getPanel( pid ),
            const_cast<char*>( label.c_str() ), // FIXME: stupid GLUI!
            NULL,
            fid,
            controlCB
        );
    }
    else {
        ctrl = _imp->_glui->add_checkbox(
            const_cast<char*>( label.c_str() ), // FIXME: stupid GLUI!
            NULL,
            fid,
            controlCB
        );
    }
    _imp->addControl( ctrl, id );
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUI::addRadioGroup( ControlID id, PanelID pid )
{
    Imp::FullControlID fid = _imp->makeFullControlID( *this, id );
    GLUI_Control* ctrl;
    if ( pid != PANEL_NONE ) {
        ctrl = _imp->_glui->add_radiogroup_to_panel(
            &_imp->getPanel( pid ),
            NULL,
            fid,
            controlCB
        );
    }
    else {
        ctrl = _imp->_glui->add_radiogroup( NULL, fid, controlCB );
    }
    _imp->addControl( ctrl, id );
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUI::addRadioButton( ControlID groupid, const String& label )
{
    GLUI_Control& ctrl = _imp->getControl( groupid );
    GLUI_RadioGroup& group = DYNAMIC_CAST_REF( GLUI_RadioGroup, ctrl );
    GLUI_Control* newctrl = _imp->_glui->add_radiobutton_to_group(
        &group, 
        const_cast<char*>( label.c_str() )  // stupid GLUI!
    );
    _imp->addControl( newctrl, CTRL_NONE );
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUI::setRotate( ControlID id, const GeMatrix4& val )
{
    GLUI_Control& ctrl = _imp->getControl( id );
    GLUI_Rotation& rot = DYNAMIC_CAST_REF( GLUI_Rotation, ctrl );
    // Stupid GLUI!
    rot.set_float_array_val( const_cast<Float*>( val.asColMajor() ) );
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUI::setTranslateX( ControlID id, Float val )
{
    GLUI_Control& ctrl = _imp->getControl( id );
    GLUI_Translation& tr = DYNAMIC_CAST_REF( GLUI_Translation, ctrl );
    tr.set_x( val );
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUI::setTranslateY( ControlID id, Float val )
{
    GLUI_Control& ctrl = _imp->getControl( id );
    GLUI_Translation& tr = DYNAMIC_CAST_REF( GLUI_Translation, ctrl );
    tr.set_y( val );
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUI::setTranslateZ( ControlID id, Float val )
{
    GLUI_Control& ctrl = _imp->getControl( id );
    GLUI_Translation& tr = DYNAMIC_CAST_REF( GLUI_Translation, ctrl );
    tr.set_z( val );
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUI::setText( ControlID id, const String& label )
{
    GLUI_Control& ctrl = _imp->getControl( id );
    GLUI_StaticText& text = DYNAMIC_CAST_REF( GLUI_StaticText, ctrl );
    // FIXME: stupid constless GLUI!
    text.set_text( const_cast<char*>( label.c_str() ) );
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUI::setEditText( ControlID id, const String& text )
{
    GLUI_Control& ctrl = _imp->getControl( id );
    GLUI_EditText& textbox = DYNAMIC_CAST_REF( GLUI_EditText, ctrl );
    // FIXME: stupid constless GLUI!
    textbox.set_text( const_cast<char*>( text.c_str() ) );
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUI::setEditInt( ControlID id, Int32 val )
{
    GLUI_Control& ctrl = _imp->getControl( id );
    GLUI_EditText& text = DYNAMIC_CAST_REF( GLUI_EditText, ctrl );
    text.set_int_val( val );
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUI::setEditFloat( ControlID id, Float val )
{
    GLUI_Control& ctrl = _imp->getControl( id );
    GLUI_EditText& text = DYNAMIC_CAST_REF( GLUI_EditText, ctrl );
    text.set_float_val( val );
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUI::setEditIntLimits(
    ControlID id,
    Int32 low,
    Int32 high,
    bool wrap
) {
    GLUI_Control& ctrl = _imp->getControl( id );
    GLUI_EditText& text = DYNAMIC_CAST_REF( GLUI_EditText, ctrl );
    text.set_int_limits( low, high, wrap ? GLUI_LIMIT_WRAP : GLUI_LIMIT_CLAMP );
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUI::setEditFloatLimits(
    ControlID id,
    Float low,
    Float high,
    bool wrap
) {
    GLUI_Control& ctrl = _imp->getControl( id );
    GLUI_EditText& text = DYNAMIC_CAST_REF( GLUI_EditText, ctrl );
    text.set_float_limits( low, high, wrap ? GLUI_LIMIT_WRAP:GLUI_LIMIT_CLAMP );
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUI::setCheckbox( ControlID id, bool value )
{
    GLUI_Control& ctrl = _imp->getControl( id );
    GLUI_Checkbox& cb = DYNAMIC_CAST_REF( GLUI_Checkbox, ctrl );
    cb.set_int_val( value ? 1 : 0 );
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUI::setRadioGroup( ControlID id, UInt32 value )
{
    GLUI_Control& ctrl = _imp->getControl( id );
    GLUI_RadioGroup& rg = DYNAMIC_CAST_REF( GLUI_RadioGroup, ctrl );
    rg.set_int_val( value );
}

//------------------------------------------------------------------------------

GeMatrix4 GfxGLWindowGLUI::getRotate( ControlID id ) const
{
    const GLUI_Control& ctrl = _imp->getControl( id );
    const GLUI_Rotation& rot = DYNAMIC_CAST_REF( const GLUI_Rotation, ctrl );
    Float data[ 16 ];
    // FIXME: stupid constless GLUI!
    const_cast<GLUI_Rotation&>( rot ).
        get_float_array_val( data );
    return GeMatrix4::colMajor( data );
}

//------------------------------------------------------------------------------

Float GfxGLWindowGLUI::getTranslateX( ControlID id ) const
{
    const GLUI_Control& ctrl = _imp->getControl( id );
    const GLUI_Translation& trans =
        DYNAMIC_CAST_REF( const GLUI_Translation, ctrl );
    // FIXME: stupid constless GLUI!
    return const_cast<GLUI_Translation&>( trans ).
        get_x();
}

//------------------------------------------------------------------------------

Float GfxGLWindowGLUI::getTranslateY( ControlID id ) const
{
    const GLUI_Control& ctrl = _imp->getControl( id );
    const GLUI_Translation& trans =
        DYNAMIC_CAST_REF( const GLUI_Translation, ctrl );
    // FIXME: stupid constless GLUI!
    return const_cast<GLUI_Translation&>( trans ).
        get_y();
}

//------------------------------------------------------------------------------

Float GfxGLWindowGLUI::getTranslateZ( ControlID id ) const
{
    const GLUI_Control& ctrl = _imp->getControl( id );
    const GLUI_Translation& trans =
        DYNAMIC_CAST_REF( const GLUI_Translation, ctrl );
    // FIXME: stupid constless GLUI!
    return const_cast<GLUI_Translation&>( trans ).
        get_z();
}

//------------------------------------------------------------------------------

bool GfxGLWindowGLUI::getCheckbox( ControlID id ) const
{
    const GLUI_Control& ctrl = _imp->getControl( id );
    const GLUI_Checkbox& cb = DYNAMIC_CAST_REF( const GLUI_Checkbox, ctrl );
    // FIXME: stupid constless GLUI!
    return const_cast<GLUI_Checkbox&>( cb ).
        get_int_val() != 0;
}

//------------------------------------------------------------------------------

UInt32 GfxGLWindowGLUI::getRadioGroup( ControlID id ) const
{
    const GLUI_Control& ctrl = _imp->getControl( id );
    const GLUI_RadioGroup& rg = DYNAMIC_CAST_REF( const GLUI_RadioGroup, ctrl );
    // FIXME: stupid constless GLUI!
    return const_cast<GLUI_RadioGroup&>( rg ).
        get_int_val();
}

//------------------------------------------------------------------------------

String GfxGLWindowGLUI::getEditText( ControlID id ) const
{
    const GLUI_Control& ctrl = _imp->getControl( id );
    const GLUI_EditText& text = DYNAMIC_CAST_REF( const GLUI_EditText, ctrl );
    // FIXME: stupid constless GLUI!
    return const_cast<GLUI_EditText&>( text ).
        get_text();
}

//------------------------------------------------------------------------------

Int32 GfxGLWindowGLUI::getEditInt( ControlID id ) const
{
    const GLUI_Control& ctrl = _imp->getControl( id );
    const GLUI_EditText& text = DYNAMIC_CAST_REF( const GLUI_EditText, ctrl );
    // FIXME: stupid constless GLUI!
    return const_cast<GLUI_EditText&>( text ).
        get_int_val();
}

//------------------------------------------------------------------------------

Float GfxGLWindowGLUI::getEditFloat( ControlID id ) const
{
    const GLUI_Control& ctrl = _imp->getControl( id );
    const GLUI_EditText& text = DYNAMIC_CAST_REF( const GLUI_EditText, ctrl );
    // FIXME: stupid constless GLUI!
    return const_cast<GLUI_EditText&>( text ).
        get_float_val();
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUI::enable( ControlID id )
{
    GLUI_Control& ctrl = _imp->getControl( id );
    ctrl.enable();
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUI::disable( ControlID id )
{
    GLUI_Control& ctrl = _imp->getControl( id );
    ctrl.disable();
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUI::registerIdleCB()
{
    GLUI_Master.set_glutIdleFunc( idleCB );
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUI::registerCBs()
{
    ::glutDisplayFunc( displayCB );
    ::glutMotionFunc( motionCB );
    ::glutPassiveMotionFunc( passiveMotionCB );
    GLUI_Master.set_glutKeyboardFunc( keyboardCB );
    GLUI_Master.set_glutMouseFunc( mouseCB );
    GLUI_Master.set_glutSpecialFunc( specialCB );
    GLUI_Master.set_glutReshapeFunc( reshapeCB );
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUI::reshapeCB( int width, int height )
{
    int tx,ty,tw,th;
    GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );
    GfxGLWindowGLUI& window = DYNAMIC_CAST_REF( GfxGLWindowGLUI, getWindow() );
    ::glViewport( tx, ty, tw, th );
    window.windowResized( tx, ty, tw, th );
    window.handleMiscEvents();
    window._width = width;
    window._height = height;
}

//------------------------------------------------------------------------------

void GfxGLWindowGLUI::controlCB( int uid )
{
    // FIXME: need contravariant getWindow method...
    GfxGLWindowGLUI& window = DYNAMIC_CAST_REF( GfxGLWindowGLUI, getWindow() );

    window.uiReceived( window._imp->getControlID( uid ) );
    window.handleMiscEvents();
}
