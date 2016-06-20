//////////////////////////////////////////////////////////////////////
// Copyright (c) 2001-2003 David Pritchard <drpritch@alumni.uwaterloo.ca>
// 
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later
// version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#ifndef freecloth_clothAppConfig_h
#define freecloth_clothAppConfig_h

#ifndef freecloth_clothApp_package_h
#include <freecloth/clothApp/package.h>
#endif

#ifndef freecloth_geom_geMatrix4_h
#include <freecloth/geom/geMatrix4.h>
#endif

#ifndef freecloth_geom_geVector_h
#include <freecloth/geom/geVector.h>
#endif

#ifndef freecloth_gfx_gfxConfig_h
#include <freecloth/gfx/gfxConfig.h>
#endif

#ifndef freecloth_resmgt_rcShdPtr_h
#include <freecloth/resmgt/rcShdPtr.h>
#endif

////////////////////////////////////////////////////////////////////////////////
// FORWARD DECLARATIONS

////////////////////////////////////////////////////////////////////////////////
/*!
 * \class ClothAppConfig freecloth/clothApp/clothAppConfig.h
 * \brief Stored configuration for cloth application.
 *
 */
class ClothAppConfig : public GfxConfig
{
public:
    //----- types and enumerations -----

    typedef GfxConfig BaseClass;

    //----- member functions -----

    ClothAppConfig();

    //@{
    //! See base.
    virtual void load( ResConfigRegistryR& reg );
    virtual void save( ResConfigRegistryW& reg ) const;
    // FIXME: should be contravariant return type...
    virtual RCShdPtr<ResConfig> clone() const;
    //@}

    //@{
    //! Accessor
    const GeMatrix4& getCameraRotate() const;
    Float getCameraZoom() const;
    const GeMatrix4& getLightRotate() const;
    Float getFogStart() const;
    Float getFogEnd() const;
    //@}

    //@{
    //! Mutator
    void setCameraRotate( const GeMatrix4& );
    void setCameraZoom( Float );
    void setLightRotate( const GeMatrix4& );
    void setFogStart( Float );
    void setFogEnd( Float );
    //@}

protected:

    //----- member functions -----

    // Default copy constructor is fine.
    //ClothAppConfig( const ClothAppConfig& );

    //----- static member functions -----
    static void writeGeVector(
        ResConfigRegistryW& reg, 
        const String& key,
        const GeVector& value
    );
    static GeVector readGeVector(
        ResConfigRegistryR& reg, 
        const String& key,
        const GeVector& defVal
    );
    static void writeGeMatrix4(
        ResConfigRegistryW& reg, 
        const String& key,
        const GeMatrix4& value
    );
    static GeMatrix4 readGeMatrix4(
        ResConfigRegistryR& reg, 
        const String& key,
        const GeMatrix4& defVal
    );

private:

    //! Disallowed.
    ClothAppConfig& operator =( const ClothAppConfig& );

    //----- data members ----

    GeMatrix4 _cameraRotate;
    Float _cameraZoom;
    GeMatrix4 _lightRotate;
    Float _fogStart, _fogEnd;
};
#endif
