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

#include <freecloth/clothApp/clothAppConfig.h>
#include <freecloth/resmgt/resConfigRegistry.imp.h>
#include <freecloth/resmgt/rcShdPtr.h>
#include <freecloth/base/algorithm>
#include <freecloth/base/vector>

////////////////////////////////////////////////////////////////////////////////
// LOCAL DECLARATIONS

namespace {
    const char* CAMERA_ROTATE_NAME = "CameraRotate";
    const char* CAMERA_ZOOM_NAME = "CameraZoom";
    const char* LIGHT_ROTATE_NAME = "LightRotate";
    const char* FOG_START_NAME = "FogStart";
    const char* FOG_END_NAME = "FogEnd";
    
    const GeMatrix4 CAMERA_ROTATE_DEFAULT = 
        GeMatrix4::rotation( GeVector::xAxis(), -M_PI / 3 );
    const Float CAMERA_ZOOM_DEFAULT = -20.f;
    const GeMatrix4 LIGHT_ROTATE_DEFAULT = GeMatrix4::identity();
    const Float FOG_START_DEFAULT = 20.f;
    const Float FOG_END_DEFAULT = 500.f;
};

////////////////////////////////////////////////////////////////////////////////
// CLASS ClothAppConfig

//------------------------------------------------------------------------------

ClothAppConfig::ClothAppConfig()
  : _cameraRotate( CAMERA_ROTATE_DEFAULT ),
    _cameraZoom( CAMERA_ZOOM_DEFAULT ),
    _lightRotate( LIGHT_ROTATE_DEFAULT ),
    _fogStart( FOG_START_DEFAULT ),
    _fogEnd( FOG_END_DEFAULT )
{
}

//------------------------------------------------------------------------------

RCShdPtr<ResConfig> ClothAppConfig::clone() const
{
    return RCShdPtr<ResConfig>( new ClothAppConfig( *this ) );
}

//------------------------------------------------------------------------------

const GeMatrix4& ClothAppConfig::getCameraRotate() const
{
    return _cameraRotate;
}

//------------------------------------------------------------------------------

Float ClothAppConfig::getCameraZoom() const
{
    return _cameraZoom;
}

//------------------------------------------------------------------------------

const GeMatrix4& ClothAppConfig::getLightRotate() const
{
    return _lightRotate;
}

//------------------------------------------------------------------------------

Float ClothAppConfig::getFogStart() const
{
    return _fogStart;
}

//------------------------------------------------------------------------------

Float ClothAppConfig::getFogEnd() const
{
    return _fogEnd;
}

//------------------------------------------------------------------------------

void ClothAppConfig::setCameraRotate( const GeMatrix4& value )
{
    _cameraRotate = value;
}

//------------------------------------------------------------------------------

void ClothAppConfig::setCameraZoom( Float value )
{
    _cameraZoom = value;
}

//------------------------------------------------------------------------------

void ClothAppConfig::setLightRotate( const GeMatrix4& value )
{
    _lightRotate = value;
}

//------------------------------------------------------------------------------

void ClothAppConfig::setFogStart( Float value )
{
    _fogStart = value;
}

//------------------------------------------------------------------------------

void ClothAppConfig::setFogEnd( Float value )
{
    _fogEnd = value;
}

//------------------------------------------------------------------------------

void ClothAppConfig::load( ResConfigRegistryR& reg )
{
    BaseClass::load( reg );
    setCameraRotate(
        readGeMatrix4( reg, CAMERA_ROTATE_NAME, CAMERA_ROTATE_DEFAULT )
    );
    setCameraZoom( reg.readFloat( CAMERA_ZOOM_NAME, CAMERA_ZOOM_DEFAULT ) );
    setLightRotate(
        readGeMatrix4( reg, LIGHT_ROTATE_NAME, LIGHT_ROTATE_DEFAULT )
    );
    setFogStart( reg.readFloat( FOG_START_NAME, FOG_START_DEFAULT ) );
    setFogEnd( reg.readFloat( FOG_END_NAME, FOG_END_DEFAULT ) );
}

//------------------------------------------------------------------------------

void ClothAppConfig::save( ResConfigRegistryW& reg ) const
{
    BaseClass::save( reg );
    writeGeMatrix4( reg, CAMERA_ROTATE_NAME, getCameraRotate() );
    reg.writeFloat( CAMERA_ZOOM_NAME, getCameraZoom() );
    writeGeMatrix4( reg, LIGHT_ROTATE_NAME, getLightRotate() );
    reg.writeFloat( FOG_START_NAME, getFogStart() );
    reg.writeFloat( FOG_END_NAME, getFogEnd() );
}

//------------------------------------------------------------------------------

void ClothAppConfig::writeGeVector(
    ResConfigRegistryW& reg, 
    const String& key,
    const GeVector& value
) {
    std::vector<Float> values;
    values.push_back( value._x );
    values.push_back( value._y );
    values.push_back( value._z );
    reg.writeFloats( key, values );
}

//------------------------------------------------------------------------------

GeVector ClothAppConfig::readGeVector(
    ResConfigRegistryR& reg, 
    const String& key,
    const GeVector& defVal
) {
    std::vector<Float> values;
    reg.readFloats( key, values );
    GeVector value( defVal );
    if( values.size() == 3 ) {
        value._x = values[ 0 ];
        value._y = values[ 1 ];
        value._z = values[ 2 ];
    }
    return value;
}

//------------------------------------------------------------------------------

void ClothAppConfig::writeGeMatrix4(
    ResConfigRegistryW& reg, 
    const String& key,
    const GeMatrix4& value
) {
    std::vector<Float> values;
    for ( UInt32 r = 0; r < 4; ++r ) {
        for ( UInt32 c = 0; c < 4; ++c ) {
            values.push_back( value( r, c ) );
        }
    }
    reg.writeFloats( key, values );
}

//------------------------------------------------------------------------------

GeMatrix4 ClothAppConfig::readGeMatrix4(
    ResConfigRegistryR& reg, 
    const String& key,
    const GeMatrix4& defVal
) {
    std::vector<Float> values;
    reg.readFloats( key, values );
    GeMatrix4 value( defVal );
    if( values.size() == 16 ) {
        for ( UInt32 r = 0; r < 4; ++r ) {
            for ( UInt32 c = 0; c < 4; ++c ) {
                value( r, c ) = values[ r * 4 + c ];
            }
        }
    }
    return value;
}
